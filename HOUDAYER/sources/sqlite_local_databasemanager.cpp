#include <QDebug>
#include <QThread>

#include "sqlite_local_databasemanager.h"
#include "ui_sqlite_local_databasemanager.h"
#include "DataStructures.h"
#include "thread_localdatabase_autosave.h"

/*!
 * \fn SQLite_Local_DatabaseManager::SQLite_Local_DatabaseManager(ConsoleWindows *TConsoleWindows,CSQLite_Local_DB *BD) : ui(new Ui::SQLite_Local_DatabaseManager)
 *
 * \brief Constructeur de la classe SQLite_Local_DatabaseManager, qui hérite de QWidget pour la gestion de l'IHM.
 *
 *        Initialise les différents attributs, créer les deux threads (Thread_Databasemanager + Thread_LocalDatabase_AutoSave).
 *
 * \param ConsoleWindows *TConsoleWindows,CSQLite_Local_DB *BD, *TConsoleWindows pointeur sur la classe parent ConsoleWindows et *BD, pointeur valide sur une instance de
 *        CSQLite_Local_DB pour la communication avec la base de données.
 */
SQLite_Local_DatabaseManager::SQLite_Local_DatabaseManager(ConsoleWindows *TConsoleWindows,CSQLite_Local_DB *BD) :
    ui(new Ui::SQLite_Local_DatabaseManager)
{
    ui->setupUi(this);
    if( (BD != NULL) || (TConsoleWindows != NULL) )
    {
        m_BD = BD;
        m_ConsoleWindows = TConsoleWindows;
        m_Thread_Database_Manager = new Thread_Databasemanager(m_ConsoleWindows, this, m_BD);
        m_Thread_Database_Manager_AutoSave = new Thread_LocalDatabase_AutoSave(this,m_BD);
        m_Page_DB_Manager = WINDEX_DB_MANAGER;
        m_Page_Loading = WINDEX_LOADING;
        m_AutoSaveInterval_Seconds = AUTOSAVE_DATABASE_TIMEOUT_SECONDS*1000;
        m_EndLoadingTimer = new QTimer();
        m_AutoSave_Database_Timer = new QTimer();

        connect(m_AutoSave_Database_Timer,SIGNAL(timeout()), this, SLOT(TimeOutAutoSave()));
        connect(m_EndLoadingTimer,SIGNAL(timeout()), this,SLOT(TimeOutLoading()),Qt::QueuedConnection);
        connect(ui->B_Home,SIGNAL(clicked()),this,SLOT(ReturnMaintenanceMenu()));
        connect(ui->B_CreateDB_Save,SIGNAL(clicked()),this,SLOT(Update_Confirm()));
        connect(ui->B_CB_CreateDB_Valid,SIGNAL(clicked()),this,SLOT(Update_Confirm()));
        connect(ui->B_LoadingValid, SIGNAL(clicked()), this, SLOT(ReturnDB_Manager()));
        connect(ui->B_Confirm_No, SIGNAL(clicked()), this,SLOT(ReturnDB_Manager()));
        connect(ui->B_Confirm_Yes,SIGNAL(clicked()), this,SLOT(Run_DB_Manager()));
        connect(ui->B_ReturnMaintenanceMenu,SIGNAL(clicked()), this,SLOT(ReturnMaintenanceMenu()));
        connect(m_BD, SIGNAL(CurrentProcessLoading(QString,int)), this,SLOT(SetStatusLoading(QString,int)));
        connect(m_Thread_Database_Manager,SIGNAL(terminatedThread(bool)), this,SLOT(terminatedThreadManager(bool)));
        connect(m_Thread_Database_Manager_AutoSave,SIGNAL(TerminedThread_LocalDatabase_AutoSave(bool)), this, SLOT(terminatedThreadAutoSave(bool)));

        ui->DB_Manager_stackedWidget->setCurrentIndex(WINDEX_DB_MANAGER);
        ui->L_AutoReturnDB_Manager->setText("");
        m_AutoSave_Database_Timer->start(m_AutoSaveInterval_Seconds);
    }
    else
        qDebug() << "[FATAL ERROR] : Uninitiatized pointer (= NULL)";
}

/*!
 * \fn void SQLite_Local_DatabaseManager::Run_DB_Manager()
 *
 * \brief Emet le signal startThreadManager() pour lancer le thread Thread_Databasemanager avec une option précédement sélectionnée
 *        et modifie le Widget actuel par la page de chargement.
 */
void SQLite_Local_DatabaseManager::Run_DB_Manager()
{
    emit startThreadManager(m_SelectedOption);
    ui->DB_Manager_stackedWidget->setCurrentIndex(WINDEX_LOADING);
}

/*!
 * \fn SQLite_Local_DatabaseManager::~SQLite_Local_DatabaseManager()
 *
 * \brief Destructeur de la classe SQLite_Local_DatabaseManager, Stop les timers et détruit l'IHM.
 */
SQLite_Local_DatabaseManager::~SQLite_Local_DatabaseManager()
{
    m_EndLoadingTimer->stop();
    m_AutoSave_Database_Timer->stop();
    delete ui;
}

