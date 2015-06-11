#ifndef DOORSMANAGER_H
#define DOORSMANAGER_H

/*!
 * \file doorsmanager.h
 *
 * \class DoorsManager
 *
 * \brief classe DoorsManager, hérite de QWidget pour la gestion de l'IHM.
 *        La classe DoorsManager permet à l'utilisateur une gestion des portes (ajout et suppression), d'effectuer des test et de pourvoir activer ou désactiver des portes défectueses.
 *
 *        <a href="https://github.com/BTS2-Projet-1/Locker_Control">Projet Locker Control sur github.</a>
 *
 * @startuml{DoorsManager.svg}
 *
 * QWidget <|-- DoorsManager
 *
 * DoorsManager *-- ConsoleWindows
 * ConsoleWindows o-- DoorsManager
 * DoorsManager o-- ConsoleWindows
 *
 * CSQLite_Local_DB o-- DoorsManager
 * ShowWidgets o-- DoorsManager
 *
 * CDoors o-- DoorsManager
 *
 * @enduml
 *
 * \author HOUDAYER Pierre
 *
 * \version 1.0
 *
 * \date 15 mai 2015
 */

#include <QWidget>
#include "csqlite_local_db.h"
#include "showwidgets.h"
#include "cdoors.h"
#include "customtreewidget.h"

#define WINDEX_DOORS_MANAGER 0
#define WINDEX_DOORS_TEST 1
#define WINDEX_ADD_DEL_DOORS 2
#define WINDEX_ACT_DES_DOORS 3

#define WINDEX_DOORS_TEST_ACTIONS 4
#define WINDEX_DOORS_TEST_CONFIRM 5

#define WINDEX_DOORS_ACT_DES_COMMAND 6

namespace Ui {
class DoorsManager;
}

class DoorsManager : public QWidget
{
    Q_OBJECT

    public:
        explicit DoorsManager(QWidget *parent = 0, CSQLite_Local_DB *BD = NULL, CDoors *Doors = NULL);
        void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}
        void CreateBoxesViewTest();
        void CreateBoxesViewActDes();
        ~DoorsManager();

        QList<struct_DoorsStatus> BoxesStatusSort(QList<struct_DoorsStatus> listtmp);

    public slots:
        void ReturnHome() {m_ShowWidgets->ShowWConsoleWindows();}
        void ReturnSettingsMenu() {m_ShowWidgets->ShowWSetting();}
        void ReturnMaintenanceMenu() {m_ShowWidgets->ShowWMaintenanceMenu();}
        void RunTestBox(int idBox);
        void GotoDoorsManager();
        void GotoDoorsTest();
        void GotoAddDelDoors();
        void GotoActDesDoors();
        void UnlockBox();
        void OpenBox();
        void CheckTestBox();
        void ActionOpenBox_ConfirmSwap();
        void ActionUnlockBox_ConfirmSwap();
        void ActivateDoor();
        void DesactivateDoor();
        void RunActDesBox(int idBox);

    signals:

        /*!
         * \fn void StartRunActDesBox(int idBox)
         *
         * \brief Emet un signal sour simuler l'appui sur le boutton de la page d'activation et de désactivation des portes.
         *
         * \param int idBox, avec "idBox" le numéro de la consigne à utiliser.
         */
        void StartRunActDesBox(int idBox);

    private:
        Ui::DoorsManager *ui;
        CSQLite_Local_DB *m_DB;
        CDoors *m_Doors;
        ShowWidgets *m_ShowWidgets;
        CustomTreeWidget *m_BoxesView_Test;
        CustomTreeWidget *m_BoxesView_ActDes;

        QList<struct struct_DoorsStatus> m_AllBoxesStatusList;
        QList<int> m_FreeBoxesList;
        int m_CurrentSelectedBox;
        bool m_OpenBoxTested;
        bool m_UnlockBoxTested;
};

#endif // DOORSMANAGER_H
