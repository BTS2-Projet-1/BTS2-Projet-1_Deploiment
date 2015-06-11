#ifndef SCANPACKAGE_H
#define SCANPACKAGE_H
/*!
 *
 * \file ScanPackage.h
 * \class ScanPackage   "Header"
 * \brief Header de la classe ScanPackage
 *
 * IHM Saisie de numéro de colis.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */
#include <QWidget>
#include <QLabel>
#include "showwidgets.h"
#include "DataStructures.h"
#include "boxchoice.h"
#include "custompushbutton.h"

namespace Ui {
class ScanPackage;
}

class ScanPackage : public QWidget
{
    Q_OBJECT

public:
    explicit ScanPackage(BoxChoice *,QWidget *parent = 0);
    ~ScanPackage();
    void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}
    bool isNumBoxExist(int numbox);
    int  getIndexListNumBoxExist(int numbox);
    void AddItemListWidget(QString NumPackage);
    void Refresh();
    int BuildingListPackages(); //construction de la QList<struct_PackagesUseBox>
    void retranslateUi();
    bool isExistNumPackage(QString NumPackage);
    void ShowMessageError(QString);

public slots:
    void StockNumberPackage();//stock les n°colis dans une qlist
    void ValidateDelivery();//recuperation de la liste des colis et demande de confirmation
    void CancelDelivery();//annule la saisie des colis pour la consigne
    void ConfirmValidate();
    void ReturnListPackage();
    void ReceiveListBoxSelected(QList<int>);
    void DeleteNumPackage(QString package);
    void Upper(QString str);

signals :
    void SendListPackageBox(QList<struct_PackagesUseBox>);
    void ShowStowPackage();
    void TextToUpper(QString);

private slots:
    void SetManual();
    void SetScanner();
    void ChangePage();
    void SetCaract(QChar caract);
    void SupprCaract();

private:
    Ui::ScanPackage *ui;
    ShowWidgets *m_ShowWidgets;
    BoxChoice *m_WBoxChoice;
    QLabel *m_labelbox;

    QList<struct_PackagesUseBox> m_listPackageBox;
    QList<QString> m_ListPackagesString;
    QList<int> m_ListBoxselected;
    QList<CustomPushButton *> m_ListCustPusButton;
    int m_idPushButton;
    int m_page;

};

#endif // SCANPACKAGE_H
