#include "csqlite_local_db.h"

#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QSqlResult>
#include <QSqlRecord>
#include <QDebug>
#include <QDateTime>
#include <QCryptographicHash>

/*!
 * \fn CSQLite_Local_DB::CSQLite_Local_DB(CLMS_DB *LMS_Server) : QObject()
 *
 * \brief Constructeur de la classe CSQLite_Local_DB, qui hérite de QObject affin de pouvoir émètre des signaux,
 *        pour certaines fonctions nécessitant l'information de l'évolution de la tâche demandée (sauvegarde de la base de données par exemple).
 *
 *        Le constructeur initalise et ouvre la base de données automatiquement grâce aux attibuts de la classe.
 *        Si l'ouverture de la base de données est correcte un attibut "m_DataBaseIsReady" accessible avec Get_DataBaseIsReady() revoie avec un booléen,
 *        si la connexion avec la base de données est effectuée sans erreurs vrai (true) ou avec un echec faux (false).
 *
 *        /!\ Attention : l'attibut m_DataBaseIsReady peux être aussi mis à faux (false) si le pointeur sur une instance de CLMS_DB passé en paramètre est égal à "NULL",
 *        de la même façon que pour la connexion à une base de données, les fonctions membres n'effectueront aucunes actions. Alors que la majeure partie des fonctions membres
 *        peuevent traiter les demandes avec la base de données mais l'acces global des fonctions seront bloquées.
 *
 * @startuml{CSQLite_Local_DB--Builder.png}
 *
 *  autonumber
 *  ConsoleWindows -> CSQLite_Local_DB : CSQLite_Local_DB()
 *  activate CSQLite_Local_DB #0000FF
 *
 *  CSQLite_Local_DB -> CSQLite_Local_DB : Connect_DB()
 *  activate CSQLite_Local_DB #00BBFF
 *
 *  database LC_SQLite_DB
 *  CSQLite_Local_DB -> LC_SQLite_DB : Try to connect, else\n build a new database.
 *  activate LC_SQLite_DB #50EEFF
 *
 *  LC_SQLite_DB --> CSQLite_Local_DB : Database status (bool).
 *  deactivate LC_SQLite_DB
 *
 *  deactivate CSQLite_Local_DB
 *  deactivate CSQLite_Local_DB
 *
 *  CSQLite_Local_DB --> ConsoleWindows : Database satuts (bool).
 *  deactivate ConsoleWindows
 *
 * @enduml
 *
 * \param CLMS_DB *LMS_Server La classe CSQLite_Local_DB à besoin d'un pointeur valide sur une instance de la classe CLMS_DB,
 *        pour communiquer avec le serveur LMS (Sauvegrades et mises à jour par exemple).
 */
CSQLite_Local_DB::CSQLite_Local_DB(CLMS_DB *LMS_Server) : QObject()
{
    if(LMS_Server == NULL)
    {
        qDebug() << "[WARNING] : Uninitialized pointer : (CLMS_DB) = NULL !";
        this->m_LMS_Server_IsReady = false;
    }
    else
        this->m_LMS_Server_IsReady = true;

    m_LMS_Server = LMS_Server;
    this->m_DataBaseIsReady = false;

    this->m_DatabaseName = "LC_SQL_files/LockerControlDatabase.sqlite";
    this->m_SQL_File_DatabaseEmpty_Path = "LC_SQL_files/LockerControl_SQL_File_DatabaseEmpty.sql";
    this->m_SQL_File_DatabaseDefault_Path = "LC_SQL_files/LockerControl_SQL_File_DatabaseDefault.sql";
    this->m_SQL_File_DatabaseLastSave_Path = "LC_SQL_files/LockerControl_SQL_File_DatabaseLastSave.sql";
    this->m_SQL_File_DatabaseLastSave_Path_FromLMS = "LC_SQL_files/LockerControl_SQL_File_DatabaseLastSave_FromLMS.sql";

    if(Connect_DB())
    {
        this->m_DataBaseIsReady = true;  // Open database : Open
        qDebug() << "[SUCCESS] : Database is ready!";
    }
    else
    {
        this->m_DataBaseIsReady = false; // Open database : Fail
        qDebug()<< "[FATAL ERROR] : No Database connection !";
    }

}

/*!
 * \fn CSQLite_Local_DB::~CSQLite_Local_DB()
 *
 * \brief Destructeur de la classe CSQLite_Local_DB, Si la base de données a été ouverte, elle sera fermée et l'instance sera détruite.
 *
 * @startuml{CSQLite_Local_DB--destroyer.svg}
 *
 * autonumber
 * ConsoleWindows -> CSQLite_Local_DB : ~CSQLite_Local_DB()
 * activate CSQLite_Local_DB #0000FF
 *
 * CSQLite_Local_DB -> CSQLite_Local_DB : if database opened\n then, close database
 * activate CSQLite_Local_DB #00A0FF
 *
 * deactivate CSQLite_Local_DB
 * destroy CSQLite_Local_DB
 *
 * @enduml
 */
CSQLite_Local_DB::~CSQLite_Local_DB()
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        // Close database
        this->m_DataBaseIsReady = false;
        this->m_DataBase.close();
    }
}

/*!
 * \fn QList<QString> CSQLite_Local_DB::DataBaseStatus()
 *
 * \brief Récupère les informations de connexion avec la base de données, les tables, le driver utilisé... (Utilisation pour le mode debug).
 *
 * \return Retourne une QList<QString>, liste de QString contenant les informations de connexion, liste vide si la base de données n'est pas ouverte.
 */
QList<QString> CSQLite_Local_DB::DataBaseStatus()
{
    QList<QString> TablesList;

    if(m_DataBaseIsReady == true)   // Database opened.
    {
        TablesList = this->m_DataBase.tables();
        this->m_Infos_Database << "--------------------------------< DATABASE INFOS >--------------------------------"
                               << "Database is opened with succes !"
                               << "Driver : " + this->m_DataBase.driverName()
                               << "Database name : " + this->m_DataBase.databaseName()
                               << "Connection name : " + this->m_DataBase.connectionName()
                               << "Connect options : " + this->m_DataBase.connectOptions()
                               << "Host : " + this->m_DataBase.hostName()
                               << "Current User : " + this->m_DataBase.hostName()
                               << "Tables : ";
                                    for(int i=0; i<TablesList.count();i++)
                                    {
                                        this->m_Infos_Database << "        " + TablesList[i];
                                    }
        this->m_Infos_Database << "----------------------------------------< END >----------------------------------------";

        return this->m_Infos_Database;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        TablesList.clear();
        TablesList.append("[Database status] : No Database connection !");
    }
    return TablesList;
}

/*!
 * \fn bool CSQLite_Local_DB::Connect_DB()
 *
 * \brief Effectue la connexion d'une base de données avec le fichier SQLite de la base de données,
 *        si ce n'est pas possible, (fichier inexistant par exemple) la fonction tente de créer une base de donnée.
 *        Si il existe les fichiers de sauvegarde, la nouvelle base de données sera construite avec sinon,
 *        si les fichiers des base SQL exixtent la nouvelle base de données sera construite avec.
 *        Enfin si aucun des cas précédent n'est possible, la fonction retoune une erreur.
 *
 * \return Retourne un booléen sur la réussite (true) ou l'échec (false) de la connexion a la base de données,
 *         en cas d'érreur il peut être utile d'appeler lastError() pour plus d'informations.
 *
 * @startuml{CSQLite_Local_DB--Connect_DB.svg}
 *
 * autonumber
 * ConsoleWindows -> CSQLite_Local_DB : Builder CSQLite_Local_DB()
 *
 * alt database file exsist
 *
 *        database LC_SQLite_DB
 *        CSQLite_Local_DB -> CSQLite_Local_DB : Connect_DB()
 *        activate CSQLite_Local_DB #0000FF
 *        deactivate CSQLite_Local_DB
 *
 *     else
 *
 *        alt SQL files exsists
 *
 *            alt SQL save file
 *
 *                CSQLite_Local_DB -> CSQLite_Local_DB : Connect_DB()
 *                activate CSQLite_Local_DB #00A0FF
 *                deactivate CSQLite_Local_DB
 *
 *                CSQLite_Local_DB -> CSQLite_Local_DB : Set_DataBaseIsReady(true)
 *            end
 *
 *             alt SQL build database files exsists
 *
 *                 CSQLite_Local_DB -> CSQLite_Local_DB : DatabaseManager(LOAD_SAVE)
 *                 activate CSQLite_Local_DB #00CCFF
 *
 *                 CSQLite_Local_DB -> LC_SQLite_DB
 *                 activate LC_SQLite_DB #00CCFF
 *
 *                 LC_SQLite_DB --> CSQLite_Local_DB
 *                 deactivate LC_SQLite_DB
 *                 deactivate CSQLite_Local_DB
 *
 *                 CSQLite_Local_DB -> CSQLite_Local_DB : Connect_DB()
 *                 activate CSQLite_Local_DB #00A0FF
 *                 deactivate CSQLite_Local_DB
 *
 *                 CSQLite_Local_DB -> CSQLite_Local_DB : Set_DataBaseIsReady(true)
 *             end
 *       else
 *
 *           CSQLite_Local_DB -> CSQLite_Local_DB : Set_DataBaseIsReady(false)
 *           activate CSQLite_Local_DB #30DDFF
 *           deactivate CSQLite_Local_DB
 *           note right : Bool used in\n all of the class
 *           CSQLite_Local_DB --> ConsoleWindows : false fatal error
 *     end
 * end
 *
 * @enduml
 */
