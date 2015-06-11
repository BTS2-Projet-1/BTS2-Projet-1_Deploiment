#include "maintenancemenu.h"
#include "ui_maintenancemenu.h"

/*!
 * \fn MaintenanceMenu::MaintenanceMenu(QWidget *parent) : QWidget(parent), ui(new Ui::MaintenanceMenu)
 *
 * \brief Constructeur de la classe MaintenanceMenu, construit l'IHM et connecte les bouttons vers les slots.
 *
 * \param QWidget *parent, Avec "parent", un pointeur sur le widget parent (peut etre à NULL).
 */
MaintenanceMenu::MaintenanceMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaintenanceMenu)
{
    ui->setupUi(this);
    QObject::connect(ui->B_Home,SIGNAL(clicked()),this,SLOT(GotoSettings()));
    QObject::connect(ui->B_SiteConfig,SIGNAL(clicked()),this,SLOT(GotoSiteConfigManager()));
    QObject::connect(ui->B_DoorsManager,SIGNAL(clicked()),this,SLOT(GotoDoorsManager()));
    //QObject::connect(ui->B_Restarts,SIGNAL(clicked()),this,SLOT(  ));
    QObject::connect(ui->B_DatabaseManager,SIGNAL(clicked()),this,SLOT(GotoSQLite_Local_DatabaseManager()));
}

/*!
 * \fn MaintenanceMenu::~MaintenanceMenu()
 *
 * \brief Destructeur de la classe MaintenanceMenu, detruit l'IHM.
 */
MaintenanceMenu::~MaintenanceMenu()
{
    delete ui;
}

/*!
 * \fn void MaintenanceMenu::ReturnHome()
 *
 * \brief Appele via le pointeur sur ShowWidgets, la fonction ShowWConsoleWindows() pour afficher la page d'accueil.
 */
void MaintenanceMenu::ReturnHome()
{
    m_ShowWidgets->ShowWConsoleWindows();
}

/*!
 * \fn void MaintenanceMenu::GotoSQLite_Local_DatabaseManager()
 *
 * \brief Appele via le pointeur sur ShowWidgets, la fonction ShowWSQLite_Local_DatabaseManager() pour afficher la page du gestionnaire de base de données.
 */
void MaintenanceMenu::GotoSQLite_Local_DatabaseManager()
{
    m_ShowWidgets->ShowWSQLite_Local_DatabaseManager();
}

/*!
 * \fn void MaintenanceMenu::GotoDoorsManager()
 *
 * \brief Appele via le pointeur sur ShowWidgets, la fonction ShowWDoorsManager() pour afficher la page du gestionaire de portes et consignes.
 */
void MaintenanceMenu::GotoDoorsManager()
{
    m_ShowWidgets->ShowWDoorsManager();
}

/*!
 * \fn void MaintenanceMenu::GotoSettings()
 *
 * \brief Appele via le pointeur sur ShowWidgets, la fonction ShowWSetting() pour afficher le menu principale des configurations.
 */
void MaintenanceMenu::GotoSettings()
{
    m_ShowWidgets->ShowWSetting();
}

/*!
 * \fn void MaintenanceMenu::GotoSiteConfigManager()
 *
 * \brief Appele via le pointeur sur ShowWidgets, la fonction ShowWSiteConfigManager() pour afficher la page de configuration du site.
 */
void MaintenanceMenu::GotoSiteConfigManager()
{
    m_ShowWidgets->ShowWSiteConfigManager();
}
