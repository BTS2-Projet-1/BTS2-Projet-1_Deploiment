#include "stowpackages.h"
#include "ui_stowpackages.h"

#include <QScrollBar>
#include <QToolTip>

#include <QDebug>

StowPackages::StowPackages(ScanPackage *WScanPackage, BoxChoice *WBoxChoice,CSQLite_Local_DB *DB,CDoors *doors ,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StowPackages)
{
    ui->setupUi(this);

    m_WScanPackage = WScanPackage;
    m_WBoxChoice = WBoxChoice;
    m_CDoors = doors;
    m_BD = DB;

    m_CustomTreeWidget = new CustomTreeWidget(ui->treeWidget);
    m_Scene = m_WBoxChoice->GetScene();
    ui->graphicsView->setScene(m_Scene);

    m_T_StatusDoors = new Thread_StatusDoors(m_CDoors);

    qRegisterMetaType< struct_DoorsStatus >( "struct_DoorsStatus" );
    connect(m_T_StatusDoors,SIGNAL(StatuDoor(struct_DoorsStatus)),this,SLOT(StatusDoors(struct_DoorsStatus)));
    connect(m_WScanPackage,SIGNAL(SendListPackageBox(QList<struct_PackagesUseBox>)),this,SLOT(ReceiveListPackageBox(QList<struct_PackagesUseBox>)));
    connect(m_WScanPackage,SIGNAL(ShowStowPackage()),this,SLOT(ShowStowP()));

    //boutons
    connect(ui->B_Cancel_Change,SIGNAL(clicked()),this,SLOT(ShowStowP()));
    connect(ui->B_ChangeBox,SIGNAL(clicked()),this,SLOT(ShowChangeBox()));
    connect(ui->B_Terminate,SIGNAL(clicked()),this,SLOT(StoragePackagesFinised()));
    connect(ui->B_Valid,SIGNAL(clicked()),this,SLOT(SaveBoxSelected()));
    connect(ui->B_Next_Change,SIGNAL(clicked()),this,SLOT(ValidCheckedbox()));
    connect(ui->B_Next_Pdef,SIGNAL(clicked()),this,SLOT(ValidCheckedboxdef()));
    connect(ui->B_Cancel_def,SIGNAL(clicked()),this,SLOT(ShowPageChangeBoxChecked()));
    connect(ui->B_Cancel_Selectbox,SIGNAL(clicked()),this,SLOT(ShowPageChangeBoxDefChecked()));

    ui->treeWidget->verticalScrollBar()->setFixedWidth(60);

}

StowPackages::~StowPackages()
{
    m_T_StatusDoors->stopThread();
    delete ui;
}

void StowPackages::ShowPageChangeBoxChecked()
{
    for(int i = 0 ; i<m_listBoxtmpunchecked.size(); i++)
    {
        m_WBoxChoice->setDoorNoChecked(m_listBoxtmpunchecked[i]-1);
    }

    ui->stackedWidget->setCurrentWidget(ui->P_checkboxechange);
}

void StowPackages::ShowPageChangeBoxDefChecked()
{
    QList<int> listbox;

    //remise a l'état initiale
    //mettre les portes qui avaient un défaut en dispo
    for(int i=0;i < m_listBoxDef.size();i++)
    {
        m_WBoxChoice->setDoorAvailable((m_listBoxDef[i]-1));//dispo
    }

    listbox = m_WBoxChoice->GetBoxSelected();
    if(!listbox.isEmpty())
    {
        for(int i=0;i < listbox.size();i++)
        {
            m_WBoxChoice->manageRect((listbox[i]-1));
        }
        for(int i=0;i < m_listBoxtmp.size();i++)
        {
            m_WBoxChoice->manageRect((m_listBoxtmp[i]-1));
        }
    }
    else{
        for(int i=0;i < m_listBoxDef.size();i++)
        {
            m_WBoxChoice->manageRect((m_listBoxDef[i]-1));//selected
        }
        for(int i=0;i < m_listBoxtmp.size();i++)
        {
            m_WBoxChoice->manageRect((m_listBoxtmp[i]-1));
        }
    }
    for(int i = 0 ; i<m_listBoxtmpunchecked.size(); i++)
    {
        m_WBoxChoice->setDoorNoChecked(m_listBoxtmpunchecked[i]-1);//selected
    }

    ui->stackedWidget->setCurrentWidget(ui->P_CheckBoxDef);
}

void StowPackages::ReceiveListPackageBox(QList<struct_PackagesUseBox> listPackagebox)
{

    ResetAttribut();

    m_listPackageBox = listPackagebox;

    for(int i =0 ; i <m_listPackageBox.size();i++)
    {
       m_listBox << m_listPackageBox[i].BoxNumber;
    }

    CreateListBoxTreeWidget();
}

