#ifndef SETTING_H
#define SETTING_H
/*!
 *
 * \file Setting.h
 * \class Setting  "Header"
 * \brief Header de la classe Setting
 *
 * IHM Menu de Maintenance/Configuration.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */
#include <QWidget>
#include "showwidgets.h"

namespace Ui {
class Setting;
}

class Setting : public QWidget
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = 0);
    void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}

    ~Setting();
    void retranslateUi();

public slots:
    void ReturnHome();
    void ShowMaintenanceMenu();

private slots:
    void ShowSettingSystem();


private:
    Ui::Setting *ui;
    ShowWidgets *m_ShowWidgets;
};

#endif // SETTING_H
