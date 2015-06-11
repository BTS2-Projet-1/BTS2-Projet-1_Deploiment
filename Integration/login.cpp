#include "login.h"
#include "ui_login.h"
#include <QDebug>
#include <QValidator>
#include <QRegExpValidator>
#include <QScrollBar>
#include <QToolTip>

#define MAINTENANCE    1
#define DELIVERYMAN    0
#define ERROR_PASSWD  -1
#define ERROR_ID      -2
#define ERROR_COMPANY -3
#define ERROR_LOGIN   -4

#define ED_SOCIETE     0
#define ED_ID          1
#define ED_PASSWD      2

Login::Login(CSQLite_Local_DB *BD, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    m_BD = BD;//base de donnée

    QRegExpValidator *reVal = new QRegExpValidator(QRegExp("^(([0-9])|([A-Z])|([a-z])){6}$"),this);
    ui->Edit_societe->setValidator(reVal);
    ui->Edit_personnel->setValidator(reVal);
    ui->Edit_passwd->setValidator(reVal);

    ui->P_Societe->installEventFilter(this);
    ui->P_ID->installEventFilter(this);
    ui->P_Password->installEventFilter(this);

    connect(ui->B_Home,SIGNAL(clicked()),this,SLOT(Cancel()));
    connect(ui->B_Suivant,SIGNAL(clicked()),this,SLOT(NextPage()));
    connect(ui->B_Precedant,SIGNAL(clicked()),this,SLOT(BeforePage()));
    connect(ui->WKeyBoard,SIGNAL(SendCarac(QChar)),this,SLOT(SetCaract(QChar)));
    connect(ui->WKeyBoard,SIGNAL(SupprLastCaract()),this,SLOT(SupprCaract()));
    connect(ui->WKeyBoard,SIGNAL(ButtonOKCliked()),this,SLOT(ValidLogin()));

    //affiche le clavier virtuel mode pavé numerique
    ui->WKeyBoard->setVisible(true);
    ui->WKeyBoard->set_ABC();

}

void Login::NextPage()
{
    if(ui->stackedWidget->currentIndex() == 2)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else{
        int indexCurrent = ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex(indexCurrent+1);
    }
}

void Login::BeforePage()
{
    if(ui->stackedWidget->currentIndex() == 0)
    {
        ui->stackedWidget->setCurrentIndex(2);
    }
    else{
        int indexCurrent = ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex(indexCurrent-1);
    }
}


bool Login::eventFilter(QObject *obj, QEvent *event)
{
    bool result = false;
    int x;
    int y;

    x = mapToGlobal(ui->B_Assistance->pos()).x()-ui->B_Assistance->width()-140;
    y = mapToGlobal(ui->B_Assistance->pos()).y()-10;

    if(obj == ui->P_Societe && event->type() == QEvent::Show)
    {
       m_LineEdit_Selected = ED_SOCIETE;

       QToolTip::showText(QPoint(x,y),tr("Cliquez sur la fl&egrave;che &agrave; droite pour acc&eacute;der au champ :<br/> Identifiant Personnelle."),ui->B_Assistance);

       result = true;
    }

    if(obj == ui->P_ID && event->type() == QEvent::Show)
    {
       m_LineEdit_Selected = ED_ID;
       QToolTip::showText(QPoint(x,y),tr("Cliquez sur la fl&egrave;che &agrave; droite pour acc&eacute;der au champ :<br/> Mot de Passe."),ui->B_Assistance);
       result = true;
    }

    if(obj == ui->P_Password && event->type() == QEvent::Show)
    {
       m_LineEdit_Selected = ED_PASSWD;
       QToolTip::showText(QPoint(x,y),tr("Si vous avez &eacute;crit dans tous les champs<br/> Cliquez sur Connexion."),ui->B_Assistance);
       result = true;
    }

    return result;
}

Login::~Login()
{
    delete ui;
}

void Login::retranslateUi()
{
    ui->retranslateUi(this);
    ui->WKeyBoard->retranslateUi();
    ui->WKeyBoard->setTextButtonOK(tr("Connexion"));
}

void Login::Cancel()
{
    ui->Edit_passwd->clear();
    ui->Edit_personnel->clear();
    ui->Edit_societe->clear();

    //retour à l'ihm home
    m_ShowWidgets->ShowWConsoleWindows();
}

