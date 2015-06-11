#ifndef CUSTOMER_H
#define CUSTOMER_H
/*!
 *
 * \file Customer.h
 * \class Customer "Header"
 * \brief Header de la classe Customer
 *
 * IHM Saisi code de retrait.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */
#include <QWidget>
#include <QRegExpValidator>

#include "showwidgets.h"
#include "csqlite_local_db.h"
#include "showpackagebox.h"


namespace Ui {
class Customer;
}

class Customer : public QWidget
{
    Q_OBJECT

public:
    explicit Customer(CSQLite_Local_DB*, QWidget *parent = 0);
    void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}
    ~Customer();

    //mise ajour de la traduction
    void retranslateUi();

public slots:
    void Valid_Code();
    void Upper(QString);
    void ShowHome();

signals:
    void SendListbox(QList<int>);
    void SendCode(QString);
    void refresh();
    void TextToUpper(QString);

private slots:
    void SetCaract(QChar caract);
    void SupprCaract();
    void on_B_Home_clicked();
    void on_B_Assistance_clicked();
    void on_B_Help_clicked();

private:
    Ui::Customer *ui;

    CSQLite_Local_DB *m_BD;
    ShowWidgets *m_ShowWidgets;
    QList<int> m_listboxcustomer;
    ShowPackageBox *m_Widget_ShowPackageBox;

    QRegExpValidator *m_regExp;//utilisation expression régulière


};

#endif // Customer_H
