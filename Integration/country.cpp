#include "country.h"
#include <QApplication>

Country::Country(LANGUAGES lang)
{
    m_Linguist = new QTranslator();

    setLanguage(lang);

}

Country::~Country()
{
    delete m_Linguist;
}

void Country::setLanguage(LANGUAGES lang)
{
    qApp->removeTranslator(m_Linguist);

    switch(lang)
    {
        case ENGLISH :
                      m_Linguist->load(":/Linguist/english");
                      break;

        case FRENCH :
                     m_Linguist->load(":/Linguist/french");
                     break;

    }

    qApp->installTranslator(m_Linguist);
}

