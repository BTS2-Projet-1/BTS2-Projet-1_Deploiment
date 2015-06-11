#ifndef SETTINGSYSTEM_H
#define SETTINGSYSTEM_H
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
#include <QLineEdit>

#include "showwidgets.h"
#include "customxml.h"
#include "csqlite_local_db.h"
#include "clms_db.h"
#include "editsettingsystem.h"
#include "threadsavedelivery.h"

class EditSettingSystem;
class ThreadSaveDelivery;
class SettingSystem;

namespace Ui {
class SettingSystem;
}

class SettingSystem : public QWidget
{
    Q_OBJECT
    Q_ENUMS(MODE_REGEXP)

public:
    explicit SettingSystem(CSQLite_Local_DB *DB, CLMS_DB *lms, CustomXml *fileconfig, CustomXml *fileSaveTemp, QWidget *parent = 0);
    void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}
    ~SettingSystem();

    void UpdateSetting();
    void setEditSetting(EditSettingSystem *editsetting);
    enum MODE_REGEXP{NORMAL = 0,IPV4 = 1,EMAIL = 2};

public slots:
    void UpdateFileXml();

private slots:
    void on_B_Cancel_clicked();
    void SetTextEdit(QString text); 
    void TestPing();

private:
    Ui::SettingSystem *ui;
    ShowWidgets *m_ShowWidgets;

    CSQLite_Local_DB *m_DB;
    CLMS_DB *m_LMS;
    ThreadSaveDelivery *m_ThreadSaveDelivery;
    CustomXml *m_Xml;
    EditSettingSystem *m_EditSettingSystem;

    int m_LineEdit_Selected;
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void SendWLineText(QString,QString,MODE_REGEXP);
    void UpdateSavelistPackagetemp();
};

#endif // SETTINGSYSTEM_H
