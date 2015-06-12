#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H
/*!
 * \file DataStructures.h
 *
 * \brief Definnitions des formats de communications des données entre les classes de l'application LC
 *
 * \author HOUDAYER Pierre
 *
 * \version 1.3
 *
 * \date 12 mai 2015
 */

#include<QString>
#include<QList>

//<<<<<<<<<<<<<<<<<<<<<==================== DEBUG MODE =====================>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//#define DEBUG  /*!< Si la macro "DEBUG" est définie, le progamme crée une base de données de test au lancement, sinon la base de données existante sera ouverte sans modifications. */
//<<<<<<<<<<<<<<<<<<<<<=====================================================>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// RebuildDatabase options
#define EMPTY_DATABASE      1   /*!< Construit une base de données avec les tables de l'application LC mais vide. */
#define DEFAULT_DATABASE    2   /*!< Construit une base de données avec les tables de l'application LC mais remplient avec les données par defaut de test. */
#define SAVE_DATABASE       3   /*!< Effectue une sauvegarde de la base de données actuel dans un fichier au format SQL. */
#define LOAD_SAVE_DATABASE  4   /*!< Charge la dernière sauvegarde de la base de données effectuée. */

// Logging errors
#define UNDEFINED_USER          -4  /*!< L'utilisateur est inconnu dans la base de données locale (tout les codes sont faux). */
#define UNDEFINED_COMPANY_PASS  -3  /*!< Le code de la société n'est pas enregistré dans la base de données locale. */
#define UNDEFINED_PERSONAL_PASS -2  /*!< Le code de personnel n'est pas enregistré dans la base de données locale. */
#define UNDEFINED_PASSWORD      -1  /*!< Le mot de passe confidentiel n'est pas enregistré dans la base de données locale. */
#define LIVREUR                  0  /*!< Tout les mots de passe sont correct, l'utilisateur est un Livreur. */
#define MAINTENANCE              1  /*!< Tout les mots de passe sont correct, l'utilisateur est un opérateur de maintenance. */

// struct_DoorGeom
#define OK        0 /*!< Charge la dernière sauvegarde de la base de données effectuée. */
#define KO        1 /*!< Charge la dernière sauvegarde de la base de données effectuée. */
#define SELECTED  2 /*!< Charge la dernière sauvegarde de la base de données effectuée. */
#define NOCHECKED 3 /*!< Charge la dernière sauvegarde de la base de données effectuée. */

/*!
 * \struct struct_PackagesUseBox DataStructures.h
 *
 * \brief Structure de donnée de l'application LC, utilisée pour l'enregistrement des livraisons avec le contenu des consignes.
 */
struct struct_PackagesUseBox
{
    int BoxNumber;                              /*!< Identifiant  de la consigne */

    /*!
     * \struct struct_BoxContainment DataStructures.h
     *
     * \brief Structure de donnée de l'application LC, inclus dans une "struct_PackagesUseBox" utiliser pour renseigner le contenu des consignes.
     */
    struct struct_BoxContainment
    {
        QString PackagesExtractCode;            /*!< Code de retrtrait du colis */
        QString ExtractCodeExpiryTime;          /*!< Date d'expiration du code de retrait*/
        QList<QString> PackageDeliveryCodeList; /*!< Liste des numéros de colis */
    }BoxContainment;
};

/*!
 * \struct struct_DoorGeom DataStructures.h
 *
 * \brief Structure de donnée de l'application LC, utilisée pour dessiner le plan de l'installation des consignes.
 */
struct struct_DoorGeom
{
    int BoxNumber;      /*!< Identifiant  de la consigne */
    QString BoxSize;    /*!< Taille de la consigne (A,AW,CW,CM)*/
    int status;         /*!< Status de la consigne : Libre (OK = 0) ou occupée/endommagée (KO = 1) */
    int x;              /*!< Coordonnées horizontale de la consigne */
    int y;              /*!< Coordonnées verticale de la consigne */
};

/*!
 * \struct struct_Database_Contains DataStructures.h
 *
 * \brief Structure de donnée de l'application LC, utilisée pour l'enregistrement et lectures des fichiers de sauvegarde.
 */
struct struct_Database_Contains
{
    QString Table;          /*!< Nom de la table dans la base de données */
    QList<QString> Values;  /*!< Liste des lignes contenant les valeurs de champs de la table */
};

/*!
 * \struct struct_DoorsStatus DataStructures.h
 *
 * \brief Structure de donnée de l'application LC, utilisée pour définir l'état des portes des consignes.
 */
struct struct_DoorsStatus
{
    int BoxNumber;      /*!< Identifiant de la porte */
    bool BoxIsUsed;     /*!< Etat de la porte, si elle est utilisée à vrai (true) ou faux (false) sinon*/
    bool BoxIsDammaged; /*!< Etat de la porte, si elle est endommagée à vrai (true) ou faux (false) sinon*/
    bool DoorIsOpen;    /*!< Etat de la porte, si elle est ouverte à vrai (true) ou faux (false) sinon*/
    bool DoorIsLock;    /*!< Etat de la porte, si elle est fermée à vrai (true) ou faux (false) sinon*/
};

#endif // DATASTRUCTURES_H
