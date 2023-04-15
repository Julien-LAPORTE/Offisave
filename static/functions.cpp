#include "functions.h"

void conversion(int &valeurO, int &valeurMo, int &valeurGo )
{
    while (valeurO >= 1048576)
    {
        valeurO -= 1048576;
        valeurMo++;
    }
    while (valeurMo >= 1024)
    {
        valeurMo -= 1024;
        valeurGo++;
    }
}

void verifMax( QFileInfo const &infoFichier, int &valeurO, int &valeurMo, int &valeurGo ,int const max )
{
    if (infoFichier.size() + valeurO < max)
    {
        valeurO += infoFichier.size();
    }
    else if (infoFichier.size() > max)
    {
        valeurMo += infoFichier.size() / 1048576;
    }
    else
    {
        valeurO += infoFichier.size();
    }

    while (valeurO >= 1048576)
    {
        valeurO -= 1048576;
        valeurMo++;
    }
    while (valeurMo >= 1024)
    {
        valeurMo -= 1024;
        valeurGo++;
    }
}


void explorerWindows(QString state)
{
    assert( (state == "off") or (state == "on") && "state doit être off ou on" );
    QProcess explorer;
    QStringList arguments;
    if (state == "off" )
    {
        explorer.setProgram("taskkill");
        arguments
                <<"/F"
                << "/IM"
                << "explorer.exe"
                   ;
    }
    else
    {
        explorer.setProgram("cmd");
        arguments
                <<"/c"
                <<"start"
                <<"%windir%\\explorer.exe"
                    ;
    }
    explorer.setArguments(arguments);
    explorer.start();
    explorer.waitForFinished();
}

QStringList listDirToSave(bool copyFiles)
{
    QStringList repToSave;

    QStringList listFile{ "user.ini", "noUser.ini", "userLocal.ini", "userRoaming.ini" };
    QString tmpDir{ QDir::homePath() + "/AppData/Local/Temp/" };

    for(auto &fileName : listFile)
    {
        QString filePath{ tmpDir + fileName };
        if (copyFiles)
        {
            QFile::setPermissions(filePath, QFileDevice::Permissions(0x2|0x4|0x20|0x40|0x200|0x400|0x2000|0x4000));
            QFile::remove(filePath);
            QFile::copy(":/"+fileName, filePath);
            //On redonne uniquement les droits de lecture et écriture au proprietaire du fichier
            QFile::setPermissions(filePath, QFileDevice::Permissions(0x4000|0x2000));
            ConfigFile configFiles;
            qDebug()<< "filePath : " << filePath;
        }

        QFile qFile{filePath};
        if (!qFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            assert (false && "ERREUR Inventaire FPNO");
        }
        else
        {
            QTextStream in(&qFile);
            if (fileName == "user.ini")
            {
                while(!in.atEnd())
                {
                    QString userDir { QDir::homePath() + "/" };
                    repToSave << userDir + in.readLine();
                }

            }
            else if (fileName == "userLocal.ini")
            {
                while(!in.atEnd())
                {
                    QString localDir { QDir::homePath() + "/AppData/Local/" };
                    repToSave << localDir + in.readLine();
                }
            }
            else if (fileName == "userRoaming.ini")
            {
                while(!in.atEnd())
                {
                    QString roamingDir { QDir::homePath() + "/AppData/Roaming/" };
                    repToSave << roamingDir + in.readLine();
                }
            }
            else
            {
                while(!in.atEnd())
                {
                    repToSave << "C:" + in.readLine().replace("\\", "/");
                }
            }
            qDebug()<< "fileName :" << fileName;
         }
            if (copyFiles)
            {
                //On supprime le fichier une fois lu
                qFile.close();
                QFile::setPermissions(filePath, QFileDevice::Permissions(0x2|0x4|0x20|0x40|0x200|0x400|0x2000|0x4000));
                QFile::remove(filePath);
            }
    }
    return repToSave;
}
bool checkConfigFile()
{
    bool isCopied{false};
    if(!QFile::exists("config.ini"))
    {
        isCopied = QFile::copy(":/config.ini", "config.ini");
        //On redonne uniquement les droits de lecture et écriture au proprietaire du fichier
        QFile::setPermissions("config.ini", QFileDevice::Permissions(0x4000|0x2000));
    }
    else
    {
        isCopied = true;
    }
    QSettings configFile{"config.ini", QSettings::IniFormat};
    configFile.beginGroup("INI_FILE");
    bool isVersion {configFile.value("Version", "DEFAULT").toString() =="1.0"};
    configFile.endGroup();

    if (isVersion)
    {
        return isVersion and isCopied;
    }
    else
    {
        qDebug()<< "!Le fichier n'existe pas!";
        QMessageBox::critical(NULL, "config.ini", "ERREUR avec le fichier config.ini    ");
        exit(1);
    }
}
