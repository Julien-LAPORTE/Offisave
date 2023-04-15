#include "NetUse.h"

NetUse::NetUse(QStringList config, QObject *parent) : QProcess(parent),
    m_path {config[0]}, m_user{config[1]}, m_password{config[2]}
{
    setProgram("cmd");
    qDebug()<< "config.size : " << config.size() ;
}

NetUse::~NetUse()
{
    qDebug()<< "Destruction de l'instance de la class NetUse";
}

bool NetUse::mount()
{
    if (unmount()) qDebug()<< "on démonte";
    QStringList arguments{ "/C", "net", "use", m_path, "/USER:"+m_user, m_password  };
    setArguments(arguments);
    start();
    return waitForFinished();
}
bool NetUse::unmount()
{
    QStringList arguments{"/C", "net", "use", m_path, "/DELETE", "/YES" };
    setArguments(arguments);
    qDebug()<<"Liste des arguments démontage :" << this->arguments();
    start();
    return waitForFinished();
}
bool NetUse::checkPath()
{
    unmount();
    qDebug()<< "m_path :" << m_path;
    qDebug()<< "m_user :" << m_user;
    qDebug()<< "m_password :" << m_password;

    QString testFile;
    QString s;
    if ( (m_path[m_path.size()-1] != "/") or (m_path[m_path.size()-1] != "\\") )
    {
        testFile="/ksjbvcifbzsefnintierjtn78676564RVUYF64563EDCJHG7IOFC5437CVY";
    }
    else
    {
        testFile="ksjbvcifbzsefnintierjtn78676564RVUYF64563EDCJHG7IOFC5437CVY";
    }

    bool resultat{false};

    QStringList arguments { "/C" };
    if ( m_path[0].isLetter() )//Se le path commence par une lettre on test la copie direct
    {
        QFile::setPermissions(m_path+testFile, QFileDevice::Permissions(0x2|0x4|0x20|0x40|0x200|0x400|0x2000|0x4000));
        QFile::remove(m_path+testFile);
        resultat = QFile::copy(":/config.ini", m_path+testFile);
        if (resultat)
        {
            QFile::setPermissions(m_path+testFile, QFileDevice::Permissions(0x2|0x4|0x20|0x40|0x200|0x400|0x2000|0x4000));
            QFile::remove(m_path+testFile);
        }
        else
        {
            QMessageBox::warning(NULL, "Erreur", "Impossible d'écrire sur : " + m_path + "\nAbandon de l'installation");
        }
        return resultat;
    }
    else //Sinon on utilise net use pour tester l'accès au partage réseau
    {
        if ( m_user.isEmpty() and m_password.isEmpty() )
        {
           arguments << "net" << "use" << m_path ;
        }

        else if ( m_password.isEmpty() )
        {
            arguments << "net" << "use" << m_path << "/USER:"+m_user ;
        }

        else
        {
           arguments << "net" << "use" << m_path << "/USER:"+m_user << m_password ;
        }
        setArguments(arguments);
        start();
        waitForFinished(5000);

        // On récupère la sortie standard
        QByteArray ba = readAllStandardOutput();
        QTextStream stream(&ba);

        QStringList standardOutput;
        int i{0};
        while (!stream.atEnd())
        {
            standardOutput << stream.readLine();
            qDebug()<< standardOutput[i];
            i++;
        }

        qDebug()<< "standardOutput.size() :" << standardOutput.size() ;
        //On compare l'avant dernière ligne de la sortie standard car la dernière est vide...
        if (standardOutput.size() < 2 and standardOutput.size() > 0)
        {
            resultat = (standardOutput[i-1] == "La commande s'est termin‚e correctement.");
        }
        else if (standardOutput.size() == 0)
        {
            resultat = false;
        }
        else
        {
            resultat = (standardOutput[i - 2] == "La commande s'est termin‚e correctement.");
        }

        //Test d'écriture sur le partage si montage OK
        if ( (resultat) and ( QFile::copy(":/config.ini", m_path+"/"+testFile) ) )
        {
            QFile::setPermissions(m_path+"/"+testFile, QFileDevice::Permissions(0x2|0x4|0x20|0x40|0x200|0x400|0x2000|0x4000));
            QFile::remove(m_path+"/"+testFile);
            unmount();
        }
        else if ( resultat )
        {
            QMessageBox::warning(NULL, "Erreur", "Le partage est accessible mais n'autorise pas l'écriture\nAbandon de l'installation");
            unmount();
            return false;
        }
        else
        {
            unmount();
            if ( m_path != "" )QMessageBox::warning(NULL, "Erreur", "Le partage n'a pas été trouvé sur le réseau\nAbandon de l'installation");
        }
        return resultat;
    }
}
