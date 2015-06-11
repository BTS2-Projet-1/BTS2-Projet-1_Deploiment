#include "doorsmanager.h"
#include "ui_doorsmanager.h"
#include "DataStructures.h"
#include <QToolTip>
#include <QDebug>

DoorsManager::DoorsManager(QWidget *parent, CSQLite_Local_DB *BD, CDoors *Doors) :
    QWidget(parent),
    ui(new Ui::DoorsManager)
{
    ui->setupUi(this);
    if( (BD != NULL) && (Doors != NULL) )
    {
        QList<int> DoorsListTmp;
        int NumberOfDoorsTmp;

        m_DB = BD;
        m_Doors = Doors;
        m_OpenBoxTested = false;
        m_UnlockBoxTested = false;
        m_BoxesView_Test = new CustomTreeWidget(ui->TW_BoxesView_Test);
        m_BoxesView_ActDes = new CustomTreeWidget(ui->TW_BoxesView_ActDes);

        NumberOfDoorsTmp=m_DB->Get_NumberOfBoxes();
        for(int i=1; i<NumberOfDoorsTmp+1; i++)
            DoorsListTmp.append(i);

        m_FreeBoxesList = m_DB->GetFreeBoxes();
        m_AllBoxesStatusList.append(BoxesStatusSort(m_Doors->DoorsStatus(DoorsListTmp)));

        m_CurrentSelectedBox = 0;

        connect(ui->B_Home,SIGNAL(clicked()), this,SLOT(ReturnSettingsMenu()));
        connect(ui->B_DoorTest,SIGNAL(clicked()), this,SLOT(GotoDoorsTest()));
        connect(ui->B_AddDel_Door,SIGNAL(clicked()), this,SLOT(GotoAddDelDoors()));
        connect(ui->B_ActDes_Door,SIGNAL(clicked()), this,SLOT(GotoActDesDoors()));
        connect(ui->B_TestValid,SIGNAL(clicked()), this,SLOT(CheckTestBox()));
        connect(ui->B_UnlockBox,SIGNAL(clicked()), this,SLOT(UnlockBox()));
        connect(ui->B_OpenBox,SIGNAL(clicked()), this,SLOT(OpenBox()));
        connect(ui->B_CancelDoorsTest,SIGNAL(clicked()), this,SLOT(GotoDoorsManager()));
        connect(ui->B_ReturnDoorsManager,SIGNAL(clicked()), this,SLOT(GotoDoorsManager()));
        connect(ui->B_ReturnDoorsManager_2,SIGNAL(clicked()), this,SLOT(GotoDoorsManager()));
        connect(ui->B_ReturnDoorsManager_3,SIGNAL(clicked()), this,SLOT(GotoDoorsManager()));
        connect(ui->B_TestConfirm,SIGNAL(clicked()), this,SLOT(GotoDoorsTest()));
        connect(ui->B_Return_ActDes,SIGNAL(clicked()), this,SLOT(GotoActDesDoors()));
        connect(ui->B_ActivateDoor,SIGNAL(clicked()), this,SLOT(ActivateDoor()));
        connect(ui->B_DesactivateDoor,SIGNAL(clicked()), this,SLOT(DesactivateDoor()));
        connect(ui->B_MaintenanceMenu,SIGNAL(clicked()), this,SLOT(ReturnMaintenanceMenu()));
        connect(ui->CheckBox_Open_Yes, SIGNAL(clicked()), this,SLOT(ActionOpenBox_ConfirmSwap()));
        connect(ui->CheckBox_Open_No, SIGNAL(clicked()), this,SLOT(ActionOpenBox_ConfirmSwap()));
        connect(ui->CheckBox_Unlock_Yes,SIGNAL(clicked()), this,SLOT(ActionUnlockBox_ConfirmSwap()));
        connect(ui->CheckBox_Unlock_No,SIGNAL(clicked()), this,SLOT(ActionUnlockBox_ConfirmSwap()));
        connect(this,SIGNAL(StartRunActDesBox(int)), this,SLOT(RunActDesBox(int)));

        ui->stackedWidget_DoorsManager->setCurrentIndex(WINDEX_DOORS_MANAGER);
    }
    else
        qDebug() << "[FATAL ERROR] : Uninitiatized pointer on object";
}

