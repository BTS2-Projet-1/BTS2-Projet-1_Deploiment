#ifndef SQLITE_LOCAL_DATABASEMANAGER_H
#define SQLITE_LOCAL_DATABASEMANAGER_H

/*!
 * \file sqlite_local_databasemanager.h
 *
 * \class SQLite_Local_DatabaseManager
 *
 * \brief classe SQLite_Local_DatabaseManager, gestion des actions et interractions entre l'interface de maintenance de la base de données et la classe CSQLite_Local_DB.
 *        La classe SQLite_Local_DatabaseManager permet aux utilisateurs de reconstruire, sauvegarder ou charger une nouvelle base de données pour Locker Control.
 *
 *        <a href="https://github.com/BTS2-Projet-1/Locker_Control">Projet Locker Control sur github.</a>
 *
 * @startuml{SQLite_Local_DatabaseManager.svg}
 *
 * QWidget <|-- SQLite_Local_DatabaseManager
 *
 * SQLite_Local_DatabaseManager *-- ConsoleWindows
 * SQLite_Local_DatabaseManager o-- ConsoleWindows
 *
 * Thread_Databasemanager *-- SQLite_Local_DatabaseManager
 * Thread_Databasemanager_AutoSave *-- SQLite_Local_DatabaseManager
 * SQLite_Local_DatabaseManager o-- Thread_Databasemanager
 * SQLite_Local_DatabaseManager o-- Thread_Databasemanager_AutoSave
 *
 * CSQLite_Local_DB *-- ConsoleWindows
 * CSQLite_Local_DB o-- ConsoleWindows
 *
 * ShowWidgets o-- SQLite_Local_DatabaseManager
 *
 * ConsoleWindows o-- SQLite_Local_DatabaseManager
 * CSQLite_Local_DB o-- SQLite_Local_DatabaseManager
 *
 * Thread_Databasemanager  o-- SQLite_Local_DatabaseManager
 * Thread_Databasemanager_AutoSave o-- SQLite_Local_DatabaseManager
 *
 * @enduml
 *
 * \author HOUDAYER Pierre
 *
 * \version 1.2
 *
 * \date 14 mai 2015
 */

#include <QWidget>
#include <QTimer>

#include "showwidgets.h"
#include "csqlite_local_db.h"
#include "thread_databasemanager.h"
#include "thread_localdatabase_autosave.h"
#include "consolewindows.h"

#define WINDEX_DB_MANAGER 0
#define WINDEX_LOADING 1
#define WINDEX_CONFIRM 2
#define CBINDEX_DB_LAST_SAVE 0
#define CBINDEX_DB_DEFAULT_EMPTY 1
#define CBINDEX_DB_DEFAULT_TEST 2
#define END_LOADING_TIMEOUT_SECONDS 5
#define AUTOSAVE_DATABASE_TIMEOUT_SECONDS 900 //15mns

namespace Ui {
class SQLite_Local_DatabaseManager;
}

class Thread_Databasemanager;
class Thread_LocalDatabase_AutoSave;
class ConsoleWindows;

class SQLite_Local_DatabaseManager : public QWidget
{
    Q_OBJECT

    public:
        explicit SQLite_Local_DatabaseManager(ConsoleWindows *TConsoleWindows = NULL,CSQLite_Local_DB *BD = NULL);

        /*!
         * \fn void SetShowWidgets(ShowWidgets *TShowWidgets)
         *
         * \brief Permet un lien via pointeur, sur une instance de la classe ShowWidgets, afin de pouvoir avoir accès au differents Widgets de Locker Control.
         *
         * \param ShowWidgets *TShowWidgets, Avec "TShowWidgets", un pointeur valide sur sur une instance de la classe ShowWidgets.
         */
        void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}
        ~SQLite_Local_DatabaseManager();

    public slots:
        void ReturnHome();
        void ReturnMaintenanceMenu();
        void ReturnDB_Manager();
        void Update_Confirm();
        void Build_EmptyDB();
        void Build_DefaultDB();
        void Build_LastSaveDB();
        void SaveDB();
        void SetStatusLoading(QString Message,int Value);
        void Run_DB_Manager();

    signals:

        /*!
         * \fn void startThreadManager(int Opt)
         *
         * \brief Emet le signal pour lancer le thread Thread_Databasemanager avec l'option choisie.
         *
         * \param int Opt, Avec "Opt", l'option de construction de la base de données sélectionnée.
         */
        void startThreadManager(int Opt);

        /*!
         * \fn void startThreadAutoSave()
         *
         * \brief Emet le signal pour lancer le thread Thread_LocalDatabase_AutoSave.
         */
        void startThreadAutoSave();

        /*!
         * \fn void EndThread_DatabaseManager(bool Value)
         *
         * \brief Emet le signal pour informer ConsoleWindows de la fin du thread Thread_Databasemanager avec l'option choisie.
         */
        void EndThread_DatabaseManager(bool Value);

    private slots:
        void TimeOutLoading();
        void TimeOutAutoSave();
        void terminatedThreadAutoSave(bool ThreadExitValue);
        void terminatedThreadManager(bool ThreadExitValue);

    private:
        Ui::SQLite_Local_DatabaseManager *ui;
        ShowWidgets *m_ShowWidgets;
        Thread_Databasemanager *m_Thread_Database_Manager;
        Thread_LocalDatabase_AutoSave *m_Thread_Database_Manager_AutoSave;
        QTimer *m_EndLoadingTimer;
        QTimer *m_AutoSave_Database_Timer;
        CSQLite_Local_DB *m_BD;
        ConsoleWindows *m_ConsoleWindows;
        int m_Page_DB_Manager;
        int m_Page_Loading;
        int m_AutoSaveInterval_Seconds;
        int m_SelectedOption;
};

#endif // SQLITE_LOCAL_DATABASEMANAGER_H