bool CSQLite_Local_DB::Connect_DB()
{
    bool Result = false;
    QFile Database_File(this->m_DatabaseName);
    QFile SQL_DB_Empty_File(this->m_SQL_File_DatabaseEmpty_Path);
    QFile SQL_DB_Test_File(this->m_SQL_File_DatabaseDefault_Path);
    QFile LastDatabaseSave_File(this->m_SQL_File_DatabaseLastSave_Path);
    QFile LastDatabaseSave_FromLMS_File(this->m_SQL_File_DatabaseLastSave_Path_FromLMS);

    if(Database_File.exists())
    {
        qDebug() << "[SUCCESS] : The database file is found, opening...";
        // Find QSLite driver
        this->m_DataBase = QSqlDatabase::addDatabase("QSQLITE");

        //Set database name
        this->m_DataBase.setDatabaseName(this->m_DatabaseName);

        if(this->m_DataBase.open())
            Result = true;  // Open database : OK
        else
            Result = false; // Open database : Fail
    }
    else
    {
         if(LastDatabaseSave_File.exists() || LastDatabaseSave_FromLMS_File.exists())
         {
             qDebug() << "[WARNING] : The database file is missing but the database save file found, building the new database...";

             this->m_DataBase = QSqlDatabase::addDatabase("QSQLITE");
             this->m_DataBase.setDatabaseName(this->m_DatabaseName);

             if(this->m_DataBase.open())
                 m_DataBaseIsReady = true;  // temporary for SQL_Database_Manager()


             if(SQL_Database_Manager(LOAD_SAVE_DATABASE))
                 Result = true;
         }
         else
         {
             if(SQL_DB_Empty_File.exists() && SQL_DB_Test_File.exists())
             {
                 this->m_DataBase = QSqlDatabase::addDatabase("QSQLITE");
                 this->m_DataBase.setDatabaseName(this->m_DatabaseName);

                 if(this->m_DataBase.open())
                     m_DataBaseIsReady = true;  // temporary for SQL_Database_Manager()

                 if(SQL_Database_Manager(DEFAULT_DATABASE))
                 {
                     qDebug() << "[WARNING] : The database file is missing but SQL_files found, building the new database...";
                     Result = true;
                 }
             }
             else
             {
                qDebug()<< " ";
                qDebug()<< "============================================================================";
                qDebug()<< "||                                                                        ||";
                qDebug()<< "||>--- _|!|_              ---< FATAL ERROR : >---               _|!|_ ---<||";
                qDebug()<< "||>--- _|!|_        \"LC_SQL_files\" directory is missing !        _|!|_ ---<||";
                qDebug()<< "||>--- _|!|_     We can't create the database without this !    _|!|_ ---<||";
                qDebug()<< "||                                                                        ||";
                qDebug()<< "============================================================================";
                qDebug()<< " ";
             }
         }
    }
    m_DataBaseIsReady = false;

    return Result;
}

/*!
 * \fn QSqlQuery CSQLite_Local_DB::SendQuery(QString Query)
 *
 * \brief Fonction utilisée uniquement dans la phase de developpement pour un accès rapide d'envoi personalisé de requêtes au format SQL.
 *
 * \param QString Query Requête souhaitée au format SQL.
 *
 * \return Retourne un QSqlQuery, avec le retour de la requête souhaitée en paramètre, ou vide si la base de données n'est pas ouverte.
 */
QSqlQuery CSQLite_Local_DB::SendQuery(QString Query)
{
    QSqlQuery query(this->m_DataBase);

    if(m_DataBaseIsReady == true)   // Database opened.
    {
        if(query.exec(Query))
            SavePerfomAction("[WARNING] EXTERN CALL",Query);

        return query;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        query.clear();
    }

    return query;
}

/*!
 * \fn QList<struct_DoorsStatus> CSQLite_Local_DB::SendQueryDoorsStatus(QString Query)
 *
 * \brief Fonction utilisée uniquement dans la phase de developpement pour simuler l'accès de classe CDoors au portes via le bus CAN aux consignes automatiques.
 *
 * \param QString Query Requête souhaitée au format SQL = "SELECT * FROM DoorLocker".
 *
 * \return Retourne une QList<struct_DoorsStatus>, liste des structutres struct_DoorsStatus contenant toutes les informations sur l'état mesuré des portes,
 *         (Numéro, taille, ouverture, fermeture, verouillage...) ou une liste vide si la base de données n'est pas ouverte.
 */
QList<struct_DoorsStatus> CSQLite_Local_DB::SendQueryDoorsStatus(QString Query)
{
    QList<struct_DoorsStatus> DoorsStatusList;

    if(m_DataBaseIsReady == true)   // Database opened.
    {
        QSqlQuery query(this->m_DataBase);
        struct_DoorsStatus DoorsStatusTmp;

        SavePerfomAction("[WARNING] EXTERN CALL (DoorsStatus)",Query);
        if(query.exec(Query))
        {
            while(query.next())
            {
                DoorsStatusTmp.BoxNumber = query.value(2).toInt();

                DoorsStatusTmp.BoxIsDammaged = query.value(3).toBool();
                DoorsStatusTmp.DoorIsOpen = query.value(3).toBool();
                DoorsStatusTmp.DoorIsLock = query.value(4).toBool();

                DoorsStatusList.append(DoorsStatusTmp);
            }
        }

        return DoorsStatusList;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        DoorsStatusList.clear();
    }
    return DoorsStatusList;
}

/*!
 * \fn bool CSQLite_Local_DB::Delete_DB()
 *
 * \brief Ferme la base de données précédament ouverte et la supprime (utilisée en fin de programme ou dans le cadre de mise à jour).
 *
 * \return Retourne un booléen sur la réussite de la suppression de la base de données (true) ou l'échec (false) si la base de données n'est pas ouverte,
 *         en cas d'érreur il peut être utile d'appeler lastError() pour plus d'informations.
 *
 * @startuml{CSQLite_Local_DB--Delete_DB.svg}
 *
 * autonumber
 * CSQLite_Local_DB -> CSQLite_Local_DB  : Delete_DB()
 * activate CSQLite_Local_DB #0000FF
 *
 * alt DataBaseIsReady
 *     CSQLite_Local_DB -> CSQLite_Local_DB : close()
 *     activate CSQLite_Local_DB #00A0FF
 *     deactivate CSQLite_Local_DB
 *     CSQLite_Local_DB -> CSQLite_Local_DB : RemoveFile()
 *     activate CSQLite_Local_DB #10BBFF
 *     deactivate CSQLite_Local_DB
 *     deactivate CSQLite_Local_DB
 * else
 *     CSQLite_Local_DB --> CSQLite_Local_DB : ReturnError()
 * end
 *
 * @enduml
 */
bool CSQLite_Local_DB::Delete_DB()
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        // Close database
        this->m_DataBase.close();

        // Remove created database file
        return QFile::remove(this->m_DatabaseName);
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return false;
    }
}

/*!
 * \fn QString CSQLite_Local_DB::Get_SiteName()
 *
 * \brief Récupère le nom du site actuel ou est installée la console de gestion des consignes automatiques,
 *        principalement utilisée pour identifier le site afin de récupérer la sauvegarde de la base de données correspondante au serveur LMS.
 *
 * \return Retourne un QString avec le nom du site ou est installée la console de gestion des consignes automatiques ou vide si la base de données n'est pas ouverte.
 */
QString CSQLite_Local_DB::Get_SiteName()
{
    QString SiteName;

    if(m_DataBaseIsReady == true)   // Database opened.
    {
        QSqlQuery query(this->m_DataBase);

        if(query.exec("SELECT idConsole FROM Console"))
        {
            while(query.next())
                SiteName = query.value(0).toString();
        }
        return SiteName;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        SiteName.clear();
    }
    return SiteName;
}

/*!
 * \fn bool CSQLite_Local_DB::Set_SiteName(QString SiteName)
 *
 * \brief Modifie le nom du site actuel ou est installée la console de gestion des consignes automatiques,
 *        principalement utilisé pour identifier le site affin de récupérer la sauvegarde de la base de données correspondante au serveur LMS.
 *
 * \param QString SiteName, Avec "SiteName", le nom du site souhaité.
 *
 * \return Retourne un booléen avec le succès de la commande (true) ou l'echec (false) en cas d'érreur il peut être utile d'appeler lastError() pour plus d'informations,
 *         ou faux (false) si la base de données n'est pas ouverte.
 */
