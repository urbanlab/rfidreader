#include <nfc/nfc.h>
#include <nfc/nfc-types.h>

#include <QObject>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QMessageBox>
#include <QStyle>
#include "rfidtask.h"

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
    emit errorDetected("Can't initialize NFC context");
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
  while(!stop)
  {

      if (!pnd) {
          pnd = nfc_open(context, NULL);
          if(!pnd)
          {
              emit errorDetected("Can't open NFC device");
              //nfc_exit(context);
              //return;
              for(int i=0;i<200 && !stop;i++)
                  msleep(10);
              continue;
          }
          if (nfc_initiator_init(pnd) < 0) {
              emit errorDetected("Can't initialize NFC device");
              //nfc_exit(context);
              //return;
              pnd=0;
              continue;
          }
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

      free(s);
    } else {
      isCurrentTagDetected=false;
    }
  }
  nfc_exit(context);
}
