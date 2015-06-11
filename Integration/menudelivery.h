#ifndef MENUDELIVERY_H
#define MENUDELIVERY_H
/*!
 *
 * \file MenuDelivery.h
 * \class MenuDelivery  "Header"
 * \brief Header de la classe MenuDelivery
 *
 * IHM Menu de Livraison.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */
#include <QWidget>

#include "showwidgets.h"

namespace Ui {
class MenuDelivery;
}

class MenuDelivery : public QWidget
{
    Q_OBJECT

public:
    explicit MenuDelivery(QWidget *parent = 0);
    void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}

    ~MenuDelivery();

    void retranslateUi();

public slots:
    void Deposit();
    void OpenBoxEmpty();
    void ReturnHome();

signals:
    void refreshGraphicsBox();
    void showDoors();

private:
    Ui::MenuDelivery *ui;
    ShowWidgets *m_ShowWidgets;

};

#endif // MENUDELIVERY_H
