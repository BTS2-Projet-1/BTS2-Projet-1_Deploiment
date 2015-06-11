#include "consolewindows.h"
#include "ui_consolewindows.h"

#include <QApplication>
#include <QDebug>

ConsoleWindows::ConsoleWindows(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConsoleWindows)
{
    ui->setupUi(this);

    m_compteur = 0;

    //serveur LMS
    m_LMS = new CLMS_DB();

    //création de la base de donnée
    m_BD = new CSQLite_Local_DB(m_LMS);
    if(m_BD->Get_DataBaseIsReady())
        qDebug()<< "[ConsoleWindows][SUCCESS] : Database connection is ready";
    else
    {
        QFile log;

        exit(-1);
        qDebug()<< "[ConsoleWindows][FATAL ERROR] : No database connection !";
    }

    //CDoors
    m_Doors = new CDoors(m_BD);

    m_fileSaveTemlist = new CustomXml("SaveListsPackageTemp.xml","ListsPackageTemp");
    m_fileConfig = new CustomXml("config.xml","SettingSystem");
    //initialisation des Widgets
    Initialization_Widgets();

    //partage des positions Widgets et du stackedwidget pour les autres classes
    Init_Show_Widgets();

    //Creation timer
    m_timer = new QTimer(this);

    ui->comboBox_country->clear();
    //test list pays
    m_pays <<"France"<<"English";

    ui->comboBox_country->addItems(m_pays);

    m_flags<<QIcon(":/flags/France")<<QIcon(":/flags/English");
    for(int i = 0 ; i<m_flags.count();i++){

        ui->comboBox_country->setItemIcon(i,m_flags[i]);
    }

   //translator
    m_country = new Country(FRENCH);

   //connection bouton

    connect(m_timer,SIGNAL(timeout()),this,SLOT(HideB_DeliveryMaintenance()));
    connect(ui->comboBox_country,SIGNAL(currentIndexChanged(int)),this,SLOT(SelectPays(int)));
    ui->B_DeliveryAndMaintenace->setVisible(false);

    //gestion des evenements des widgets
    ui->B_DeliveryAndMaintenace->installEventFilter(this);
    ui->P_Home->installEventFilter(this);

}

ConsoleWindows::~ConsoleWindows()
{
    delete m_Widget_Assistance;
    delete m_Widget_BoxChoice;
    delete m_Widget_Customer;
    delete m_Widget_DeliveryComfirm;
    delete m_Widget_DoorsManager;
    delete m_Widget_EditSettingSystem;
    delete m_Widget_EmptyBox;
    delete m_Widget_Login;
    delete m_Widget_MaintenanceMenu;
    delete m_Widget_MenuDelivery;
    delete m_Widget_ScanPackage;
    delete m_Widget_Setting;
    delete m_Widget_SettingSystem;
    delete m_Widget_ShowPackageBox;
    delete m_Widget_SiteConfigManager;
    delete m_Widget_SQLite_Local_DatabaseManager;
    delete m_Widget_StowPackages;
    delete m_Doors;
    delete m_BD;
    delete m_timer;
    delete m_LMS;
    delete m_fileSaveTemlist;
    delete m_fileConfig;
    delete m_country;
    delete ui;
}

