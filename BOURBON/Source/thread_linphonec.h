#ifndef THREAD_LINPHONEC_H
#define THREAD_LINPHONEC_H

#include <QThread>
#include <QProcess>
#include "assistance.h"

class Assistance;

class Thread_linphonec : public QThread
{
    Q_OBJECT

private:
    Assistance *m_assistance;
    QProcess *m_linphonec;
    bool m_loopevent;
    QByteArray m_ByteArray;
public:
    explicit Thread_linphonec(Assistance *assist = NULL, QThread *parent = 0);
    ~Thread_linphonec();
    void run();

signals:

public slots:
    void startThread();
    void EndThread();
};

#endif // THREAD_LINPHONEC_H