void StowPackages::ResetAttribut()
{
    m_CustomTreeWidget->ClearListItem();
    ui->treeWidget->clear();
    ui->ListBoxes->clear();
    ui->listBOX_checkDef->clear();
    m_listPackageBox.clear();
    m_listBox.clear();
    m_listBoxtmpunchecked.clear();
}

void StowPackages::ShowDeliveryConfirm()
{
    emit ShowDeliveryC();
    m_ShowWidgets->ShowWDeliveryConfirm();
}

void StowPackages::StatusDoors(struct_DoorsStatus doorstatus)
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

void StowPackages::ShowStowP()
{

    ui->B_ChangeBox->setVisible(true);

    if(!m_listBoxtmpunchecked.isEmpty())
    {
        for(int i = 0 ; i<m_listBoxtmpunchecked.size(); i++)
        {
            m_WBoxChoice->setDoorNoChecked(m_listBoxtmpunchecked[i]-1);
        }
    }

    ui->stackedWidget->setCurrentIndex(0);

}

void StowPackages::AddItemListBox()
{
    for(int i =0 ;i<m_listBox.size();i++)
    {
        QListWidgetItem *item;
        item = new QListWidgetItem(QString("Porte : "+QString::number(m_listBox[i])));
        item->setCheckState(Qt::Unchecked);
        ui->ListBoxes->addItem(item);
    }

}

void StowPackages::ShowChangeBox()
{
    ui->B_ChangeBox->setVisible(false);
    ui->ListBoxes->clear();
    m_listBoxtmpunchecked.clear();

    //ajoute liste des boxes
    AddItemListBox();
    ui->stackedWidget->setCurrentWidget(ui->P_checkboxechange);

}

void StowPackages::ValidCheckedbox()
{
    ui->listBOX_checkDef->clear();
    m_listBoxtmpunchecked.clear();

    QList<QListWidgetItem*> listitemtmp;

    if(!m_listBox.isEmpty())
    {
        for(int i=0;i<m_listBox.size();i++)
        {
            //recupere les items

            QListWidgetItem *item;
            item = new QListWidgetItem(QString("Porte : "+QString::number(m_listBox[i])));
            item->setCheckState(Qt::Unchecked);
            listitemtmp << item;

        }

        for(int i=0;i<ui->ListBoxes->count();i++)
        {
            if(ui->ListBoxes->item(i)->checkState() == Qt::Unchecked)
            {
                int numBox;
                numBox = ui->ListBoxes->item(i)->text().remove("Porte : ").toInt();
                m_listBoxtmpunchecked << numBox;

            }

            if(ui->ListBoxes->item(i)->checkState() == Qt::Checked)
            {
                //recupere les items ckecked
                listitemtmp[i]->setCheckState(Qt::Unchecked);
                ui->listBOX_checkDef->addItem(listitemtmp[i]);

            }
        }

        if(ui->listBOX_checkDef->count() == 0)
        {
            ShowMessageError("Erreur!!!<br/>Aucune porte n'est selectionné");
        }
        else{
              ui->stackedWidget->setCurrentWidget(ui->P_CheckBoxDef);
        }
   }
   else{
        //si listbox empty message alerte
    }
}

void StowPackages::ValidCheckedboxdef()
{

    QList<QListWidgetItem*> listitem;

    m_listBoxDef.clear();
    m_listBoxtmp.clear();

    //si listbox empty message alerte

    for(int i=0;i < ui->listBOX_checkDef->count();i++)
    {
        //recupere les items
         listitem << ui->listBOX_checkDef->item(i);
    }

    for(int i=0;i< listitem.size();i++)
    {
        if(listitem[i]->checkState() == Qt::Checked)
        {
            //recupere les numeros des portes ckecked def
            m_listBoxDef << listitem[i]->text().remove("Porte : ").toInt();
        }
        else{
            //recupere les numeros des portes qui n'ont pas de defaut
            m_listBoxtmp << listitem[i]->text().remove("Porte : ").toInt();
        }
    }

    //affiche les portes
    qDebug()<<"ListPortedef:"<<m_listBoxDef;
    qDebug()<<"ListPortechange"<<m_listBoxtmp;

    for(int i = 0 ; i<m_listBoxtmpunchecked.size(); i++)
    {
        m_WBoxChoice->setDoorNoChecked(m_listBoxtmpunchecked[i]-1);
    }

    //mettre les portes qui ont un defaut en non disponible
    for(int i=0;i < m_listBoxDef.size();i++)
    {        
        m_WBoxChoice->setDoorUnvailable((m_listBoxDef[i]-1));
    }

    //déselectionne les portes qui sont selectionne
    for(int i=0;i < m_listBoxtmp.size();i++)
    {
        m_WBoxChoice->manageRect((m_listBoxtmp[i]-1));
    }
    int nbox;
    nbox = m_listBoxDef.size() + m_listBoxtmp.size();
    ui->label_selectionbox->setText(QString(QString(trUtf8("Veuillez sélectionner ")) + QString::number(nbox) + QString(trUtf8(" consigne(s)").toUtf8())));
    ui->stackedWidget->setCurrentWidget(ui->P_changebox);

}

