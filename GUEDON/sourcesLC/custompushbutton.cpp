#include "custompushbutton.h"

CustomPushButton::CustomPushButton(int id, QString numpackage, QString file, QWidget *parent):QPushButton(parent)
{
    m_id = id;
    m_NumPackage = numpackage;
    m_icon.addFile(file);

    setIcon(m_icon);
    setIconSize(QSize(70,70));
    setFixedWidth(70);

    setStyleSheet("QPushButton{position: right;background-color: rgb(255, 255, 255);color:black;font:15pt;};QPushButton:disable{ position: center;background-color:#F1F1F1;color:#787878;font:15pt;}");

}

CustomPushButton::CustomPushButton(int id,QString text ,QWidget *parent):QPushButton(parent)
{
    m_id = id;
    setText(text);

    setFont(QFont(text,13));
    setFixedWidth(250);
    setFixedHeight(85);

    setStyleSheet("position: center;background-color: rgb(231, 231, 231);");
}

CustomPushButton::~CustomPushButton()
{

}

void CustomPushButton::mousePressEvent(QMouseEvent *event)
{
    emit clicked(m_NumPackage);
    emit clicked(m_id);
}

void CustomPushButton::setDisabledB(bool value)
{
    if(value)
    {
        setDisabled(value);
        setStyleSheet("position: center;background-color:white;color:white;font:15pt;");

    }
    else{
        setStyleSheet("position: center;background-color: rgb(231, 231, 231);color:black;font:15pt;");
        setDisabled(value);
    }
}
