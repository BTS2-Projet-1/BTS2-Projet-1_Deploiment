#include "scanpackage.h"
#include "ui_scanpackage.h"

#include <QScrollBar>
#include <QToolTip>

#include <QDebug>

#define PAGE_LISTPACKAGE 0
#define PAGE_KEYBOARD    1

#define ERROR_LISTEMPTY  -1
#define ERROR_BOXEMPTY   -2


ScanPackage::ScanPackage(BoxChoice *WBoxChoice, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanPackage)
{
    ui->setupUi(this);
    m_ShowWidgets = NULL;

    m_WBoxChoice = WBoxChoice;
    m_idPushButton = 0;

    QRegExpValidator *reVal = new QRegExpValidator( QRegExp("^(([0-9])|([A-Z])|([a-z]))+$"), this );
    ui->Edit_ncoli->setValidator(reVal);
    m_page = 0;
    m_labelbox = new QLabel(ui->page);
    m_labelbox->setGeometry(0,0,120,350);
    m_labelbox->setFixedHeight(350);
    m_labelbox->setMaximumWidth(120);
    m_labelbox->setFocus(Qt::NoFocusReason);

    m_labelbox->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    m_labelbox->setFont(QFont("Helvetica",15,QFont::Bold,true));
    m_labelbox->setStyleSheet("color:white;background:transparent;");

    //connection aux boutons
    connect(ui->WKeyBoard,SIGNAL(ButtonOKCliked()),this,SLOT(StockNumberPackage()));
    connect(ui->WKeyBoard,SIGNAL(SendCarac(QChar)),this,SLOT(SetCaract(QChar)));
    connect(ui->WKeyBoard,SIGNAL(SupprLastCaract()),this,SLOT(SupprCaract()));
    connect(ui->B_Cancel,SIGNAL(clicked()),this,SLOT(CancelDelivery()));
    connect(ui->B_Valide,SIGNAL(clicked()),this,SLOT(ValidateDelivery()));
    connect(m_WBoxChoice,SIGNAL(SendListBoxSelected(QList<int>)),this,SLOT(ReceiveListBoxSelected(QList<int>)));
    connect(ui->B_Yes,SIGNAL(clicked()),this,SLOT(ConfirmValidate()));
    connect(ui->B_Non,SIGNAL(clicked()),this,SLOT(ReturnListPackage()));
    connect(ui->Edit_ncoli,SIGNAL(textChanged(QString)),this,SLOT(Upper(QString)));
    connect(ui->B_ModeManual,SIGNAL(clicked()),this,SLOT(SetManual()));
    connect(ui->B_ModeScanCodeBarre,SIGNAL(clicked()),this,SLOT(SetScanner()));
    connect(ui->B_ChangePos,SIGNAL(clicked()),this,SLOT(ChangePage()));
    connect(this,SIGNAL(TextToUpper(QString)),ui->Edit_ncoli,SLOT(setText(QString)));

    //affiche le clavier virtuel mode
    ui->WKeyBoard->setVisible(true);
    ui->WKeyBoard->setTextButtonOK(tr("Ajouter"));


}

ScanPackage::~ScanPackage()
{
    delete m_labelbox;
    delete ui;
}

void ScanPackage::Upper(QString str)
{
    str = str.toUpper();
    emit TextToUpper(str);
}

void ScanPackage::SetCaract(QChar caract)
{
    ui->Edit_ncoli->insert(QString(caract));
}

void ScanPackage::SupprCaract()
{
    ui->Edit_ncoli->backspace();
}

void ScanPackage::ChangePage()
{

    switch(m_page)
    {
        case PAGE_LISTPACKAGE://affiche page clavier
                             m_page = PAGE_KEYBOARD;
                             ui->B_ChangePos->setIcon(QIcon(":/ImgList/list"));
                             ui->stackedWidget_2->setCurrentIndex(PAGE_KEYBOARD);
                             break;

        case PAGE_KEYBOARD://affiche page listpackage
                             m_page = PAGE_LISTPACKAGE;
                             ui->B_ChangePos->setIcon(QIcon(":/ImgList/keyboard"));
                             ui->stackedWidget_2->setCurrentIndex(PAGE_LISTPACKAGE);
                             break;
    }

}

