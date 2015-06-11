#ifndef CUSTOMXML_H
#define CUSTOMXML_H
/*!
 *
 * \file CustomXml.h
 * \class CustomXml "Header"
 * \brief Header de la classe CustomXml
 *
 * Gestion d'un fichier xml.
 *
 * \author GUEDON Damien
 * \version 1
 * \date 8 mai 2015
 *
 */
#include <QtXml>
#include <QIODevice>
#include <QFile>
#include <QTextStream>

#include <DataStructures.h>

class CustomXml
{
    public:
        CustomXml(QString filename , QString Domroot);
        CustomXml();
        ~CustomXml();

        bool OpenFileXml(QIODevice::OpenMode mode = QIODevice::ReadWrite);
        void CloseFileXml();
        void SetFilename(const QString &filename);

        bool WriteFileXml(QString childroot,QString element, QString attribut);
        bool WriteFileXml(QString childroot, QString element, QString attribut,int value);
        bool WriteFileXml(QString childroot,QString element, QString attribut, QString value);

        void removeLastChild();

        QString Getvalue(QString childroot,QString element,QString attribut);
        int GetvaluetoInt(QString childroot,QString element,QString attribut);
        QList< QList<struct_PackagesUseBox> > GetListSaveTempDelivery();
        bool SaveListPackagesTemp(QList<struct_PackagesUseBox> tempackagelist);
        int ChildRootCount();


        bool IsOpenFile();
        bool IsFileXmlEmtpy();
        bool isElemExist(QString childroot, QString element);
        bool isChildrootExist(QString childroot);


    private:
        bool InitializationFileXml(QIODevice::OpenMode mode = QIODevice::ReadWrite);

        QDomDocument *m_doc;
        QFile *m_fileXml;

        QDomElement m_root;
        QDomElement m_childroot;
        QDomElement m_element;

        QDomNode m_noeud_root;
        QDomNode m_noeud_child;

        QString m_Filename , m_Domroot;

};

#endif // CUSTOMXML_H
