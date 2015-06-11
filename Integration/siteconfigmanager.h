#ifndef SITECONFIGMANAGER_H
#define SITECONFIGMANAGER_H

#include <QWidget>

#include "showwidgets.h"
#include "maintenancemenu.h"
#include "csqlite_local_db.h"

class SiteConfigManager : public QWidget
{
    Q_OBJECT

    private:
        ShowWidgets *m_ShowWidgets;
        MaintenanceMenu *m_MaintenanceMenu;
        CSQLite_Local_DB *m_BD;

    public:
        explicit SiteConfigManager(MaintenanceMenu *TMaintenanceMenu = NULL, CSQLite_Local_DB *BD = NULL);
        void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}
        ~SiteConfigManager();

    signals:

    public slots:

};

#endif // SITECONFIGMANAGER_H