void StowPackages::CreateListBoxTreeWidget()
{
    for(int i = 0 ; i <m_listBox.size();i++)
    {
        CustomPushButton *buttonOpen;
        buttonOpen = new CustomPushButton(m_listBox[i],tr("OUVRIR"));
        connect(buttonOpen,SIGNAL(clicked(int)),this,SLOT(OpenDoor(int)));

        m_CustomTreeWidget->AddItem(m_listBox[i],buttonOpen);
    }

    //deverrouiller les portes concernées
    if(m_CDoors->UnlockDoors(m_listBox))
    {
        m_CDoors->OpenDoors(m_listBox);
        qDebug()<<"ouverture des portes";

        m_T_StatusDoors->setListBox(m_listBox);
        qDebug()<<"modification liste des portes";

        m_T_StatusDoors->start();
        qDebug()<<"demarrer thread";
    }
}

void StowPackages::OpenDoor(int door)
{

    if(m_CDoors->UnlockDoor(door))
    {
        m_CDoors->OpenDoor(door);
    }
}

void StowPackages::StoragePackagesFinised()
{
    if(m_CustomTreeWidget->isDoorsClose(m_listBox))
    {
       m_T_StatusDoors->stopThread();
       m_T_StatusDoors->wait(1000);

       for(int i = 0 ; i<m_listBox.size();i++)
       {
           if(m_listBox[i] != m_listPackageBox[i].BoxNumber)
               m_listPackageBox[i].BoxNumber = m_listBox[i];
       }

       //envoi la liste des n°colis
       emit SendListPackageBox(m_listPackageBox);
       ShowDeliveryConfirm();
    }
    else{
        //message alerte
        ShowMessageError(tr("Attention!!!<br/> Les portes ne sont pas ferm&eacute;es."));
    }


}

void StowPackages::SaveBoxSelected()
{
    QList<int> listbox;

    for(int i = 0 ; i<m_listBoxtmpunchecked.size(); i++)
    {
        m_WBoxChoice->setDoorChecked(m_listBoxtmpunchecked[i]-1);
    }

    listbox = m_WBoxChoice->GetBoxSelected();

    qDebug()<<"listbox :"<<listbox;
    qDebug()<<"m_listbox :"<<m_listBox;



    if(listbox.isEmpty())
    {
        //message erreur aucune consigne selectionné
        ShowMessageError(tr("Attention!!!<br/> Vous n'avez pas s&eacute;lectionn&eacute; de consigne."));
    }
    else{

        m_T_StatusDoors->stopThread();
        m_T_StatusDoors->wait(1000);
        ui->treeWidget->clear();
        m_CustomTreeWidget->ClearListItem();

        if(listbox.size() == m_listBox.size())
        {

            for(int i = 0; i < m_listBox.size();i++)
            {
                if(listbox[i] != m_listBox[i])
                {
                    //replace le numero de la porte deselectionné par le nouveau numero de porte selectionné
                    m_listBox[i]=listbox[i];
                    qDebug()<<"replace ";

                }
            }

            UpdateBD();
            CreateListBoxTreeWidget();
            ShowStowP();
        }
        else{
            for(int i = 0 ; i<m_listBoxtmpunchecked.size(); i++)
            {
                m_WBoxChoice->setDoorNoChecked(m_listBoxtmpunchecked[i]-1);
            }
             ShowMessageError(tr("Attention!!!<br/> Vous n'avez pas le m&ecirc;me nombre de consigne."));
        }
    }
}

void StowPackages::UpdateBD()
{
    if(!m_listBoxDef.isEmpty())
    {
         m_BD->SetDammagedBoxes(m_listBoxDef);
    }
}

//affiche message error mesg = code html
void StowPackages::ShowMessageError(QString mesg)
{
    int x;
    int y;
    x =  mapToGlobal(ui->B_Assistance->pos()).x()-ui->B_Assistance->width()-140;
    y =  mapToGlobal(ui->B_Assistance->pos()).y()-10;

    QToolTip::showText(QPoint(x,y),mesg,this);
}

void StowPackages::retranslateUi()
{
    ui->retranslateUi(this);
}

void StowPackages::on_B_Cancel_Range_clicked()
{
    //retour scanpackage
    m_T_StatusDoors->stopThread();
    m_ShowWidgets->ShowWScanPackage();
}

void StowPackages::on_B_CancelDelivery_clicked()
{
    m_T_StatusDoors->stopThread();
    m_ShowWidgets->ShowWMenuDelivery();
}

