#ifndef CUSTOMPUSHBUTTON_H
#define CUSTOMPUSHBUTTON_H
/*!
 *
 * \file CustomPushButton.h
 * \class CustomPushButton "Header"
 * \brief Header de la classe CustomPushButton
 *
 * Personnalisation d'PushButton.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */

#include <QPushButton>
#include <QMouseEvent>
#include <QLayout>

class CustomPushButton : public QPushButton
{
    Q_OBJECT
public:
    CustomPushButton(int id, QString numpackage, QString file, QWidget * parent = 0);
    CustomPushButton(int id, QString text ,QWidget *parent=0);

    ~CustomPushButton();

    void mousePressEvent(QMouseEvent *event);

    void setDisabledB(bool value);
signals:
    void clicked(QString);
    void clicked(int);

private:
    int m_id;
    QString m_NumPackage;
    QIcon m_icon;

};

#endif // CUSTOMPUSHBUTTON_H
