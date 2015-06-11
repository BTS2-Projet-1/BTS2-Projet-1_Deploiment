#ifndef LOGIN_H
#define LOGIN_H
/*!
 *
 * \file Login.h
 * \class Login  "Header"
 * \brief Header de la classe Login
 *
 * IHM Login.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */
#include <QWidget>
#include "showwidgets.h"
#include "csqlite_local_db.h"

class Login;

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login( CSQLite_Local_DB *BD, QWidget *parent = 0);
    void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}
    bool eventFilter(QObject *obj, QEvent *event);
    ~Login();

    void retranslateUi();

public slots:
    void Cancel();//retour ihm client
    void ValidLogin();
    void Reset();//remet à zéro


private slots:
    void SupprCaract();
    void SetCaract(QChar); 
    void BeforePage();
    void NextPage();

private:
    Ui::Login *ui;
    ShowWidgets *m_ShowWidgets;
    CSQLite_Local_DB *m_BD;

    int m_LineEdit_Selected;
};

#endif // LOGIN_H
