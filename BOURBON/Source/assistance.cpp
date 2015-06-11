#include "assistance.h"
#include "ui_assistance.h"


Assistance::Assistance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Assistance)
{
    ui->setupUi(this);
    m_ShowWidgets = NULL;
    m_posWidgetCurrent = 0;

    m_thread_linphone = new Thread_linphonec(this);

    //connection des boutons
    connect(ui->B_CallOn,SIGNAL(clicked()),this,SLOT(Call()));
    connect(ui->B_CallOFF,SIGNAL(clicked()),this,SLOT(EndCall()));
    connect(ui->B_ReturnWidgetCurent,SIGNAL(clicked()),this,SLOT(ReturnWidgetCurrent()));

}

Assistance::~Assistance()
{
    delete m_thread_linphone;
    delete ui;
}

void Assistance::ReceiveIndexWidget(int index)
{
    m_posWidgetCurrent = index;
}

void Assistance::Call()
{
    //Thread a appeler
    emit startThreadLinphonec();

}

void Assistance::EndCall()
{
    //Thread envoi signal pour terminer l'appel
    emit EndThreadLinphonec();

}

void Assistance::ReturnWidgetCurrent()
{
    m_ShowWidgets->ShowWidgetCurrent(m_posWidgetCurrent);
}
