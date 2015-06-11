#include "thread_databasemanager.h"
#include <QDebug>

/*!
 * \fn Thread_Databasemanager::Thread_Databasemanager(ConsoleWindows *TConsoleWindows, SQLite_Local_DatabaseManager *DatabaseManager, CSQLite_Local_DB *BD) : QThread()
 *
 * \brief Constructeur de la classe Thread_Databasemanager, qui hérite de QThread pour la gestion de d'un thread permettant la gestion de la base de données,
 *        sans bloquer l'IHM.
 *
 *        Initialise les différents attributs et connecte les signaux.
 *
 * \param ConsoleWindows *TConsoleWindows, SQLite_Local_DatabaseManager *DatabaseManager, CSQLite_Local_DB *BD.\n
 *        *TConsoleWindows pointeur valide sur la classe parent ConsoleWindows\n
 *        *DatabaseManager pointeur valide sur une instance de SQLite_Local_DatabaseManager pour faire remonter les informations par les signaux\n
 *        *BD, pointeur valide sur une instance de CSQLite_Local_DB pour la communication avec la base de données.\n
 */
Thread_Databasemanager::Thread_Databasemanager(ConsoleWindows *TConsoleWindows, SQLite_Local_DatabaseManager *DatabaseManager, CSQLite_Local_DB *BD) : QThread()
{
    if( (BD != NULL) || (DatabaseManager != NULL) || (TConsoleWindows != NULL) )
    {
        m_DB = BD;
        m_DatabaseManager = DatabaseManager;
        m_ConsoleWindows = TConsoleWindows;

        connect(m_DB,SIGNAL(SlowProcess(int)), this,SLOT(SlowThread(int)));
        connect(m_DatabaseManager,SIGNAL(startThreadManager(int)), this,SLOT(startThreadManager(int)));
        connect(m_ConsoleWindows,SIGNAL(Build_Databse(int)), this,SLOT(startThreadManager(int)));

    }
    else
    {
        qDebug() << "[FATAL ERROR] : Uninitiatized pointer (NULL)";
    }

}

/*!
 * \fn Thread_Databasemanager::~Thread_Databasemanager()
 *
 * \brief Destructeur de la classe Thread_Databasemanager.
 */
Thread_Databasemanager::~Thread_Databasemanager()
{}

/*!
 * \fn void Thread_Databasemanager::run()
 *
 * \brief Fonction de traitement du thread, lance la tache de construction de la base de données avec l'option selectionnée.
 */
void Thread_Databasemanager::run()
{
    bool Result = false;

    Result = m_DB->SQL_Database_Manager(m_SelectedOption);
    emit terminatedThread(Result);

    if(Result)
        exit(0);
    else
        exit(-1);

}

/*!
 * \fn void Thread_Databasemanager::SlowThread(int Timems)
 *
 * \brief Reception du signal SlowProcess() de CSQLite_Local_DB pour ralentir le thread (fait un msleep()).
 *
 * \param int Timems, avec "Timems" le temps d'arrêt en millisecondes.
 */
void Thread_Databasemanager::SlowThread(int Timems)
{
     msleep(Timems);
}

/*!
 * \fn void Thread_Databasemanager::startThreadManager(int opt)
 *
 * \brief Initalise l'option selectionnée pour la construction de la base de données et démarre le thread.
 *
 * \param int opt, avec "opt" l'option selectionnée pour la construction de la base de données.
 */
void Thread_Databasemanager::startThreadManager(int opt)
{
    m_SelectedOption = opt;
    start();
}
