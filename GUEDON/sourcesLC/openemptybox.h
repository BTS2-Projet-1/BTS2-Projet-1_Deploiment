#ifndef OPENEMPTYBOX_H
#define OPENEMPTYBOX_H
/*!
 *
 * \file OpenEmptyBox .h
 * \class OpenEmptyBox   "Header"
 * \brief Header de la classe OpenEmptyBox
 *
 * IHM Ouverture des consignes vides.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */
#include <QWidget>

#include "showwidgets.h"
#include "csqlite_local_db.h"
#include "menudelivery.h"
#include "custompushbutton.h"
#include "customtreewidget.h"
#include "thread_statusdoors.h"
#include "cdoors.h"

class OpenEmptyBox;

namespace Ui {
class OpenEmptyBox;
}

class OpenEmptyBox : public QWidget
{
    Q_OBJECT
    
public:
    explicit OpenEmptyBox(CSQLite_Local_DB *bd, MenuDelivery *WMenuDelivery,CDoors *Doors, QWidget *parent = 0);
    void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}
    void ReinitializeAttribut();
    ~OpenEmptyBox();

    void retranslateUi();
    void ShowMessageError(QString mes);

public slots:
    void ReturnMenuDelivery();
    void CreateListBoxEmpty();    
    void StatusDoors(struct_DoorsStatus doorstatus);
    void OpenDoor(int door);

private slots:


private:
    Ui::OpenEmptyBox *ui;
    ShowWidgets *m_ShowWidgets;
    CSQLite_Local_DB *m_DB;
    MenuDelivery *m_WMenuDelivery;
    QList<int> m_ListBoxEmpty;

    CustomTreeWidget *m_CustomTreeWidget;

    //CDOORS
    Thread_StatusDoors *m_T_StatusDoors;
    CDoors *m_CDoors;
};

#endif // OPENEMPTYBOX_H
