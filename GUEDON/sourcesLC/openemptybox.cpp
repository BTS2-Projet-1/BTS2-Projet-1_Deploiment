#include "openemptybox.h"
#include "ui_openemptybox.h"

#include <QScrollBar>
#include <QToolTip>

OpenEmptyBox::OpenEmptyBox(CSQLite_Local_DB *bd,MenuDelivery *WMenuDelivery,CDoors *Doors,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenEmptyBox)
{
    ui->setupUi(this);
    m_ShowWidgets= NULL;
    m_DB = bd;
    m_CDoors = Doors;
    m_WMenuDelivery = WMenuDelivery;
    m_CustomTreeWidget = new CustomTreeWidget(ui->ListPackage_empty);
    m_T_StatusDoors = new Thread_StatusDoors(m_CDoors);

    qRegisterMetaType< struct_DoorsStatus >("struct_DoorsStatus");
    connect(m_T_StatusDoors,SIGNAL(StatuDoor(struct_DoorsStatus)),this,SLOT(StatusDoors(struct_DoorsStatus)));

    ui->ListPackage_empty->setColumnCount(3);
    ui->ListPackage_empty->setHeaderLabels(QStringList() << "N° Porte" << "Etat"<< "Demande de Réouverture");
    connect(ui->B_Retour,SIGNAL(clicked()),this,SLOT(ReturnMenuDelivery()));
    connect(m_WMenuDelivery,SIGNAL(showDoors()),this,SLOT(CreateListBoxEmpty()));
    ui->ListPackage_empty->verticalScrollBar()->setFixedWidth(60);


}

void OpenEmptyBox::ReinitializeAttribut()
{
    m_CustomTreeWidget->ClearListItem();
    ui->ListPackage_empty->clear();
    m_ListBoxEmpty.clear();
}

OpenEmptyBox::~OpenEmptyBox()
{
    delete ui;
}

void OpenEmptyBox::ReturnMenuDelivery()
{
    //verification des portes si elles sont fermées
    bool result;

    result = m_CustomTreeWidget->isDoorsClose(m_ListBoxEmpty);
    if(result)
    {
        //arret du thread
         m_T_StatusDoors->stopThread();

        //verrouille les portes
         m_CDoors->LockDoors(m_ListBoxEmpty);

        //retour menu livraison
         m_ShowWidgets->ShowWMenuDelivery();
    }
    else{

        //message porte non ferme
        ShowMessageError(tr("Attention!!!<br/> Les portes ne sont pas ferm&eacute;es."));
    }

}

void OpenEmptyBox::CreateListBoxEmpty()
{
    ReinitializeAttribut();

    //recupere list des consignes vides
    m_ListBoxEmpty  = m_DB->GetFreeBoxes();

    if(!m_ListBoxEmpty.isEmpty())
    {
        if(m_CDoors->UnlockDoors(m_ListBoxEmpty))
        {
            m_CDoors->OpenDoors(m_ListBoxEmpty);

            for(int i = 0 ;i < m_ListBoxEmpty.size();i++)
            {
                //ajout item
                CustomPushButton *buttonOpen = new CustomPushButton(m_ListBoxEmpty[i],tr("OUVRIR"));
                connect(buttonOpen,SIGNAL(clicked(int)),this,SLOT(OpenDoor(int)));
                m_CustomTreeWidget->AddItem(m_ListBoxEmpty[i],buttonOpen);

            }
            m_T_StatusDoors->setListBox(m_ListBoxEmpty);
            m_T_StatusDoors->start();
        }

    }
    else{
        //message aucune consigne est vide
        ShowMessageError(tr("Il n'y a pas de consigne vide."));
    }

}

void OpenEmptyBox::OpenDoor(int door)
{
    QList<int> Ldoor;
    Ldoor << door;
    if(m_CDoors->UnlockDoors(Ldoor))
    {
        m_CDoors->OpenDoors(Ldoor);
    }
}

void OpenEmptyBox::StatusDoors(struct_DoorsStatus doorstatus)
{
    if(doorstatus.DoorIsOpen)
    {
        m_CustomTreeWidget->EditedItem_ColumnStatusOfDoor(doorstatus.BoxNumber,tr("OUVERT"),QColor(Qt::darkGreen));
        m_CustomTreeWidget->ButtonVisible(doorstatus.BoxNumber,true);
    }
    else{
        m_CustomTreeWidget->EditedItem_ColumnStatusOfDoor(doorstatus.BoxNumber,tr("FERME"),QColor(Qt::red));
        m_CustomTreeWidget->ButtonVisible(doorstatus.BoxNumber,false);
    }
}

void OpenEmptyBox::retranslateUi()
{
    ui->retranslateUi(this);
}

void OpenEmptyBox::ShowMessageError(QString mes)
{
    int x;
    int y;
    x = mapToGlobal(ui->B_Assistance->pos()).x()+ui->B_Assistance->width()+10;
    y = mapToGlobal(ui->B_Assistance->pos()).y()-10;
    QToolTip::showText(QPoint(x,y),mes,this);
}

