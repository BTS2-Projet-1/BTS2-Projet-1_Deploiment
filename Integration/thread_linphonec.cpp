#include "thread_linphonec.h"
#include <QDebug>

Thread_linphonec::Thread_linphonec(Assistance *assist, QThread *parent):QThread(parent)
{
    m_assistance = assist;
    m_linphonec = new QProcess();
    if (m_assistance != NULL)
    {
        connect(m_assistance,SIGNAL(startThreadLinphonec()),this,SLOT(startThread()));
        connect(m_assistance,SIGNAL(EndThreadLinphonec()),this,SLOT(EndThread()));
        m_loopevent = true;
    }
}

Thread_linphonec::~Thread_linphonec()
{

}

void Thread_linphonec::run()
{
    QString readout;
    int result;
    while(m_loopevent)
    {
        m_linphonec->waitForReadyRead(1000);
        m_ByteArray = m_linphonec->readAll();
        readout.append(m_ByteArray);
        qDebug()<<readout;

    }

    m_linphonec->write("terminate\n");

    m_linphonec->waitForBytesWritten(1000);
    m_linphonec->write("quit\n");

    result = m_linphonec->exitCode();
    if(result != 0)
    {
        qDebug()<<"error exit process";
    }
}

void Thread_linphonec::startThread()
{
    start();

    QStringList arg;
        arg << "-V";

    //Démarrage du programme Linphonec en mode vidéo
    m_linphonec->start("/usr/bin/./linphonec",arg);

    if (!m_linphonec->waitForStarted())
        qDebug()<<"help";

    if(m_linphonec->write("call 600\n")==-1)
     {
        qDebug()<<"error ecriture";
     }

   // m_linphonec->waitForBytesWritten(1000);
}

void Thread_linphonec::EndThread()
{
    m_loopevent = false;

}
