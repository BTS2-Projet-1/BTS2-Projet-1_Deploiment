#include "settingsystem.h"
#include "ui_settingsystem.h"
#include <QRegExpValidator>

#define ED_CODE                 0
#define ED_SITE                 1
#define ED_NETWORK_IP           2
#define ED_NETWORK_MASK         3
#define ED_NETWORK_GATEWAY      4
#define ED_NETWORK_DNS1         5
#define ED_NETWORK_DNS2         6
#define ED_LMS_IP               7
#define ED_RSYNC_IP             8
#define ED_RSYNC_USER           9
#define ED_RSYNC_PASSWD        10
#define ED_SOFTPHONE_USER      11
#define ED_SOFTPHONE_PASSWD    12
#define ED_SOFTPHONE_DOMAIN    13
#define ED_SOFTPHONE_PROXY     14


SettingSystem::SettingSystem(CSQLite_Local_DB *DB,CLMS_DB *lms,CustomXml *fileconfig,CustomXml *fileSaveTemp, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingSystem)
{
   ui->setupUi(this);

   m_LineEdit_Selected = 0;
   m_DB = DB;
   m_LMS = lms;
   m_ThreadSaveDelivery = new ThreadSaveDelivery(m_DB,m_LMS,fileSaveTemp);
   m_Xml = fileconfig;

   qRegisterMetaType<MODE_REGEXP>("MODE_REGEXP");
   if(!m_Xml->isChildrootExist("Softphone"))
   {
        m_Xml->WriteFileXml("ID_Console","id","code");
        m_Xml->WriteFileXml("ID_Console","id","site",m_DB->Get_SiteName());
        m_Xml->WriteFileXml("Devices","type","name");
        m_Xml->WriteFileXml("Devices","type","port");
        m_Xml->WriteFileXml("Network","address","ip");
        m_Xml->WriteFileXml("Network","address","subnet_mask");
        m_Xml->WriteFileXml("Network","address","gateway");
        m_Xml->WriteFileXml("Network","address","DNS_preferred");
        m_Xml->WriteFileXml("Network","address","DNS_alernate");
        m_Xml->WriteFileXml("Server_LMS","address","ip");
        m_Xml->WriteFileXml("Server_rsync","address","ip");
        m_Xml->WriteFileXml("Server_rsync","user","name");
        m_Xml->WriteFileXml("Server_rsync","password","passwd");
        m_Xml->WriteFileXml("Softphone","sip","username");
        m_Xml->WriteFileXml("Softphone","sip","password");
        m_Xml->WriteFileXml("Softphone","sip","domain");
        m_Xml->WriteFileXml("Softphone","sip","proxy");
    }

    UpdateSetting();

    //install eventfilter lineEdit
    //ID console
    ui->LineE_code->installEventFilter(this);
    ui->LineE_site->installEventFilter(this);

    //Config Reseau
    ui->LineE_IP->installEventFilter(this);
    ui->LineE_Sub_Mask->installEventFilter(this);
    ui->LineE_gateway->installEventFilter(this);
    ui->LineE_dns1->installEventFilter(this);
    ui->LineE_dns2->installEventFilter(this);

    //rsync
    ui->LineE_IP_rsync->installEventFilter(this);
    ui->LineE_Rsync_User->installEventFilter(this);
    ui->LineE_Rsync_Passwd->installEventFilter(this);

    //LMS
    ui->LineE_IP_LMS->installEventFilter(this);

    //Softphone
    ui->LineE_SIP_Domain->installEventFilter(this);
    ui->LineE_SIP_Passwd->installEventFilter(this);
    ui->LineE_SIP_Proxy->installEventFilter(this);
    ui->LineE_SIP_User->installEventFilter(this);

    connect(ui->B_Save,SIGNAL(clicked()),this,SLOT(UpdateFileXml()));
    connect(ui->B_Ping,SIGNAL(clicked()),this,SLOT(TestPing()));
    connect(this,SIGNAL(UpdateSavelistPackagetemp()),m_ThreadSaveDelivery,SLOT(startThread()));
}

void SettingSystem::setEditSetting(EditSettingSystem *editsetting)
{
    m_EditSettingSystem = editsetting;
    connect(m_EditSettingSystem,SIGNAL(sendTextEdit(QString)),this,SLOT(SetTextEdit(QString)));
}

SettingSystem::~SettingSystem()
{
    delete m_ThreadSaveDelivery;
    delete ui;
}

void SettingSystem::on_B_Cancel_clicked()
{
    m_ShowWidgets->ShowWSetting();
    UpdateSetting();
}

