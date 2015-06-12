#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QProcess>
#include <QSqlQuery>
#include <iostream>
#include <unistd.h>

#include "clms_db.h"
#include "csqlite_local_db.h"

#define COLORRESET  "\033[0m"               /* Default color */
#define BLACK       "\033[30m"              /* Black */
#define RED         "\033[31m"              /* Red */
#define GREEN       "\033[32m"              /* Green */
#define YELLOW      "\033[33m"              /* Yellow */
#define BLUE        "\033[34m"              /* Blue */
#define MAGENTA     "\033[35m"              /* Magenta */
#define CYAN        "\033[36m"              /* Cyan */
#define WHITE       "\033[37m"              /* White */
#define BOLDBLACK   "\033[1m\033[30m"       /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"       /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"       /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"       /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"       /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"       /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"       /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"       /* Bold White */

//Max number of used boxes for normal use.
#define MAX_NORMAL_USE_BOXES 5

class UnitTest_CSQLite_Local_DB : public QObject
{
        Q_OBJECT

    public:
        UnitTest_CSQLite_Local_DB();
        void Set_AutoMode(bool Tmode) {m_AutoMode = Tmode;}

    private:
        CSQLite_Local_DB *m_LC_Database;
        CLMS_DB *m_LMS_Database;
        QString randString(int len);
        bool m_AutoMode;

    private Q_SLOTS:
        void initTestCase();
        void cleanupTestCase();
        void Test_CSQLite_Local_DB();
        void Test_Login();
        void Test_UpdateLocalData_NormalUse();
        void Test_UpdateLocalData_NormalUse_AllBoxes();
        void Test_UpdateLocalData_EmptyList();
        void Test_UpdateLocalData_WrongBoxes();
        void Test_UpdateLocalData_No_database();
};

UnitTest_CSQLite_Local_DB::UnitTest_CSQLite_Local_DB()
{
    m_LMS_Database = new CLMS_DB(NULL);
    m_LC_Database = new CSQLite_Local_DB(m_LMS_Database);

    m_AutoMode = true;
}

QString UnitTest_CSQLite_Local_DB::randString(int len)
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

void UnitTest_CSQLite_Local_DB::initTestCase()
{
    m_LMS_Database->setAddressIP("127.0.0.1");
    m_LC_Database->SQL_Database_Manager(DEFAULT_DATABASE);
    
    if(!m_AutoMode)
    {
        qDebug() << "Press enter to continue...";
        getwchar();
    }
}

void UnitTest_CSQLite_Local_DB::cleanupTestCase()
{
    delete m_LMS_Database;
    delete m_LC_Database;
}

void UnitTest_CSQLite_Local_DB::Test_CSQLite_Local_DB()
{
    if(!m_LC_Database->IsReady())
        std::cout << BOLDRED << "FAIL   : Database not ready !" << COLORRESET << std::endl;
    else
        std::cout << BOLDGREEN << "PASS   : Database is ready !" << COLORRESET << std::endl;

    QVERIFY(m_LC_Database->IsReady());

    if(!m_AutoMode)
    {
        qDebug() << "Press enter to continue...";
        getwchar();
    }
}

void UnitTest_CSQLite_Local_DB::Test_Login()
{
    QStringList LoginParamList;
    QList<int> CompareValue;

    m_LC_Database->SQL_Database_Manager(DEFAULT_DATABASE);

    LoginParamList.append("100200300400");
    CompareValue.append(MAINTENANCE);
    LoginParamList.append("500600700800");
    CompareValue.append(LIVREUR);
    LoginParamList.append("100200111111");
    CompareValue.append(UNDEFINED_PASSWORD);
    LoginParamList.append("500600111111");
    CompareValue.append(UNDEFINED_PASSWORD);
    LoginParamList.append("100111300400");
    CompareValue.append(UNDEFINED_PERSONAL_PASS);
    LoginParamList.append("500111700800");
    CompareValue.append(UNDEFINED_PERSONAL_PASS);
    LoginParamList.append("111200300400");
    CompareValue.append(UNDEFINED_COMPANY_PASS);
    LoginParamList.append("111600700800");
    CompareValue.append(UNDEFINED_COMPANY_PASS);
    LoginParamList.append("353356487999");
    CompareValue.append(UNDEFINED_USER);
    LoginParamList.append("/**ù$^^^&!>>");
    CompareValue.append(UNDEFINED_USER);
    LoginParamList.append("1342");
    CompareValue.append(UNDEFINED_USER);
    LoginParamList.append("sdfsdfs545s54d545sdfdsf");
    CompareValue.append(UNDEFINED_USER);
    LoginParamList.append("");
    CompareValue.append(UNDEFINED_USER);

    for(int i=0; i<LoginParamList.count(); i++)
    {
        int LoginResult = -5;

        LoginResult = m_LC_Database->Login(LoginParamList[i]);

        if(LoginResult == CompareValue[i])
            std::cout << BOLDGREEN << "PASS   : Login(\" " << LoginParamList[i].toStdString() << " \") ! is " << QString::number(CompareValue[i],10).toStdString() << COLORRESET << std::endl;
        else
            std::cout << BOLDRED << "FAIL   : Login(\" " << LoginParamList[i].toStdString() << " \") ! is " << QString::number(CompareValue[i],10).toStdString() << COLORRESET << std::endl;

        QCOMPARE(LoginResult,CompareValue[i]);
    }

    if(!m_AutoMode)
    {
        qDebug() << "Press enter to continue...";
        getwchar();
    }
}

