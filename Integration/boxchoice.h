#ifndef BOXCHOICE_H
#define BOXCHOICE_H

/*!
 *
 * \file boxchoice.h
 * \class BoxChoice "Header"
 * \brief Header de la classe BoxChoice
 *
 * IHM Sélection des consignes.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */

#include <QWidget>
#include <QGraphicsScene>
#include <QList>

#include "showwidgets.h"
#include "drawbox.h"
#include "DataStructures.h"
#include "menudelivery.h"
#include "csqlite_local_db.h"

namespace Ui {
class BoxChoice;
}

class BoxChoice : public QWidget
{
    Q_OBJECT

    static const int HEIGHT = 100;
    static const int WIDTH = 100;
    static const int STARTX = 0;
    static const int STARTY = 0;

    public:

        explicit BoxChoice(MenuDelivery *WMenuDelivery=NULL, CSQLite_Local_DB *bd=NULL, QWidget *parent = 0);
        ~BoxChoice();

        void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}

        QGraphicsView* GetGraphicsView();
        QGraphicsScene* GetScene();

        QList<int> GetBoxSelected();

        //traduction
        void retranslateUi();

        //gestion des portes du dessin
        void setDoorUnvailable(int id);
        void setDoorNoChecked(int id);
        void setDoorAvailable(int id);
        void setDoorChecked(int id);

        //affiche message erreur
        void ShowMessageError(QString mesg);


public slots:

        void manageRect(int);
        void Refresh();

    signals:
        void SendListBoxSelected(QList<int>);

    private slots:
        void on_B_Valide_clicked();
        void on_B_Cancel_clicked();
        void on_B_Help_clicked();
        void on_B_Assistance_clicked();

    private:
        Ui::BoxChoice *ui;
        ShowWidgets *m_ShowWidgets;
        QGraphicsScene *m_Scene;
        MenuDelivery *m_WMenuDelivery;

        //lien base de donnée
        CSQLite_Local_DB *m_BD;

        QList<struct struct_DoorGeom> m_ListBox;
        QList<drawBox *> m_listdrawBox;

        void createBoxes(const QList<struct_DoorGeom> &ListBox);

};

#endif // BOXCHOICE_H