void ConsoleWindows::Initialization_Widgets()
{
    m_position_WConsole = ui->stackedWidget->currentIndex();

  //Partie Client
   //Widget Customer
    m_Widget_Customer = new Customer(m_BD);
    m_position_WCustomer = ui->stackedWidget->addWidget(m_Widget_Customer);

   //Widget ShowPackageBox
    m_Widget_ShowPackageBox = new ShowPackageBox(m_Widget_Customer,m_BD,m_Doors);
    m_position_WShowPackageBox = ui->stackedWidget->addWidget(m_Widget_ShowPackageBox);

    //Widget Login
     m_Widget_Login = new Login(m_BD);
     m_position_WLogin = ui->stackedWidget->addWidget(m_Widget_Login);

  //Partie Livraison
    //Widget MenuDelivery
    m_Widget_MenuDelivery = new MenuDelivery();
    m_position_WMenuDelivery = ui->stackedWidget->addWidget(m_Widget_MenuDelivery);

   //Widget Consignes Vides
    m_Widget_EmptyBox = new OpenEmptyBox(m_BD,m_Widget_MenuDelivery,m_Doors);
    m_position_WEmptyBox = ui->stackedWidget->addWidget(m_Widget_EmptyBox);

   //Widget selection consigne
    m_Widget_BoxChoice = new BoxChoice(m_Widget_MenuDelivery,m_BD);
    m_position_WBoxChoice = ui->stackedWidget->addWidget(m_Widget_BoxChoice);

   //Widget pour saisir les n°colis
    m_Widget_ScanPackage = new ScanPackage(m_Widget_BoxChoice);
    m_position_WScanPackage = ui->stackedWidget->addWidget(m_Widget_ScanPackage);

   //Widget stowpackages
    m_Widget_StowPackages = new StowPackages(m_Widget_ScanPackage,m_Widget_BoxChoice,m_BD,m_Doors);
    m_position_WStowPackages = ui->stackedWidget->addWidget(m_Widget_StowPackages);

   //Widget confirmation de la livraison
    m_Widget_DeliveryComfirm = new DeliveryConfirm(m_BD,m_LMS,m_Widget_StowPackages,m_Widget_BoxChoice,m_fileSaveTemlist);
    m_position_WDeliveryComfirm = ui->stackedWidget->addWidget(m_Widget_DeliveryComfirm);

  //Partie Systeme Maintenance
   //Widget Setting
    m_Widget_Setting = new Setting();
    m_position_WSetting = ui->stackedWidget->addWidget(m_Widget_Setting);

   //Widget SettingSystem
    m_Widget_SettingSystem = new SettingSystem(m_BD,m_LMS,m_fileConfig,m_fileSaveTemlist);
    m_position_WSettingSystem = ui->stackedWidget->addWidget(m_Widget_SettingSystem);

    m_Widget_EditSettingSystem = new EditSettingSystem(m_Widget_SettingSystem);
    m_position_WEditSettingSystem = ui->stackedWidget->addWidget(m_Widget_EditSettingSystem);
    m_Widget_SettingSystem->setEditSetting(m_Widget_EditSettingSystem);

   //Widget MaintenanceMenu
    m_Widget_MaintenanceMenu = new MaintenanceMenu(m_Widget_Setting);
    m_position_WMaintenanceMenu = ui->stackedWidget->addWidget(m_Widget_MaintenanceMenu);

   //Widget SQLite_Local_DatabaseManager
    m_Widget_SQLite_Local_DatabaseManager = new SQLite_Local_DatabaseManager(this,m_BD);
    m_position_WSQLite_Local_DatabaseManager = ui->stackedWidget->addWidget(m_Widget_SQLite_Local_DatabaseManager);

   // <<<<<<<<<<<<<<<<<<<<================================= DATABASE LOADING OPTION ==================================>>>>>>>>>>>>>>>>>>>>>
    connect(m_Widget_SQLite_Local_DatabaseManager,SIGNAL(EndThread_DatabaseManager(bool)), this,SLOT(EndBuild_Database(bool)));

    #ifdef DEBUG   //DataStructures.h
        emit Build_Databse(DEFAULT_DATABASE);
    #endif
   // <<<<<<<<<<<<<<<<<<<<============================================================================================>>>>>>>>>>>>>>>>>>>>>

   //Widget DoorsManager
    m_Widget_DoorsManager = new DoorsManager(m_Widget_MaintenanceMenu,m_BD,m_Doors);
    m_position_WDoorsManager = ui->stackedWidget->addWidget(m_Widget_DoorsManager);

   //Widget SiteConfigManager
    m_Widget_SiteConfigManager = new SiteConfigManager(m_Widget_MaintenanceMenu,m_BD);
    m_position_WSiteConfigManager = ui->stackedWidget->addWidget(m_Widget_SiteConfigManager);

   //Widget Assistance
    m_Widget_Assistance = new Assistance();
    m_position_WAssistance = ui->stackedWidget->addWidget(m_Widget_Assistance);

}
void ConsoleWindows::Init_Show_Widgets()
{

    //partage positions widget entre widgets
    m_WidgetsShow.SetPosBoxChoice(m_position_WBoxChoice);
    m_WidgetsShow.SetPosConsoleWindows(m_position_WConsole);
    m_WidgetsShow.SetPosCustomer(m_position_WCustomer);
    m_WidgetsShow.SetPosDeliveryConfirm(m_position_WDeliveryComfirm);
    m_WidgetsShow.SetPosLogin(m_position_WLogin);
    m_WidgetsShow.SetPosMenuDelivery(m_position_WMenuDelivery);
    m_WidgetsShow.SetPosOpenEmptyBox(m_position_WEmptyBox);
    m_WidgetsShow.SetPosScanPackage(m_position_WScanPackage);
    m_WidgetsShow.SetPosSetting(m_position_WSetting);
    m_WidgetsShow.SetPosShowPackageBox(m_position_WShowPackageBox);
    m_WidgetsShow.SetPosStowPackages(m_position_WStowPackages);
    m_WidgetsShow.SetStackedWidget(ui->stackedWidget);
    m_WidgetsShow.SetPosMaintenanceMenu(m_position_WMaintenanceMenu);
    m_WidgetsShow.setPosDoorsManager(m_position_WDoorsManager);
    m_WidgetsShow.SetPosSQLite_Local_DatabaseManager(m_position_WSQLite_Local_DatabaseManager);
    m_WidgetsShow.SetPosSettingSystem(m_position_WSettingSystem);
    m_WidgetsShow.SetPosEditSettingSystem(m_position_WEditSettingSystem);
    m_WidgetsShow.SetPosSiteConfigManager(m_position_WSiteConfigManager);
    m_WidgetsShow.SetPosAssistance(m_position_WAssistance);


    //Widget Customer
    m_Widget_Customer->SetShowWidgets(&m_WidgetsShow);
    m_Widget_ShowPackageBox->SetShowWidgets(&m_WidgetsShow);


   //Widget MenuDelivery
    m_Widget_MenuDelivery->SetShowWidgets(&m_WidgetsShow);


   //Widget Consignes Vides
    m_Widget_EmptyBox->SetShowWidgets(&m_WidgetsShow);


   //Widget selection consigne
    m_Widget_BoxChoice->SetShowWidgets(&m_WidgetsShow);


   //Widget pour saisir les n°colis
    m_Widget_ScanPackage->SetShowWidgets(&m_WidgetsShow);

    m_Widget_StowPackages->SetShowWidgets(&m_WidgetsShow);

   //Widget confirmation de la livraison
    m_Widget_DeliveryComfirm->SetShowWidgets(&m_WidgetsShow);


   //Widget Login
    m_Widget_Login->SetShowWidgets(&m_WidgetsShow);


   //Widget Setting
    m_Widget_Setting->SetShowWidgets(&m_WidgetsShow);
    m_Widget_SettingSystem->SetShowWidgets(&m_WidgetsShow);
    m_Widget_EditSettingSystem->SetShowWidgets(&m_WidgetsShow);


   //Widget Maintenance
    m_Widget_MaintenanceMenu->SetShowWidgets(&m_WidgetsShow);
    m_Widget_SQLite_Local_DatabaseManager->SetShowWidgets(&m_WidgetsShow);
    m_Widget_DoorsManager->SetShowWidgets(&m_WidgetsShow);

   //Widget Assistance
    m_Widget_Assistance->SetShowWidgets(&m_WidgetsShow);

}


