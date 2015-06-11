#include "threadsavedelivery.h"
#include "deliveryconfirm.h"

#define SAVEDELIVERY   0
#define UPDATELISTTEMP 1

ThreadSaveDelivery::ThreadSaveDelivery(CSQLite_Local_DB *db, CLMS_DB *lms,CustomXml *filesavetempxml, QObject *parent):QThread(parent)
{
    m_DB = db;
    m_LMS = lms;

    m_fileSaveTemlist = filesavetempxml;

    m_choice = SAVEDELIVERY ;
}

ThreadSaveDelivery::~ThreadSaveDelivery()
{
    wait(200);
}

void ThreadSaveDelivery::run()
{
    if(m_choice == SAVEDELIVERY)
    {
        SaveListPackage();
    }
    else{

        UpdateSaveTempFile();
    }

    finished();
    exit();
}

void ThreadSaveDelivery::startThread(QList<struct_PackagesUseBox> listpackages)
{
    m_Packageslist.clear();
    m_tempPackageList.clear();
    m_tempListPackages.clear();

    m_choice = SAVEDELIVERY;

    //recuperer la liste savaugarder dans le fichier xml
    m_tempListPackages = m_fileSaveTemlist->GetListSaveTempDelivery();

    m_tempPackageList = listpackages;

    start();
}

void ThreadSaveDelivery::startThread()
{
    m_Packageslist.clear();
    m_tempPackageList.clear();
    m_tempListPackages.clear();

    m_choice = UPDATELISTTEMP;

    //recuperer la liste sauvegarder dans le fichier xml
    m_tempListPackages = m_fileSaveTemlist->GetListSaveTempDelivery();

    start();
}

void ThreadSaveDelivery::SaveListPackage()
{
    m_value = 12;

    emit SetValueProgress("Enregistrement des colis au serveur LMS...",m_value);

    //transmission de la liste au serveur LMS et renvoie le code de retrait
    m_Packageslist = m_LMS->GenerateExtractCode(m_DB->Get_SiteName(),m_tempPackageList);

    if(m_Packageslist.isEmpty())
    {
        //sauvegarde dans le fichier xml
        SaveListpackagetemp(m_tempPackageList);
    }
    else{

        //envoie la liste avec code retrait a  la BD Local
        m_DB->UpdateLocalData(m_Packageslist);

        if(!m_tempListPackages.isEmpty())
        {

            for(int i = m_tempListPackages.size()-1 ; i >= 0;i--)
            {
               m_Packageslist.clear();

               //transmission de la liste au serveur LMS et renvoie le code de retrait
               m_Packageslist = m_LMS->GenerateExtractCode(m_DB->Get_SiteName(),m_tempListPackages[i]);

               if(m_Packageslist.isEmpty())
               {
                   m_fileSaveTemlist->removeLastChild();

                   //sauvegarde dans le fichier xml
                   SaveListpackagetemp(m_tempListPackages[i]);
               }
               else{
                   //envoie la liste avec code retrait a  la BD Local
                   m_DB->UpdateLocalData(m_Packageslist);
                   m_fileSaveTemlist->removeLastChild();
               }

            }
        }

    }

    //fini la progression
    for(int value=m_value;value <=100; value++)
    {
        emit SetValueProgress("Mise à jour de la Base de Données Locale...",value);
        msleep(100);
        if(value == 100)
        {
            emit SetValueProgress("Mise à jour terminée.",value);
            msleep(100);
        }
    }
}

void ThreadSaveDelivery::UpdateSaveTempFile()
{
    if(!m_tempListPackages.isEmpty())
    {

        for(int i = m_tempListPackages.size()-1 ; i >= 0;i--)
        {
           m_Packageslist.clear();

           //transmission de la liste au serveur LMS et renvoie le code de retrait
           m_Packageslist = m_LMS->GenerateExtractCode(m_DB->Get_SiteName(),m_tempListPackages[i]);

           if(m_Packageslist.isEmpty())
           {
               m_fileSaveTemlist->removeLastChild();

               //sauvegarde dans le fichier xml
               SaveListpackagetemp(m_tempListPackages[i]);
           }
           else{
               //envoie la liste avec code retrait a  la BD Local
               m_DB->UpdateLocalData(m_Packageslist);
               m_fileSaveTemlist->removeLastChild();
           }

        }
    }
}

void ThreadSaveDelivery::SaveListpackagetemp(QList<struct_PackagesUseBox> tempackagelist)
{
    m_value++;
    emit SetValueProgress("Echec connexion au serveur LMS...",m_value);
    msleep(1500);

    m_value++;
    emit SetValueProgress("Enregistrement de la liste temporaire...",m_value);
    msleep(2000);

    //enregistre dans fichier xml
    if(m_fileSaveTemlist->SaveListPackagesTemp(tempackagelist))
    {
        for(int i = 0;i< tempackagelist.size();i++)
        {
            m_value++;
            emit SetValueProgress("Mise a jour temporaire de la Base de Données Locale...",m_value);
            msleep(500);

            m_DB->Set_Temporary_BoxUsed(tempackagelist[i].BoxNumber);//met les consignes selectionnées en non disponible

        }
    }
    else{

        m_value++;
        emit SetValueProgress("Erreur liste vide...",m_value);
        msleep(500);
    }


}





