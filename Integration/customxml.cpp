#include "customxml.h"
#include <QDir>

CustomXml::CustomXml(QString filename, QString Domroot)
{
    QDir dirxml;//creation d'un dossier
    dirxml.mkpath("XML");

    m_Filename = "./XML/"+filename;
    m_Domroot = Domroot;

    m_fileXml = new QFile(m_Filename);

    m_doc = new QDomDocument();

    if(InitializationFileXml())
    {
         qDebug()<<"Creation "<<m_Filename<<" reussi";
         if (!m_doc->setContent(m_fileXml))
         {
               m_fileXml->close();
               qDebug()<<"doc ouverture impossible";
         }
         m_fileXml->close();
    }
    else{
         qDebug()<<"Creation file xml failed";
    }

}

CustomXml::CustomXml()
{
    m_doc = new QDomDocument();
    m_fileXml  = new QFile();
}

CustomXml::~CustomXml()
{
    delete m_doc;
    delete m_fileXml;
}

bool CustomXml::InitializationFileXml( QIODevice::OpenMode mode)
{

    bool retour = true;
    int taille = 10;

    if(!m_fileXml->open(mode))
    {
       m_fileXml->close();
       retour = false;
       qDebug()<<"ouverture impossible";
    }
    else{

            if(IsFileXmlEmtpy())
            {
                m_fileXml->close();
                QDomProcessingInstruction procInstr = m_doc->createProcessingInstruction( "xml",
                       "version='1.0' encoding='UTF-8'");

                m_doc->appendChild(procInstr);
                m_doc->insertBefore(procInstr,m_doc->firstChild());

                m_root = m_doc->createElement(m_Domroot);
                m_doc->appendChild(m_root);

                m_fileXml->open(QIODevice::WriteOnly);
                QTextStream stream(m_fileXml);
                m_doc->save(stream,taille);
                m_fileXml->close();

            }
            else{

                m_root = m_doc->documentElement();
            }
           m_fileXml->close();
    }

    return retour;
}

bool CustomXml::OpenFileXml(QIODevice::OpenMode mode)
{
    bool retour = false;


    if(m_fileXml->open(mode))
    {
        retour = true;
    }

    return retour;
}

void CustomXml::CloseFileXml()
{
    m_fileXml->close();
}

void CustomXml::SetFilename(const QString &filename)
{
    m_Filename ="./XML/"+filename;

    m_fileXml->setFileName(m_Filename);

    m_fileXml->open(QIODevice::ReadOnly);
    if (!m_doc->setContent(m_fileXml))
    {
          m_fileXml->close();
          qDebug()<<"doc ouverture impossible";
    }
    m_fileXml->close();
}

bool CustomXml::IsOpenFile()
{
    bool retour = false;

    if(m_fileXml->isOpen())
    {
        retour = true;
    }

    return retour;

}

bool CustomXml::IsFileXmlEmtpy()
{
    bool retour = false;

    QByteArray byteA = m_fileXml->readAll();

    if(byteA.isEmpty())
    {
        retour = true;
    }

    return retour;
}

bool CustomXml::isChildrootExist(QString childroot)
{

    bool retour = false;

        m_root = m_doc->documentElement();
        m_noeud_root = m_root.firstChild();

        while(!m_noeud_root.isNull() && (retour == false))
        {
            if(m_noeud_root.toElement().tagName() == childroot)
            {
                m_childroot = m_noeud_root.toElement();
                retour = true;
            }
            m_noeud_root = m_noeud_root.nextSibling();
        }


    return retour;
}

bool CustomXml::isElemExist(QString childroot,QString element)
{

    bool retour = false;


        m_root = m_doc->documentElement();
        m_noeud_root = m_root.firstChild();

        while(!m_noeud_root.isNull() && (retour == false))
        {
            if(m_noeud_root.toElement().tagName() == childroot)
            {
                m_noeud_child = m_noeud_root.toElement().firstChild();

                while(!m_noeud_child.isNull() && (retour == false))
                {
                    if(m_noeud_child.toElement().tagName() == element)
                    {
                        m_element = m_noeud_child.toElement();
                        retour = true;
                    }
                    m_noeud_child = m_noeud_child.nextSibling();
                }

            }
            m_noeud_root = m_noeud_root.nextSibling();
        }


    return retour;
}

