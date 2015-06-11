#ifndef EDITSETTINGSYSTEM_H
#define EDITSETTINGSYSTEM_H
/*!
 *
 * \file EditSettingSystem.h
 * \class EditSettingSystem  "Header"
 * \brief Header de la classe EditSettingSystem
 *
 * Editer un champ d'édition.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */

#include <QWidget>
#include <QLineEdit>
#include <QRegExpValidator>
#include "settingsystem.h"
#include "showwidgets.h"

class EditSettingSystem;
class SettingSystem;

enum MODE_REGEXP{NORMAL = 0,IPV4 = 1,EMAIL =2};

namespace Ui {
class EditSettingSystem;
}

class EditSettingSystem : public QWidget
{
    Q_OBJECT
    
public:
    explicit EditSettingSystem(SettingSystem *settingsystem,QWidget *parent = 0);
    void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}
    ~EditSettingSystem();
    

private:
    Ui::EditSettingSystem *ui;
    SettingSystem *m_SettingSystem;
    ShowWidgets *m_ShowWidgets;
    QRegExpValidator *m_RegExp;
    MODE_REGEXP m_modeRegExp;

signals:
    void sendTextEdit(QString);

public slots:
    void receiveEditText(QString label, QString text,MODE_REGEXP );
    void ClickedButtonOK();
    void SupprCaract();
    void SetCaract(QChar);

};

#endif // EDITSETTINGSYSTEM_H
