#include "boxchoice.h"
#include "ui_boxchoice.h"


/*!
 *
 * \file boxchoice.cpp
 * \class BoxChoice "Definition"
 * \brief Definition de la classe BoxChoice
 *
 * IHM Sélection des consignes.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */

#include <QGraphicsTextItem>
#include <QToolTip>

#include <QDebug>


BoxChoice::BoxChoice(MenuDelivery *WMenuDelivery,CSQLite_Local_DB *bd,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoxChoice)
{
    ui->setupUi(this);

    m_BD = bd;
    m_WMenuDelivery = WMenuDelivery;

    //connexion boutons
    connect(m_WMenuDelivery,SIGNAL(refreshGraphicsBox()),this,SLOT(Refresh()));

    //creation de la scene
    m_Scene = new QGraphicsScene(this);
    ui->G_Show_Box->setScene(m_Scene);

    //récupération de la liste des box
    m_ListBox.append(m_BD->GetBoxesList());

    createBoxes(m_ListBox);


}

BoxChoice::~BoxChoice()
{
    delete ui;
}

/*!
 * \fn void BoxChoice::retranslateUi()
 * \brief Mis à jour de la traduction .
 */
void BoxChoice::retranslateUi()
{
    ui->retranslateUi(this);
}

QGraphicsView* BoxChoice::GetGraphicsView()
{
    return ui->G_Show_Box;
}

QGraphicsScene* BoxChoice::GetScene()
{
    return m_Scene;
}

/*!
 * \fn QList<int> BoxChoice::GetBoxSelected()
 * \brief Récupère la liste des consignes selectionées.
 * \return Retourne la liste des consignes sélectionnées.
 *         Si aucune consigne n'est sélectionnée alors elle retourne une liste vide.
 */
QList<int> BoxChoice::GetBoxSelected()
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

/*!
 * \fn void BoxChoice::createBoxes()
 * \brief Créer les boites dans le QGraphicsScene.
 */

void BoxChoice::createBoxes(const QList<struct_DoorGeom> &ListBox)
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


void BoxChoice::manageRect(int id)
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

            default :
                break;
        }
        m_listdrawBox[id]->setColor(m_ListBox[id].status);
    }

}

void BoxChoice::setDoorUnvailable(int id)
{

    m_ListBox[id].status = KO;

    m_listdrawBox[id]->setColor(m_ListBox[id].status);

}

void BoxChoice::setDoorAvailable(int id)
{

    m_ListBox[id].status = OK;

    m_listdrawBox[id]->setColor(m_ListBox[id].status);

}

void BoxChoice::setDoorNoChecked(int id)
{

    m_ListBox[id].status = NOCHECKED;
    m_listdrawBox[id]->setColor(m_ListBox[id].status);

}

void BoxChoice::setDoorChecked(int id)
{

    m_ListBox[id].status = SELECTED;
    m_listdrawBox[id]->setColor(m_ListBox[id].status);

}

void BoxChoice::ShowMessageError(QString mesg)
{
   int x;
   int y;

   x = mapToGlobal(ui->B_Assistance->pos()).x()+ui->B_Assistance->width()+10;
   y = mapToGlobal(ui->B_Assistance->pos()).y()-10;

   QToolTip::showText(QPoint(x,y),mesg,ui->B_Assistance);
}

void BoxChoice::Refresh(){

   m_Scene->clear();
   m_listdrawBox.clear();
   m_ListBox.clear();

   m_ListBox.append(m_BD->GetBoxesList());


   createBoxes(m_ListBox);

   ui->G_Show_Box->update();

}

//buttons clicked

void BoxChoice::on_B_Valide_clicked()
{
    QList<int> boxSelected;

    //recupère liste des consignes selectionnées
    boxSelected = GetBoxSelected();

    if(!boxSelected.isEmpty()){
       //envoi de la liste a ScanPackage
       emit SendListBoxSelected(boxSelected);

       //affiche IHM Scanpackage
       m_ShowWidgets->ShowWScanPackage();
    }
    else{
       //affiche message erreur
        ShowMessageError(tr("Attention!!!<br/> Vous n'avez pas s&eacute;lectionn&eacute; de consigne."));
    }

}

void BoxChoice::on_B_Cancel_clicked()
{
    //retour au menu de livraison
    m_ShowWidgets->ShowWMenuDelivery();
}

void BoxChoice::on_B_Help_clicked()
{

}

void BoxChoice::on_B_Assistance_clicked()
{

}
