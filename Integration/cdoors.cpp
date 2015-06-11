#include "cdoors.h"
#include <QDebug>

CDoors::CDoors(CSQLite_Local_DB *BD)
{
    LC_DataBase = BD;
    if(!LC_DataBase->Get_DataBaseIsReady())
        qDebug()<<"[CDoors][FATAL ERROR] : No database connection !";
    m_compt=10;
}

bool CDoors::OpenDoors(QList<int> DoorsList)
{
    for(int i=0; i<DoorsList.count();i++)
    {
        qDebug() << "Opening door : " << DoorsList[i];
        LC_DataBase->SendQuery("UPDATE DoorLocker SET DoorLocker_OpenState='true' WHERE Linked_idDoor='"+QString::number(DoorsList[i],10)+"'");
    }
    return true;
}

bool CDoors::CloseDoors(QList<int> DoorsList)
{
    for(int i=0; i<DoorsList.count();i++)
    {
        qDebug() << "Closing door : " << DoorsList[i];
        LC_DataBase->SendQuery("UPDATE DoorLocker SET DoorLocker_OpenState='false' WHERE Linked_idDoor='"+QString::number(DoorsList[i],10)+"'");
    }
    return true;
}

bool CDoors::LockDoors(QList<int> DoorsList)
{
    for(int i=0; i<DoorsList.count();i++)
    {
        qDebug() << "Locking door : " << DoorsList[i];
        LC_DataBase->SendQuery("UPDATE DoorLocker SET DoorLocker_LockState='true' WHERE Linked_idDoor='"+QString::number(DoorsList[i],10)+"'");
    }
    return true;
}

bool CDoors::UnlockDoors(QList<int> DoorsList)
{
    for(int i=0; i<DoorsList.count();i++)
    {
        qDebug() << "Unlocking door : " << DoorsList[i];
        LC_DataBase->SendQuery("UPDATE DoorLocker SET DoorLocker_LockState='false' WHERE Linked_idDoor='"+QString::number(DoorsList[i],10)+"'");
    }
    return true;
}

bool CDoors::OpenDoor(int idDoor)
{
    QList<int> Door;
    Door.append(idDoor);
    return this->OpenDoors(Door);
}

bool CDoors::CloseDoor(int idDoor)
{
    QList<int> Door;
    Door.append(idDoor);
    return this->CloseDoors(Door);
}

bool CDoors::LockDoor(int idDoor)
{
    QList<int> Door;
    Door.append(idDoor);
    return this->LockDoors(Door);
}

bool CDoors::UnlockDoor(int idDoor)
{
    QList<int> Door;
    Door.append(idDoor);
    return this->UnlockDoors(Door);
}

bool CDoors::DoorsIsClosed(QList<int> DoorsList)
{
    bool Result = false;

    for(int i=0; i<DoorsList.count(); i++)
    {
        Result = LC_DataBase->SendQueryDoorsIsClosed("SELECT * FROM DoorLocker WHERE DoorLocker_OpenState='true' AND Linked_idDoor='"
                                                    +QString::number(DoorsList[i],10)+"'");
        if(Result == false)
            return Result;
    }

    return Result;
}

struct_DoorsStatus CDoors::DoorStatus(int idDoor)
{
    QList<struct_DoorsStatus> DoorStatus;
    QList<int> Door;

    Door.append(idDoor);
    DoorStatus = DoorsStatus(Door);

    return DoorStatus[0];
}

bool CDoors::DoorIsClosed(int idDoor)
{
    QList<int> Door;
    Door.append(idDoor);
    return this->DoorsIsClosed(Door);
}

QList<struct_DoorsStatus> CDoors::DoorsStatus(QList<int> DoorsList)
{
    QList<struct_DoorsStatus> DoorsStatusListComplete;
    QList<struct_DoorsStatus> DoorsStatusList;
    QList<int> DammagedBoxes;
    QList<int> FreeBoxes;
    struct_DoorsStatus DoorsStatusTemp;

    DammagedBoxes = LC_DataBase->GetDammagedBoxes();
    DoorsStatusListComplete = LC_DataBase->SendQueryDoorsStatus("SELECT * FROM DoorLocker");
    FreeBoxes = LC_DataBase->GetFreeBoxes();

    for(int i=0; i<DoorsStatusListComplete.size(); i++)
    {
        for(int j=0; j<DoorsList.size();j++)
        {
            if( DoorsStatusListComplete[i].BoxNumber == DoorsList[j] )
            {
                DoorsStatusTemp.BoxNumber = DoorsStatusListComplete[i].BoxNumber;
                if(DammagedBoxes.contains(DoorsStatusTemp.BoxNumber))
                    DoorsStatusTemp.BoxIsDammaged = true;
                else
                    DoorsStatusTemp.BoxIsDammaged = false;

                if(FreeBoxes.contains(DoorsStatusTemp.BoxNumber))
                    DoorsStatusTemp.BoxIsUsed = false;
                else
                    DoorsStatusTemp.BoxIsUsed = true;

                DoorsStatusTemp.DoorIsOpen = DoorsStatusListComplete[i].DoorIsOpen;
                DoorsStatusTemp.DoorIsLock = DoorsStatusListComplete[i].DoorIsLock;

                DoorsStatusList.append(DoorsStatusTemp);
            }
        }
    }

    if(DoorsIsClosed(DoorsList))
    {
        m_compt-=1;

        if(m_compt == 0)
        {
            CloseDoors(DoorsList);
            m_compt=10;
        }
    }
    return DoorsStatusList;
}
