#include "keyboard.h"
#include "ui_keyboard.h"

KeyBoard::KeyBoard(QWidget *parent,MODE mode) :
    QWidget(parent),
    ui(new Ui::KeyBoard)
{
    ui->setupUi(this);

    m_Mode = mode;

    m_listButtons << ui->B_A
                  << ui->B_B
                  << ui->B_C
                  << ui->B_D
                  << ui->B_E
                  << ui->B_F
                  << ui->B_G
                  << ui->B_H
                  << ui->B_I
                  << ui->B_J
                  << ui->B_K
                  << ui->B_L
                  << ui->B_M
                  << ui->B_N
                  << ui->B_O
                  << ui->B_P
                  << ui->B_Q
                  << ui->B_R
                  << ui->B_S
                  << ui->B_T
                  << ui->B_U
                  << ui->B_V
                  << ui->B_W
                  << ui->B_X
                  << ui->B_Y
                  << ui->B_Z;

    connect(ui->B_Shift,SIGNAL(clicked()),this,SLOT(ChangeCaract()));
    connect(ui->B_ABC,SIGNAL(clicked()),this,SLOT(set_ABC()));
    connect(ui->B_123,SIGNAL(clicked()),this,SLOT(set_123()));
    connect(ui->B_SUPP,SIGNAL(pressed()),this,SLOT(SUPPR()));
    connect(ui->B_SUPPR,SIGNAL(pressed()),this,SLOT(SUPPR()));
    connect(ui->B_OK,SIGNAL(clicked()),this,SLOT(Ok()));
    connect(ui->B_OK_2,SIGNAL(clicked()),this,SLOT(Ok()));
    connect(ui->B_A,SIGNAL(clicked()),this,SLOT(set_A()));
    connect(ui->B_B,SIGNAL(clicked()),this,SLOT(set_B()));
    connect(ui->B_C,SIGNAL(clicked()),this,SLOT(set_C()));
    connect(ui->B_D,SIGNAL(clicked()),this,SLOT(set_D()));
    connect(ui->B_E,SIGNAL(clicked()),this,SLOT(set_E()));
    connect(ui->B_F,SIGNAL(clicked()),this,SLOT(set_F()));
    connect(ui->B_G,SIGNAL(clicked()),this,SLOT(set_G()));
    connect(ui->B_H,SIGNAL(clicked()),this,SLOT(set_H()));
    connect(ui->B_I,SIGNAL(clicked()),this,SLOT(set_I()));
    connect(ui->B_J,SIGNAL(clicked()),this,SLOT(set_J()));
    connect(ui->B_K,SIGNAL(clicked()),this,SLOT(set_K()));
    connect(ui->B_L,SIGNAL(clicked()),this,SLOT(set_L()));
    connect(ui->B_M,SIGNAL(clicked()),this,SLOT(set_M()));
    connect(ui->B_N,SIGNAL(clicked()),this,SLOT(set_N()));
    connect(ui->B_O,SIGNAL(clicked()),this,SLOT(set_O()));
    connect(ui->B_P,SIGNAL(clicked()),this,SLOT(set_P()));
    connect(ui->B_Q,SIGNAL(clicked()),this,SLOT(set_Q()));
    connect(ui->B_R,SIGNAL(clicked()),this,SLOT(set_R()));
    connect(ui->B_S,SIGNAL(clicked()),this,SLOT(set_S()));
    connect(ui->B_T,SIGNAL(clicked()),this,SLOT(set_T()));
    connect(ui->B_U,SIGNAL(clicked()),this,SLOT(set_U()));
    connect(ui->B_V,SIGNAL(clicked()),this,SLOT(set_V()));
    connect(ui->B_W,SIGNAL(clicked()),this,SLOT(set_W()));
    connect(ui->B_X,SIGNAL(clicked()),this,SLOT(set_X()));
    connect(ui->B_Y,SIGNAL(clicked()),this,SLOT(set_Y()));
    connect(ui->B_Z,SIGNAL(clicked()),this,SLOT(set_Z()));
    connect(ui->B_0,SIGNAL(clicked()),this,SLOT(set_0()));
    connect(ui->B_1,SIGNAL(clicked()),this,SLOT(set_1()));
    connect(ui->B_2,SIGNAL(clicked()),this,SLOT(set_2()));
    connect(ui->B_3,SIGNAL(clicked()),this,SLOT(set_3()));
    connect(ui->B_4,SIGNAL(clicked()),this,SLOT(set_4()));
    connect(ui->B_5,SIGNAL(clicked()),this,SLOT(set_5()));
    connect(ui->B_6,SIGNAL(clicked()),this,SLOT(set_6()));
    connect(ui->B_7,SIGNAL(clicked()),this,SLOT(set_7()));
    connect(ui->B_8,SIGNAL(clicked()),this,SLOT(set_8()));
    connect(ui->B_9,SIGNAL(clicked()),this,SLOT(set_9()));
    connect(ui->B_Point,SIGNAL(clicked()),this,SLOT(set_Point()));

    this->setVisible(false);
    UpdateMode();
}

