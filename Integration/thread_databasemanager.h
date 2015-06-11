#ifndef THREAD_DATABASEMANAGER_H
#define THREAD_DATABASEMANAGER_H

/*!
 * \file thread_databasemanager.h
 *
 * \class Thread_Databasemanager
 *
 * \brief classe Thread_Databasemanager, hérite de QThread pour effectuer le traitement sur la base de données sans bloquer l'IHM.
 *
 *        <a href="https://github.com/BTS2-Projet-1/Locker_Control">Projet Locker Control sur github.</a>
 *
 * @startuml{Thread_Databasemanager.svg}
 *
 * QThread <|-- Thread_Databasemanager
 *
 * Thread_Databasemanager *-- SQLite_Local_DatabaseManager
 * SQLite_Local_DatabaseManager o-- Thread_Databasemanager
 *
 * CSQLite_Local_DB o-- Thread_Databasemanager
 *
 * Thread_Databasemanager  o-- SQLite_Local_DatabaseManager
 *
 * @enduml
 *
 * \author HOUDAYER Pierre
 *
 * \version 1.1
 *
 * \date 15 mai 2015
 */

#include <QThread>

#include "csqlite_local_db.h"
#include "DataStructures.h"
#include "sqlite_local_databasemanager.h"
#include "consolewindows.h"

class Thread_Databasemanager;
class SQLite_Local_DatabaseManager;
class ConsoleWindows;

class Thread_Databasemanager :  public QThread
{
    Q_OBJECT

    public:
        Thread_Databasemanager(ConsoleWindows *TConsoleWindows = NULL,SQLite_Local_DatabaseManager *DatabaseManager = NULL, CSQLite_Local_DB *BD = NULL);
        ~Thread_Databasemanager();
        void run();

    private:
        int m_SelectedOption;
        CSQLite_Local_DB *m_DB;
        ConsoleWindows *m_ConsoleWindows;
        SQLite_Local_DatabaseManager *m_DatabaseManager;

    signals:

        /*!
         * \fn void terminatedThread(bool)
         *
         * \brief Emet le signal de fin du thread avec la valeur de retour du thread en paramètre.
         *
         * \param bool La valeur de retour de fin du thread.
         */
        void terminatedThread(bool);

    public slots:
        void SlowThread(int Timems);
        void startThreadManager(int opt);
};

#endif // THREAD_DATABASEMANAGER_H