void ScanPackage::SetManual()
{
    ui->stackedWidget_2->setCurrentIndex(PAGE_KEYBOARD);
    ui->B_ChangePos->setIcon(QIcon(":/ImgList/list"));
    m_page = PAGE_KEYBOARD;
    ui->stackedWidget->setCurrentIndex(1);
}

void ScanPackage::SetScanner()
{

    //scanner
    ui->stackedWidget_2->setCurrentIndex(PAGE_LISTPACKAGE);
    ui->B_ChangePos->setIcon(QIcon(":/ImgList/keyboard"));
    m_page = PAGE_LISTPACKAGE;
    ui->stackedWidget->setCurrentIndex(1);
}

void ScanPackage::ReceiveListBoxSelected(QList<int> listbox)
{
  Refresh();
  m_ListBoxselected = listbox;
  QString box;

  for(int i = 0; i < m_ListBoxselected.size();i++)
  {
      if(i < m_ListBoxselected.size()-1)
      {
         box += QString::number(m_ListBoxselected[i]) + QString(" , ");
      }

      if(i == m_ListBoxselected.size()-1)
      {
          box += QString::number(m_ListBoxselected[i]) + QString(".");
      }
  }

  if(m_ListBoxselected.size() > 1)
  {
      ui->Label_BoxSelected->setText(trUtf8("Vous avez sélectionné les consignes : \n") + box);
      box.clear();
      for(int i = 0; i < m_ListBoxselected.size();i++)
      {
          if(i < m_ListBoxselected.size()-1)
          {
             box += QString::number(m_ListBoxselected[i]) + QString(",\n      ");
          }

          if(i == m_ListBoxselected.size()-1)
          {
              box += QString::number(m_ListBoxselected[i]) + QString(".");
          }
      }
      m_labelbox->setText(trUtf8("Consignes:\n      ")+box);

  }
  if(m_ListBoxselected.size() == 1)
  {
      ui->Label_BoxSelected->setText(trUtf8("Vous avez sélectionné la consigne : \n")+ box );
      m_labelbox->setText(trUtf8("Consigne: ")+box);

  }


}

void ScanPackage::StockNumberPackage()
{

  if(ui->Edit_ncoli->text().compare("") != 0)
  {
    m_ListPackagesString << ui->Edit_ncoli->text();
    AddItemListWidget(ui->Edit_ncoli->text());
  }
    ui->Edit_ncoli->clear();
}

bool ScanPackage::isExistNumPackage(QString NumPackage)
{
   bool result = false;

   for(int i = 0;i<ui->ListPackages->count();i++)
   {
       if(ui->ListPackages->item(i)->text().compare(NumPackage) == 0)
       {
           result = true;
       }
   }

   return result;
}

void ScanPackage::ShowMessageError(QString mes)
{
    int x;
    int y;
    x = mapToGlobal(ui->stackedWidget->pos()).x()+ui->stackedWidget->width()/2;
    y = mapToGlobal(ui->stackedWidget->pos()).y()+ui->stackedWidget->height()/2;
    QToolTip::showText(QPoint(x,y),mes,this);
}

void ScanPackage::AddItemListWidget(QString NumPackage)
{

    if(!isExistNumPackage(NumPackage))
    {
        QListWidgetItem *item;
        CustomPushButton *Button_Suppr;

        item = new QListWidgetItem(NumPackage);
        Button_Suppr = new CustomPushButton(m_idPushButton,NumPackage,":/ImgList/BSuppr");
        qDebug()<<NumPackage;

        connect(Button_Suppr,SIGNAL(clicked(QString)),this,SLOT(DeleteNumPackage(QString)));

        Button_Suppr->setGeometry(70,70,ui->ListPackages->width()/2,0);
        QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight);
        layout->addWidget(Button_Suppr,0,(Qt::AlignRight));

        QWidget *widget = new QWidget();
        widget->setLayout(layout);
        widget->setStyleSheet("background:transparent;");

        ui->ListPackages->addItem(item);
        item->setTextAlignment(Qt::AlignVCenter);
        ui->ListPackages->setItemWidget(item,widget);

        ui->ListPackages->setContentsMargins(5,5,5,5);
        m_ListCustPusButton << Button_Suppr;
        m_idPushButton++;
    }
    else{

        ShowMessageError(tr("Erreur!!!<br/> Le num&eacutero de colis a d&eacutej&agrave; s&eacutets&eacute saisi."));
    }
}

