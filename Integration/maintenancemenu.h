#ifndef MAINTENANCEMENU_H
#define MAINTENANCEMENU_H

/*!
 * \file maintenancemenu.h
 *
 * \class MaintenanceMenu
 *
 * \brief classe MaintenanceMenu, Gestion du menu de maintenance de Locker Control\n
 *        <a href="https://github.com/BTS2-Projet-1/Locker_Control">Projet Locker Control sur github.</a>
 *
 * @startuml{MaintenanceMenu.svg}
 *
 * QWidget <|-- MaintenanceMenu
 * MaintenanceMenu *-- ConsoleWindows
 * MaintenanceMenu o-- ConsoleWindows
 * ShowWidgets o-- MaintenanceMenu
 *
 * @enduml
 *
 * \author HOUDAYER Pierre
 *
 * \version 1.1
 *
 * \date 14 mai 2015
 *
 */

#include <QWidget>
#include "showwidgets.h"

namespace Ui {
class MaintenanceMenu;
}

class MaintenanceMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MaintenanceMenu(QWidget *parent = 0);

    /*!
     * \fn void SetShowWidgets(ShowWidgets *TShowWidgets)
     *
     * \brief Permet un lien via pointeur, sur une instance de la classe ShowWidgets, afin de pouvoir avoir accès au differents Widgets de Locker Control.
     *
     * \param ShowWidgets *TShowWidgets, Avec "TShowWidgets", un pointeur valide sur sur une instance de la classe ShowWidgets.
     */
    void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}

    ~MaintenanceMenu();

public slots:
    void ReturnHome();
    void GotoSQLite_Local_DatabaseManager();
    void GotoDoorsManager();
    void GotoSettings();
    void GotoSiteConfigManager();

private:
    Ui::MaintenanceMenu *ui;
    ShowWidgets *m_ShowWidgets;
};

#endif // MAINTENANCEMENU_H