//affiche ihm principal
void ConsoleWindows::Show_WConsole()
{
    ui->stackedWidget->setCurrentIndex(m_position_WConsole);
}


bool ConsoleWindows::eventFilter(QObject *obj, QEvent *event)
{
    bool result = false;

    if(obj == ui->P_Home && event->type()==QEvent::MouseButtonPress)
    {
        m_compteur++;
        if(m_compteur == 3)
        {
            ui->B_DeliveryAndMaintenace->setVisible(true);

        }
        result=true;
    }

    if(obj == ui->B_DeliveryAndMaintenace && event->type()==QEvent::Show)
    {
        m_timer->start(10000);
        result = true;
    }

    if(obj == ui->B_DeliveryAndMaintenace && event->type()==QEvent::Hide)
    {
        m_timer->stop();
        result = true;
    }

    return result;
}

void ConsoleWindows::HideB_DeliveryMaintenance()
{
    m_compteur = 0;
    ui->B_DeliveryAndMaintenace->setVisible(false);

}

void ConsoleWindows::SelectPays(int flags)
{

    QEvent *e = new QEvent(QEvent::LanguageChange);

    switch (flags) {
    case 0://select french
             m_country->setLanguage(FRENCH);
             changeEvent(e);
             break;

    case 1://select english
            m_country->setLanguage(ENGLISH);
            changeEvent(e);
            break;
    default:
        break;
    }
    delete e;
}

void ConsoleWindows::EndBuild_Database(bool Value)
{
    if(Value)
        qDebug()<<"Build database : OK";
    else
    {
        qDebug()<<"Build database : ERREUR !";
        QApplication::exit(-1);
    }
}

void ConsoleWindows::changeEvent(QEvent *e)
{
    if(e->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi(this);
        m_Widget_BoxChoice->retranslateUi();
        m_Widget_Customer->retranslateUi();
        m_Widget_DeliveryComfirm->retranslateUi();
        m_Widget_EmptyBox->retranslateUi();
        m_Widget_Login->retranslateUi();
        m_Widget_MenuDelivery->retranslateUi();
        m_Widget_ScanPackage->retranslateUi();
        m_Widget_Setting->retranslateUi();
        m_Widget_ShowPackageBox->retranslateUi();
        m_Widget_StowPackages->retranslateUi();

    }
}

//buttons clicked

void ConsoleWindows::on_B_TakePackage_clicked()
{
    //cache bouton livraison
    HideB_DeliveryMaintenance();

    //affiche page customer
    ui->stackedWidget->setCurrentIndex(m_position_WCustomer);
}

void ConsoleWindows::on_B_DeliveryAndMaintenace_clicked()
{
    HideB_DeliveryMaintenance();

    //affichage du login
    m_Widget_Login->Reset();
    m_WidgetsShow.ShowWLogin();
}


