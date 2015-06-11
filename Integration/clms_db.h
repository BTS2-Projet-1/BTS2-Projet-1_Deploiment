#ifndef CLMS_DB_H
#define CLMS_DB_H

#include "DataStructures.h"

class CLMS_DB
{
    private :
        QString randString(int len);
        QString m_addressIP;

    public:
        CLMS_DB(QString ip ="1.1.1.1");
        QList<struct struct_PackagesUseBox> GenerateExtractCode(QString SiteName,QList<struct struct_PackagesUseBox> PackageStorageListEmpty);
        QList<QString> Get_LastDatabase_Save(QString DB_Site);
        bool AutoSave_DatabaseLMS(QString DB_Site, QString FilePath);
        bool ManualSave_DatabaseLMS(QString DB_Site, QString FilePath);
        bool isConnected();

        QString getAddressIP(){return m_addressIP;}
        void setAddressIP(QString ip);
        int shell(const QString &cmd);
};

#endif // CLMS_DB_H