void Login::ValidLogin()
{
    int result;
    int x;
    int y;

    QString Societe;
    QString Id;
    QString Passwd;
    QString code;


    Societe = ui->Edit_societe->text();
    Id = ui->Edit_personnel->text();
    Passwd = ui->Edit_passwd->text();

    if((Societe !="") && (Id !="") && (Passwd!="")) //TEST si l'utilisateur a ecrit dans tous les champs
    {
        code = Societe + Id + Passwd;
        qDebug()<<"login : "<< code;

        //authentification
        result = m_BD->Login(code);


        x = mapToGlobal(ui->B_Assistance->pos()).x()-ui->B_Assistance->width()-230;
        y = mapToGlobal(ui->B_Assistance->pos()).y()-10;


        switch (result) {

            case DELIVERYMAN :
                                //affichage l'ihm menu livraison
                                m_ShowWidgets->ShowWMenuDelivery();
                                break;

            case MAINTENANCE:
                                //affichage l'ihm maintenance
                                m_ShowWidgets->ShowWSetting();
                                break;


            case ERROR_ID:
                                ui->Edit_personnel->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid white;border-radius:10px;color:red");
                                ui->Edit_passwd->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid white;border-radius:10px;color:red");
                                QToolTip::showText(QPoint(x,y),tr("Erreur!!!<br/>Votre Identifiant personnel et votre Mot de passe sont incorrect.<br/>R&eacute;essayer."),this);
                                break;

            case ERROR_PASSWD:
                                ui->Edit_passwd->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid white;border-radius:10px;color:red");
                                QToolTip::showText(QPoint(x,y),tr("Erreur!!!<br/>Votre Mot de passe est incorrect.<br/>R&eacute;essayer."),this);
                                break;

            default:            //si ERROR_LOGIN ou ERROR_COMPANY
                                ui->Edit_societe->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid white;border-radius:10px;color:red");
                                ui->Edit_personnel->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid white;border-radius:10px;color:red");
                                ui->Edit_passwd->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid white;border-radius:10px;color:red");
                                QToolTip::showText(QPoint(x,y),tr("Erreur de connexion!!!<br/>Vos Identifiants sont incorrect.<br/>R&eacute;essayer."),this);
                                qDebug()<<"login incorrect";
                                break;
        }
    }
    else{

        x = mapToGlobal(ui->B_Assistance->pos()).x()-ui->B_Assistance->width()-150;
        y = mapToGlobal(ui->B_Assistance->pos()).y()-10;
        QToolTip::showText(QPoint(x,y),tr("Erreur!!!<br/>Les champs sont vides."),this);
    }

}

void Login::Reset()
{
    ui->Edit_passwd->clear();
    ui->Edit_personnel->clear();
    ui->Edit_societe->clear();
    ui->Edit_societe->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid white;border-radius:10px;white;color:black");
    ui->Edit_personnel->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid white;border-radius:10px;color:black");
    ui->Edit_passwd->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid white;border-radius:10px;color:black");
    ui->stackedWidget->setCurrentIndex(0);

}

void Login::SetCaract(QChar carac)
{

    ui->Edit_societe->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid white;border-radius:10px;color:black");
    ui->Edit_personnel->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid white;border-radius:10px;color:black");
    ui->Edit_passwd->setStyleSheet("background-color: rgb(255, 255, 255);border:1px solid white;border-radius:10px;color:black");

    switch(m_LineEdit_Selected)
    {
        case ED_SOCIETE:

                        ui->Edit_societe->insert(QString(carac));
                        break;

        case ED_ID:                     
                        ui->Edit_personnel->insert(QString(carac));
                        break;

        case ED_PASSWD:

                        ui->Edit_passwd->insert(QString(carac));
                        break;
        default:
                        break;

    }
}

void Login::SupprCaract()
{

    switch(m_LineEdit_Selected)
    {
        case ED_SOCIETE:
                        ui->Edit_societe->backspace();
                        break;

        case ED_ID:
                        ui->Edit_personnel->backspace();
                        break;

        case ED_PASSWD:
                        ui->Edit_passwd->backspace();
                        break;
        default:
                        break;
    }
}