KeyBoard::~KeyBoard()
{
    delete ui;
}

void KeyBoard::retranslateUi()
{
    ui->retranslateUi(this);
}

void KeyBoard::setTextButtonOK(QString text)
{
    ui->B_OK->setText(text);
    ui->B_OK_2->setText(text);
}

void KeyBoard::setMode(MODE mode)
{
    m_Mode = mode;
    UpdateMode();
}

void KeyBoard::UpdateMode()
{
    switch(m_Mode)
    {
        case UPPER:
                    for(int i=0; i <m_listButtons.size() ; i++)
                    {
                        m_listButtons[i]->setText(QString('A'+i));
                    }

                    ui->B_Shift->setText("abc");
                    ui->B_ABC->setVisible(true);
                    break;

        case LOWER:
                    for(int i=0; i <m_listButtons.size() ; i++)
                    {
                        m_listButtons[i]->setText(QString('a'+i));
                    }

                    ui->B_Shift->setText("ABC");
                    ui->B_ABC->setVisible(true);
                    break;


        case NUM:
                    ui->B_ABC->setVisible(false);
                    break;
    }
}

void KeyBoard::ChangeCaract()
{
   switch(m_Mode)
   {
        case UPPER:
                    m_Mode = LOWER;
                    break;
        case LOWER:
                    m_Mode = UPPER;
                    break;
        case NUM:
                    break;
   }
   UpdateMode();
}

void KeyBoard::set_Point()
{
    emit SendCarac('.');
}

void KeyBoard::setVisibleB_Ok(bool visible)
{
    ui->B_OK->setVisible(visible);
    ui->B_OK_2->setVisible(visible);
}

void KeyBoard::set_ABC()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void KeyBoard::set_123()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void KeyBoard::set_A()
{
    if(m_Mode == UPPER)
        emit SendCarac('A');

    else if(m_Mode == LOWER)
        emit SendCarac('a');
}

void KeyBoard::set_B()
{
    if(m_Mode == UPPER)
        emit SendCarac('B');

    else if(m_Mode == LOWER)
        emit SendCarac('b');
}

void KeyBoard::set_C()
{
    if(m_Mode == UPPER)
        emit SendCarac('C');

    else if(m_Mode == LOWER)
        emit SendCarac('c');
}

void KeyBoard::set_D()
{
    if(m_Mode == UPPER)
        emit SendCarac('D');

    else if(m_Mode == LOWER)
        emit SendCarac('d');
}

void KeyBoard::set_E()
{
    if(m_Mode == UPPER)
        emit SendCarac('E');

    else if(m_Mode == LOWER)
        emit SendCarac('e');
}

void KeyBoard::set_F()
{
    if(m_Mode == UPPER)
        emit SendCarac('F');

    else if(m_Mode == LOWER)
        emit SendCarac('f');
}

void KeyBoard::set_G()
{
    if(m_Mode == UPPER)
        emit SendCarac('G');

    else if(m_Mode == LOWER)
        emit SendCarac('g');
}

void KeyBoard::set_H()
{
    if(m_Mode == UPPER)
        emit SendCarac('H');

    else if(m_Mode == LOWER)
        emit SendCarac('h');
}