void DoorsManager::CreateBoxesViewTest()
{
    QList<int> DoorsListTmp;
    QList<int> DoorsList;

    m_FreeBoxesList = m_DB->GetFreeBoxes();

    DoorsList = m_DB->Get_All_idBox();
    m_AllBoxesStatusList.append(BoxesStatusSort(m_Doors->DoorsStatus(DoorsList)));

    for(int i = 0; i <m_AllBoxesStatusList.size();i++)
    {
        CustomPushButton *buttonTestDoor;
        buttonTestDoor = new CustomPushButton(m_AllBoxesStatusList[i].BoxNumber,tr("TESTER"));
        connect(buttonTestDoor,SIGNAL(clicked(int)),this,SLOT(RunTestBox(int)));

        m_BoxesView_Test->AddItem(m_AllBoxesStatusList[i].BoxNumber,buttonTestDoor);
        m_BoxesView_Test->ButtonVisible(m_AllBoxesStatusList[i].BoxNumber,false);

        if( (m_FreeBoxesList.contains(m_AllBoxesStatusList[i].BoxNumber) == false) )    // Doors X is Free : no
        {
            if( m_AllBoxesStatusList[i].BoxIsDammaged == false )    // Door is not free but not dammaged
                m_BoxesView_Test->EditedItem_ColumnStatusOfDoor(m_AllBoxesStatusList[i].BoxNumber, tr("OCCUPEE"), QColor(Qt::red));

            m_BoxesView_Test->ButtonVisible(m_AllBoxesStatusList[i].BoxNumber,true);
        }
        else    // Doors X is Free : yes
            m_BoxesView_Test->EditedItem_ColumnStatusOfDoor(m_AllBoxesStatusList[i].BoxNumber, tr("LIBRE"), QColor(Qt::green));

        if(m_AllBoxesStatusList[i].BoxIsDammaged == true)   // Door is dammaged
            m_BoxesView_Test->EditedItem_ColumnStatusOfDoor(m_AllBoxesStatusList[i].BoxNumber, tr("DEFAUT SIGNALE"), QColor(Qt::yellow));

        if( (m_AllBoxesStatusList[i].BoxIsUsed == true) && (m_AllBoxesStatusList[i].BoxIsDammaged == true) )    //door is dammaged dans used
        {
            // new custom button
            m_BoxesView_Test->RemoveLastItem();

            buttonTestDoor = new CustomPushButton(m_AllBoxesStatusList[i].BoxNumber,tr("CORRIGER"));
            connect(buttonTestDoor,SIGNAL(clicked(int)),this,SLOT(RunTestBox(int)));

            m_BoxesView_Test->AddItem(m_AllBoxesStatusList[i].BoxNumber,buttonTestDoor);
            m_BoxesView_Test->EditedItem_ColumnStatusOfDoor(m_AllBoxesStatusList[i].BoxNumber, tr("OCCUPEE + DEFAUT"), QColor(Qt::darkRed));
            m_BoxesView_Test->ButtonVisible(m_AllBoxesStatusList[i].BoxNumber,false);
        }
    }
}

void DoorsManager::CreateBoxesViewActDes()
{
    QList<int> DoorsListTmp;
    QList<int> DoorsList;

    DoorsList = m_DB->Get_All_idBox();
    m_AllBoxesStatusList.append(BoxesStatusSort(m_Doors->DoorsStatus(DoorsList)));

    for(int i = 0 ; i <m_AllBoxesStatusList.size();i++)
    {
        CustomPushButton *buttonActDesDoor;
        buttonActDesDoor = new CustomPushButton(m_AllBoxesStatusList[i].BoxNumber,tr("GERER"));
        connect(buttonActDesDoor,SIGNAL(clicked(int)),this,SLOT(RunActDesBox(int)));

        m_BoxesView_ActDes->AddItem(m_AllBoxesStatusList[i].BoxNumber,buttonActDesDoor);
        m_BoxesView_ActDes->ButtonVisible(m_AllBoxesStatusList[i].BoxNumber,false);

        if(m_AllBoxesStatusList[i].BoxIsUsed)
        {
            if(m_AllBoxesStatusList[i].BoxIsDammaged)
            {
                m_BoxesView_ActDes->EditedItem_ColumnStatusOfDoor(m_AllBoxesStatusList[i].BoxNumber, tr("OCCUPEE + DEFAUT"), QColor(Qt::darkRed));
                disconnect(buttonActDesDoor,SIGNAL(clicked(int)),this,SLOT(RunActDesBox(int)));
                connect(buttonActDesDoor,SIGNAL(clicked(int)),this,SLOT(RunTestBox(int)));
            }
            else
                m_BoxesView_ActDes->EditedItem_ColumnStatusOfDoor(m_AllBoxesStatusList[i].BoxNumber, tr("DESACTIVEE"), QColor(Qt::red));
        }
        else
            m_BoxesView_ActDes->EditedItem_ColumnStatusOfDoor(m_AllBoxesStatusList[i].BoxNumber, tr("ACTIVEE"), QColor(Qt::green));
    }
}

