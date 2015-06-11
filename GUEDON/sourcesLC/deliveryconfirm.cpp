#include "deliveryconfirm.h"
#include "ui_deliveryconfirm.h"

#include <QDebug>

#define NEWDELIV    0
#define FINISHDELIV 1

DeliveryConfirm::DeliveryConfirm(CSQLite_Local_DB *db, CLMS_DB *lms, StowPackages *WStowPackage,BoxChoice *WBoxChoice,CustomXml *fileSaveTemlist, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeliveryConfirm)
{
    ui->setupUi(this);

    m_BD = db;
    m_LMS = lms;
    m_WStowPackage = WStowPackage;
    m_WBoxChoice = WBoxChoice;
    m_clickselect = 0;
    m_threadSaveD = new ThreadSaveDelivery(m_BD,m_LMS,fileSaveTemlist);

    //connection
    connect(this,SIGNAL(SendListPackage(QList<struct_PackagesUseBox>)),m_threadSaveD,SLOT(startThread(QList<struct_PackagesUseBox>)));
    connect(m_threadSaveD,SIGNAL(SetValueProgress(QString,int)),this,SLOT(SetValueProgress(QString,int)));
    connect(m_threadSaveD,SIGNAL(finished()),this,SLOT(ThreadFinished()));
    connect(ui->B_NewClient,SIGNAL(clicked()),this,SLOT(NewDelivery()));
    connect(ui->B_Finish_Delivery,SIGNAL(clicked()),this,SLOT(FinishDelivery()));
    connect(m_WStowPackage,SIGNAL(SendListPackageBox(QList<struct_PackagesUseBox>)),this,SLOT(ReceiveListPackageBox(QList<struct_PackagesUseBox>)));
    connect(m_WStowPackage,SIGNAL(ShowDeliveryC()),this,SLOT(ShowChoiceConfirm()));

}

DeliveryConfirm::~DeliveryConfirm()
{
    delete m_threadSaveD;
    delete ui;
}

void DeliveryConfirm::retranslateUi()
{
    ui->retranslateUi(this);
}

void DeliveryConfirm::NewDelivery()
{
    m_clickselect = NEWDELIV;
    SaveDelivery();
}

void DeliveryConfirm::FinishDelivery()
{
    m_clickselect = FINISHDELIV;
    SaveDelivery();
}

void DeliveryConfirm::ReceiveListPackageBox(QList<struct_PackagesUseBox> listpackagebox)
{
    m_tempPackageList = listpackagebox;
}

void DeliveryConfirm::ShowChoiceConfirm()
{
    ui->progressBar->setValue(0);
    ui->stackedWidget->setCurrentWidget(ui->ChoiceConfirm);
}

void DeliveryConfirm::ShowLoadSave()
{
    ui->stackedWidget->setCurrentWidget(ui->LoadSave);
}

void DeliveryConfirm::SaveDelivery()
{

    ShowLoadSave();

    ui->label->setText(tr("Enregistrement des colis au serveur LMS..."));

    emit SendListPackage(m_tempPackageList);

}

void DeliveryConfirm::SetValueProgress(QString mesg, int val)
{
    ui->progressBar->setValue(val);

    ui->label->setText(mesg);

}

void DeliveryConfirm::ThreadFinished()
{

    //remise a zero de la liste
     m_tempPackageList.clear();

     switch(m_clickselect)
     {
          case NEWDELIV:
                        //affichage de l'ihm selection des consignes
                        m_WBoxChoice->Refresh();
                        m_ShowWidgets->ShowWBoxChoice();
                        break;

          case FINISHDELIV:
                          //affichage de l'ihm menu de livraison
                          m_ShowWidgets->ShowWMenuDelivery();
                          break;
     }

}