void UnitTest_CSQLite_Local_DB::Test_UpdateLocalData_NormalUse()
{
    QList<struct struct_PackagesUseBox> PackageStorageListEmpty,PackageStorageListComplete;
    struct struct_PackagesUseBox tmp;

    m_LC_Database->SQL_Database_Manager(DEFAULT_DATABASE);

    //Normal use
    for(int i=1; i<=MAX_NORMAL_USE_BOXES; i++)
    {
        tmp.BoxNumber = i;
        for(int j=0; j<3; j++)
        {
            tmp.BoxContainment.PackageDeliveryCodeList << QString::number(qrand(),10)+randString(6);
        }
        PackageStorageListEmpty.append(tmp);
        tmp.BoxContainment.PackagesExtractCode.clear();
        tmp.BoxContainment.PackageDeliveryCodeList.clear();
    }

    PackageStorageListComplete = m_LMS_Database->GenerateExtractCode(m_LC_Database->Get_SiteName(),PackageStorageListEmpty);

    bool UpdateLocalData_Status = false;
    UpdateLocalData_Status = m_LC_Database->UpdateLocalData(PackageStorageListComplete);

    if(UpdateLocalData_Status)
        std::cout << BOLDGREEN << "PASS   : Database updated !" << COLORRESET << std::endl;
    else
        std::cout << BOLDRED << "FAIL   : Database not updated !" << COLORRESET << std::endl;

    QVERIFY(UpdateLocalData_Status);

    for(int i=1; i<m_LC_Database->Get_NumberOfBoxes()+1; i++)
    {
        bool FreeBox = false;

        FreeBox = m_LC_Database->GetFreeBoxes().contains(i);

        if(FreeBox)
            std::cout << BOLDGREEN << "INFO   : Box " << QString::number(i,10).toStdString() << ", is expected free" << COLORRESET << std::endl;
        else
            std::cout << BOLDRED << "INFO   : Box " << QString::number(i,10).toStdString() << ", is unexpected not free" << COLORRESET << std::endl;

        if(i>MAX_NORMAL_USE_BOXES)
            QVERIFY(FreeBox);
        else
            QCOMPARE(FreeBox,false);
    }
    
    if(!m_AutoMode)
    {
        qDebug() << "Press enter to continue...";
        getwchar();
    }
}

