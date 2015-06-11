#include "drawbox.h"

#include <QDebug>
#include <QBrush>
#include <QColor>
#include "DataStructures.h"
#include <QTextItem>


drawBox::drawBox(int id, QGraphicsItem * parent ) :
   QObject(), QGraphicsRectItem(parent)
{
    m_IdRect = id;
    m_porte = QString::number(m_IdRect);

    this->setAcceptedMouseButtons(Qt::LeftButton);
}

drawBox::~drawBox()
{

}

void drawBox::mousePressEvent ( QGraphicsSceneMouseEvent  * event )
{

    emit MyId(m_IdRect);

    QGraphicsRectItem::mousePressEvent(event);
}

void drawBox::drawRect(int x, int y, int width, int height)
{
    QPen pen;

    pen.setColor(Qt::black);
    pen.setWidth(2);
    setRect( x , y , width, height);
    setPen(pen);
}


void drawBox::setColor(int status)
{
    QColor couleur;
    QRgb rgb;

    switch (status)
    {
        case  OK :
            rgb = 0xFF58dd58;//vert
            couleur.setRgba(rgb);
            break;

        case  KO :
            rgb = 0xFFff5b52;//rouge
            couleur.setRgba(rgb);
            break;

        case  SELECTED :
            rgb = 0xFF1DACD6;//bleu
            couleur.setRgba(rgb);
            break;

        case  NOCHECKED :
             couleur = Qt::gray;
        break;

        default :
        rgb = 0xFFFFFFFF;//ETAT INCONNU
        couleur.setRgba(rgb);
        break;
    }
    setBrush( QBrush(couleur));
}