bool CSQLite_Local_DB::Set_SiteName(QString SiteName)
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        QSqlQuery query(this->m_DataBase);
        bool Result = false;

        if(query.exec("UPDATE Console SET idConsole='"+ SiteName +"' WHERE rowid='1'"))
            Result = true;

        return Result;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return false;
    }
}

/*!
 * \fn int CSQLite_Local_DB::Get_NumberOfBoxes()
 *
 * \brief Récupère dans la base de données ou est installée la console de gestion des consignes automatiques,
 *        le nombre des consignes installées et configurées sur le site (Libre + occupées + défectueuses).
 *        Pour récupérer la liste des consignes libre lors de la livraison il est possible d'appeler GetFreeBoxes().
 *
 * \return Retourne un int, entier avec le nombre de consignes installées ou -1 si la base de données n'est pas ouverte.
 */
int CSQLite_Local_DB::Get_NumberOfBoxes()
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        QSqlQuery query(this->m_DataBase);
        int NumberOfBoxes = 0;

        if(query.exec("SELECT LinkedBoxes FROM Console"))
        {
            while(query.next())
                NumberOfBoxes = query.value(0).toInt();
        }

        return NumberOfBoxes;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return -1;
    }
}

/*!
 * \fn bool CSQLite_Local_DB::BoxIsDammaged(int idBox)
 *
 * \brief Vérifie grâce à la base de données si la consigne est signalée comme endommagée.
 *
 * \param int idBox, Avec "idBox", le Numéro de la consigne a vérifier.
 *
 * \return Retourne un booléen, à vrai (true) si la consigne demmandée est signalée comme endommagée, faux (false) sinon ou si la base de données n'est pas ouverte.
 */
bool CSQLite_Local_DB::BoxIsDammaged(int idBox)
{
    bool DammagedBox = false;

    if(m_DataBaseIsReady == true)   // Database opened.
    {
        QSqlQuery query(this->m_DataBase);

        if(query.exec("SELECT BoxIsDammaged FROM Box WHERE idBox='"+ QString::number(idBox,10) +"'"))
        {
            while(query.next())
                DammagedBox = query.value(0).toBool();
        }

        return DammagedBox;
    }
    else
        qDebug()<< "[FATAL ERROR] : No Database connection !";

    return DammagedBox;
}

/*!
 * \fn bool CSQLite_Local_DB::Set_Temporary_BoxUsed(int IdBox)
 *
 * \brief Place temporairement la consigne en état occupée lorsque qu'une livraison à été efectuée et que le serveur LMS est indisponible.
 *
 * \param int IdBox, Avec "IdBox" L'identifiant de la consigne a placer à l'état occupée.
 *
 * \return Retourne un booléen, à vrai (true) si l'écriture s'est effectuée, faux (false) sinon ou si la base de données n'est pas ouverte.
 */
bool CSQLite_Local_DB::Set_Temporary_BoxUsed(int IdBox)
{
    bool Result = false;

    if(m_DataBaseIsReady == true)   // Database opened.
    {
        QSqlQuery query(this->m_DataBase);

        if(query.exec("UPDATE Box SET BoxIsUsed='true' WHERE idBox='"+ QString::number(IdBox,10) +"'"))
            Result = true;
    }
    else
        qDebug()<< "[FATAL ERROR] : No Database connection !";

    return Result;
}

/*!
 * \fn bool CSQLite_Local_DB::Unset_Temporary_BoxUsed(int IdBox)
 *
 * \brief Replace la consigne à l'état libre lors de sa mise à jour temporaire de livraison sans serveur LMS.
 *
 * \param int IdBox, Avec "IdBox" L'identifiant de la consigne a placer à l'état libre.
 *
 * \return Retourne un booléen, à vrai (true) si l'écriture s'est effectuée, faux (false) sinon ou si la base de données n'est pas ouverte.
 */
bool CSQLite_Local_DB::Unset_Temporary_BoxUsed(int IdBox)
{

    bool Result = false;

    if(m_DataBaseIsReady == true)   // Database opened.
    {
        QSqlQuery query(this->m_DataBase);

        if(query.exec("UPDATE Box SET BoxIsUsed='false' WHERE idBox='"+ QString::number(IdBox,10) +"'"))
            Result = true;
    }
    else
        qDebug()<< "[FATAL ERROR] : No Database connection !";

    return Result;
}

/*!
 * \fn QString CSQLite_Local_DB::lastError()
 *
 * \brief Récupère la dèrenière érreur survenue suite à l'interaction sur la base de données.
 *
 * \return Retourne un QString, Avec le code d'erreur et sa description sui à été relevée.
 */
QString CSQLite_Local_DB::lastError()
{
    // If opening database has failed user can ask
    return m_DataBase.lastError().text();
}

/*!
 * \fn int CSQLite_Local_DB::Login(QString IndentifiactionCode)
 *
 * \brief Effectue différentes vérifications avec le code d'identification passé en paramètre sur la base de données,
 *        pour déterminer si le code est valide et quel est le type d'utilisateur enregistré (Livreur ou opérateur de maintenance).
 *        Le chiffrements des mots de passe confidentiels de la base de données sont réalisées avec l'algorithme "Sha1".
 *
 * \param QString IndentifiactionCode, Avec "IndentifiactionCode" le code d'identification à vérifier.
 *
 * \return Retourne un int, pour (Inclure "DataStructures.h"):\n
 *              -5 : Si la base de données n'est pas ouverte\n
 *              -4 (UNDEFINED_USER) : Utilisateur non reconnu (non valide).\n
 *              -3 (UNDEFINED_COMPANY_PASS) : Mauvais code de société (non valide).\n
 *              -2 (UNDEFINED_PERSONAL_PASS) : Mauvais mot de passe personnel (non valide).\n
 *              -1 (UNDEFINED_PASSWORD) : Mauvais mot de passe confidentiel (non valide).\n
 *              0  (LIVREUR) : Utilisateur autorisé est de type livreur (valide).\n
 *              1  (MAINTENANCE) : Utilisateur autorisé est de type opérateur de maintenance (valide).\n
 *
 * @startuml{CSQLite_Local_DB--Login.svg}
 *
 * autonumber
 * ConsoleWindows-> CSQLite_Local_DB  : Login()
 * activate CSQLite_Local_DB #0000FF
 *
 * alt Database is ready
 *      database LC_Database
 *      CSQLite_Local_DB  -> CSQLite_Local_DB : HashingPassword()
 *      CSQLite_Local_DB -> LC_Database : SQL_Query(SELECT UserType FROM User)
 *      activate LC_Database #00A5FF
 *      deactivate LC_Database
 *      
 *      alt UserCompanyPass, UserPersonalPass,\n UserPassWord, NOT found in database
 *          CSQLite_Local_DB  --> ConsoleWindows : (UNDEFINED_USER) -4
 *      else
 *
 *          alt Livreur (0) or Maintenance (1) user
 *          CSQLite_Local_DB  --> ConsoleWindows : UserType
 *
 *          else
 *
 *              alt UserCompanyPass is OK
 *              else
 *                   CSQLite_Local_DB -> LC_Database : SQL_Query(SELECT UserCompanyPass FROM User)
 *                   activate LC_Database #11A5FF
 *                   deactivate LC_Database
 *                   CSQLite_Local_DB  --> ConsoleWindows:  (UNDEFINED_COMPANY_PASS) -3
 *              end
 *
 *              alt UserPersonalPass is OK
 *              else
 *                   CSQLite_Local_DB -> LC_Database : SQL_Query(SELECT UserPersonalPass FROM User)
 *                   activate LC_Database #00CCFF
 *                   deactivate LC_Database
 *                   CSQLite_Local_DB  --> ConsoleWindows: (UNDEFINED_PERSONAL_PASS) -3
 *              end
 *
 *              alt UserPassWord is OK
 *              else
 *                   CSQLite_Local_DB -> LC_Database : SQL_Query(SELECT UserPassWord FROM User)
 *                   activate LC_Database #10DDFF
 *                   deactivate LC_Database
 *                   CSQLite_Local_DB  --> ConsoleWindows: (UNDEFINED_PASSWORD) -1
 *              end
 *          end
 *      end
 *
 * else
 *      CSQLite_Local_DB  --> ConsoleWindows : (No database connection) -5
 *      deactivate CSQLite_Local_DB
 * end
 *
 * @enduml
 */
