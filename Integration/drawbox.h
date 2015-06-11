#ifndef DRAWBOX_H
#define DRAWBOX_H
/*!
 *
 * \file drawBox.h
 * \class drawBox  "Header"
 * \brief Header de la classe drawBox
 *
 * Dessine des consignes.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

class drawBox : public QObject, public QGraphicsRectItem
{
   Q_OBJECT

private :
    int m_IdRect;

    
public:
    explicit drawBox(int id = 0, QGraphicsItem * parent = 0 );
    ~drawBox();

    int getIdRect(){return m_IdRect;}
    void mousePressEvent ( QGraphicsSceneMouseEvent  * event );


    void setColor(int);
    void drawRect(int ,int , int, int);


signals :
    void MyId(int);

private:
    QString m_porte;

};

#endif // DRAWBOX_H