void UnitTest_CSQLite_Local_DB::Test_UpdateLocalData_NormalUse_AllBoxes()
{
    QList<struct struct_PackagesUseBox> PackageStorageListEmpty,PackageStorageListComplete;
    struct struct_PackagesUseBox tmp;

    m_LC_Database->SQL_Database_Manager(DEFAULT_DATABASE);

    for(int i=1; i<=m_LC_Database->Get_NumberOfBoxes(); i++)
    {
        tmp.BoxNumber = i;
        for(int j=0; j<3; j++)
        {
            tmp.BoxContainment.PackageDeliveryCodeList << QString::number(qrand(),10)+randString(6);
        }
        PackageStorageListEmpty.append(tmp);
        tmp.BoxContainment.PackagesExtractCode.clear();
        tmp.BoxContainment.PackageDeliveryCodeList.clear();
    }

    PackageStorageListComplete = m_LMS_Database->GenerateExtractCode(m_LC_Database->Get_SiteName(),PackageStorageListEmpty);

    bool UpdateLocalData_Status = false;
    UpdateLocalData_Status = m_LC_Database->UpdateLocalData(PackageStorageListComplete);

    if(UpdateLocalData_Status)
        std::cout << BOLDGREEN << "PASS   : Database updated !" << COLORRESET << std::endl;
    else
        std::cout << BOLDRED << "FAIL   : Database NOT updated !" << COLORRESET << std::endl;

    QVERIFY(UpdateLocalData_Status);

    for(int i=1; i<=m_LC_Database->Get_NumberOfBoxes(); i++)
    {
        bool FreeBox = false;

        FreeBox = m_LC_Database->GetFreeBoxes().contains(i);

        if(!FreeBox)
            std::cout << BOLDGREEN << "INFO   : Box " << QString::number(i,10).toStdString() << ", is expected free" << COLORRESET << std::endl;
        else
            std::cout << BOLDRED << "INFO   : Box " << QString::number(i,10).toStdString() << ", is unexpected not free" << COLORRESET << std::endl;

        QCOMPARE(FreeBox,false);
    }

    if(!m_AutoMode)
    {
        qDebug() << "Press enter to continue...";
        getwchar();
    }
}

void UnitTest_CSQLite_Local_DB::Test_UpdateLocalData_EmptyList()
{
    QList<struct struct_PackagesUseBox> PackageStorageListEmpty;

    qDebug()<< "Start Test_UpdateLocalData_EmptyList()";
    m_LC_Database->SQL_Database_Manager(DEFAULT_DATABASE);

    bool UpdateLocalData_Status = false;
    UpdateLocalData_Status = m_LC_Database->UpdateLocalData(PackageStorageListEmpty);

    if(UpdateLocalData_Status == false)
        std::cout << BOLDGREEN << "PASS   : Database NOT updated !" << COLORRESET << std::endl;
    else
        std::cout << BOLDRED << "FAIL   : Database updated !" << COLORRESET << std::endl;

    QCOMPARE(false,UpdateLocalData_Status);

    if(!m_AutoMode)
    {
        qDebug() << "Press enter to continue...";
        getwchar();
    }
}

void UnitTest_CSQLite_Local_DB::Test_UpdateLocalData_WrongBoxes()
{
    QList<struct struct_PackagesUseBox> PackageStorageListEmpty,PackageStorageListComplete;
    struct struct_PackagesUseBox tmp;

    m_LC_Database->SQL_Database_Manager(DEFAULT_DATABASE);

    for(int i=100; i<=m_LC_Database->Get_NumberOfBoxes()+100; i++)
    {
        tmp.BoxNumber = i;
        for(int j=0; j<3; j++)
        {
            tmp.BoxContainment.PackageDeliveryCodeList << QString::number(qrand(),10)+randString(6);
        }
        PackageStorageListEmpty.append(tmp);
        tmp.BoxContainment.PackagesExtractCode.clear();
        tmp.BoxContainment.PackageDeliveryCodeList.clear();
    }

    PackageStorageListComplete = m_LMS_Database->GenerateExtractCode(m_LC_Database->Get_SiteName(),PackageStorageListEmpty);

    bool UpdateLocalData_Status = false;
    UpdateLocalData_Status = m_LC_Database->UpdateLocalData(PackageStorageListComplete);

    if(UpdateLocalData_Status == false)
        std::cout << BOLDGREEN << "PASS   : Database NOT updated !" << COLORRESET << std::endl;
    else
        std::cout << BOLDRED << "FAIL   : Database updated !" << COLORRESET << std::endl;

    QCOMPARE(false,UpdateLocalData_Status);

    for(int i=1; i<=m_LC_Database->Get_NumberOfBoxes(); i++)
    {
        bool FreeBox = false;

        FreeBox = m_LC_Database->GetFreeBoxes().contains(i);

        if(FreeBox)
            std::cout << BOLDGREEN << "INFO   : Box " << QString::number(i,10).toStdString() << ", is expected free" << COLORRESET << std::endl;
        else
            std::cout << BOLDRED << "INFO   : Box " << QString::number(i,10).toStdString() << ", is unexpected not free" << COLORRESET << std::endl;

        QVERIFY(FreeBox);
    }

    QSqlQuery *query1 = new QSqlQuery(m_LC_Database->SendQuery("SELECT * FROM Package"));
    bool DataAviable = false;

    DataAviable = query1->next();

    if(DataAviable == false)
        std::cout << BOLDGREEN << "PASS   : Database NOT updated !" << COLORRESET << std::endl;
    else
        std::cout << BOLDRED << "FAIL   : Database updated !" << COLORRESET << std::endl;

    QCOMPARE(false,DataAviable);

    delete query1;

    QSqlQuery *query2 = new QSqlQuery(m_LC_Database->SendQuery("SELECT * FROM ExtractCode"));
    DataAviable = query2->next();

    if(DataAviable == false)
        std::cout << BOLDGREEN << "PASS   : Database NOT updated !" << COLORRESET << std::endl;
    else
        std::cout << BOLDRED << "FAIL   : Database updated !" << COLORRESET << std::endl;

    QCOMPARE(false,DataAviable);

    delete query2;

    if(!m_AutoMode)
    {
        qDebug() << "Press enter to continue...";
        getwchar();
    }
}