int CSQLite_Local_DB::Login(QString IndentifiactionCode)
{
    int UserType = -4;

    if(m_DataBaseIsReady == true)   // Database opened.
    {
        QSqlQuery query(this->m_DataBase);

        IndentifiactionCode.resize(12);

        QString CompanyPass;
        CompanyPass.append(QString(IndentifiactionCode[0]));
        CompanyPass.append(QString(IndentifiactionCode[1]));
        CompanyPass.append(QString(IndentifiactionCode[2]));
        bool InvalidCompanyPass = false;

        QString PersonalPass;
        PersonalPass.append(QString(IndentifiactionCode[3]));
        PersonalPass.append(QString(IndentifiactionCode[4]));
        PersonalPass.append(QString(IndentifiactionCode[5]));
        bool InvalidPersonalPass = false;

        QString PassWord;
        PassWord.append(QString(IndentifiactionCode[6]));
        PassWord.append(QString(IndentifiactionCode[7]));
        PassWord.append(QString(IndentifiactionCode[8]));
        PassWord.append(QString(IndentifiactionCode[9]));
        PassWord.append(QString(IndentifiactionCode[10]));
        PassWord.append(QString(IndentifiactionCode[11]));
        bool InvalidPassWord = false;


        QString Hashed_Password = QString(QCryptographicHash::hash((PassWord.toLocal8Bit()),QCryptographicHash::Sha1).toHex()); //Hashed password

        //Research user type with : Company Pass,Personal Pass,PassWord and check which one is a mistake.
        SavePerfomAction("User","SELECT UserType");
        if(query.exec("SELECT UserType FROM User WHERE UserCompanyPass = '"+CompanyPass+"' AND UserPersonalPass = '"+PersonalPass+"' AND UserPassWord = '"+Hashed_Password+"';"))
        {
             if(query.next())   //get user type
                UserType = query.value(0).toInt();

             //identify which one are an error.
             SavePerfomAction("User","SELECT UserCompanyPass");
             if(query.exec("SELECT UserCompanyPass FROM User WHERE UserCompanyPass = '"+CompanyPass+"'"))
             {
                 if(!query.next())
                 {
                     UserType = UNDEFINED_COMPANY_PASS; //UserCompanyPass : Undefined
                     InvalidCompanyPass = true;
                 }
             }
             SavePerfomAction("User","SELECT UserPersonalPass");
             if(query.exec("SELECT UserPersonalPass FROM User WHERE UserPersonalPass = '"+PersonalPass+"'"))
             {
                 if(!query.next())
                 {
                     UserType = UNDEFINED_PERSONAL_PASS; //UserPersonalPass : Undefined
                     InvalidPersonalPass = true;
                 }
             }
             SavePerfomAction("User","SELECT UserPassWord");
             if(query.exec("SELECT UserPassWord FROM User WHERE UserPassWord = '"+Hashed_Password+"'"))
             {
                 if(!query.next())
                 {
                     UserType = UNDEFINED_PASSWORD; //UserPassWord : Undefined
                     InvalidPassWord = true;
                 }
             }

             if(InvalidCompanyPass && InvalidPersonalPass && InvalidPassWord)
                 UserType = UNDEFINED_USER; //User : Unknown
        }
        else
        {
            UserType = UNDEFINED_USER; //User : Unknown
        }
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        UserType = -5;
    }

    return UserType;
}

/*!
 * \fn bool CSQLite_Local_DB::SavePerfomAction(QString Table,QString Action)
 *
 * \brief Enregistre dans la base de données l'action suivante qui va être demandée, utilisée pour garder toutes les actions effectuée dans la base de données.
 *        Cette fonction doit être appelée avant un exec() d'un QSqlQuery par exemple.
 *
 * \param QString Table,QString Action, Avec "Table" pour le nom de la table qui va être modifiée ou intérrogée et "Action" pour la requête qui sera envoyée.
 *
 * \return Retourne un booléen, à vrai (true) si la commande est sauvegardée dans la base de données, faux (false) sinon ou si la base de données n'est pas ouverte.
 */
bool CSQLite_Local_DB::SavePerfomAction(QString Table,QString Action)
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        bool Result = false;
        QSqlQuery query(this->m_DataBase);
        QString TimeStamp;

        TimeStamp = QDateTime::currentDateTimeUtc().toString();
        if(query.exec("INSERT INTO PerformedActions (EditedTable,ActionMessage,Time) VALUES ('"+Table+"','"+Action+"','"+TimeStamp+"')"))
             Result = true;
        else
             Result = false;

        return Result;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return false;
    }
}

/*!
 * \fn int CSQLite_Local_DB::CheckExtractCode(QString ExtractCode)
 *
 * \brief Recherche dans la base de données si le code passé en paramètre est valide et peut être utilisé pour le retrait de colis.
 *
 * \param QString ExtractCode, Avec "ExtractCode" pour le code à vérifier.
 *
 * \return Retourne un int, entier ayant pour valeur :\n
 *              -1 : si la base de données n'est pas ouverte.\n
 *              0 : si le code de retrait est correct, mais il est expiré (14 jours par defaut).\n
 *              1 : si le code de retrait est valide et non expiré (Utilisable par le client).\n
 *
 * @startuml{CSQLite_Local_DB--CheckExtractCode.svg}
 *
 *  autonumber
 *  ConsoleWindows -> CSQLite_Local_DB : CheckExtractCode()
 *  activate CSQLite_Local_DB #AABBFF
 *
 *  alt Database is ready
 *
 *      database LC_Database
 *      CSQLite_Local_DB -> LC_Database: SQL_Query(SELECT idExtractCode)
 *      activate LC_Database #AADDFF
 *      deactivate LC_Database
 *
 *      alt SQL_Query success
 *
 *          CSQLite_Local_DB -> LC_Database: SQL_Query(SELECT ExtractCodeExpiryTime)
 *          activate LC_Database #AADDFF
 *          deactivate LC_Database
 *
 *          alt ExtractCodeExpiryTime > TimeStampNow
 *              CSQLite_Local_DB --> ConsoleWindows : 1 ExtractCode exist and is valid
 *          else
 *             CSQLite_Local_DB --> ConsoleWindows : ExtractCode is invalid
 *          end
 *
 *      else
 *          CSQLite_Local_DB --> ConsoleWindows : -1 no database connection
 *      end
 *  else
 *      CSQLite_Local_DB --> ConsoleWindows : -1 no database connection
 *      deactivate CSQLite_Local_DB
 *  end
 *
 * @enduml
 */
int CSQLite_Local_DB::CheckExtractCode(QString ExtractCode)
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        int Result = -1;
        QString TimeStampExtractCodeDB;
        QSqlQuery query(this->m_DataBase);

        SavePerfomAction("ExtractCode","SELECT idExtractCode");
        if(query.exec("SELECT idExtractCode FROM ExtractCode WHERE idExtractCode='"+ExtractCode+"'"))
        {
            while(query.next())
            {
                SavePerfomAction("ExtractCode","SELECT ExtractCodeExpiryTime");
                if(query.exec("SELECT ExtractCodeExpiryTime FROM ExtractCode WHERE idExtractCode='"+ExtractCode+"'"))
                {
                    while(query.next())
                    {
                        TimeStampExtractCodeDB = query.value(0).toString();

                        QDateTime TimeStampExtractCode;
                        QDateTime TimeStampNow;
                        TimeStampExtractCode = QDateTime::fromString(TimeStampExtractCodeDB);
                        TimeStampNow = TimeStampNow.currentDateTimeUtc();

                        if(TimeStampExtractCode > TimeStampNow)
                            Result = 1; //ExtractCode exist and is valid
                        else
                            return 0; //ExtractCode is invalid
                    }
                }
            }
        }

        return Result;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return -1;
    }
}

/*!
 * \fn bool CSQLite_Local_DB::DeleteExtractCode(QString UsedExtractCode)
 *
 * \brief Supprime dans la base de données le code de retrait passé en paramètre et tout les colis lui appartenant, enfin libère la ou les consignes utilisées par le code de retrait.
 *        Pour ouvrir les consignes consernées il faut appeler la fonction OpenDoors() de la classe CDoors avant de supprimer les codes de retrait,
 *        celle-ci ouvre la liste des consignes qui peut être récupérée avec GetBoxesListConsumer() qui retourne la liste des consignes utilisée par un cocde de retrait client.
 *
 * \param QString UsedExtractCode, Avec "UsedExtractCode" pour le code de retrait à supprimer et les consignes liées à liberer.
 *
 * \return Retourne un booléen, à vrai (true) si la suppression s'est effectuée, faux (false) sinon ou si la base de données n'est pas ouverte.
 *
 * @startuml{CSQLite_Local_DB--DeleteExtractCode.svg}
 *
 *  autonumber
 *  ConsoleWindows -> CSQLite_Local_DB : DeleteExtractCode()
 *  activate CSQLite_Local_DB #30DDFF
 *
 *  alt Database is ready
 *
 *      database LC_Database
 *
 *      CSQLite_Local_DB -> LC_Database: SQL_Query(SELECT idBox)
 *      activate LC_Database #30DDFF
 *      deactivate LC_Database
 *
 *      CSQLite_Local_DB -> LC_Database: SQL_Query(SELECT idPackage)
 *      activate LC_Database #30DDFF
 *      deactivate LC_Database
 *
 *      CSQLite_Local_DB -> LC_Database: SQL_Query(DELETE ExtractCode)
 *      activate LC_Database #30DDFF
 *      deactivate LC_Database
 *
 *      loop idPackage
 *           CSQLite_Local_DB -> LC_Database: SQL_Query(DELETE idPackage)
 *           activate LC_Database #30DDFF
 *           deactivate LC_Database
 *
 *           loop idBox
 *                CSQLite_Local_DB -> LC_Database: SQL_Query(UPDATE Box)
 *                activate LC_Database #30DDFF
 *                deactivate LC_Database
 *           end
 *      end
 *
 *      alt Delete success
 *          CSQLite_Local_DB -> ConsoleWindows : true deleted
 *      else
 *          CSQLite_Local_DB -> ConsoleWindows : false no delete
 *      end
 *
 *  else
 *      CSQLite_Local_DB -> ConsoleWindows : false no database connection
 *      deactivate CSQLite_Local_DB
 *  end
 *
 * @enduml
 */
