#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QMainWindow>
#include <QGraphicsView>
#include <QGridLayout>
#include <QGraphicsScene>
#include <QPushButton>
#include <QWidget>
#include <QtGui>

#include "DataStructures.h"
#include "drawbox.h"

class Test_CreateBoxesTest : public QMainWindow
{
    Q_OBJECT

public:
    Test_CreateBoxesTest(QMainWindow *parent=0);
    void createBoxes(const QList<struct_DoorGeom> &ListBox);

private:
    QWidget *m_widget;
    QGraphicsView *m_graphicsview;
    QPushButton *m_button;
    QGraphicsScene *m_Scene;
    QList<struct_DoorGeom> m_ListBoxNormal;
    QList<struct_DoorGeom> m_ListBox;
    QList<struct_DoorGeom> m_ListBoxEmpty;
    QList<struct_DoorGeom> m_ListBoxTypeInvalid;
    QList<struct_DoorGeom> m_ListBoxStatusInvalid;
    QList<int> m_ListBoxSelected;
    QList<drawBox *> m_listdrawBox;

    QList<int> GetBoxSelected();

    //methode initcase
    void initCaseNormal();
    void initCaseListEmpty();
    void initCaseTypeBoxInvalid();
    void initCaseStatusBoxInvalid();


public slots:
    void manageRect(int);


private Q_SLOTS:

    void cleanupTestCase();
    void test_createBoxe_UseNormal();
    void test_createBoxe_UseListEmpty();
    void test_createBoxe_UseTypeBoxInvalid();
    void test_createBoxe_UseStatusBoxInvalid();

};

Test_CreateBoxesTest::Test_CreateBoxesTest(QMainWindow *parent):QMainWindow(parent)
{
    //creation de l'interface
    m_widget = new QWidget(this);
    this->setGeometry(0,0,650,600);

    QGridLayout *layout1 = new QGridLayout(this);
    setCentralWidget(m_widget);
    m_graphicsview = new QGraphicsView(this);

    layout1->addWidget(m_graphicsview);

    m_Scene = new QGraphicsScene(this);
    m_graphicsview->setScene(m_Scene);
    m_widget->setLayout(layout1);

}
//creation listnormal
void Test_CreateBoxesTest::initCaseNormal()
{
    //creation de la listeboxNormal
    for(int i = 0 ; i < 2 ; i++)
    {
       struct_DoorGeom boxdoorgeom;

       boxdoorgeom.BoxNumber = i+1;
       boxdoorgeom.x = 0;
       boxdoorgeom.y = i*2;

       if((boxdoorgeom.BoxNumber%2) == 0)
       {
           boxdoorgeom.status = OK;
           boxdoorgeom.BoxSize ="A";
       }
       else{

           boxdoorgeom.status = KO;
           boxdoorgeom.BoxSize ="CM";
       }
     m_ListBoxNormal << boxdoorgeom;
    }

    for(int i = 0 ; i < 2 ; i++)
    {
       struct_DoorGeom boxdoorgeom;

       boxdoorgeom.BoxNumber = 3+i;
       boxdoorgeom.x = 1;
       boxdoorgeom.y = i*2;

       if((boxdoorgeom.BoxNumber%2) == 0)
       {
           boxdoorgeom.status = KO;
           boxdoorgeom.BoxSize ="AW";
       }
       else{

           boxdoorgeom.status = OK;
           boxdoorgeom.BoxSize ="CW";
       }
     m_ListBoxNormal << boxdoorgeom;
    }

}

void Test_CreateBoxesTest::initCaseListEmpty()
{
    m_ListBoxEmpty.clear();
}

void Test_CreateBoxesTest::initCaseTypeBoxInvalid()
{
    initCaseNormal();
    m_ListBoxTypeInvalid.append(m_ListBoxNormal);

    for(int i = 1 ; i< m_ListBoxTypeInvalid.size();i++)
    {
        m_ListBoxTypeInvalid[i].BoxSize.clear();
        m_ListBoxTypeInvalid[i].BoxSize = 'a'+i;
    }

}

void Test_CreateBoxesTest::initCaseStatusBoxInvalid()
{
    initCaseNormal();
    m_ListBoxStatusInvalid.append(m_ListBoxNormal);

    for(int i = 1 ; i< m_ListBoxStatusInvalid.size();i++)
    {
        m_ListBoxStatusInvalid[i].status = 10+i;
    }
}


void Test_CreateBoxesTest::cleanupTestCase()
{
}

void Test_CreateBoxesTest::test_createBoxe_UseNormal()
{
    initCaseNormal();

    qDebug()<<"test cas normal";
    //test normal

    m_ListBox.clear();
    m_ListBox.append(m_ListBoxNormal);

    createBoxes(m_ListBox);

    for(int i = 0; i< m_ListBox.size();i++)
    {
        if(m_ListBox[i].status == OK)
        {
           qDebug()<<"Etat consigne "<<m_ListBox[i].BoxNumber <<": "<< "disponible";
        }

        if(m_ListBox[i].status == KO)
        {
           qDebug()<<"Etat consigne "<<m_ListBox[i].BoxNumber <<": "<< "indisponible";
        }

    }

    for(int i=0;i< m_listdrawBox.size();i++)
    {
        qDebug()<<"Consigne "<<m_ListBox[i].BoxNumber<< " Taille: "<<m_ListBox[i].BoxSize<<" :"<< m_listdrawBox[i]->rect().width()<<"x"<<m_listdrawBox[i]->rect().height();
    }

    QCOMPARE(m_ListBox.size(),m_listdrawBox.size());

}

