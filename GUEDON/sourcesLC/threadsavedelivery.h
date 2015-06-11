#ifndef THREADSAVEDELIVERY_H
#define THREADSAVEDELIVERY_H
/*!
 *
 * \file ThreadSaveDelivery.h
 * \class ThreadSaveDelivery "Header"
 * \brief Header de la classe ThreadSaveDelivery
 *
 * Thread ThreadSaveDelivery est lancé lors du procéssus de livraison pour récupérer au près du serveur LMS
 * les codes de retrait des livraisons et met à jour la base de données.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */
#include <QThread>

#include "clms_db.h"
#include "csqlite_local_db.h"
#include "DataStructures.h"
#include "customxml.h"


class ThreadSaveDelivery;

class ThreadSaveDelivery : public QThread
{
    Q_OBJECT
    public:
         ThreadSaveDelivery(CSQLite_Local_DB *db, CLMS_DB *lms, CustomXml *filesavetempxml, QObject *parent=0);

        ~ThreadSaveDelivery();
        void run();
        void SaveListpackagetemp(QList<struct_PackagesUseBox> tempackagelist);
        void SaveListPackage();
        void UpdateSaveTempFile();


public slots:
        void startThread(QList<struct_PackagesUseBox> listpackage);
        void startThread();

    signals:
        void SetValueProgress(QString, int);

    private:
        QList<struct_PackagesUseBox> m_tempPackageList;
        QList< QList<struct_PackagesUseBox> > m_tempListPackages;
        QList<struct_PackagesUseBox> m_Packageslist;

        CSQLite_Local_DB *m_DB;
        CLMS_DB *m_LMS;

        CustomXml *m_fileSaveTemlist;
        int m_choice;
        int m_value;

};

#endif // THREADSAVEDELIVERY_H