bool CSQLite_Local_DB::DeleteExtractCode(QString UsedExtractCode)
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        bool Result = false;
        QSqlQuery query(this->m_DataBase);
        QList<int> IdBoxUsed;
        QStringList IdPackage;

        IdBoxUsed.append(-1);
        SavePerfomAction("Package","SELECT Linked_idBox");
        if(query.exec("SELECT Linked_idBox FROM Package WHERE Linked_idExtractCode='"+UsedExtractCode+"'"))
        {
            while(query.next())
                IdBoxUsed << query.value(0).toInt();
        }

        SavePerfomAction("Package","SELECT idPackage");
        if(query.exec("SELECT idPackage FROM Package WHERE Linked_idExtractCode='"+UsedExtractCode+"'"))
        {
            while(query.next())
                IdPackage << query.value(0).toString();
        }

        SavePerfomAction("ExtractCode","DELETE idExtractCode");
        if(query.exec("DELETE FROM ExtractCode WHERE idExtractCode='"+UsedExtractCode+"'"))
        {
            //Delete packages, set empty box with id
            for(int i=0; i<IdPackage.count(); i++)
            {
                SavePerfomAction("Package","DELETE idPackage");
                if(query.exec("DELETE FROM Package WHERE idPackage ='"+IdPackage[i]+"'"))
                {
                    for(int j=0; j<IdBoxUsed.count(); j++)
                    {
                        SavePerfomAction("Box","UPDATE BoxIsUsed");
                        if(query.exec("UPDATE Box SET BoxIsUsed='false' WHERE idBox='"+QString::number(IdBoxUsed[j],10)+"'"))
                        {
                            Result = true;
                        }
                    }
                }
            }
        }
        return Result;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return false;
    }
}

/*!
 * \fn QList<int> CSQLite_Local_DB::GetFreeBoxes()
 *
 * \brief Recherche dans la base de données les consignes qui ne sont pas endommagées et celles qui sont libres.
 *
 * \return Retourne une QList<int>, liste avec les identifiants des consignes libres sinon une liste vide si aucune consignes disponibles, ou si la base de données n'est pas ouverte.
 */
QList<int> CSQLite_Local_DB::GetFreeBoxes()
{
    QList<int> FreeBoxes;

    if(m_DataBaseIsReady == true)   // Database opened.
    {
        QSqlQuery query(this->m_DataBase);

        SavePerfomAction("Box","SELECT idBox");
        if(query.exec("SELECT idBox FROM Box WHERE BoxIsUsed='false' AND BoxIsDammaged='false'"))
        {
             while (query.next())
                 FreeBoxes << query.value(0).toInt();
        }

        return FreeBoxes;   // /!\ FreeBoxes can be an empty list !
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        FreeBoxes.clear();
    }
    return FreeBoxes;
}

/*!
 * \fn QList<int> CSQLite_Local_DB::GetBoxesListConsumer(QString ExtractCode)
 *
 * \brief Recherche dans la base de données la liste des consignes utilisées par le code de retrait d'un client.
 *
 * \param QString ExtractCode, Avec "ExtractCode" le code pour identifier les consignes utilisées par les colis qui lui sont liée.
 *
 * \return Retourne une QList<int>, liste avec les indentifiants des consignes utilisées pour le code de retrait sinon une liste vide,
 *         si le code de retrait n'est pas valide ou si la base de données n'est pas ouverte.
 *
 * @startuml{CSQLite_Local_DB--GetBoxesListConsumer.svg}
 *
 * autonumber
 * ConsoleWindows -> CSQLite_Local_DB : GetBoxesListConsumer()
 * activate CSQLite_Local_DB #00CCFF
 *
 *  alt Database is ready
 *
 *      database LC_Database
 *      CSQLite_Local_DB -> CSQLite_Local_DB : CheckExtractCode()
 *
 *      alt CheckExtractCode is valid = 1
 *
 *          CSQLite_Local_DB -> LC_Database : SQL_Query(SELECT idPackage)
 *          activate LC_Database #30DDFF
 *          deactivate LC_Database
 *
 *          alt idPackage is not an empty list
 *
 *              CSQLite_Local_DB -> LC_Database : SQL_Query(SELECT Linked_idBox)
 *              activate LC_Database #30DDFF
 *              deactivate LC_Database
 *
 *              alt Linked_idBox found
 *                  CSQLite_Local_DB -> ConsoleWindows : BoxesList
 *              else
 *                  CSQLite_Local_DB -> ConsoleWindows : empty list no Linked_idBox
 *              end
 *          else
 *              CSQLite_Local_DB -> ConsoleWindows : empty list no idPackage
 *          end
 *
 *      else
 *          CSQLite_Local_DB -> ConsoleWindows : empty list CheckExtractCode invalid
 *      end
 *
 *  else
 *      CSQLite_Local_DB -> ConsoleWindows : empty list no database connection
 *      deactivate CSQLite_Local_DB
 *  end
 *
 * @enduml
 */
QList<int> CSQLite_Local_DB::GetBoxesListConsumer(QString ExtractCode)
{
    QList<int> BoxesList;

    if(m_DataBaseIsReady == true)   // Database opened.
    {
        QStringList idPackagesList;
        QSqlQuery query(this->m_DataBase);

        if(CheckExtractCode(ExtractCode) == 1)
        {
            SavePerfomAction("ExtractCode","SELECT Linked_idPackage");
            if(query.exec("SELECT Linked_idPackage FROM ExtractCode WHERE idExtractCode='"+ExtractCode+"'"))
            {
                 while (query.next())
                     idPackagesList << query.value(0).toString();

                 if(!idPackagesList.isEmpty())
                 {
                     for(int i=0; i<idPackagesList.count(); i++)
                     {
                         SavePerfomAction("Package","SELECT Linked_idBox");
                         if(query.exec("SELECT Linked_idBox FROM Package WHERE idPackage='"+idPackagesList[i]+"'"))
                         {
                             if(query.next())
                                 BoxesList << query.value(0).toInt();
                         }
                     }
                 }
            }
        }

        return BoxesList;   // /!\ BoxesList can be an empty list !
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        BoxesList.clear();
    }
    return BoxesList;
}

/*!
 * \fn QList<struct struct_DoorGeom> CSQLite_Local_DB::GetBoxesList()
 *
 * \brief Récupère dans le base de données toutes les portes installées et indique l'état des portes, utilisée pour dessiner le plan de l'installation des consignes lors de la livraison.
 *
 * \return Retourne une QList<struct_DoorsStatus>, liste de structutre struct_DoorsStatus contenant toutes les informations sur l'état mesuré de la porte,
 *         (Numéro, Ouverture, fermeture, verouillage...) ou une liste vide si la base de données n'est pas ouverte.
 */
QList<struct struct_DoorGeom> CSQLite_Local_DB::GetBoxesList()
{
    QList<struct struct_DoorGeom> BoxesList;

    if(m_DataBaseIsReady == true)   // Database opened.
    {
        QSqlQuery query(this->m_DataBase);

        SavePerfomAction("Box","SELECT *");
        if(query.exec("SELECT * FROM Box "))
        {
            bool BoxIsUsed;
            bool BoxIsDammaged;

            while (query.next())
            {
                struct_DoorGeom geom;

                geom.BoxNumber = query.value(0).toInt();
                geom.BoxSize = query.value(1).toString();

                BoxIsUsed = query.value(2).toBool();
                BoxIsDammaged = query.value(3).toBool();
                if(BoxIsUsed || BoxIsDammaged)
                    geom.status = KO;
                else
                    geom.status = OK;

                geom.x = query.value(6).toInt();
                geom.y = query.value(7).toInt();

                BoxesList << geom;
            }
        }
        return BoxesList;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        BoxesList.clear();
    }
    return BoxesList;
}

/*!
 * \fn QList<int> CSQLite_Local_DB::Get_All_idBox()
 *
 * \brief Récupère dans le base de données les identifiants de toutes les portes installées.
 *
 * \return Retourne une QList<int>, liste avec les identifiants des consignes.
 */
QList<int> CSQLite_Local_DB::Get_All_idBox()
{
    QList<int> DoorsList;

    if(m_DataBaseIsReady == true)   // Database opened.
    {
        QSqlQuery query(this->m_DataBase);
        if(query.exec("SELECT idBox From Box"))
        {
            while (query.next())
                DoorsList.append(query.value(0).toInt());
        }
    }
    else
    {
        DoorsList.clear();
        qDebug()<< "[FATAL ERROR] : No Database connection !";
    }

    return DoorsList;
}

