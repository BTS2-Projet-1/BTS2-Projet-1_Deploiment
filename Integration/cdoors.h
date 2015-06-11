#ifndef CDOORS_H
#define CDOORS_H

#include "DataStructures.h"
#include "csqlite_local_db.h"

class CDoors
{
private :
    CSQLite_Local_DB *LC_DataBase;
    int m_compt;
public:
    CDoors(CSQLite_Local_DB *BD);
    bool OpenDoors(QList<int> DoorsList);
    bool CloseDoors(QList<int> DoorsList);
    bool LockDoors(QList<int> DoorsList);
    bool UnlockDoors(QList<int> DoorsList);

    bool OpenDoor(int idDoor);
    bool CloseDoor(int idDoor);
    bool LockDoor(int idDoor);
    bool UnlockDoor(int idDoor);

    QList<struct_DoorsStatus> DoorsStatus(QList<int> DoorsList);
    bool DoorsIsClosed(QList<int> DoorsList);

    struct_DoorsStatus DoorStatus(int idDoor);
    bool DoorIsClosed(int idDoor);
};

#endif // CDOORS_H
