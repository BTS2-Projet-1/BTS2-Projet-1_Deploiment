#include <QDebug>

#include "thread_localdatabase_autosave.h"

/*!
 * \fn Thread_LocalDatabase_AutoSave::Thread_LocalDatabase_AutoSave(SQLite_Local_DatabaseManager *parent, CSQLite_Local_DB *BD) : QThread()
 *
 * \brief Constructeur de la classe Thread_LocalDatabase_AutoSave, qui hérite de QThread pour la gestion de d'un thread permettant,
 *        la création de la sauvegarde de la base de données, sans bloquer l'IHM.
 *
 *        Initialise les différents attributs et connecte les signaux.
 *
 * \param SQLite_Local_DatabaseManager *parent, CSQLite_Local_DB *BD.\n
 *        *DatabaseManager pointeur valide sur une instance de SQLite_Local_DatabaseManager pour faire remonter les informations par les signaux\n
 *        *BD, pointeur valide sur une instance de CSQLite_Local_DB pour la communication avec la base de données.\n
 */
Thread_LocalDatabase_AutoSave::Thread_LocalDatabase_AutoSave(SQLite_Local_DatabaseManager *parent, CSQLite_Local_DB *BD) : QThread()
{
    if( (BD != NULL) || (parent != NULL) )
    {
        m_BD = BD;
        m_DatabaseManager = parent;

        connect(m_DatabaseManager, SIGNAL(startThreadAutoSave()), this, SLOT(StartThread_LocalDatabase_AutoSave()));
    }
    else
        qDebug()<< "[FATAL ERROR] : Uninitialized pointer (NULL)";
}

/*!
 * \fn void Thread_LocalDatabase_AutoSave::run()
 *
 * \brief Fonction de traitement du thread, lance la création de la sauvegarde de la base de données avec l'option selectionnée.
 */
void Thread_LocalDatabase_AutoSave::run()
{
    bool Result = false;

    Result = m_BD->DataBase_AutoSave_Needed();
    emit TerminedThread_LocalDatabase_AutoSave(Result);

    if(Result)
        exit(0);
    else
        exit(-1);
}

/*!
 * \fn Thread_LocalDatabase_AutoSave::~Thread_LocalDatabase_AutoSave()
 *
 * \brief Destructeur de la classe Thread_LocalDatabase_AutoSave.
 */
Thread_LocalDatabase_AutoSave::~Thread_LocalDatabase_AutoSave()
{}

/*!
 * \fn void Thread_LocalDatabase_AutoSave::StartThread_LocalDatabase_AutoSave()
 *
 * \brief Lance le thread de création de la base de données
 */
void Thread_LocalDatabase_AutoSave::StartThread_LocalDatabase_AutoSave()
{
    start();
}