/*!
 * \fn bool CSQLite_Local_DB::UpdateLocalData(QList<struct struct_PackagesUseBox> UpdatedPackagesUseBoxList)
 *
 * \brief Met à jour la base de données locale avec les informations envoyées au serveur LMS lors de la livraison.
 *
 * \param QList<struct struct_PackagesUseBox> UpdatedPackagesUseBoxList, Avec "UpdatedPackagesUseBoxList" la liste des colis utilisant les consignes et les codes de retrait associées.
 *
 * \return Retourne un booléen, à vrai (true) si la mise à jour s'est effectuée, faux (false) sinon ou si la base de données n'est pas ouverte.
 *
 * @startuml{CSQLite_Local_DB--UpdateLocalData.svg}
 *
 * autonumber
 * ConsoleWindows -> CSQLite_Local_DB : UpdateLocalData()
 * activate CSQLite_Local_DB #00CCFF
 *
 *  alt Database is ready
 *
 *      database LC_Database
 *      loop data list
 *
 *          loop packages list
 *              CSQLite_Local_DB -> LC_Database : SQL_Query(INSERT ExtractCode)
 *              activate LC_Database #00CCFF
 *              deactivate LC_Database
 *          end
 *
 *          loop extracts codes list
 *              CSQLite_Local_DB -> LC_Database : SQL_Query(INSERT Package)
 *              activate LC_Database #00A5FF
 *              deactivate LC_Database
 *          end
 *
 *          CSQLite_Local_DB -> LC_Database : SQL_Query(UPDATE Box)
 *          activate LC_Database #00A5FF
 *          deactivate LC_Database
 *
 *          CSQLite_Local_DB -> LC_Database : SQL_Query(UPDATE Doorlocker)
 *          activate LC_Database #00A5FF
 *          deactivate LC_Database
 *      end
 *
 *      alt Update success
 *          CSQLite_Local_DB --> ConsoleWindows : true database successful updated
 *      else
 *          CSQLite_Local_DB --> ConsoleWindows : false no database update
 *      end
 *
 *  else
 *      CSQLite_Local_DB --> ConsoleWindows : false no database connection
 *      deactivate CSQLite_Local_DB
 *  end
 *
 * @enduml
 */
bool CSQLite_Local_DB::UpdateLocalData(QList<struct struct_PackagesUseBox> UpdatedPackagesUseBoxList)
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        bool Result = false;
        QSqlQuery query(this->m_DataBase);
		
		if(!UpdatedPackagesUseBoxList.isEmpty())
	    {
			for(int i=0; i<UpdatedPackagesUseBoxList.count(); i++)
			{
				if(UpdatedPackagesUseBoxList[i].BoxNumber > Get_NumberOfBoxes())
					return false;

				//New ExtractCode
				for(int j=0; j<UpdatedPackagesUseBoxList[i].BoxContainment.PackageDeliveryCodeList.count(); j++)
				{
					SavePerfomAction("ExtractCode","INSERT INTO");
					query.prepare("INSERT INTO ExtractCode (idExtractCode, Linked_idPackage, ExtractCodeExpiryTime)"
								  "VALUES(:idExtractCode,:Linked_idPackage,:ExtractCodeExpiryTime)");

					query.bindValue(":idExtractCode", UpdatedPackagesUseBoxList[i].BoxContainment.PackagesExtractCode);
					query.bindValue(":Linked_idPackage", UpdatedPackagesUseBoxList[i].BoxContainment.PackageDeliveryCodeList[j]);
					query.bindValue(":ExtractCodeExpiryTime", UpdatedPackagesUseBoxList[i].BoxContainment.ExtractCodeExpiryTime);
					query.exec();
				}

				//New Package
				for(int j=0; j<UpdatedPackagesUseBoxList[i].BoxContainment.PackageDeliveryCodeList.count(); j++)
				{
					SavePerfomAction("Package","INSERT INTO");
					query.prepare("INSERT INTO Package (idPackage, Linked_idExtractCode, Linked_idBox) "
								  "VALUES(:idPackage,:Linked_idExtractCode,:Linked_idBox)");

					query.bindValue(":idPackage", UpdatedPackagesUseBoxList[i].BoxContainment.PackageDeliveryCodeList[j]);
					query.bindValue(":Linked_idExtractCode", UpdatedPackagesUseBoxList[i].BoxContainment.PackagesExtractCode);
					query.bindValue(":Linked_idBox", UpdatedPackagesUseBoxList[i].BoxNumber);
					query.exec();
				}

				//Box update
				SavePerfomAction("Box","UPDATE BoxIsUsed");
				query.exec("UPDATE Box SET BoxIsUsed='true' WHERE idBox='"+QString::number(UpdatedPackagesUseBoxList[i].BoxNumber,10)+"'");
				SavePerfomAction("Doorlocker","UPDATE DoorLocker_LockState");
				query.exec("UPDATE Doorlocker SET DoorLocker_LockState='true' WHERE linked_idDoor='"+QString::number(UpdatedPackagesUseBoxList[i].BoxNumber,10)+"'");

				Result = true;
			}
		}
		else
			Result = false;

        return Result;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return false;
    }
}

/*!
 * \fn QList<QString> CSQLite_Local_DB::ReadSQL_File(QString FilePath)
 *
 * \brief Lit le contenu d'un fichier SQL, utilisée pour lire les fichiers de constructions ou de sauvegardes de la base de données par exemple.
 *
 * \param QString FilePath, Avec "FilePath" le chemain du fichier à lire.
 *
 * \return Retourne une QList<QString>, liste des requêtes SQL lues dans le fichier ou une liste vide si le fichier n'exsite pas ou si il est vide.
 */
QList<QString> CSQLite_Local_DB::ReadSQL_File(QString FilePath)
{
    QList<QString> QueryList;
    QString temp;
    QString RdyQuery;
    QFile SQL_File(FilePath);
    QTextStream SQL_FileStream(&SQL_File);
    temp = " ";

    if(SQL_File.open(QIODevice::ReadOnly))
    {
         while(!SQL_FileStream.atEnd())
         {
             while(!temp.contains(';'))
             {
                 temp = SQL_FileStream.readLine();
                 RdyQuery += temp;
             }
             QueryList.append(RdyQuery);
             RdyQuery.clear();
             temp = " ";
         }
         SQL_File.close();
    }
    else
        QueryList.clear();

    return QueryList;
}

/*!
 * \fn bool CSQLite_Local_DB::WriteSQL_File(QString FilePath, QString Data)
 *
 * \brief Ecrit un fichier SQL en mode "append", soit a la fin d'un fichier deja existant.
 *
 * \param QString FilePath, QString Data, Avec "FilePath" le chemain du fichier ou écrire les "Data", données au format SQL.
 *
 * \return Retourne un booléen, à vrai (true) si l'écriture s'est effectuée, faux (false) sinon.
 */
bool CSQLite_Local_DB::WriteSQL_File(QString FilePath, QString Data)
{
    bool Result = false;
    QFile SQL_File(FilePath);
    QTextStream Stream(&SQL_File);

    if(SQL_File.open(QIODevice::Append))
    {
        Stream << Data;
        Stream.flush();
        SQL_File.close();
        Result = true;
    }

    return Result;
}

/*!
 * \fn bool CSQLite_Local_DB::SQL_Database_Manager(int Option)
 *
 * \brief Fonction de traitement de la base de données, Crée une nouvelle base de données vide, par defaut, une sauvegarde, et charge une sauvegarde.
 *        La fonction evoie des signaux CurrentProcessLoading() et SlowProcess() pour l'information de l'avencée du traitement.
 *
 * \param int Option, Avec "Option" Le traitement choisis comme (inclure le fichier "DataStructures.h"):
 *                  (EMPTY_DATABASE)     = 1 : Construit une base de données avec les tables de l'application LC mais vide.
 *                  (DEFAULT_DATABASE)   = 2 : Construit une base de données avec les tables de l'application LC mais remplient avec les données par defaut de test.
 *                  (SAVE_DATABASE)      = 3 : Effectue une sauvegarde de la base de données actuel dans un fichier au format SQL.
 *                  (LOAD_SAVE_DATABASE) = 4 : Charge la dernière sauvegarde de la base de données effectuée.
 *
 * \return  Retourne un booléen, à vrai (true) si le traitement s'est effectuée, faux (false) sinon ou si la base de données n'est pas ouverte.
 */