bool CustomXml::WriteFileXml(QString childroot, QString element, QString attribut, QString value)
{
    bool retour = true;
    int taille = 10;


    m_root = m_doc->documentElement();

    if(!isChildrootExist(childroot))
    {
       m_childroot =  m_doc->createElement(childroot);
       m_root.appendChild(m_childroot);
       m_element = m_doc->createElement(element);
       m_element.setAttribute(attribut,value);
       m_childroot.appendChild(m_element);
       OpenFileXml();
       QTextStream stream(m_fileXml);
       m_doc->save(stream,taille);
       CloseFileXml();
    }
    else{

        if(!isElemExist(childroot,element))
        {

            m_element = m_doc->createElement(element);
            m_element.setAttribute(attribut,value);
            m_childroot.appendChild(m_element);
            OpenFileXml();
            QTextStream stream(m_fileXml);
            m_doc->save(stream,taille);
            CloseFileXml();


        }
        else{

            m_element.setAttribute(attribut,value);
            m_childroot.appendChild(m_element);
            OpenFileXml();
            QTextStream stream(m_fileXml);
            m_doc->save(stream,taille);
            CloseFileXml();
        }
    }


    return retour;

}

bool CustomXml::WriteFileXml(QString childroot, QString element, QString attribut)
{
    bool retour = true;
    int taille = 10;


    m_root = m_doc->documentElement();

    if(!isChildrootExist(childroot))
    {

       m_childroot =  m_doc->createElement(childroot);
       m_root.appendChild(m_childroot);
       m_element = m_doc->createElement(element);
       m_element.setAttribute(attribut,"");
       m_childroot.appendChild(m_element);
       OpenFileXml();
       QTextStream stream(m_fileXml);
       m_doc->save(stream,taille);
       CloseFileXml();
    }
    else{

        if(!isElemExist(childroot,element))
        {

            m_element = m_doc->createElement(element);
            m_element.setAttribute(attribut,"");
            m_childroot.appendChild(m_element);
            OpenFileXml();
            QTextStream stream(m_fileXml);
            m_doc->save(stream,taille);
            CloseFileXml();

        }
        else{

            m_element.setAttribute(attribut,"");
            m_childroot.appendChild(m_element);
            OpenFileXml();
            QTextStream stream(m_fileXml);
            m_doc->save(stream,taille);
            CloseFileXml();
        }
    }


   return retour;
}

bool CustomXml::WriteFileXml(QString childroot, QString element, QString attribut, int value)
{
    bool retour = true;
    int taille = 10;


    m_root = m_doc->documentElement();

    if(!isChildrootExist(childroot))
    {
       m_childroot =  m_doc->createElement(childroot);
       m_root.appendChild(m_childroot);
       m_element = m_doc->createElement(element);
       m_element.setAttribute(attribut,value);
       m_childroot.appendChild(m_element);
       OpenFileXml();
       QTextStream stream(m_fileXml);
       m_doc->save(stream,taille);
       CloseFileXml();
    }
    else{

        if(!isElemExist(childroot,element))
        {

            m_element = m_doc->createElement(element);
            m_element.setAttribute(attribut,value);
            m_childroot.appendChild(m_element);
            OpenFileXml();
            QTextStream stream(m_fileXml);
            m_doc->save(stream,taille);
            CloseFileXml();


        }
        else{

            m_element.setAttribute(attribut,value);
            m_childroot.appendChild(m_element);
            OpenFileXml();
            QTextStream stream(m_fileXml);
            m_doc->save(stream,taille);
            CloseFileXml();
        }
    }


    return retour;
}

void CustomXml::removeLastChild()
{
    m_root = m_doc->documentElement();
    QDomElement child = m_root.lastChild().toElement();

    QDomNode node = m_root.removeChild(child);

    if(!node.isNull())
    {
        CloseFileXml();
        m_fileXml->remove();
        m_fileXml->setFileName(m_Filename);
        CloseFileXml();
        OpenFileXml();
        QTextStream stream(m_fileXml);
        stream << m_doc->toString();
        CloseFileXml();
    }
    else{
        qDebug()<<"failure suppr element child xml";
    }
}

QString CustomXml::Getvalue(QString childroot, QString element, QString attribut)
{
    QString value;

    m_root = m_doc->documentElement();
    m_noeud_root = m_root.firstChild();

    while(!m_noeud_root.isNull())
    {
        if(m_noeud_root.toElement().tagName() == childroot)
        {
            m_noeud_child = m_noeud_root.toElement().firstChild();

            while(!m_noeud_child.isNull())
            {
                if(m_noeud_child.toElement().tagName() == element)
                {
                    if(m_noeud_child.toElement().hasAttribute(attribut))
                    {
                        value = m_noeud_child.toElement().attribute(attribut,0);
                    }
                }
                m_noeud_child = m_noeud_child.nextSibling();
            }

        }
        m_noeud_root = m_noeud_root.nextSibling();
   }

   return value;

}