void KeyBoard::set_I()
{
    if(m_Mode == UPPER)
        emit SendCarac('I');

    else if(m_Mode == LOWER)
        emit SendCarac('i');
}

void KeyBoard::set_J()
{
    if(m_Mode == UPPER)
        emit SendCarac('J');

    else if(m_Mode == LOWER)
        emit SendCarac('j');
}

void KeyBoard::set_K()
{
    if(m_Mode == UPPER)
        emit SendCarac('K');

    else if(m_Mode == LOWER)
        emit SendCarac('k');
}

void KeyBoard::set_L()
{
    if(m_Mode == UPPER)
        emit SendCarac('L');

    else if(m_Mode == LOWER)
        emit SendCarac('l');
}

void KeyBoard::set_M()
{
    if(m_Mode == UPPER)
        emit SendCarac('M');

    else if(m_Mode == LOWER)
        emit SendCarac('m');
}

void KeyBoard::set_N()
{
    if(m_Mode == UPPER)
        emit SendCarac('N');

    else if(m_Mode == LOWER)
        emit SendCarac('n');
}

void KeyBoard::set_O()
{
    if(m_Mode == UPPER)
        emit SendCarac('O');

    else if(m_Mode == LOWER)
        emit SendCarac('o');
}

void KeyBoard::set_P()
{
    if(m_Mode == UPPER)
        emit SendCarac('P');

    else if(m_Mode == LOWER)
        emit SendCarac('p');
}

void KeyBoard::set_Q()
{
    if(m_Mode == UPPER)
        emit SendCarac('Q');

    else if(m_Mode == LOWER)
        emit SendCarac('q');
}

void KeyBoard::set_R()
{
    if(m_Mode == UPPER)
        emit SendCarac('R');

    else if(m_Mode == LOWER)
        emit SendCarac('r');
}

void KeyBoard::set_S()
{
    if(m_Mode == UPPER)
        emit SendCarac('S');

    else if(m_Mode == LOWER)
        emit SendCarac('s');
}

void KeyBoard::set_T()
{
    if(m_Mode == UPPER)
        emit SendCarac('T');

    else if(m_Mode == LOWER)
        emit SendCarac('t');
}

void KeyBoard::set_U()
{
    if(m_Mode == UPPER)
        emit SendCarac('U');

    else if(m_Mode == LOWER)
        emit SendCarac('u');
}

void KeyBoard::set_V()
{
    if(m_Mode == UPPER)
        emit SendCarac('V');

    else if(m_Mode == LOWER)
        emit SendCarac('v');
}

void KeyBoard::set_W()
{
    if(m_Mode == UPPER)
        emit SendCarac('W');

    else if(m_Mode == LOWER)
        emit SendCarac('w');
}

void KeyBoard::set_X()
{
    if(m_Mode == UPPER)
        emit SendCarac('X');

    else if(m_Mode == LOWER)
        emit SendCarac('x');
}

void KeyBoard::set_Y()
{
    if(m_Mode == UPPER)
        emit SendCarac('Y');

    else if(m_Mode == LOWER)
        emit SendCarac('y');
}

void KeyBoard::set_Z()
{
    if(m_Mode == UPPER)
        emit SendCarac('Z');

    else if(m_Mode == LOWER)
        emit SendCarac('z');
}

void KeyBoard::set_0()
{
    emit SendCarac('0');
}

void KeyBoard::set_1()
{
    emit SendCarac('1');
}

void KeyBoard::set_2()
{
    emit SendCarac('2');
}

void KeyBoard::set_3()
{
    emit SendCarac('3');
}

void KeyBoard::set_4()
{
    emit SendCarac('4');
}

void KeyBoard::set_5()
{
    emit SendCarac('5');
}

void KeyBoard::set_6()
{
    emit SendCarac('6');
}

void KeyBoard::set_7()
{
    emit SendCarac('7');
}

void KeyBoard::set_8()
{
    emit SendCarac('8');
}

void KeyBoard::set_9()
{
    emit SendCarac('9');
}

void KeyBoard::SUPPR()
{
    emit SupprLastCaract();
}

void KeyBoard::Ok()
{   
    emit ButtonOKCliked();
}
