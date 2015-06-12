#include "clms_db.h"
#include "DataStructures.h"
#include <QProcess>
#include <QDateTime>
#include <QDebug>

CLMS_DB::CLMS_DB(QString ip)
{
    m_addressIP = ip;
}

QString CLMS_DB::randString(int len)
{
    QString str;
    QTime Time = QTime::currentTime();
    int RandomNumber;

    str.resize(len);
    for (int i = 0; i<len ; i++)
    {
        RandomNumber = (Time.second()*Time.minute()+Time.msec())+qrand();
        str[i] = QChar('A' + char(RandomNumber % ('Z' - 'A')));
    }
    return str;
}

QList<struct struct_PackagesUseBox> CLMS_DB::GenerateExtractCode(QString SiteName, QList<struct struct_PackagesUseBox> PackageStorageListEmpty)
{

    if(isConnected())
    {
        QString ExtractCode;
        ExtractCode = randString(10);

        qDebug() << "LMS : " << " Generating extract code : " << ExtractCode;
        qDebug() << "Delivering site : " << SiteName;

        if (!PackageStorageListEmpty.isEmpty())
        {
            for(int i=0; i<PackageStorageListEmpty.count(); i++)
            {

                qDebug() << ExtractCode;
                PackageStorageListEmpty[i].BoxContainment.PackagesExtractCode.append(ExtractCode);

                QDateTime TimeStampExpiryTime;
                TimeStampExpiryTime = TimeStampExpiryTime.currentDateTimeUtc();
                TimeStampExpiryTime = TimeStampExpiryTime.addDays(14);
                PackageStorageListEmpty[i].BoxContainment.ExtractCodeExpiryTime = TimeStampExpiryTime.toString();
            }
        }
    }
    else{
        PackageStorageListEmpty.clear();

    }

    return PackageStorageListEmpty;
}

QList<QString> CLMS_DB::Get_LastDatabase_Save(QString DB_Site)
{
    QList<QString> QueryList;
    qDebug() << "[WARNING] : LMS Server offline, unable to get the database save for : " << DB_Site;
    return QueryList; //QueryList is empty = LMS Server offline
}

bool CLMS_DB::AutoSave_DatabaseLMS(QString DB_Site, QString FilePath)
{
    /*
     * isConnected()
     * Try to save the local database
     * Try to send the local database save to the LMS server
     *
     */

    qDebug() << "[WARNING] : LMS Server is currently offline. We can't build the database save for : " << DB_Site << ", with the file : " << FilePath;
    return false; // LMS Server offline
}

bool CLMS_DB::ManualSave_DatabaseLMS(QString DB_Site, QString FilePath)
{
    /*
     *
     * Try to save the local database
     * Try to send the local database save to the LMS server
     *
     */

    qDebug() << "[WARNING] : LMS Server is currently offline. We can't build the database save for : " << DB_Site << ", with the file : " << FilePath;
    return false; // LMS Server offline
}

bool CLMS_DB::isConnected()
{
    bool retour = false;
    int result;
    QString cmdping;

    cmdping = "ping -c 4 "+m_addressIP;

    result = shell(cmdping);
    if(result == 0)
    {
       retour = true;
    }

    return retour;
}

void CLMS_DB::setAddressIP(QString ip)
{
    m_addressIP.clear();
    m_addressIP = ip;
}

int CLMS_DB::shell(const QString &cmd)
{
    QProcess procTerminal;

    procTerminal.start(cmd);
    procTerminal.waitForFinished(20000);

    return procTerminal.exitCode();

}