bool CSQLite_Local_DB::SQL_Database_Manager(int Option)
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        bool Result = false;
        QSqlQuery query(this->m_DataBase);
        QList<QString> QueryList;

        switch(Option)
        {
            case EMPTY_DATABASE:
            {
                emit CurrentProcessLoading("Lancement du système de creation de base de données (empty) ...",12);
                emit SlowProcess(2000);
                QueryList = ReadSQL_File(this->m_SQL_File_DatabaseEmpty_Path);

                for(int i=0; i<QueryList.count(); i++)
                    query.exec(QueryList[i]);

                emit CurrentProcessLoading("Fin de creation de base de données (empty).",100);
                emit SlowProcess(500);
                Result = true;
                emit CurrentProcessLoading("END",101);
                emit SlowProcess(5000);
                emit CurrentProcessLoading("",0);
                break;
            }
            case DEFAULT_DATABASE:
            {
                emit CurrentProcessLoading("Lancement du système de creation de base de données (Default) ...",12);
                emit SlowProcess(2000);

                QueryList = ReadSQL_File(this->m_SQL_File_DatabaseEmpty_Path);

                for(int i=0; i<QueryList.count(); i++)
                    query.exec(QueryList[i]);

                query.clear();
                QueryList.clear();

                QueryList = ReadSQL_File(this->m_SQL_File_DatabaseDefault_Path);

                for(int i=0; i<QueryList.count(); i++)
                    query.exec(QueryList[i]);

                emit CurrentProcessLoading("Fin de creation de base de données (Default).",100);
                emit SlowProcess(500);
                Result = true;
                emit CurrentProcessLoading("END",101);
                emit SlowProcess(5000);
                emit CurrentProcessLoading("",0);
                break;
            }
            case SAVE_DATABASE:
            {
                if(m_LMS_Server_IsReady == true)
                {
                        if(m_LMS_Server->ManualSave_DatabaseLMS(Get_SiteName(),m_SQL_File_DatabaseLastSave_Path) == false)
                        {
                            emit CurrentProcessLoading("Lancement du système de sauvegarde ...",12);
                            emit SlowProcess(2000);
                            if(SaveDatabase(this->m_SQL_File_DatabaseLastSave_Path))
                                Result = true;
                            emit CurrentProcessLoading("Fin du système de sauvegarde ...",100);
                            emit SlowProcess(2000);
                            emit CurrentProcessLoading("END",101);
                        }
                        else
                            Result = false;
                }
                else
                	Result = false;

                break;
            }
            case LOAD_SAVE_DATABASE:
            {
                if(m_LMS_Server_IsReady == true)
                {
                    QFile SQL_File(m_SQL_File_DatabaseLastSave_Path_FromLMS);

                    QueryList = m_LMS_Server->Get_LastDatabase_Save(Get_SiteName());
                    if(QueryList.isEmpty())
                    {
                        emit CurrentProcessLoading("Lancement du système de chargement de sauvegardes ...",12);
                        emit SlowProcess(2000);
                        if(LoadDatabaseSave(this->m_SQL_File_DatabaseLastSave_Path))
                            Result = true;
                        emit CurrentProcessLoading("Chargement de la sauvegarde terminée",100);
                        emit SlowProcess(2000);
                        emit CurrentProcessLoading("END",101);
                    }
                    else  //Serveur LMS online => Read database save from LMS.
                    {
                        if(SQL_File.open(QIODevice::WriteOnly)) //delete file
                            qDebug() << "[WARNING] : The last database save From LMS Server : " << m_SQL_File_DatabaseLastSave_Path_FromLMS << " is DELETED !";
                        SQL_File.close();

                        for(int i=0; i<QueryList.count(); i++)
                            WriteSQL_File(m_SQL_File_DatabaseLastSave_Path_FromLMS,QueryList[i]);

                        if(LoadDatabaseSave(m_SQL_File_DatabaseLastSave_Path_FromLMS))
                            Result = true;
                    }
                }
                else
                    Result = false;

                break;
            }
            default:
            {
                Result = false;
                break;
            }
        }

        return Result;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return false;
    }
}

/*!
 * \fn bool CSQLite_Local_DB::SaveDatabase(QString FilePath)
 *
 * \brief Supprime l'ancien fichier et effectue une sauvegarde de la base de données actuel dans un fichier (chemain dans m_SQL_File_DatabaseLastSave_Path),
 *        au format SQL qui est stocké en local.
 *        Utilisée en cas de rupture de communication vers le serveur LMS.
 *        La fonction evoie des signaux CurrentProcessLoading() et SlowProcess() pour l'information de l'avencée du traitement.
 *
 * \param QString FilePath, Avec "FilePath" le chemain du fichier de sauvegarde (m_SQL_File_DatabaseLastSave_Path).
 *
 * \return  Retourne un booléen, à vrai (true) si le traitement s'est effectuée, faux (false) sinon ou si la base de données n'est pas ouverte.
 */
bool CSQLite_Local_DB::SaveDatabase(QString FilePath)
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        bool Result = false;
        QStringList TablesList;
        QList<QString> EmptyDatabaseQueryList;
        QList<struct struct_Database_Contains> Database_Contains;
        struct struct_Database_Contains Database_Contains_Temp;
        QSqlQuery query(this->m_DataBase);
        QFile SQL_File(FilePath);

        TablesList = this->m_DataBase.tables();
        if(TablesList.isEmpty())
        {
            TablesList.append("Box");
            TablesList.append("Console");
            TablesList.append("DCB_Controler");
            TablesList.append("DoorLocker");
            TablesList.append("ExtractCode");
            TablesList.append("Package");
            TablesList.append("PerformedActions");
            TablesList.append("User");
        }

        emit CurrentProcessLoading("Surppression du fichier de sauvegarde ...",15);
        emit SlowProcess(1000);

        if(SQL_File.open(QIODevice::WriteOnly)) //delete file
            SQL_File.close();
        else
            Result = false;

        EmptyDatabaseQueryList = ReadSQL_File(m_SQL_File_DatabaseEmpty_Path);
        for(int i=0; i<EmptyDatabaseQueryList.count(); i++)
            WriteSQL_File(FilePath,EmptyDatabaseQueryList[i]+"\n");

        for(int Table=0; Table<TablesList.count(); Table++)
        {
            emit CurrentProcessLoading("Création du fichier de sauvegarde ...",10+((Table+1)*10));
            emit SlowProcess(1000);

            Database_Contains_Temp.Table = TablesList[Table];
            if(query.exec("SELECT * FROM " + TablesList[Table]))
            {
                while(query.next())
                {
                    QString QueryValue;

                    for(int Col=0; Col<=7; Col++)
                    {
                        if(query.value(Col).toString() != "")
                        {
                            QueryValue += "'"+query.value(Col).toString()+"'" + ",";
                            Database_Contains_Temp.Values.append(QueryValue);
                        }
                    }
                    QueryValue.replace(QueryValue.size()-1,sizeof(QChar)," ");
                    WriteSQL_File(FilePath,"INSERT INTO "+TablesList[Table]+" VALUES("+QueryValue+");\n");
                    QueryValue.clear();
                }
            }

            Database_Contains.append(Database_Contains_Temp);
            Database_Contains_Temp.Values.clear();
            Result = true;
        }
        emit CurrentProcessLoading("Fichier de sauvegarde terminé ...",100);
        emit SlowProcess(1000);
        emit CurrentProcessLoading("END",101);
        return Result;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return false;
    }
}

/*!
 * \fn bool CSQLite_Local_DB::LoadDatabaseSave(QString FilePath)
 *
 * \brief Coupe la connexion avec la base de données et en crée une nouvelle avec les informations contenue dans le fichier de sauvegarde,
 *        La fonction evoie des signaux CurrentProcessLoading() et SlowProcess() pour l'information de l'avencée du traitement.
 *
 * \param QString FilePath, Avec "FilePath" le chemain du fichier de sauvegarde (m_SQL_File_DatabaseLastSave_Path ou m_SQL_File_DatabaseLastSave_Path_FromLMS).
 *
 * \return  Retourne un booléen, à vrai (true) si le traitement s'est effectuée, faux (false) sinon ou si la base de données n'est pas ouverte.
 */
bool CSQLite_Local_DB::LoadDatabaseSave(QString FilePath)
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        bool Result;
        QFile SQL_File(FilePath);
        QList<QString> QueryList;

        emit CurrentProcessLoading("Fermeture de la base de données ...",20);
        emit SlowProcess(500);

        this->m_DataBaseIsReady = false;
        this->m_DataBase.close();

        if(Delete_DB()) //delete database
        {
            emit CurrentProcessLoading("Connexion sur la nouvelle de la base de données ...",40);
            emit SlowProcess(500);
            if(Connect_DB())
            {
                this->m_DataBaseIsReady = true;  // Open database : Open

                QSqlQuery query(this->m_DataBase);

                if(SQL_File.exists())
                {
                    emit CurrentProcessLoading("Ecriture des valeurs sauvegardées ...",70);
                    Result = true;
                    QueryList = ReadSQL_File(FilePath);
                    for(int i=0; i<QueryList.count(); i++)
                    {
                        query.exec(QueryList[i]);
                    }
                    emit CurrentProcessLoading("Fin du chargement de la sauvegarde.",100);
                    emit SlowProcess(500);
                }
                else
                    Result = false;
            }
            else
            {
                this->m_DataBaseIsReady = false; // Open database : Fail
                Result = false;
            }
        }
        else
            Result = false;

        return Result;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return false;
    }
}

