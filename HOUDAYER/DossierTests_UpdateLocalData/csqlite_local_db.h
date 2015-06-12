#ifndef CSQLITE_LOCAL_DB_H
#define CSQLITE_LOCAL_DB_H

/*!
 * \file csqlite_local_db.h
 *
 * \class CSQLite_Local_DB
 *
 * \brief classe CSQLite_Local_DB, gestion des communications entre une base de données locale de type SQLite, avec l'application LC (LockerControl).
 *
 *        La classe CSQLite_Local_DB offre aux utilisateurs de LC (Locker Control) un moyen simple et efficace de communication avec une base de données locale.
 *
 *        /!\ Attention : Il est obligatoire de passer en réference ou par pointeur, l'intance de cette classe entre les objets de LC, pour ne pas supprimer l'ancienne
 *        connexion établie sur la base de données et obtenir un comportement innatendu.
 *
 *        Dans le cas ou le programme tente de réaliser des actions avec la base de données avec l'attibut m_DataBaseIsReady égal à faux (false),
 *        les fonctions membres retouneront une erreur et aucunes des actions demendées ne seront réalisées.
 *
 *        <a href="https://github.com/BTS2-Projet-1/Locker_Control">Projet Locker Control sur github.</a>
 *
 * @startuml{CSQLite_Local_DB.svg}
 *
 * QObject <|-- CSQLite_Local_DB : need\n signals
 * ConsoleWindows *-- CSQLite_Local_DB
 * ConsoleWindows o-- CSQLite_Local_DB
 * ConsoleWindows *-- CLMS_DB
 * CLMS_DB o-- CSQLite_Local_DB : Linked by\n ConsoleWindows
 * ConsoleWindows o-- CLMS_DB
 * ConsoleWindows *-- Class_Need_Database_Access
 * ConsoleWindows o-- Class_Need_Database_Access
 * class Class_Need_Database_Access
 * note bottom : Class_Need_Database_Access can have an access to the database\n with a pointer on CSQLite_Local_DB gived by ConsoleWindows\n when ConsoleWindows build Class_Need_Database_Access
 *
 * @enduml
 *
 * \author HOUDAYER Pierre
 *
 * \version 2.6
 *
 * \date 14 mai 2015
 *
 */

#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QObject>

#include "DataStructures.h"
#include "clms_db.h"


#define MAX_SQL_COLUMN_READ 10  /*!< Définie la taille maximum des champs de la base de données pour la lecture des fichiers de sauvegarde par exemple. */

class CSQLite_Local_DB : public QObject
{
    Q_OBJECT

    private:
        CLMS_DB *m_LMS_Server;
        QSqlDatabase m_DataBase;
        QString m_DatabaseName;
        QString m_SQL_File_DatabaseEmpty_Path;
        QString m_SQL_File_DatabaseDefault_Path;
        QString m_SQL_File_DatabaseLastSave_Path;
        QString m_SQL_File_DatabaseLastSave_Path_FromLMS;
        QList<QString> m_Infos_Database;
        bool m_DataBaseIsReady;
        bool m_LMS_Server_IsReady;

    protected:
        QList<QString> ReadSQL_File(QString FilePath);
        bool WriteSQL_File(QString FilePath,QString Data);
        bool SaveDatabase(QString FilePath);
        bool LoadDatabaseSave(QString FilePath);
        bool Connect_DB();
        bool Delete_DB();

    public:
        explicit CSQLite_Local_DB(CLMS_DB *LMS_Server = NULL);
        ~CSQLite_Local_DB();

        QList<QString> DataBaseStatus();

        /*!
         * \fn bool Get_DataBaseIsReady()
         *
         * \brief Permet de récupérer la valeur (état), de la connexion (ouverture) de la base de donnée.
         *
         * \return Retourne un booléen, à vrai (true) si la base de donnée est prête pour l'utilisation, faux (false) sinon.
         */
        bool Get_DataBaseIsReady(){return this->m_DataBaseIsReady;}

        /*!
         * \fn bool IsReady()
         *
         * \brief Permet de récupérer la valeur (état), de la connexion (ouverture) de la base de donnée.
         *
         * \return Retourne un booléen, à vrai (true) si la base de donnée est prête pour l'utilisation, faux (false) sinon.
         */
        bool IsReady(){return this->m_DataBaseIsReady;}

        /*!
         * \fn bool DatabaseIsReady()
         *
         * \brief Permet de récupérer la valeur (état), de la connexion (ouverture) de la base de donnée.
         *
         * \return Retourne un booléen, à vrai (true) si la base de donnée est prête pour l'utilisation, faux (false) sinon.
         */
        bool DatabaseIsReady(){return this->m_DataBaseIsReady;}
        QString lastError();
        QSqlQuery SendQuery(QString Query);
        QList<struct_DoorsStatus> SendQueryDoorsStatus(QString Query);
        int Login(QString);
        bool SavePerfomAction(QString Table,QString Action);
        int CheckExtractCode(QString ExtractCode);
        bool DeleteExtractCode(QString UsedExtractCode);
        QList<int> GetBoxesListConsumer(QString ExtractCode);
        QList<struct struct_DoorGeom> GetBoxesList();
        QList<int> Get_All_idBox();
        bool UpdateLocalData(QList<struct struct_PackagesUseBox> UpdatedPackagesUseBoxList);
        bool SQL_Database_Manager(int Option);
        QList<int> GetFreeBoxes();
        bool SendQueryDoorsIsClosed(QString Query);
        bool SetDammagedBoxes(QList<int> DammagedBoxes);
        bool SetDammagedBox(int DammagedBox);
        bool UnsetDammagedBoxes(QList<int> FixedBoxes);
        bool UnsetDammagedBox(int FixedBox);
        QList<int> GetDammagedBoxes();
        bool DataBase_AutoSave_Needed();
        QString Get_SiteName();
        bool Set_SiteName(QString SiteName);
        int Get_NumberOfBoxes();
        bool BoxIsDammaged(int idBox);
        bool Set_Temporary_BoxUsed(int IdBox);
        bool Unset_Temporary_BoxUsed(int IdBox);
        bool Close_Database(bool Delete_Database);

    signals:

        /*!
         * \fn void CurrentProcessLoading(QString,int);
         *
         * \brief Signal emit pour informer une instance qui s'y ait connecté, de l'état (avencée) de la tâche demendée.
         */
        void CurrentProcessLoading(QString,int);

        /*!
         * \fn void SlowProcess(int);
         *
         * \brief Signal emit avec une valeur normalement utilisée pour effectuer un msleep() afin de ralentir le processus qui s'y ait connecté.
         */
        void SlowProcess(int);

};

#endif // CSQLITE_LOCAL_DB_H