//supprimer le numero de colis  dans la liste
void ScanPackage::DeleteNumPackage(QString package)
{
    int index;
    for(int i = 0 ; i < m_ListPackagesString.size();i++)
    {
        if(m_ListPackagesString[i].compare(package) == 0)
        {
            index = i;
        }
    }

    m_ListPackagesString.removeAt(index);

    QListWidgetItem *item;
    item = ui->ListPackages->takeItem(index);
    ui->ListPackages->removeItemWidget(item);
    m_ListCustPusButton.removeAt(index);


    qDebug()<< m_ListPackagesString;

}

void ScanPackage::ValidateDelivery()
{

    int result;

    //CONSTRUCTION DE LA LISTE
    result = BuildingListPackages();

    switch (result) {

        case ERROR_BOXEMPTY:
                            ShowMessageError(tr("Erreur!!!<br/> Une consigne n'est pas utilis&eacute;.<br>Veuillez saisir un num&eacute;ro de colis ou annuler."));
                            break;

        case ERROR_LISTEMPTY:
                           ShowMessageError(tr("Erreur!!!<br/> Liste vide."));
                            break;
        case OK :
                 //affiche confirmation de la validation
                 ui->stackedWidget->setCurrentIndex(2);
                 break;

        default:

                break;
    }


}

void ScanPackage::CancelDelivery()
{
    //renvoie sur imh selection des consignes
    Refresh();
    m_ShowWidgets->ShowWBoxChoice();
}

void ScanPackage::Refresh()
{
    m_listPackageBox.clear();
    m_ListPackagesString.clear();
    ui->Edit_ncoli->clear();
    ui->ListPackages->clear();
    m_ListBoxselected.clear();
    m_idPushButton = 0;
    m_ListCustPusButton.clear();
    ui->stackedWidget->setCurrentIndex(0);

}

int ScanPackage::BuildingListPackages()
{
    int result ;
    int sizeListPackages;
    int nbBox;

    nbBox = m_ListBoxselected.size();
    sizeListPackages = m_ListPackagesString.size();
    struct_PackagesUseBox tempPackage;

    m_listPackageBox.clear();

    if(!m_ListPackagesString.isEmpty())
    {
        for(int i = 0 ; i < nbBox ; i++)
        {
            tempPackage.BoxNumber = m_ListBoxselected[i];
            m_listPackageBox.append(tempPackage);

        }
        if(m_ListBoxselected.size() <= m_ListPackagesString.size())
        {
            for(int i = 0 ; i < m_listPackageBox.size(); i++)
            {
                m_listPackageBox[i].BoxContainment.PackageDeliveryCodeList << m_ListPackagesString[i];

            }

            // si reste des colis
            if(sizeListPackages > nbBox)
            {
                for(int i = nbBox ; i < sizeListPackages ;i++)
                {
                    m_listPackageBox[0].BoxContainment.PackageDeliveryCodeList << m_ListPackagesString[i];
                }
            }
            result = OK;
        }
        else{
            result = ERROR_BOXEMPTY;
        }
    }
    else{
        result = ERROR_LISTEMPTY;
    }

    return result;
}

void ScanPackage::ConfirmValidate()
{

    //envoi la liste des n°colis
    emit SendListPackageBox(m_listPackageBox);
    emit ShowStowPackage();

    //affiche rangement des colis
    m_ShowWidgets->ShowWStowPackages();
    ui->stackedWidget->setCurrentWidget(ui->page);

}

void ScanPackage::ReturnListPackage()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void ScanPackage::retranslateUi()
{
    ui->retranslateUi(this);
    ui->WKeyBoard->setTextButtonOK(tr("Ajouter"));

}
