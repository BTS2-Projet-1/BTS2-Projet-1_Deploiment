#ifndef STOWPACKAGES_H
#define STOWPACKAGES_H
/*!
 *
 * \file StowPackages.h
 * \class StowPackages"Header"
 * \brief Header de la classe StowPackages
 *
 * IHM Rangement des colis dans les consignes et changement de consigne.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */

#include <QWidget>

#include "showwidgets.h"
#include "scanpackage.h"
#include "custompushbutton.h"
#include "customtreewidget.h"
#include "thread_statusdoors.h"
#include "cdoors.h"
#include "drawbox.h"
#include "boxchoice.h"
#include "csqlite_local_db.h"

class StowPackages;

namespace Ui {
class StowPackages;
}

class StowPackages : public QWidget
{
    Q_OBJECT
    
public:
    explicit StowPackages(ScanPackage *WScanPackage,BoxChoice *WBoxChoice,CSQLite_Local_DB *DB,CDoors *doors, QWidget *parent = 0);
    void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}
    ~StowPackages();
    
    void ResetAttribut();
    void CreateListBoxTreeWidget();
    void retranslateUi();
    void AddItemListBox();    
    void UpdateBD();

    //affiche message erreur
    void ShowMessageError(QString mesg);

public slots:
    void ReceiveListPackageBox(QList<struct_PackagesUseBox> listPackagebox);
    void ShowDeliveryConfirm();
    void StatusDoors(struct_DoorsStatus doorstatus);

    void ShowStowP();
    void ShowChangeBox();
    void StoragePackagesFinised();
    void SaveBoxSelected();

signals:
    void SendListPackageBox(QList<struct_PackagesUseBox>);
    void ShowDeliveryC();

private slots:
    void ValidCheckedbox();
    void ValidCheckedboxdef();
    void ShowPageChangeBoxChecked();
    void ShowPageChangeBoxDefChecked();
    void OpenDoor(int door);

    void on_B_Cancel_Range_clicked();
    void on_B_CancelDelivery_clicked();

private:
    Ui::StowPackages *ui;
    ShowWidgets *m_ShowWidgets;
    ScanPackage *m_WScanPackage;
    BoxChoice *m_WBoxChoice;
    CDoors *m_CDoors;

    QGraphicsScene *m_Scene;

    QList<struct_PackagesUseBox> m_listPackageBox;
    QList<int> m_listBox;
    QList<int> m_listBoxDef;//liste des consignes qui ont un défaut
    QList<int> m_listBoxtmp;
    QList<int> m_listBoxtmpunchecked;//liste des consignes qui ont pas été selectionné pour etre changé


    CSQLite_Local_DB *m_BD;

    CustomTreeWidget *m_CustomTreeWidget;

    Thread_StatusDoors *m_T_StatusDoors;

};

#endif // STOWPACKAGES_H
