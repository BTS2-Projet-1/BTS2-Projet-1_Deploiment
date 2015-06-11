#ifndef COUNTRY_H
#define COUNTRY_H
/*!
 *
 * \file country.h
 * \class Country "Header"
 * \brief Header de la classe Country
 *
 * Permet de changer de langue.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */
#include <QTranslator>

enum LANGUAGES{FRENCH = 0,ENGLISH = 1};

class Country
{

private:
    QTranslator *m_Linguist;

public:
    Country(LANGUAGES lang=ENGLISH);
    ~Country();

    void setLanguage(LANGUAGES lang);

};

#endif // COUNTRY_H
