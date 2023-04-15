#include "QRes.h"


QRes::QRes()
{
}
QRes::~QRes()
{
}

void QRes::backup(QString backupFilePath)
{
    qDebug()<<"Sauvegarde de la résolution dans :" << backupFilePath;
    int x {QApplication::desktop()->width()};
    int y {QApplication::desktop()->height()};
    if (x == 1024 and y == 768) return;
    QFile res{backupFilePath};
    if ( res.open(QIODevice::WriteOnly))
    {
        QTextStream flux{&res};
        res.resize(0);
        flux << x << Qt::endl << y;
    }
}
void QRes::restore(QString backupFilePath)
{
    int x{1024};
    int y{768};
    QFile res{backupFilePath};
    if (res.open(QIODevice::ReadOnly))
    {
        x = res.readLine().toInt();
        y = res.readLine().toInt();
    }

    qDebug().nospace()<< "La résolution à restaurer est de : " << x << "x" << y;
    qDebug()<<"Changement de la résolution";
    DEVMODE devMode;  // une structure contenant les infos sur la résolution
    devMode.dmSize = sizeof(DEVMODE); // initialisation d'un élément de la structure à faire
    LPDEVMODE lpDevMode = &devMode; // pointeur sur la structure
    EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,lpDevMode); // on récupère les infos de la résolution actuelle (pour remplir correctement la structure [qui est un peu compliqué])
    // on apporte nos modifications
    devMode.dmPelsWidth = x; // largeur
    devMode.dmPelsHeight = y; // hauteur
    devMode.dmBitsPerPel = 32; // bit par pixel
    ChangeDisplaySettingsEx(NULL,lpDevMode,NULL,CDS_UPDATEREGISTRY,NULL); // on applique cette résolution d'écran avec mise à jour du registre

    if (x == 1024 and y == 768)//Pour le plein écran avec lancement du script WPI de claude
    {
    QProcess pleinEcran;
    pleinEcran.setProgram("cmd");
    QString pathVbs;

    bool wpiExist{false};
    bool pcInstallExist{false};
    if (QFile::exists("C:/pharmagest/PCInstall"))
    {
        pcInstallExist = true ;
    }
    else if (QFile::exists("C:/WPI"))
    {
        wpiExist = true ;
    }

        if (pcInstallExist)
        {
            pathVbs = "C:\\pharmagest\\PCInstall\\apps\\Personnalisation\\";
        }
        else if (wpiExist)
        {
            pathVbs = "C:\\WPI\\Install\\Finalisation\\";
        }
        else
        {
            QMessageBox::warning(nullptr, QCoreApplication::applicationName(),"WPI/PCInstall non présent sur le poste");
        }

        if (wpiExist or pcInstallExist)
        {
        qDebug()<< "File's path vbs =" << pathVbs+"resolution\\PleinEcran.vbs";
        QStringList arguments{ "/C", pathVbs+"resolution\\PleinEcran.vbs" };
        pleinEcran.setArguments(arguments);
        pleinEcran.start();
        pleinEcran.waitForFinished();
        }

        ChangeDisplaySettingsEx(NULL,lpDevMode,NULL,CDS_UPDATEREGISTRY,NULL); // on applique cette résolution d'écran avec mise à jour du registre

        if (wpiExist or pcInstallExist)
        {
        pleinEcran.start();
        pleinEcran.waitForFinished();
        }
    }
}
