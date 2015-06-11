#include <QDebug>

#include "siteconfigmanager.h"

SiteConfigManager::SiteConfigManager(MaintenanceMenu *TMaintenanceMenu, CSQLite_Local_DB *BD)
{
    if( (TMaintenanceMenu != NULL) || (BD != NULL) )
    {
        m_MaintenanceMenu = TMaintenanceMenu;
        m_BD = BD;


    }
    else
        qDebug() << "[FATAL ERROR] : Uninitiatized pointer (= NULL)";
}

SiteConfigManager::~SiteConfigManager()
{

}