void Test_CreateBoxesTest::test_createBoxe_UseListEmpty()
{
    qDebug()<<"test cas ou on donne une liste vide";

    initCaseListEmpty();

    m_listdrawBox.clear();

    //test quand la liste est vide
    createBoxes(m_ListBoxEmpty);
}

void Test_CreateBoxesTest::test_createBoxe_UseTypeBoxInvalid()
{
    qDebug()<<"test cas ou on donne une liste avec des type de taille invalide";
    initCaseTypeBoxInvalid();
    m_listdrawBox.clear();
    m_ListBox.clear();
    m_ListBox.append(m_ListBoxTypeInvalid);

    createBoxes(m_ListBox);

    for(int i=0;i< m_listdrawBox.size();i++)
    {
        qDebug()<<"Consigne "<<m_ListBoxTypeInvalid[i].BoxNumber<< " Taille: "<<m_ListBoxTypeInvalid[i].BoxSize<<" :"<< m_listdrawBox[i]->rect().width()<<"x"<<m_listdrawBox[i]->rect().height();
    }
}

void Test_CreateBoxesTest::test_createBoxe_UseStatusBoxInvalid()
{
    qDebug()<<"test cas ou on donne une liste avec des type de taille invalide";
    initCaseStatusBoxInvalid();
    m_listdrawBox.clear();
    m_ListBox.clear();
    m_ListBox.append(m_ListBoxStatusInvalid);

    createBoxes(m_ListBox);

    for(int i=0;i< m_listdrawBox.size();i++)
    {
        if(m_ListBoxStatusInvalid[i].status == OK)
        {
           qDebug()<<"Consigne "<<m_ListBoxStatusInvalid[i].BoxNumber<< " etat: "<< "disponible";
        }

        else if(m_ListBoxStatusInvalid[i].status == KO)
        {
            qDebug()<<"Consigne "<<m_ListBoxStatusInvalid[i].BoxNumber<< " etat: "<< "indisponible";
        }

        else{
            qDebug()<<"Consigne "<<m_ListBoxStatusInvalid[i].BoxNumber<< " etat: "<< "inconnu";
        }
    }
}


void Test_CreateBoxesTest::createBoxes(const QList<struct_DoorGeom> &ListBox)
{
    int height = HEIGHT;
    int posX;
    int posY;
    bool typeunknown = false;

    if(!ListBox.isEmpty())//si la liste n'est pas vide
    {

        for (int i=0; i < ListBox.size(); i++)
        {
            //on donne un id pour repérer le clic de la souris
            drawBox *box = new drawBox(i);

            posX = STARTX + ListBox[i].x*WIDTH;
            posY = STARTY + ListBox[i].y*HEIGHT;

            if ((ListBox[i].BoxSize.compare("A")==0) || (ListBox[i].BoxSize.compare("AW")==0)) //casier simple
            {
                height = HEIGHT;
            }
            else if (ListBox[i].BoxSize.compare("CM")==0 || (ListBox[i].BoxSize.compare("CW")==0)) //casier double
            {
                height = HEIGHT*2;
            }
            else{

                typeunknown = true;

            }

            //dessine la consigne
            box->drawRect(posX,posY, WIDTH, height);

            //couleur status
            box->setColor(ListBox[i].status);

            //affiche le numéro de la consigne
            QGraphicsTextItem *text = new QGraphicsTextItem(box);

            if(!typeunknown)
            {
                text->setPlainText(QString::number(ListBox[i].BoxNumber));
            }
            else{
                QString message = QString::number(ListBox[i].BoxNumber) + "\n\n        Taille\n     inconnue";
                text->setPlainText(message);
            }

            //position du text
            text->setPos(posX,posY);

            //ajoute la box a la scene
            m_Scene->addItem(box);

            //ajoute poigné à la porte
            m_Scene->addEllipse(posX+WIDTH-12,posY+height/2-3,6,6,QPen(),QBrush(Qt::black));

            connect(box,SIGNAL(MyId(int)),this,SLOT(manageRect(int)));

            //pour changer la couleur
            m_listdrawBox << box;
        }
    }
    else{

        //aucune consigne
        m_Scene->addText(QString::fromUtf8("     Aucune consigne trouvée.\nMerci de contacter la maintenance."),QFont(this->font().family(),15,-1,true));
    }

}

QList<int> Test_CreateBoxesTest::GetBoxSelected()
{
    QList<int> boxSelected;

    if(!m_ListBox.isEmpty())
    {
      for(int i = 0;i < m_ListBox.size(); i++)
      {
         if(m_ListBox[i].status == SELECTED)
         {
             boxSelected << m_ListBox[i].BoxNumber;
         }
      }
    }

    return boxSelected;
}



void Test_CreateBoxesTest::manageRect(int id)
{

    if((id >=0) && (id < m_ListBox.size()))
    {
        switch (m_ListBox[id].status)
        {
            case  OK :
                m_ListBox[id].status = SELECTED;
                break;

            case  SELECTED :
                m_ListBox[id].status = OK;
                break;

            case  KO:
            m_ListBox[id].status = OK;
            break;
            default :
                break;
        }
        m_listdrawBox[id]->setColor(m_ListBox[id].status);
    }
}


//QTEST_MAIN(Test_CreateBoxesTest)

#include "tst_test_createboxestest.moc"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Test_CreateBoxesTest testbox;

    QStringList param;
    param.append("testbox");
    param.append("-v2");
    if(argc > 1)
    {
        param.append(argv[1]);
    }

    testbox.show();

    QTest::qExec(&testbox,param);

    return app.exec();
}
