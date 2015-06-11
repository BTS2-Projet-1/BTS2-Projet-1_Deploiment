#ifndef THREAD_STATUSDOORS_H
#define THREAD_STATUSDOORS_H
/*!
 *
 * \file Thread_StatusDoors.h
 * \class Thread_StatusDoors "Header"
 * \brief Header de la classe Thread_StatusDoors
 *
 * Thread: vérification de l'état des consignes.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */
#include <QThread>
#include "DataStructures.h"
#include "cdoors.h"


class Thread_StatusDoors : public QThread
{
    Q_OBJECT

public:
    Thread_StatusDoors(CDoors *Cdoors);
    ~Thread_StatusDoors();
    void setListBox(QList<int> list);

    void stopThread();
    void run();

signals :
    void StatuDoor(struct struct_DoorsStatus);

private:
    QList<int> m_listdoors;
    CDoors *m_CDoors;
    bool m_termined;


};

#endif // THREAD_STATUSDOORS_H
