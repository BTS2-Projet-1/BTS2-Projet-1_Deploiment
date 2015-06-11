#ifndef KEYBOARD_H
#define KEYBOARD_H
/*!
 *
 * \file KeyBoard.h
 * \class KeyBoard  "Header"
 * \brief Header de la classe KeyBoard
 *
 * Widget Clavier Virtuel.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */

#include <QWidget>
#include <QPushButton>

//mode Majuscules ou minuscule
enum MODE{UPPER = 0, LOWER = 1, NUM = 2};


namespace Ui {
class KeyBoard;
}

class KeyBoard : public QWidget
{
  Q_OBJECT

  public:
    explicit KeyBoard(QWidget *parent = 0, MODE mode = UPPER);
    ~KeyBoard();
    void setVisibleB_Ok(bool visible);   
    void setTextButtonOK(QString text);

    //clavier mode
    void setMode(MODE mode);
    MODE getMode(){return m_Mode;}
    void UpdateMode();

    //mise a jour Traduction
    void retranslateUi();


public slots:
    void set_ABC();
    void set_123();

 private slots:
    void set_A();
    void set_B();
    void set_C();
    void set_D();
    void set_E();
    void set_F();
    void set_G();
    void set_H();
    void set_I();
    void set_J();
    void set_K();
    void set_L();
    void set_M();
    void set_N();
    void set_O();
    void set_P();
    void set_Q();
    void set_R();
    void set_S();
    void set_T();
    void set_U();
    void set_V();
    void set_W();
    void set_X();
    void set_Y();
    void set_Z();  
    void SUPPR();
    void Ok();
    void set_Point();
    void set_0();
    void set_1();
    void set_2();
    void set_3();
    void set_4();
    void set_5();
    void set_6();
    void set_7();
    void set_8();
    void set_9();
    void ChangeCaract();

signals:
    void SendCarac(QChar);
    void SupprLastCaract();
    void ButtonOKCliked();

  private:
    Ui::KeyBoard *ui;
    QList<QPushButton *> m_listButtons;
    MODE m_Mode;

};

#endif // KEYBOARD_H
