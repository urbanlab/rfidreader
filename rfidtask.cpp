#include <nfc/nfc.h>
#include <nfc/nfc-types.h>

#include <QObject>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMessageBox>
#include <QStyle>
#include "rfidtask.h"

typedef char connstring_t[1024];

void return_id(char ** dst, const nfc_target *pnt)
{
  int i;
  *dst = (char *)(malloc(1000));
  strcpy(*dst, "");
  switch (pnt->nm.nmt) {
    case NMT_ISO14443A:
    {
      //strcpy(*dst, "0");
      int idlength=0;
      int sak=pnt->nti.nai.btSak;
      if((sak==0x9) || (sak==0x08) || (sak==0x18) || (sak==0x28) || (sak==0x88) || (sak==0x38))
        idlength=4;
      else
        idlength=7;

      for(i=0;i<idlength;i++)
        sprintf(*dst, "%s%02x",*dst,(int)(pnt->nti.nai.abtUid[i]));
    }
    break;
    case NMT_JEWEL:
    //strcpy(*dst, "1");
    for(i=0;i<4;i++)
      sprintf(*dst, "%s%02x",*dst,(int)(pnt->nti.nji.btId[i]));
    break;
    case NMT_FELICA:
    //strcpy(*dst, "2");
    for(i=0;i<8;i++)
      sprintf(*dst, "%s%02x",*dst,(int)(pnt->nti.nfi.abtId[i]));
    break;
    case NMT_ISO14443B:
    //strcpy(*dst, "3");
    for(i=0;i<4;i++)
      sprintf(*dst, "%s%02x",*dst,(int)(pnt->nti.nbi.abtPupi[i]));
    break;
    case NMT_ISO14443BI:
    //strcpy(*dst, "4");
    for(i=0;i<4;i++)
      sprintf(*dst, "%s%02x",*dst,(int)(pnt->nti.nii.abtDIV[i]));
    break;
    case NMT_ISO14443B2SR:
    //strcpy(*dst, "5");
    for(i=0;i<8;i++)
      sprintf(*dst, "%s%02x",*dst,(int)(pnt->nti.nsi.abtUID[i]));
      break;
    case NMT_ISO14443B2CT:
    //strcpy(*dst, "6");
    for(i=0;i<4;i++)
      sprintf(*dst, "%s%02x",*dst,(int)(pnt->nti.nci.abtUID[i]));
      break;
    case NMT_DEP:
    //strcpy(*dst, "7");
    for(i=0;i<10;i++)
      sprintf(*dst, "%s%02x",*dst,(int)(pnt->nti.ndi.abtNFCID3[i]));
      break;
  }
}




void RfidTask::run()
{
  static nfc_context *context;
  static nfc_device *pnd = NULL;
  isCurrentTagDetected = false;
  stop = false;

  nfc_init(&context);
  if (context == NULL) {
    emit errorDetected(tr("Can't initialize NFC context"));
    return;
  }




  const uint8_t uiPollNr = 1;
  const uint8_t uiPeriod = 1;
  nfc_modulation nmModulations[5];
  nmModulations[0].nmt = NMT_ISO14443A;
  nmModulations[0].nbr = NBR_106;
  nmModulations[1].nmt = NMT_ISO14443B;
  nmModulations[1].nbr = NBR_106;
  nmModulations[2].nmt = NMT_FELICA;
  nmModulations[2].nbr = NBR_212;
  nmModulations[3].nmt = NMT_FELICA;
  nmModulations[3].nbr = NBR_424;
  nmModulations[4].nmt = NMT_JEWEL;
  nmModulations[4].nbr = NBR_106;
  const size_t szModulations = 5;
  nfc_target nt;
  int attempts = 5;
  bool first_after_init = true;
  bool first_fail = true;
  while(!stop)
  {

      if (!pnd) {
          //pnd = nfc_open(context, "acr122_usb:003:003-072f-2200-00-00");
          //pnd = nfc_open(context, "acr122_pcsc");
          //pnd = nfc_open(context, "pn53x_usb");
          //pnd = nfc_open(context, "acr122_usb");
          connstring_t cs[10];
          nfc_list_devices(context, &cs[0], 10);
          int preferedi=0;
          for(int i=0;i<10;i++)
          {

              if(strstr(cs[i],"pcsc")!=0)
                  preferedi=i;
          }
          printf("Trying to open %d: %s\n", preferedi, cs[preferedi]);
          pnd = nfc_open(context, cs[preferedi]);
          if(!pnd)
          {
              //nfc_exit(context);
              //return;
              for(int i=0;i<100 && !stop;i++)
                  msleep(10);
              if(attempts-->0)
                  continue;
              if(first_fail)
                  emit errorDetected(tr("Can't open NFC device"));
              first_fail = false;
              continue;
          }
          if (nfc_initiator_init(pnd) < 0) {
              if(first_fail)
                  emit errorDetected(tr("Can't initialize NFC device"));
              first_fail = false;
              //nfc_exit(context);
              //return;
              pnd=0;
              continue;
          }
      }

    if(first_after_init)
    {
        emit readerReady();
        first_after_init = false;
    }
    int res = nfc_initiator_poll_target(pnd,
                                        nmModulations,
                                        szModulations,
                                        uiPollNr,
                                        uiPeriod,
                                        &nt);
    if (res < 0) {
      nfc_perror(pnd, "nfc_initiator_poll_target");
      isCurrentTagDetected=false;
    }

    if (res > 0) {
      char *s;
      return_id(&s, &nt);

      QString sid = QString(s);
      if((!isCurrentTagDetected)||(lastTagId!=sid))
      {
        lastTagId = sid;
        isCurrentTagDetected = true;
        emit tagDetected(sid);
      }

      //free(s);
    } else {
      isCurrentTagDetected=false;
    }
  }

  //nfc_abort_command(pnd);
  nfc_close(pnd);
  nfc_exit(context);
  this->exit();
}