/*!
 * \fn void SQLite_Local_DatabaseManager::ReturnHome()
 *
 * \brief Appele via le pointeur sur ShowWidgets, la fonction ShowWConsoleWindows() pour afficher la page d'accueil.
 */
void SQLite_Local_DatabaseManager::ReturnHome()
{
    m_ShowWidgets->ShowWConsoleWindows();
}

/*!
 * \fn void SQLite_Local_DatabaseManager::ReturnMaintenanceMenu()
 *
 * \brief Appele via le pointeur sur ShowWidgets, la fonction ShowWMaintenanceMenu() pour afficher la page du menu de maintenance.
 */
void SQLite_Local_DatabaseManager::ReturnMaintenanceMenu()
{
    m_ShowWidgets->ShowWMaintenanceMenu();
}

/*!
 * \fn void SQLite_Local_DatabaseManager::ReturnDB_Manager()
 *
 * \brief Change la page actuelle par le menu de gestion de la base de données.
 */
void SQLite_Local_DatabaseManager::ReturnDB_Manager()
{
    m_SelectedOption = -1;
    ui->DB_Manager_stackedWidget->setCurrentIndex(WINDEX_DB_MANAGER);
    ui->B_LoadingValid->setEnabled(false);
}

/*!
 * \fn void SQLite_Local_DatabaseManager::Update_Confirm()
 *
 * \brief Demande �  l'utilisateur, via l'IHM, de confirmer la commande précédement demandée.
 */
void SQLite_Local_DatabaseManager::Update_Confirm()
{
    if(sender() == this->ui->B_CreateDB_Save)
    {
        ui->L_DB_Manager_Confirm->setText("Créer une nouvelle sauvegarde de la base de données LC.");
        m_SelectedOption = SAVE_DATABASE;
    }
    else
    {
        m_SelectedOption = ui->CB_DB_Create->currentIndex();
        switch (m_SelectedOption)
        {
            case CBINDEX_DB_DEFAULT_EMPTY:
            {
                m_SelectedOption = EMPTY_DATABASE;
                ui->L_DB_Manager_Confirm->setText("Créer une nouvelle base de données LC vide/neuve (avec les tables mais sans données/configurations).");
                break;
            }
            case CBINDEX_DB_DEFAULT_TEST:
            {
                m_SelectedOption = DEFAULT_DATABASE;
                ui->L_DB_Manager_Confirm->setText("Créer une nouvelle base de données LC par defaut/de test (avec les tables et des données/configurations préconfigurées).");
                break;
            }
            case CBINDEX_DB_LAST_SAVE:
            {
                m_SelectedOption = LOAD_SAVE_DATABASE;
                ui->L_DB_Manager_Confirm->setText("Créer une nouvelle base de données LC avec la dernère sauvegarde effectué.");
                break;
            }
        }
    }

    ui->DB_Manager_stackedWidget->setCurrentIndex(WINDEX_CONFIRM);
}

/*!
 * \fn void SQLite_Local_DatabaseManager::Build_EmptyDB()
 *
 * \brief Emet le signal startThreadManager() pour lancer le thread Thread_Databasemanager en mode EMPTY_DATABASE et modifie le Widget actuel par la page de chargement.
 */
void SQLite_Local_DatabaseManager::Build_EmptyDB()
{
    emit startThreadManager(m_SelectedOption);
    ui->DB_Manager_stackedWidget->setCurrentIndex(WINDEX_LOADING);
    qDebug()<< "-----------DB_Manager : EMPTY_DATABASE";
}

/*!
 * \fn void SQLite_Local_DatabaseManager::Build_DefaultDB()
 *
 * \brief Emet le signal startThreadManager() pour lancer le thread Thread_Databasemanager en mode DEFAULT_DATABASE et modifie le Widget actuel par la page de chargement.
 */
void SQLite_Local_DatabaseManager::Build_DefaultDB()
{
    emit startThreadManager(m_SelectedOption);
    ui->DB_Manager_stackedWidget->setCurrentIndex(WINDEX_LOADING);
    qDebug()<< "-----------DB_Manager : DEFAULT_DATABASE";
}

/*!
 * \fn void SQLite_Local_DatabaseManager::Build_LastSaveDB()
 *
 * \brief Emet le signal startThreadManager() pour lancer le thread Thread_Databasemanager en mode LOAD_SAVE_DATABASE et modifie le Widget actuel par la page de chargement.
 */
void SQLite_Local_DatabaseManager::Build_LastSaveDB()
{
    emit startThreadManager(m_SelectedOption);
    ui->DB_Manager_stackedWidget->setCurrentIndex(WINDEX_LOADING);
    qDebug()<< "-----------DB_Manager : LOAD_SAVE_DATABASE";
}

/*!
 * \fn void SQLite_Local_DatabaseManager::SaveDB()
 *
 * \brief Emet le signal startThreadManager() pour lancer le thread Thread_Databasemanager en mode SAVE_DATABASE et modifie le Widget actuel par la page de chargement.
 */
