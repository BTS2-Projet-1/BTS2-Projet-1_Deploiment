#ifndef DELIVERYCONFIRM_H
#define DELIVERYCONFIRM_H
/*!
 *
 * \file DeliveryConfirm .h
 * \class DeliveryConfirm  "Header"
 * \brief Header de la classe DeliveryConfirm
 *
 * IHM confirmation de la livraison.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */
#include <QWidget>

#include "showwidgets.h"
#include "csqlite_local_db.h"
#include "clms_db.h"
#include "DataStructures.h"
#include "stowpackages.h"
#include "threadsavedelivery.h"

namespace Ui {
class DeliveryConfirm;
}

class DeliveryConfirm : public QWidget
{
    Q_OBJECT

public:
    explicit DeliveryConfirm(CSQLite_Local_DB*, CLMS_DB*, StowPackages*, BoxChoice*, CustomXml *fileSaveTemlist, QWidget *parent = 0);
    void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}
    void SaveDelivery();
    ~DeliveryConfirm();

    void retranslateUi();

public slots:
    void NewDelivery();//nouvelle livraison pour un nouveau client
    void FinishDelivery();//finir la livraison du client
    void ReceiveListPackageBox(QList<struct_PackagesUseBox> listpackagebox);
    void SetValueProgress(QString, int);
    void ThreadFinished();

private slots:
    void ShowChoiceConfirm();
    void ShowLoadSave();

signals:
    void SendListPackage(QList<struct_PackagesUseBox>);


private:
    Ui::DeliveryConfirm *ui;
    ShowWidgets *m_ShowWidgets;
    CSQLite_Local_DB *m_BD;
    CLMS_DB *m_LMS;
    StowPackages *m_WStowPackage;
    BoxChoice *m_WBoxChoice;
    QList<struct_PackagesUseBox> m_tempPackageList;
    ThreadSaveDelivery *m_threadSaveD;
    int m_clickselect;


};

#endif // DELIVERYCONFIRM_H
