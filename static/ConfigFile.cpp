#include "ConfigFile.h"

ConfigFile::ConfigFile()
{
    if ( QFile::exists("config.ini") )
    {
        qDebug()<< "Le fichier existe !";
        QSettings config { "config.ini", QSettings::IniFormat };
        QStringList cles { config.allKeys() };
        auto addLines = [&config](QString fileName, QString &cle)-> void
        {
            QString valeur { config.value(cle, "default").toString() };
            qDebug()<< valeur;
            QFile file{QDir::homePath() + "/AppData/Local/Temp/" + fileName};

            if (file.open(QIODevice::Append | QIODevice::Text))
            {
                QTextStream out(&file);
                out << valeur.replace("/", "\\") << "\n";
            }
        };
		

        for (auto &cle : cles)
        {
            QString group { cle };
            group.truncate(group.indexOf( QChar('/') ) );

            if ( group == "REPERTOIRES" ) //on ajoute les repertoires du fichier config au fichier noUser
            {
                addLines("noUser.ini" , cle);
            }
            else if ( group == "UTILISATEUR" )
            {
                addLines("user.ini" , cle);

            }
            else if ( group == "TACHES" )
            {
                addLines("taches.ini" , cle);

            }
            else if ( group == "BUREAU" )
            {
                addLines("bureau.ini" , cle);

            }
            else if ( group == "ROAMING" )
            {
                addLines("userRoaming.ini" , cle);

            }
            else if ( group == "LOCAL" )
            {
                addLines("userLocal.ini" , cle);

            }
        }
    }
    else
    {
        qDebug()<< "!Le fichier n'existe pas!";
        QMessageBox::critical(NULL, "config.ini", "Le fichier config.ini n'est pas accessible    ");
        exit(1);
    }

}
ConfigFile::~ConfigFile()
{

}
