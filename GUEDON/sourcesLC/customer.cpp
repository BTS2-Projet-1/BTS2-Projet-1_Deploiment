#include "customer.h"
#include "ui_customer.h"

#include<QToolTip>

#include <QDebug>

#define VALID   1
#define UNVALID 0

Customer::Customer(CSQLite_Local_DB *bd, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Customer)
{
    ui->setupUi(this);
    m_BD = bd;
    m_ShowWidgets = NULL;

    m_regExp = new QRegExpValidator( QRegExp("^(([0-9])|([A-Z])|([a-z]))+$"), this );//Suppression des caractère spéciaux
    ui->lineEdit->setValidator(m_regExp);

    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(Upper(QString)));
    connect(this,SIGNAL(TextToUpper(QString)),ui->lineEdit,SLOT(setText(QString)));

    connect(ui->B_Home,SIGNAL(clicked()),this,SLOT(ShowHome()));
    connect(ui->WKeyBoard,SIGNAL(SendCarac(QChar)),this,SLOT(SetCaract(QChar)));
    connect(ui->WKeyBoard,SIGNAL(SupprLastCaract()),this,SLOT(SupprCaract()));
    connect(ui->WKeyBoard,SIGNAL(ButtonOKCliked()),this,SLOT(Valid_Code()));

    ui->WKeyBoard->setVisible(true);
    ui->WKeyBoard->set_ABC();

}

Customer::~Customer()
{
    delete ui;
}

void Customer::retranslateUi()
{
    ui->retranslateUi(this);
    ui->WKeyBoard->retranslateUi();
    ui->WKeyBoard->setTextButtonOK(tr("Valider"));
}


void Customer::ShowHome()
{
    ui->lineEdit->clear();
    m_ShowWidgets->ShowWConsoleWindows();
}


void Customer::Valid_Code()
{
    int result;
    int x;
    int y;
    QString ExtractCode;

    m_listboxcustomer.clear();

    ExtractCode = ui->lineEdit->text();

    if(ExtractCode.compare("")!=0)
    {

        //verification du code de retrait dans la BD Local
          result = m_BD->CheckExtractCode(ExtractCode);
          qDebug()<<"result verif code retrait:"<<result;

         //si le code de retrait est valide
            //demande d'ouverture des consignes consernées
          if(result == VALID)
          {
            //récupération de la liste des consignes à l'aide du code de retrait
              m_listboxcustomer = m_BD->GetBoxesListConsumer(ExtractCode);

              //envoi la liste des portes selectionner et le code de retrait
              emit SendListbox(m_listboxcustomer);
              emit SendCode(ExtractCode);

              //affichage des consignes ouvertes
              m_ShowWidgets->ShowWPackageBox();

          }
          else{
              //sinon code incorrecte

              x = mapToGlobal(ui->B_Assistance->pos()).x()-ui->B_Assistance->width()-230;
              y = mapToGlobal(ui->B_Assistance->pos()).y()-10;
              QToolTip::showText(QPoint(x,y),tr("Erreur!!!<br/> Le code de retrait est incorrect.<br/> Veuillez r&eacute;essayer."),ui->B_Assistance);
          }
    }
    else{
        //message erreur
        x = mapToGlobal(ui->lineEdit->pos()).x()+ui->lineEdit->width()/4;
        y = mapToGlobal(ui->lineEdit->pos()).y()-ui->lineEdit->height();
        QToolTip::showText(QPoint(x,y),tr("Erreur!!!,Vous n'avez rien tapez"),ui->lineEdit);

    }

    ui->lineEdit->clear();

}

void Customer::SetCaract(QChar caract)
{
    ui->lineEdit->insert(QString(caract));
}

void Customer::SupprCaract()
{   
    ui->lineEdit->backspace();
}

void Customer::Upper(QString str)
{
    str = str.toUpper();
    emit TextToUpper(str);
}

//button slots
void Customer::on_B_Home_clicked()
{
    //affiche console windows
    ui->lineEdit->clear();
    m_ShowWidgets->ShowWConsoleWindows();
}

void Customer::on_B_Assistance_clicked()
{
    //affiche assistance
    m_ShowWidgets->ShowWAssistance();

}

void Customer::on_B_Help_clicked()
{
    //affiche aide
}