int CustomXml::GetvaluetoInt(QString childroot, QString element, QString attribut)
{
    int value =-1;

    m_root = m_doc->documentElement();
    m_noeud_root = m_root.firstChild();

    while(!m_noeud_root.isNull())
    {
        if(m_noeud_root.toElement().tagName() == childroot)
        {
            m_noeud_child = m_noeud_root.toElement().firstChild();

            while(!m_noeud_child.isNull())
            {
                if(m_noeud_child.toElement().tagName() == element)
                {
                    if(m_noeud_child.toElement().hasAttribute(attribut))
                    {
                        value = m_noeud_child.toElement().attribute(attribut,0).toInt();
                    }
                }
                m_noeud_child = m_noeud_child.nextSibling();
            }

        }
        m_noeud_root = m_noeud_root.nextSibling();
   }

   return value;

}

QList<QList<struct_PackagesUseBox> > CustomXml::GetListSaveTempDelivery()
{
   QList<QList<struct_PackagesUseBox> > listsPackages;
   QList<struct_PackagesUseBox> listpackagetmp;
   int taillechildroot =0;
   int nbbox = 0;

   taillechildroot = ChildRootCount();//recupere le nombre de livraison sauvegarder
   qDebug()<<"taillechild:"<<taillechildroot;
   m_root = m_doc->documentElement();
   m_noeud_root = m_root.firstChild();

    if(taillechildroot >0)
    {
       while(!m_noeud_root.isNull())
       {
               listpackagetmp.clear();
               m_noeud_child = m_noeud_root.toElement().firstChild();
               int j=0;

               while(!m_noeud_child.isNull())
               {

                   for(int i=0;i<m_noeud_child.toElement().attributes().count();i++)
                   {

                        QString name_attribut = m_noeud_child.toElement().attributes().item(i).nodeName();
                        qDebug()<<"childattr:"<<name_attribut;
                       if(j==0)
                       {
                           qDebug()<<"attribut :"<<m_noeud_child.toElement().attribute(name_attribut).toInt();
                           struct_PackagesUseBox package;
                           package.BoxNumber  = m_noeud_child.toElement().attribute(name_attribut).toInt();
                            listpackagetmp<<package;
                            nbbox = i+1;
                       }
                       else{

                           if((nbbox-1) <= i)
                           {
                             listpackagetmp[nbbox-1].BoxContainment.PackageDeliveryCodeList <<  m_noeud_child.toElement().attribute(name_attribut);
                           }
                           else{
                               listpackagetmp[i].BoxContainment.PackageDeliveryCodeList <<  m_noeud_child.toElement().attribute(name_attribut);

                           }

                       }
                   }
                   j++;
                   m_noeud_child = m_noeud_child.nextSibling();
               }

            listsPackages << listpackagetmp;
            m_noeud_root = m_noeud_root.nextSibling();
      }

    }

    return listsPackages;
}

bool CustomXml::SaveListPackagesTemp(QList<struct_PackagesUseBox> tempackagelist)
{

    bool result = true;
    int numberDelivery;
    int sizelist=0;

    //recupere le nombre de livraison sauvegarder dans le fichier
    numberDelivery = this->ChildRootCount();
    numberDelivery++;

    QString numbdelivery = "Livraison_"+QString::number(numberDelivery);

    if(!tempackagelist.isEmpty())
    {
        for(int i = 0;i< tempackagelist.size();i++)
        {

            //enregistre dans fichier xml
            QString numBox = "Num_"+QString::number(i+1);
            this->WriteFileXml(numbdelivery,"BoxNumbers",numBox,tempackagelist[i].BoxNumber);

            for(int j=0;j < tempackagelist[i].BoxContainment.PackageDeliveryCodeList.size();j++)
            {
                QString numPackage;

                if(i==0)
                {
                    numPackage = "NumP_"+QString::number(j+1);
                    sizelist = tempackagelist[i].BoxContainment.PackageDeliveryCodeList.size();
                }
                else{
                    sizelist++;
                    numPackage = "NumP_"+QString::number(sizelist);

                }
                this->WriteFileXml(numbdelivery,"ListPackages",numPackage,tempackagelist[i].BoxContainment.PackageDeliveryCodeList[j]);
            }
            if(i>0)
            {
               sizelist += tempackagelist[i-1].BoxContainment.PackageDeliveryCodeList.size();
            }
        }

    }
    else{
        result = false;
    }

    return result;
}

int CustomXml::ChildRootCount()
{
    int taille = 0;

    m_root = m_doc->documentElement();
    m_noeud_root = m_root.firstChild().toElement();

    while(!m_noeud_root.isNull())
    {
        taille++;
        m_noeud_root = m_noeud_root.nextSibling();
    }

    return taille;
}