QList<struct_DoorsStatus> DoorsManager::BoxesStatusSort(QList<struct_DoorsStatus> listtmp)
{
    QList<struct_DoorsStatus> listbox;
    bool isExist = false;

    for(int i=0;i < listtmp.size();i++)
    {
       isExist = false;
        for(int j=i+1; j < listtmp.size();j++)
        {
            if(listtmp[i].BoxNumber==listtmp[j].BoxNumber)
                isExist = true;
        }
        if(!isExist)
            listbox.append(listtmp[i]);
    }
    return listbox;
}

void DoorsManager::GotoDoorsManager()
{
    ui->stackedWidget_DoorsManager->setCurrentIndex(WINDEX_DOORS_MANAGER);
}

void DoorsManager::GotoDoorsTest()
{
    m_BoxesView_Test->ClearListItem();
    ui->TW_BoxesView_Test->clear();
    m_AllBoxesStatusList.clear();
    CreateBoxesViewTest();

    ui->CheckBox_Unlock_No->setChecked(false);
    ui->CheckBox_Unlock_Yes->setChecked(false);
    ui->CheckBox_Open_Yes->setChecked(false);
    ui->CheckBox_Open_No->setChecked(false);

    ui->stackedWidget_DoorsManager->setCurrentIndex(WINDEX_DOORS_TEST);
}

void DoorsManager::GotoAddDelDoors()
{
    ui->stackedWidget_DoorsManager->setCurrentIndex(WINDEX_ADD_DEL_DOORS);
}

void DoorsManager::GotoActDesDoors()
{
    m_BoxesView_ActDes->ClearListItem();
    ui->TW_BoxesView_ActDes->clear();
    m_AllBoxesStatusList.clear();
    CreateBoxesViewActDes();

    ui->stackedWidget_DoorsManager->setCurrentIndex(WINDEX_ACT_DES_DOORS);
}

void DoorsManager::UnlockBox()
{
    m_UnlockBoxTested = true;

    if(m_Doors->UnlockDoor(m_CurrentSelectedBox))
    {
        ui->CheckBox_Unlock_No->setChecked(false);
        ui->CheckBox_Unlock_Yes->setChecked(true);
    }
    else
    {
        ui->CheckBox_Unlock_Yes->setChecked(false);
        ui->CheckBox_Unlock_No->setChecked(true);
    }
}

void DoorsManager::OpenBox()
{
    m_OpenBoxTested = true;

    if(m_Doors->OpenDoor(m_CurrentSelectedBox))
    {
        ui->CheckBox_Open_No->setChecked(false);
        ui->CheckBox_Open_Yes->setChecked(true);
    }
    else
    {
        ui->CheckBox_Open_Yes->setChecked(false);
        ui->CheckBox_Open_No->setChecked(true);
    }
}

void DoorsManager::CheckTestBox()
{
    QList<int> DoorList;
    DoorList.append(m_CurrentSelectedBox);

    if ( (m_OpenBoxTested && m_UnlockBoxTested) == true )
    {
        if( (ui->CheckBox_Open_Yes->isChecked() == false) && (ui->CheckBox_Unlock_Yes->isChecked() == false) &&
            (ui->CheckBox_Open_No->isChecked() == false) && (ui->CheckBox_Unlock_No->isChecked() == false) )
        {
            qDebug() << "Any Checkbox checked !";
            int x;
            int y;
            x = mapToGlobal(ui->B_Assistance->pos()).x()-ui->B_Assistance->width()-100;
            y = mapToGlobal(ui->B_Assistance->pos()).y()-10;
            QToolTip::showText(QPoint(x,y),tr("Attention!!!<br/> Vous n'avez pas s&eacute;lectionn&eacute; de réponse aux tests !"),ui->B_Assistance);
        }
        else
        {
            if( (ui->CheckBox_Open_Yes->isChecked()) && (ui->CheckBox_Unlock_Yes->isChecked()) ) // No fail on the door
            {
                QList<int> DammagedBoxesList;

                DammagedBoxesList = m_DB->GetDammagedBoxes();
                if(DammagedBoxesList.contains(m_CurrentSelectedBox))
                {
                    m_DB->UnsetDammagedBox(m_CurrentSelectedBox);
                    ui->L_TestConfirm->setText("La porte "+ QString::number(m_CurrentSelectedBox,10) +" à été retirée des portes défectueses !>");
                }
                else
                {
                    ui->L_TestConfirm->setText("La porte "+ QString::number(m_CurrentSelectedBox,10) +" est fonctionnelle !");
                }
            }
            else
            {
                m_DB->SetDammagedBox(m_CurrentSelectedBox);
                ui->L_TestConfirm->setText("La porte "+ QString::number(m_CurrentSelectedBox,10) +" à été ajoutée aux portes défectueses !");
            }

            ui->stackedWidget_DoorsManager->setCurrentIndex(WINDEX_DOORS_TEST_CONFIRM);
            m_OpenBoxTested = false;
            m_UnlockBoxTested = false;
        }
    }
    else
    {
        qDebug() << "Any test executed !";
        int x;
        int y;
        x = mapToGlobal(ui->B_Assistance->pos()).x()-ui->B_Assistance->width()-100;
        y = mapToGlobal(ui->B_Assistance->pos()).y()-10;
        QToolTip::showText(QPoint(x,y),tr("Attention!!!<br/> Vous n'avez pas executé de tests !"),ui->B_Assistance);
    }
}