void SettingSystem::UpdateFileXml()
{
    m_Xml->WriteFileXml("ID_Console","id","code",ui->LineE_code->text());
    m_Xml->WriteFileXml("ID_Console","id","site",ui->LineE_site->text());
    m_DB->Set_SiteName(ui->LineE_site->text());
    m_Xml->WriteFileXml("Devices","type","name");
    m_Xml->WriteFileXml("Devices","type","port");
    m_Xml->WriteFileXml("Network","address","ip",ui->LineE_IP->text());
    m_Xml->WriteFileXml("Network","address","subnet_mask",ui->LineE_Sub_Mask->text());
    m_Xml->WriteFileXml("Network","address","gateway",ui->LineE_gateway->text());
    m_Xml->WriteFileXml("Network","address","DNS_preferred",ui->LineE_dns1->text());
    m_Xml->WriteFileXml("Network","address","DNS_alernate",ui->LineE_dns2->text());
    m_Xml->WriteFileXml("Server_LMS","address","ip",ui->LineE_IP_LMS->text());
    m_Xml->WriteFileXml("Server_rsync","address","ip",ui->LineE_IP_rsync->text());
    m_Xml->WriteFileXml("Server_rsync","user","name",ui->LineE_Rsync_User->text());
    m_Xml->WriteFileXml("Server_rsync","password","passwd",ui->LineE_Rsync_Passwd->text());
    m_Xml->WriteFileXml("Softphone","sip","username",ui->LineE_SIP_User->text());
    m_Xml->WriteFileXml("Softphone","sip","password",ui->LineE_SIP_Passwd->text());
    m_Xml->WriteFileXml("Softphone","sip","domain",ui->LineE_SIP_Domain->text());
    m_Xml->WriteFileXml("Softphone","sip","proxy",ui->LineE_SIP_Proxy->text());

    m_ShowWidgets->ShowWSetting();

}

void SettingSystem::UpdateSetting()
{
    ui->LineE_code->setText(m_Xml->Getvalue("ID_Console","id","code"));
    ui->LineE_site->setText(m_Xml->Getvalue("ID_Console","id","site"));
    ui->LineE_IP->setText(m_Xml->Getvalue("Network","address","ip"));
    ui->LineE_Sub_Mask->setText(m_Xml->Getvalue("Network","address","subnet_mask"));
    ui->LineE_gateway->setText(m_Xml->Getvalue("Network","address","gateway"));
    ui->LineE_dns1->setText(m_Xml->Getvalue("Network","address","DNS_preferred"));
    ui->LineE_dns2->setText(m_Xml->Getvalue("Network","address","DNS_alernate"));
    ui->LineE_IP_LMS->setText(m_Xml->Getvalue("Server_LMS","address","ip"));
    ui->LineE_IP_rsync->setText(m_Xml->Getvalue("Server_rsync","address","ip"));
    ui->LineE_Rsync_User->setText(m_Xml->Getvalue("Server_rsync","user","name"));
    ui->LineE_Rsync_Passwd->setText(m_Xml->Getvalue("Server_rsync","password","passwd"));
    ui->LineE_SIP_User->setText(m_Xml->Getvalue("Softphone","sip","username"));
    ui->LineE_SIP_Passwd->setText(m_Xml->Getvalue("Softphone","sip","password"));
    ui->LineE_SIP_Domain->setText(m_Xml->Getvalue("Softphone","sip","domain"));
    ui->LineE_SIP_Proxy->setText(m_Xml->Getvalue("Softphone","sip","proxy"));
    TestPing();

}