/*!
 * \fn bool CSQLite_Local_DB::SendQueryDoorsIsClosed(QString Query)
 *
 * \brief Fonction utilisée uniquement dans la phase de developpement pour simuler l'accès de classe CDoors au portes des consignes automatiques.
 *
 * \param QString Query, Avec "Query" la requête au format SQL pour "SELECT * FROM DoorLocker WHERE DoorLocker_OpenState='true' AND Linked_idDoor='BoxNumber'"
 *
 * \return  Retourne un booléen, à vrai (true) si le traitement s'est effectuée, faux (false) sinon ou si la base de données n'est pas ouverte.
 */
bool CSQLite_Local_DB::SendQueryDoorsIsClosed(QString Query)
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        QSqlQuery query(this->m_DataBase);
        bool Result = false;

        SavePerfomAction("[WARNING] EXTERN CALL (DoorsIsClosed)",Query);
        if(query.exec(Query))
        {
            while (query.next())
            {
                Result = query.value(3).toBool();
                if(Result == false)
                    return Result;
            }
        }

        return Result;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return false;
    }
}

/*!
 * \fn bool CSQLite_Local_DB::SetDammagedBoxes(QList<int> DammagedBoxes)
 *
 * \brief Met à jour la base de données avec les portes/consignes signalées comme défectueses.
 *
 * \param QList< int > DammagedBoxes, Avec "DammagedBoxes" la liste d'indentifants des consignes endomagées.
 *
 * \return  Retourne un booléen, à vrai (true) si le traitement s'est effectuée, faux (false) sinon ou si la base de données n'est pas ouverte.
 */
bool CSQLite_Local_DB::SetDammagedBoxes(QList<int> DammagedBoxes)
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        bool Result = false;
        QSqlQuery query(this->m_DataBase);

        for(int i=0; i<DammagedBoxes.count(); i++)
        {
            if(query.exec("UPDATE Box SET BoxIsDammaged='true' WHERE idBox='"+QString::number(DammagedBoxes[i],10)+"';"))
                Result = true;
            else
                return false;
        }

        return Result;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return false;
    }
}

/*!
 * \fn bool CSQLite_Local_DB::SetDammagedBox(int DammagedBox)
 *
 * \brief Met à jour la base de données avec l'identifiant de la porte/consigne signalée comme défectuese.
 *
 * \param int DammagedBox, Avec "DammagedBox" l'identifiant de la porte/consigne endomagée.
 *
 * \return  Retourne un booléen, à vrai (true) si le traitement s'est effectuée, faux (false) sinon ou si la base de données n'est pas ouverte.
 */
bool CSQLite_Local_DB::SetDammagedBox(int DammagedBox)
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        bool Result = false;
        QSqlQuery query(this->m_DataBase);

        if(query.exec("UPDATE Box SET BoxIsDammaged='true' WHERE idBox='"+QString::number(DammagedBox,10)+"';"))
            Result = true;
        else
            return false;
        return Result;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return false;
    }
}

/*!
 * \fn bool CSQLite_Local_DB::UnsetDammagedBoxes(QList<int> FixedBoxes)
 *
 * \brief Met à jour la base de données avec la liste des portes/consignes réparées.
 *
 * \param QList<int> FixedBoxes, Avec "FixedBoxes" l'identifiant des portes/consignes réparées.
 *
 * \return  Retourne un booléen, à vrai (true) si le traitement s'est effectuée, faux (false) sinon ou si la base de données n'est pas ouverte.
 */
bool CSQLite_Local_DB::UnsetDammagedBoxes(QList<int> FixedBoxes)
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        bool Result = false;
        QSqlQuery query(this->m_DataBase);

        for(int i=0; i<FixedBoxes.count(); i++)
        {
            if(query.exec("UPDATE Box SET BoxIsDammaged='false' WHERE idBox='"+QString::number(FixedBoxes[i],10)+"'"))
                Result = true;
            else
                return false;
        }

        return Result;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return false;
    }
}

/*!
 * \fn bool CSQLite_Local_DB::UnsetDammagedBox(int FixedBox)
 *
 * \brief Met à jour la base de données avec l'identifant de la porte/consigne réparée.
 *
 * \param int FixedBox, Avec "FixedBox" l'identifiant de la porte/consigne réparée.
 *
 * \return  Retourne un booléen, à vrai (true) si le traitement s'est effectuée, faux (false) sinon ou si la base de données n'est pas ouverte.
 */
bool CSQLite_Local_DB::UnsetDammagedBox(int FixedBox)
{
    if(m_DataBaseIsReady == true)   // Database opened.
    {
        bool Result = false;
        QSqlQuery query(this->m_DataBase);

        if(query.exec("UPDATE Box SET BoxIsDammaged='false' WHERE idBox='"+QString::number(FixedBox,10)+"'"))
            Result = true;
        else
            return false;
        return Result;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        return false;
    }
}

/*!
 * \fn QList<int> CSQLite_Local_DB::GetDammagedBoxes()
 *
 * \brief Récupère dans la base de données la liste des portes/consignes signalées comme endomagées.
 *
 * \return  Retourne la liste des des portes/consignes signalées comme endomagées sinon
 *          une liste vide si il y a aucunes portes/consignes signalées comme endomagées ou si la base de données n'est pas ouverte.
 */
QList<int> CSQLite_Local_DB::GetDammagedBoxes()
{
    QList<int> DammagedBoxes;

    if(m_DataBaseIsReady == true)   // Database opened.
    {
        int IdBoxTemp = 0;
        QSqlQuery query(this->m_DataBase);

        if(query.exec("SELECT idBox FROM Box WHERE BoxIsDammaged='true'"))
        {
            while (query.next())
            {
                IdBoxTemp = query.value(0).toInt();
                DammagedBoxes.append(IdBoxTemp);
            }
        }

        return DammagedBoxes;
    }
    else
    {
        qDebug()<< "[FATAL ERROR] : No Database connection !";
        DammagedBoxes.clear();
    }

    return DammagedBoxes;
}

/*!
 * \fn bool CSQLite_Local_DB::DataBase_AutoSave_Needed()
 *
 * \brief Fonction apellée automatiquement pas le systeme de mise à jour de la base de données, la sauvegarde est générée et envoyée au serveur LMS.
 *        Si le serveur LMS n'est pas disponible, la fonction créer une sauvegarde locale qui sera envoyée au prochain appel.
 *
 * \return  Retourne un booléen, à vrai (true) si le traitement s'est effectuée, faux (false) sinon ou si la base de données n'est pas ouverte.
 */
bool CSQLite_Local_DB::DataBase_AutoSave_Needed()
{
    bool Result = false;

    if(m_DataBaseIsReady == true)   // Database opened.
    {
        Result = m_LMS_Server->AutoSave_DatabaseLMS(Get_SiteName(),m_SQL_File_DatabaseLastSave_Path);

        if(Result == false) //LMS offline local database save
        {
            QStringList TablesList;
            QList<QString> EmptyDatabaseQueryList;
            QList<struct struct_Database_Contains> Database_Contains;
            struct struct_Database_Contains Database_Contains_Temp;
            QSqlQuery query(this->m_DataBase);
            QFile SQL_File(m_SQL_File_DatabaseLastSave_Path);

            TablesList = this->m_DataBase.tables();
            if(TablesList.isEmpty())
            {
                TablesList.append("Box");
                TablesList.append("Console");
                TablesList.append("DCB_Controler");
                TablesList.append("DoorLocker");
                TablesList.append("ExtractCode");
                TablesList.append("Package");
                TablesList.append("PerformedActions");
                TablesList.append("User");
            }

            if(SQL_File.open(QIODevice::WriteOnly)) //delete file
            {
                SQL_File.close();

                EmptyDatabaseQueryList = ReadSQL_File(m_SQL_File_DatabaseEmpty_Path);
                for(int i=0; i<EmptyDatabaseQueryList.count(); i++)
                    WriteSQL_File(m_SQL_File_DatabaseLastSave_Path,EmptyDatabaseQueryList[i]+"\n");

                for(int Table=0; Table<TablesList.count(); Table++)
                {
                    Database_Contains_Temp.Table = TablesList[Table];
                    if(query.exec("SELECT * FROM " + TablesList[Table]))
                    {
                        while(query.next())
                        {
                            QString QueryValue;

                            for(int Col=0; Col<=MAX_SQL_COLUMN_READ; Col++)
                            {
                                if(query.value(Col).toString() != "")
                                {
                                    QueryValue += "'"+query.value(Col).toString()+"'" + ",";
                                    Database_Contains_Temp.Values.append(QueryValue);
                                }
                            }
                            QueryValue.replace(QueryValue.size()-1,sizeof(QChar)," ");
                            WriteSQL_File(m_SQL_File_DatabaseLastSave_Path,"INSERT INTO "+TablesList[Table]+" VALUES("+QueryValue+");\n");
                            QueryValue.clear();
                        }
                    }

                    Database_Contains.append(Database_Contains_Temp);
                    Database_Contains_Temp.Values.clear();
                 }
            }
        }
        else
        {
            qDebug() << "[SUCCESS] : Last database save sended to the LMS Server !";
            Result = true;
        }
    }
    else
        qDebug()<< "[FATAL ERROR] : No Database connection !";

    return Result;
}