void UnitTest_CSQLite_Local_DB::Test_UpdateLocalData_No_database()
{
    QList<struct struct_PackagesUseBox> PackageStorageListEmpty,PackageStorageListComplete;
    struct struct_PackagesUseBox tmp;

    m_LC_Database->SQL_Database_Manager(DEFAULT_DATABASE);

    for(int i=1; i<=3; i++)
    {
        tmp.BoxNumber = i;
        for(int j=0; j<2; j++)
        {
            tmp.BoxContainment.PackageDeliveryCodeList << QString::number(qrand(),10)+randString(6);
        }
        PackageStorageListEmpty.append(tmp);
        tmp.BoxContainment.PackagesExtractCode.clear();
        tmp.BoxContainment.PackageDeliveryCodeList.clear();
    }

    PackageStorageListComplete = m_LMS_Database->GenerateExtractCode(m_LC_Database->Get_SiteName(),PackageStorageListEmpty);

    m_LC_Database->Close_Database(true);
	
    bool UpdateLocalData_Status = false;
    UpdateLocalData_Status = m_LC_Database->UpdateLocalData(PackageStorageListComplete);

    if(UpdateLocalData_Status == false)
        std::cout << BOLDGREEN << "PASS   : Database NOT updated !" << COLORRESET << std::endl;
    else
        std::cout << BOLDRED << "FAIL   : Database updated !" << COLORRESET << std::endl;

    QCOMPARE(false,UpdateLocalData_Status);

    if(!m_AutoMode)
    {
        qDebug() << "Press enter to continue...";
        getwchar();
    }
}

#include "unittest_csqlite_local_db.moc"

int main(int argc, char *argv[])
{
    UnitTest_CSQLite_Local_DB Test_CSQLite_Local_DB;
    QStringList PrgArgsList;
    int Result;
    bool AutoMode = true;

    if(argc == 1)
    {
        PrgArgsList.append("Test_CSQLite_Local_DB");
        PrgArgsList.append("-v1");

        Test_CSQLite_Local_DB.Set_AutoMode(true);
    }

    if(argc > 2)
    {
        PrgArgsList.append("Test_CSQLite_Local_DB");
        for(int i = 1; i<=argc-1; i++)
        {
            qDebug() << "Param : " << i << "Total : " << argc;
            PrgArgsList.append(QString(argv[i]));
            PrgArgsList.append("-v2");
        }
        Test_CSQLite_Local_DB.Set_AutoMode(false);
        AutoMode = false;
    }

    if(argc == 2)
    {
        if(strcmp(argv[1],"UpdateLocalData") == 0)
        {
            PrgArgsList.append("Test_CSQLite_Local_DB");
            PrgArgsList.append("Test_UpdateLocalData_NormalUse");
            PrgArgsList.append("Test_UpdateLocalData_NormalUse_AllBoxes");
            PrgArgsList.append("Test_UpdateLocalData_EmptyList");
            PrgArgsList.append("Test_UpdateLocalData_WrongBoxes");
            PrgArgsList.append("Test_UpdateLocalData_No_database");

            Test_CSQLite_Local_DB.Set_AutoMode(false);
            AutoMode = false;
        }
        else
        {
            PrgArgsList.append("Test_CSQLite_Local_DB");
            PrgArgsList.append(QString(argv[1]));
            PrgArgsList.append("-v2");

            Test_CSQLite_Local_DB.Set_AutoMode(false);
            AutoMode = false;
        }
    }

    Result = QTest::qExec(&Test_CSQLite_Local_DB, PrgArgsList);

    if(!AutoMode)
    {
        qDebug() << "End of Test_CSQLite_Local_DB, press enter to close...";
        getwchar();
    }

    return Result;
}