void DoorsManager::ActionOpenBox_ConfirmSwap()
{
    if(sender() == ui->CheckBox_Open_Yes)
        ui->CheckBox_Open_No->setChecked(false);

    if(sender() == ui->CheckBox_Open_No)
        ui->CheckBox_Open_Yes->setChecked(false);
}

void DoorsManager::ActionUnlockBox_ConfirmSwap()
{
    if(sender() == ui->CheckBox_Unlock_Yes)
        ui->CheckBox_Unlock_No->setChecked(false);

    if(sender() == ui->CheckBox_Unlock_No)
        ui->CheckBox_Unlock_Yes->setChecked(false);
}

void DoorsManager::ActivateDoor()
{
    if(m_DB->UnsetDammagedBox(m_CurrentSelectedBox))
        ui->L_ActDesDoorStatuts->setText("La porte "+ QString::number(m_CurrentSelectedBox,10) +" à été retirée des portes défectueuses ");
    else
        ui->L_ActDesDoorStatuts->setText("ERREUR ! La porte "+ QString::number(m_CurrentSelectedBox,10) +" n'est pas activée !");

    emit StartRunActDesBox(m_CurrentSelectedBox);
}

void DoorsManager::DesactivateDoor()
{
    if(m_DB->SetDammagedBox(m_CurrentSelectedBox))
        ui->L_ActDesDoorStatuts->setText("La porte "+ QString::number(m_CurrentSelectedBox,10) +" à été ajoutée aux portes défectueuses ");
    else
        ui->L_ActDesDoorStatuts->setText("ERREUR ! La porte "+ QString::number(m_CurrentSelectedBox,10) +" n'est pas désactivée !");

    emit StartRunActDesBox(m_CurrentSelectedBox);
}

void DoorsManager::RunActDesBox(int idBox)
{
    ui->stackedWidget_DoorsManager->setCurrentIndex(WINDEX_DOORS_ACT_DES_COMMAND);
    m_CurrentSelectedBox = idBox;
    ui->L_ActDesDoor->setText("Actions sur la porte "+ QString::number(m_CurrentSelectedBox,10) + " :");

    if(m_DB->BoxIsDammaged(m_CurrentSelectedBox))
    {
        ui->L_ActDesDoorStatuts->setText("La porte "+ QString::number(m_CurrentSelectedBox,10) +" est DESACTIVEE");
        ui->B_DesactivateDoor->setEnabled(false);
        ui->B_ActivateDoor->setEnabled(true);
    }
    else
    {
        ui->L_ActDesDoorStatuts->setText("La porte "+ QString::number(m_CurrentSelectedBox,10) +" est ACTIVEE");
        ui->B_ActivateDoor->setEnabled(false);
        ui->B_DesactivateDoor->setEnabled(true);
    }
}

DoorsManager::~DoorsManager()
{
    delete ui;
}

void DoorsManager::RunTestBox(int idBox)
{
    ui->stackedWidget_DoorsManager->setCurrentIndex(WINDEX_DOORS_TEST_ACTIONS);
    m_CurrentSelectedBox = idBox;
    ui->L_DoorNumber->setText("Actions sur la porte "+ QString::number(m_CurrentSelectedBox,10) + " :");
}