bool SettingSystem::eventFilter(QObject *obj, QEvent *event)
{
    bool result = false;

    if(obj==ui->LineE_code && event->type()==QEvent::MouseButtonPress)
    {

        emit SendWLineText(ui->label_code->text(),ui->LineE_code->text(),NORMAL);
        m_LineEdit_Selected  = ED_CODE;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    if(obj==ui->LineE_site && event->type()==QEvent::MouseButtonPress)
    {
        emit SendWLineText(ui->label_site->text(),ui->LineE_site->text(),NORMAL);
        m_LineEdit_Selected  = ED_SITE;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    if(obj==ui->LineE_IP && event->type()==QEvent::MouseButtonPress)
    {

        emit SendWLineText(ui->label_Network_ip->text(),ui->LineE_IP->text(),IPV4);
        m_LineEdit_Selected  = ED_NETWORK_IP;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    if(obj==ui->LineE_gateway && event->type()==QEvent::MouseButtonPress)
    {
        emit SendWLineText(ui->label_Network_gateway->text(),ui->LineE_gateway->text(),IPV4);
        m_LineEdit_Selected  = ED_NETWORK_GATEWAY;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    if(obj==ui->LineE_Sub_Mask && event->type()==QEvent::MouseButtonPress)
    {

        emit SendWLineText(ui->label_Network_Sub_mask->text(),ui->LineE_Sub_Mask->text(),IPV4);
        m_LineEdit_Selected  = ED_NETWORK_MASK;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    if(obj==ui->LineE_dns1 && event->type()==QEvent::MouseButtonPress)
    {
        emit SendWLineText(ui->label_Network_DNS1->text(),ui->LineE_dns1->text(),IPV4);
        m_LineEdit_Selected  = ED_NETWORK_DNS1;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    if(obj==ui->LineE_dns2 && event->type()==QEvent::MouseButtonPress)
    {

        emit SendWLineText(ui->label_Network_DNS2->text(),ui->LineE_dns2->text(),IPV4);
        m_LineEdit_Selected  = ED_NETWORK_DNS2;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    if(obj==ui->LineE_IP_rsync && event->type()==QEvent::MouseButtonPress)
    {
        emit SendWLineText(ui->label_rsync_ip->text(),ui->LineE_IP_rsync->text(),IPV4);
        m_LineEdit_Selected  = ED_RSYNC_IP;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    if(obj==ui->LineE_Rsync_Passwd && event->type()==QEvent::MouseButtonPress)
    {

        emit SendWLineText(ui->label_rsync_passwd->text(),ui->LineE_Rsync_Passwd->text(),NORMAL);
        m_LineEdit_Selected  = ED_RSYNC_PASSWD;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    if(obj==ui->LineE_Rsync_User && event->type()==QEvent::MouseButtonPress)
    {
        emit SendWLineText(ui->label_rsync_user->text(),ui->LineE_Rsync_User->text(),NORMAL);
        m_LineEdit_Selected  = ED_RSYNC_USER;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    if(obj==ui->LineE_IP_LMS && event->type()==QEvent::MouseButtonPress)
    {

        emit SendWLineText(ui->label_LMS_ip->text(),ui->LineE_IP_LMS->text(),IPV4);
        m_LineEdit_Selected  = ED_LMS_IP;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    if(obj==ui->LineE_SIP_User && event->type()==QEvent::MouseButtonPress)
    {
        emit SendWLineText(ui->label_Sip_user->text(),ui->LineE_SIP_User->text(),NORMAL);
        m_LineEdit_Selected  = ED_SOFTPHONE_USER;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    if(obj==ui->LineE_SIP_Passwd && event->type()==QEvent::MouseButtonPress)
    {

        emit SendWLineText(ui->label_Sip_passwd->text(),ui->LineE_SIP_Passwd->text(),NORMAL);
        m_LineEdit_Selected  = ED_SOFTPHONE_PASSWD;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    if(obj==ui->LineE_SIP_Domain && event->type()==QEvent::MouseButtonPress)
    {

        emit SendWLineText(ui->label_Sip_Domain->text(),ui->LineE_SIP_Domain->text(),NORMAL);
        m_LineEdit_Selected  = ED_SOFTPHONE_DOMAIN;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    if(obj==ui->LineE_SIP_Proxy && event->type()==QEvent::MouseButtonPress)
    {
        emit SendWLineText(ui->label_Sip_proxy->text(),ui->LineE_SIP_Proxy->text(),NORMAL);
        m_LineEdit_Selected  = ED_SOFTPHONE_PROXY;
        m_ShowWidgets->ShowWEditSettingSystem();
        return true;
    }

    return result;
}

void SettingSystem::SetTextEdit(QString text)
{
    switch(m_LineEdit_Selected)
    {
        case ED_CODE:
                        ui->LineE_code->setText(text);
                        break;

        case ED_SITE:
                        ui->LineE_site->setText(text);
                        break;

        case ED_NETWORK_IP:
                        ui->LineE_IP->setText(text);
                        break;

        case ED_NETWORK_GATEWAY:
                        ui->LineE_gateway->setText(text);
                        break;

        case ED_NETWORK_MASK:
                        ui->LineE_Sub_Mask->setText(text);
                        break;

        case ED_NETWORK_DNS1:
                        ui->LineE_dns1->setText(text);
                        break;

        case ED_NETWORK_DNS2:
                        ui->LineE_dns2->setText(text);
                        break;

        case ED_RSYNC_IP:
                        ui->LineE_IP_rsync->setText(text);
                        break;

        case ED_RSYNC_PASSWD:
                        ui->LineE_Rsync_Passwd->setText(text);
                        break;

        case ED_RSYNC_USER:
                        ui->LineE_Rsync_User->setText(text);
                        break;

        case ED_LMS_IP:
                        ui->LineE_IP_LMS->setText(text);
                        break;

        case ED_SOFTPHONE_USER:
                        ui->LineE_SIP_User->setText(text);
                        break;

        case ED_SOFTPHONE_PASSWD:
                        ui->LineE_SIP_Passwd->setText(text);
                        break;

        case ED_SOFTPHONE_DOMAIN:
                        ui->LineE_SIP_Domain->setText(text);
                        break;

        case ED_SOFTPHONE_PROXY:
                        ui->LineE_SIP_Proxy->setText(text);
                        break;

    }
}

void SettingSystem::TestPing()
{
    int result;
    QString ip;
    QString cmdping;

    ip = ui->LineE_IP_LMS->text();

    cmdping = "ping -c 4 " + ip;

    result = m_LMS->shell(cmdping);

    if(result == 0)
    {
        m_LMS->setAddressIP(ip);
        //img ok
        ui->label_status->setPixmap(QPixmap(":/ImgList/success"));
        //envoi signal save
        UpdateSavelistPackagetemp();
    }
    else{
        //img off
        ui->label_status->setPixmap(QPixmap(":/ImgList/error"));
    }
}
