#include "menudelivery.h"
#include "ui_menudelivery.h"


MenuDelivery::MenuDelivery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuDelivery)
{
    ui->setupUi(this);
    m_ShowWidgets = NULL;

   //connection aux boutons
     connect(ui->B_Quitter,SIGNAL(clicked()),this,SLOT(ReturnHome()));
     connect(ui->B_Depot_coli,SIGNAL(clicked()),this,SLOT(Deposit()));
     connect(ui->B_Consigne_Vide,SIGNAL(clicked()),this,SLOT(OpenBoxEmpty()));


}

MenuDelivery::~MenuDelivery()
{
    delete ui;
}

void MenuDelivery::retranslateUi()
{
    ui->retranslateUi(this);
}

void MenuDelivery::Deposit()
{
    //afficher l'ihm selection des consignes
    emit refreshGraphicsBox();
    m_ShowWidgets->ShowWBoxChoice();

}

void MenuDelivery::ReturnHome()
{
    //retour a l'acceuil
    m_ShowWidgets->ShowWConsoleWindows();

}

void MenuDelivery::OpenBoxEmpty()
{
    emit showDoors();
    m_ShowWidgets->ShowWOpenEmptyBox();

}

