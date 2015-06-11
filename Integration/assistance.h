#ifndef ASSISTANCE_H
#define ASSISTANCE_H

#include <QWidget>
#include "showwidgets.h"
#include "thread_linphonec.h"

class Thread_linphonec;

namespace Ui {
class Assistance;
}

class Assistance : public QWidget
{
    Q_OBJECT
    
public:
    explicit Assistance(QWidget *parent = 0);
    ~Assistance();

    void SetShowWidgets(ShowWidgets *TShowWidgets){m_ShowWidgets = TShowWidgets;}

public slots:
    void ReceiveIndexWidget(int index);
    void Call();
    void EndCall();
    void ReturnWidgetCurrent();

signals:
    void startThreadLinphonec();
    void EndThreadLinphonec();
private:
    Ui::Assistance *ui;

    ShowWidgets *m_ShowWidgets;
    int m_posWidgetCurrent;
    Thread_linphonec *m_thread_linphone;
};

#endif // ASSISTANCE_H
