#include "editsettingsystem.h"
#include "ui_editsettingsystem.h"

EditSettingSystem::EditSettingSystem(SettingSystem *settingsystem, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditSettingSystem)
{
    ui->setupUi(this);
    ui->WKeyBoard->setVisible(true);

    m_SettingSystem = settingsystem;

    m_modeRegExp = NORMAL;
    m_RegExp=NULL;

    connect(ui->WKeyBoard,SIGNAL(SendCarac(QChar)),this,SLOT(SetCaract(QChar)));
    connect(ui->WKeyBoard,SIGNAL(SupprLastCaract()),this,SLOT(SupprCaract()));
    connect(ui->WKeyBoard,SIGNAL(ButtonOKCliked()),this,SLOT(ClickedButtonOK()));
    connect(m_SettingSystem,SIGNAL(SendWLineText(QString,QString,MODE_REGEXP )),this,SLOT(receiveEditText(QString,QString,MODE_REGEXP)));


}

EditSettingSystem::~EditSettingSystem()
{
    delete m_RegExp;
    delete ui;
}

void EditSettingSystem::receiveEditText(QString label,QString text,MODE_REGEXP mode)
{
    ui->label->setText(label);
    ui->lineEdit->setText(text);
    m_modeRegExp = mode;

    //remise a zero
    ui->lineEdit->setValidator(0);

    switch(m_modeRegExp)
    {

        case IPV4  :
                    delete m_RegExp;
                    m_RegExp = new QRegExpValidator( QRegExp("^(25[0-5]|2[0-4]\\d|[0-1]?\\d?\\d)(\\.(25[0-5]|2[0-4]\\d|[0-1]?\\d?\\d)){3}\\z$"));
                    ui->lineEdit->setValidator(m_RegExp);
                    break;

        case EMAIL :
                    break;

        default ://mode NORMAL
                    ui->lineEdit->setValidator(0);
                    break;

    }
}

void EditSettingSystem::SetCaract(QChar caract)
{
    ui->lineEdit->insert(QString(caract));
}

void EditSettingSystem::SupprCaract()
{
   ui->lineEdit->backspace();
}

void EditSettingSystem::ClickedButtonOK()
{
    emit sendTextEdit(ui->lineEdit->text());
    ui->lineEdit->clear();
    ui->label->clear();

    m_ShowWidgets->ShowWSettingSystem();
}
