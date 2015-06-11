#ifndef SHOWPACKAGEBOX_H
#define SHOWPACKAGEBOX_H
/*!
 *
 * \file SettingSystem.h
 * \class SettingSystem  "Header"
 * \brief Header de la classe SettingSystem
 *
 * IHM Configuration Système.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */
#include <QWidget>
#include <QTimer>
#include "showwidgets.h"
#include "csqlite_local_db.h"

#include "threaddeleteextractcode.h"
#include "thread_statusdoors.h"
#include "customtreewidget.h"
#include "cdoors.h"

class Customer;
class Thread_StatusDoors;

namespace Ui {
class ShowPackageBox;
}

class ShowPackageBox : public QWidget
{
    Q_OBJECT

public:
    explicit ShowPackageBox(Customer *Wcustomer, CSQLite_Local_DB *bd,CDoors *Doors,QWidget *parent = 0);
    void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}
    void ResetAttribut();
    ~ShowPackageBox();

    bool isCloseDoors();    
    void retranslateUi();

public slots:
    void ManageListBox(QList<int> list);
    void ReturnHome();
    void ReceiveCode(QString);
    void TermitedThreadCode(int,bool);
    bool isNumBoxExist(int box);
    void OpenDoor(int);
    void StatusDoors(struct_DoorsStatus doorstatus);

signals:
    void SendCodeThread(QString);


private slots:


private:
    Ui::ShowPackageBox *ui;

    ShowWidgets *m_ShowWidgets;
    Customer *m_Wcustomer;

    QList<int> m_listbox;
    CSQLite_Local_DB *m_BD;
    QString m_ExtractCode;

    QList<ThreadDeleteExtractCode*> m_threadDEC;
    int m_idThread;

    CustomTreeWidget *m_CustomTreeWidget;

    Thread_StatusDoors *m_T_StatusDoors;
    CDoors *m_CDoors;

    QTimer *m_timer;

};

#endif // SHOWPACKAGEBOX_H
