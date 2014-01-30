#include <QThread>
#include <QMap>

class RfidTask : public QThread
{
    Q_OBJECT
public:
    RfidTask(QObject *parent = 0) : QThread(parent)
    {}

    //QMap<QString, QString> links;
    QString url;
    QString lastTagId;
    bool isCurrentTagDetected;
    bool showTagId;
    bool stop;

    void run();

signals:
    void tagDetected(QString id);
    void errorDetected(QString err);
    void readerReady();
};
