#include "threaddeleteextractcode.h"
#include "showpackagebox.h"

ThreadDeleteExtractCode::ThreadDeleteExtractCode(int id,CSQLite_Local_DB *BD,ShowPackageBox *WShowPackageBox) : QThread()
{
    m_Id = id;
    m_DB = BD;
    m_WShowPackageBox = WShowPackageBox;

    connect(m_WShowPackageBox,SIGNAL(SendCodeThread(QString)),this,SLOT(startThread(QString)));

}

void ThreadDeleteExtractCode::run()
{
    bool retour;

    retour = m_DB->DeleteExtractCode(m_ExtractCode);
    emit terminatedThread(m_Id,retour);
    exit();
}

void ThreadDeleteExtractCode::startThread(QString code)
{
    m_ExtractCode = code;
    start();
}
