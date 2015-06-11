#include "thread_statusdoors.h"
#include <QDebug>

Thread_StatusDoors::Thread_StatusDoors(CDoors *Cdoors) : QThread()
{

    m_CDoors = Cdoors;
    m_termined = false;
}

Thread_StatusDoors::~Thread_StatusDoors()
{
    terminate();
}

void Thread_StatusDoors::setListBox(QList<int> list){
    m_listdoors.clear();
    m_listdoors.append(list);
}

void Thread_StatusDoors::stopThread()
{
    m_termined = true;

}

void Thread_StatusDoors::run()
{
    QList<struct_DoorsStatus> listdoorstatustmp;

    m_termined = false;

    while(!m_termined)
    {
        listdoorstatustmp = m_CDoors->DoorsStatus(m_listdoors);
        qDebug()<<"recupe liste";
        if(listdoorstatustmp.isEmpty())
        {
            qDebug()<<"liststatut vide";
        }
        else{
            //tri les status des portes selectionnés
            int i=0;
            while((i<listdoorstatustmp.size()) && (!m_termined))
            {

                   emit StatuDoor(listdoorstatustmp[i]);
                   qDebug()<<"envoi signal";
                   msleep(300);
                   i++;

            }
        }
    }

}