void SQLite_Local_DatabaseManager::SaveDB()
{
    ui->DB_Manager_stackedWidget->setCurrentIndex(WINDEX_LOADING);
    emit startThreadManager(m_SelectedOption);
    qDebug()<< "-----------DB_Manager : SAVE_DATABASE";
}

/*!
 * \fn void SQLite_Local_DatabaseManager::terminatedThreadManager(bool ThreadExitValue)
 *
 * \brief Reception du signal terminatedThread() de Thread_Databasemanager.
 */
void SQLite_Local_DatabaseManager::terminatedThreadManager(bool ThreadExitValue)
{
    if(ThreadExitValue)
        qDebug()<< "[SUCCESS] : Database manager operation";
    else
        qDebug()<< "[ERROR] : Database manager operation";

    emit EndThread_DatabaseManager(ThreadExitValue);    //For ConsoleWindows.
}

/*!
 * \fn void SQLite_Local_DatabaseManager::SetStatusLoading(QString Message, int Value)
 *
 * \brief Reception du signal CurrentProcessLoading() de CSQLite_Local_DB et modifie la barre de chargement en fonction des paramètres reçu.
 *
 * \param QString Message, int Value, Avec "Message" le message qui sera affiché sur l'IHM, et "Value" la valeur en pour cent de la barre de chargement souhaité.
 */
void SQLite_Local_DatabaseManager::SetStatusLoading(QString Message, int Value)
{
    if(ui->DB_Manager_stackedWidget->currentIndex() == WINDEX_LOADING)
    {
        if(Value < 12)
        {
            ui->progressBar->setValue(0);
            ui->Label_ProgressBar->setText("Terminé");
            ui->B_LoadingValid->setEnabled(true);
            m_EndLoadingTimer->start(1);
        }

        if(Value == 100)
        {
            m_EndLoadingTimer->start(1);
            ui->B_LoadingValid->setEnabled(true);
            ui->Label_ProgressBar->setText("Terminé");
            ui->progressBar->setValue(Value);
        }

        if(Value > 100)
        {
            ui->B_LoadingValid->setEnabled(true);
            ui->Label_ProgressBar->setText("Terminé");
            ui->progressBar->setValue(100);
        }
        else
        {
            ui->progressBar->setValue(Value);
            ui->Label_ProgressBar->setText(Message);
        }
    }
    else
    {
        m_EndLoadingTimer->stop();
        ui->Label_ProgressBar->setText(" ");
    }
}

/*!
 * \fn void SQLite_Local_DatabaseManager::TimeOutLoading()
 *
 * \brief Reception du signal timeout() du timer de fin de chargement pour un retour automatique au menu de gestion si il n'y a aucune action de l'utilisateur.
 */
void SQLite_Local_DatabaseManager::TimeOutLoading()
{
    static int Cmpt = 0;

    if(ui->DB_Manager_stackedWidget->currentIndex() == WINDEX_LOADING)
    {
        if(Cmpt == END_LOADING_TIMEOUT_SECONDS)
        {
            Cmpt = 0;
            ui->L_AutoReturnDB_Manager->setText("");
            m_EndLoadingTimer->stop();
            ReturnDB_Manager();
        }
        else
        {
            m_EndLoadingTimer->start(1000);
            ui->L_AutoReturnDB_Manager->setText("Retour automatique dans "+ QString::number(END_LOADING_TIMEOUT_SECONDS-Cmpt,10) +" secondes.");
            Cmpt++;
        }
    }
    else
    {
        m_EndLoadingTimer->stop();
        ui->L_AutoReturnDB_Manager->setText("");
        Cmpt = 0;
    }
}

/*!
 * \fn void SQLite_Local_DatabaseManager::TimeOutLoading()
 *
 * \brief Reception du signal timeout() du timer de sauvegardes automatiques, appelé toutes les 15 minutes par defaut, pour effectuer une sauvegarde de la base de données.
 */
void SQLite_Local_DatabaseManager::TimeOutAutoSave()
{
   if(m_Thread_Database_Manager_AutoSave->wait(2000))
    {
        qDebug() << "-----------------------------<[ AUTO SAVE NEEDED ]>------------------------------";
        emit startThreadAutoSave();
        m_AutoSave_Database_Timer->start(m_AutoSaveInterval_Seconds);
   }
   else{

       qDebug() <<"erreur wait";
   }
}

/*!
 * \fn void SQLite_Local_DatabaseManager::TimeOutLoading()
 *
 * \brief Reception du signal timeout() terminatedThread() de Thread_Databasemanager_AutoSave, si la sauvegarde n'as pas été effectué au serveur LMS,
 *        l'intervale d'appel est d'une minute tant que l'erreur survient.
 */
void SQLite_Local_DatabaseManager::terminatedThreadAutoSave(bool ThreadExitValue)
{
    if(ThreadExitValue)
        qDebug()<< "[SUCCESS] : (AutoSave) Database manager operation";
    else
    {
        qDebug()<< "[WARNING] : (AutoSave) LMS offline, creating local database... try to send it again in 1 minute !";
        m_AutoSave_Database_Timer->stop();
        m_AutoSave_Database_Timer->start(60000);
    }
}
