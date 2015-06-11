#include "showpackagebox.h"
#include "ui_showpackagebox.h"
#include "customer.h"
#include "custompushbutton.h"

#include <QScrollBar>
#include <QToolTip>

#include "QDebug"

ShowPackageBox::ShowPackageBox(Customer *Wcustomer , CSQLite_Local_DB *bd,CDoors *Doors, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowPackageBox)
{
    ui->setupUi(this);

    m_BD = bd;
    m_Wcustomer = Wcustomer;
    m_idThread = 0;
    m_timer = new QTimer(this);
    m_CDoors = Doors;
    m_CustomTreeWidget = new CustomTreeWidget(ui->ListPackagesWidget);
    m_T_StatusDoors = new Thread_StatusDoors(m_CDoors);

    qRegisterMetaType< struct_DoorsStatus >( "struct_DoorsStatus" );
    connect(m_T_StatusDoors,SIGNAL(StatuDoor(struct_DoorsStatus)),this,SLOT(StatusDoors(struct_DoorsStatus)));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(ReturnHome()));
    connect(ui->B_OK,SIGNAL(clicked()),this,SLOT(ReturnHome()));
    connect(m_Wcustomer,SIGNAL(SendListbox(QList<int>)),this,SLOT(ManageListBox(QList<int>)));
    connect(m_Wcustomer,SIGNAL(SendCode(QString)),this,SLOT(ReceiveCode(QString)));

    ui->ListPackagesWidget->verticalScrollBar()->setFixedWidth(60);

}

ShowPackageBox::~ShowPackageBox()
{
    delete m_timer;
    delete ui;
}

void ShowPackageBox::ManageListBox(QList<int> list)
{
    ResetAttribut();//réinitialise les attributs

    qDebug()<< "liste des portes:"<< list;
    for(int i = 0;i < list.size();i++)
    {
        if(!isNumBoxExist(list[i]))
        {
            m_listbox << list[i];

            CustomPushButton *buttonOpen = new CustomPushButton(list[i],"Ouvrir");
            connect(buttonOpen,SIGNAL(clicked(int)),this,SLOT(OpenDoor(int)));
            m_CustomTreeWidget->AddItem(list[i],buttonOpen);

        }
    }

    //deverrouiller les portes concernées
    if(m_CDoors->UnlockDoors(m_listbox))
    {
        m_CDoors->OpenDoors(m_listbox);
        qDebug()<<"ouverture des portes";

        m_T_StatusDoors->setListBox(m_listbox);
        m_T_StatusDoors->start();
    }
}

void ShowPackageBox::OpenDoor(int door)
{
    QList<int> Ldoor;
    Ldoor << door;
    if(m_CDoors->UnlockDoors(Ldoor))
    {
        m_CDoors->OpenDoors(Ldoor);
    }
}

void ShowPackageBox::StatusDoors(struct_DoorsStatus doorstatus)
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

    if(isCloseDoors())
    {
        if(!m_timer->isActive())
            m_timer->start(40000);
    }
    else{

        if(m_timer->isActive())
        {
            m_timer->stop();
        }
    }

}

//test si le numero de la porte est deja dans liste pour eviter les doublons
bool ShowPackageBox::isNumBoxExist(int box)
{
    bool result = false;

    if(!m_listbox.isEmpty())
    {
        for(int i=0;i < m_listbox.size();i++)
        {
            if(box == m_listbox[i])
            {
                result = true;
            }
        }
    }

    return result;
}

//bouton RETOUR
void ShowPackageBox::ReturnHome()
{
   m_timer->stop();
   if(isCloseDoors())
   {
      //arrete le thread
      m_T_StatusDoors->stopThread();
      m_T_StatusDoors->wait(1000);
      m_timer->stop();
      //verrouillé les portes
      m_CDoors->LockDoors(m_listbox);

      ThreadDeleteExtractCode *threadDEC;
      threadDEC = new ThreadDeleteExtractCode(m_idThread,m_BD,this);

      connect(threadDEC,SIGNAL(terminatedThread(int,bool)),this,SLOT(TermitedThreadCode(int,bool)));
      emit SendCodeThread(m_ExtractCode); //suppression du code de retrait

      m_threadDEC.prepend(threadDEC);
      m_idThread++;
      m_ShowWidgets->ShowWConsoleWindows();
   }
   else{
        //signale porte ne son pas fermé
       int x;
       int y;
       x = mapToGlobal(ui->B_Assistance->pos()).x()+ui->B_Assistance->width()+10;
       y =  mapToGlobal(ui->B_Assistance->pos()).y()-10;
       QToolTip::showText(QPoint(x,y),tr("Attention!!!<br/> Les portes ne sont pas ferm&eacute;es."),this);
   }

}

bool ShowPackageBox::isCloseDoors()
{
    bool result = false;

    result = m_CustomTreeWidget->isDoorsClose(m_listbox);

    return result;
}

void ShowPackageBox::ReceiveCode(QString code)
{
   m_ExtractCode = code;
}

void ShowPackageBox::TermitedThreadCode(int id, bool result)
{
    int index;
    if(result)
    {
        qDebug()<<"thread terminate";
        for(int i=0;i < m_threadDEC.size();i++)
        {
            int idthreadcurrent = m_threadDEC[i]->getId();
            if(id == idthreadcurrent)
            {
                index = i;
            }
        }
        //delete the thread
        m_threadDEC.removeAt(index);

    }
    else{
        qDebug()<<"thread no terminate";
    }
}

void ShowPackageBox::ResetAttribut()
{
    ui->ListPackagesWidget->clear();
    m_CustomTreeWidget->ClearListItem();
    m_listbox.clear();
}

void ShowPackageBox::retranslateUi()
{
    ui->retranslateUi(this);
}
