#ifndef THREAD_LOCALDATABASE_AUTOSAVE_H
#define THREAD_LOCALDATABASE_AUTOSAVE_H

/*!
 * \file thread_databasemanager_autosave.h
 *
 * \class Thread_LocalDatabase_AutoSave
 *
 * \brief classe Thread_LocalDatabase_AutoSave, hérite de QThread pour effectuer la sauvegarde de la base de données sans bloquer l'IHM.
 *
 *        <a href="https://github.com/BTS2-Projet-1/Locker_Control">Projet Locker Control sur github.</a>
 *
 * @startuml{Thread_LocalDatabase_AutoSave.svg}
 *
 * QThread <|-- Thread_LocalDatabase_AutoSave
 *
 * Thread_LocalDatabase_AutoSave *-- SQLite_Local_DatabaseManager
 * SQLite_Local_DatabaseManager o-- Thread_LocalDatabase_AutoSave
 *
 * CSQLite_Local_DB o-- Thread_LocalDatabase_AutoSave
 *
 * Thread_LocalDatabase_AutoSave  o-- SQLite_Local_DatabaseManager
 *
 * @enduml
 *
 * \author HOUDAYER Pierre
 *
 * \version 1.0
 *
 * \date 15 mai 2015
 */

#include <QObject>
#include <QThread>

#include "sqlite_local_databasemanager.h"
#include "csqlite_local_db.h"

class SQLite_Local_DatabaseManager;

class Thread_LocalDatabase_AutoSave : public QThread
{
    Q_OBJECT

    private:
        CSQLite_Local_DB *m_BD;
        SQLite_Local_DatabaseManager *m_DatabaseManager;

    public:
        explicit Thread_LocalDatabase_AutoSave(SQLite_Local_DatabaseManager *parent = 0, CSQLite_Local_DB *BD = NULL);
        void run();
        ~Thread_LocalDatabase_AutoSave();

    signals:

        /*!
         * \fn void TerminedThread_LocalDatabase_AutoSave(bool ThreadExitValue)
         *
         * \brief Emet le signal de fin du thread avec la valeur de retour du thread en paramètre.
         *
         * \param bool valeur de retour de fin du thread.
         */
        void TerminedThread_LocalDatabase_AutoSave(bool ThreadExitValue);

    public slots:
        void StartThread_LocalDatabase_AutoSave();
};

#endif // THREAD_LOCALDATABASE_AUTOSAVE_H
