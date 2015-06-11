#ifndef SHOWWIDGETS_H
#define SHOWWIDGETS_H
/*!
 *
 * \file ShowWidgets.h
 * \class ShowWidgets "Header"
 * \brief Header de la classe ShowWidgets
 *
 * Permet de changer de page.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */
#include <QStackedWidget>


class ShowWidgets
{

  private:

    QStackedWidget *m_StackedWidget;

    //Positions Widgets
    int m_position_WLogin;
    int m_position_WSetting;
    int m_position_WSettingSystem;
    int m_position_WCustomer;
    int m_position_WConsole;
    int m_position_WMenuDelivery;
    int m_position_WEmptyBox;
    int m_position_WBoxChoice;
    int m_position_WScanPackage;
    int m_position_WDeliveryComfirm;
    int m_position_WShowPackageBox;
    int m_position_WStowPackages;
    int m_position_WMaintenanceMenu;
    int m_position_WDoorsManager;
    int m_position_WSQLite_Local_DatabaseManager;
    int m_position_WEditSettingSystem;
    int m_position_WSiteConfigManager;
    int m_position_WAssistance;

  public:
    ShowWidgets();
    ~ShowWidgets();

    //getter
    int GetPosConsoleWindows(){return m_position_WConsole;}
    int GetPosBoxChoice(){return m_position_WBoxChoice;}
    int GetPosCustomer(){return m_position_WCustomer;}
    int GetPosDeliveryConfirm(){return m_position_WDeliveryComfirm;}
    int GetPosLogin(){return m_position_WLogin;}
    int GetPosMenuDelivery(){return m_position_WMenuDelivery;}
    int GetPosOpenEmptyBox(){return m_position_WEmptyBox;}
    int GetPosScanPackage(){return m_position_WScanPackage;}
    int GetPosShowPackageBox(){return m_position_WShowPackageBox;}
    int GetPosSetting(){return m_position_WSetting;}
    int GetPosSettingSystem(){return m_position_WSettingSystem;}
    int GetPosStowPackages(){return m_position_WStowPackages;}
    int GetPosMaintenanceMenu(){return m_position_WMaintenanceMenu;}
    int GetPosDoorsManager() {return m_position_WDoorsManager;}
    int GetPosSQLite_Local_DatabaseManager(){return m_position_WSQLite_Local_DatabaseManager;}
    int GetPosEditSettingSystem(){return m_position_WEditSettingSystem;}
    int GetPosSiteConfigManager() {return m_position_WSiteConfigManager;}

    template <typename T> T* GetWidget(int pos){
        return (T*)m_StackedWidget->widget(pos);
    }

    //setter
    void SetStackedWidget(QStackedWidget *stackedW){m_StackedWidget = stackedW;}
    void SetPosConsoleWindows(int TposConsole){m_position_WConsole = TposConsole;}
    void SetPosBoxChoice(int TposBoxChoice){m_position_WBoxChoice = TposBoxChoice;}
    void SetPosCustomer(int TposCustumer){m_position_WCustomer = TposCustumer;}
    void SetPosDeliveryConfirm(int TposDeliComf){m_position_WDeliveryComfirm = TposDeliComf;}
    void SetPosLogin(int TposLogin){m_position_WLogin = TposLogin;}
    void SetPosMenuDelivery(int TposMenuDeli){m_position_WMenuDelivery = TposMenuDeli;}
    void SetPosOpenEmptyBox(int TposOpenEmptybox){m_position_WEmptyBox = TposOpenEmptybox;}
    void SetPosScanPackage(int TposScanPackage){m_position_WScanPackage = TposScanPackage;}
    void SetPosShowPackageBox(int TposShowPackageBox){m_position_WShowPackageBox = TposShowPackageBox;}
    void SetPosSetting(int TposSetting){m_position_WSetting = TposSetting;}
    void SetPosSettingSystem(int TposSettingSystem){m_position_WSettingSystem = TposSettingSystem;}
    void SetPosStowPackages(int TposStowPackage){m_position_WStowPackages = TposStowPackage;}
    void SetPosMaintenanceMenu(int TposMaintenanceMenu){m_position_WMaintenanceMenu = TposMaintenanceMenu;}
    void setPosDoorsManager(int TposDoorsManager) {m_position_WDoorsManager = TposDoorsManager;}
    void SetPosSQLite_Local_DatabaseManager(int TposSQLite_Local_DatabaseManager) {m_position_WSQLite_Local_DatabaseManager = TposSQLite_Local_DatabaseManager;}
    void SetPosEditSettingSystem(int TposEditSettingSystem){m_position_WEditSettingSystem = TposEditSettingSystem;}
    void SetPosSiteConfigManager(int TposSiteConfigManager) {m_position_WSiteConfigManager = TposSiteConfigManager;}
    void SetPosAssistance(int TposAssistance) {m_position_WAssistance = TposAssistance;}

    //show Widgets
    void ShowWConsoleWindows();
    void ShowWBoxChoice();
    void ShowWCustomer();
    void ShowWDeliveryConfirm();
    void ShowWLogin();
    void ShowWMenuDelivery();
    void ShowWOpenEmptyBox();
    void ShowWScanPackage();
    void ShowWPackageBox();
    void ShowWSetting();
    void ShowWStowPackages();
    void ShowWMaintenanceMenu();
    void ShowWDoorsManager();
    void ShowWSQLite_Local_DatabaseManager();
    void ShowWSettingSystem();
    void ShowWEditSettingSystem();
    void ShowWSiteConfigManager();
    void ShowWAssistance();
    void ShowWidgetCurrent(int index);

};

#endif // SHOWWIDGETS_H
