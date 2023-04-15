#include "PlanificationWindow.h"

PlanificationWindow::PlanificationWindow(QString hostname, QWidget *parent) : QWidget(parent), m_hostname{hostname}, m_pathByName{}
{
    QStringList name { "Installer", "Désinstaller", "Retour" };
    QStringList toolTips { "null", "null", "null" };
    QStringList confirmationName { "null", "null", "null" };
    QStringList confirmationText { "null", "null", "null" };
    m_menu = new MainWindow { name, toolTips, confirmationName, confirmationText };
    m_menu->show();
    connect ( m_menu, &MainWindow::buttonClicked, this, &PlanificationWindow::s_buttonClicked );
}
PlanificationWindow::~PlanificationWindow()
{
    qDebug()<< "Destruction de PlanificationWindow OK !!!";
    delete m_menu;
}

void PlanificationWindow::s_buttonClicked(QString name)
{

    if (name == "Installer")
    {
        m_menu->hide();
        uninstall();
        ConfigWindow configWindow{m_hostname};
        QStringList config { configWindow.getConfig() };
        m_config=config;
        m_pathByName = m_config[0];
        if ( !m_pathByName[0].isLetter() )
        {
            qDebug()<< "m_pathByName par défaut :" << m_pathByName;
            m_pathByName.replace("\\", "/");
            qDebug()<< "m_pathByName après .replace:" << m_pathByName;

            m_pathByName.remove(0, 2);
            qDebug()<< "m_pathByName après remove :" << m_pathByName;

            m_pathByName.remove(0,  m_pathByName.indexOf(QChar('/')) );
            qDebug()<< "m_pathByName après indexOf :" << m_pathByName;

            m_pathByName.insert(0, "//nasAutosave");
            qDebug()<< "m_pathByName après insert:" << m_pathByName;

            m_pathByName.replace("/", "\\");
            qDebug()<< "m_pathByName après replace:" << m_pathByName;
        }
        qDebug()<< "m_config :" << m_config;
        qDebug()<< "m_pathByName :" << m_pathByName;

        if (!m_config[0][0].isLetter()) addLineToHosts(m_config[0]);

        config[0] = m_pathByName;
        m_config[0] = m_pathByName;
        NetUse netUse(config);
        if ( netUse.checkPath() )
        {
            install();
        }
        else
        {
            uninstall();
            m_menu->show();
        }
    }
    else if (name == "Désinstaller")
    {
        m_menu->hide();
        uninstall();
        QMessageBox::information(NULL, "Désinstallation", "La désinstallation complète du programme de sauvegarde auto s'est bien effectuée");
        emit backButton();
        delete this;
    }
    else if (name == "Retour")
    {
        emit backButton();
        delete this;
    }
    else
    {
        assert(false && "Le bouton n'existe pas");
    }
}

void PlanificationWindow::setConfig(QStringList config)
{
    assert(config.size() > 2 && "config.size() est inférieur à 3");//On a besoin du chemin, du login et du mot de passe
    NetUse dirBackup{ config, this };
    if (dirBackup.checkPath()) qDebug()<<"Le partage est accessible !";
}


void PlanificationWindow::uninstall()
{
    QDir rep;
    rep.setPath("C:/ProgramData/Offisave/");
    //Suppression de la tâche windows
    QProcess proc;
    proc.setProgram("SCHTASKS");
    QStringList argumentsCrontab;
    argumentsCrontab
            << "/delete"
            << "/TN"
            << "Sauvegarde Offisave"
            << "/F"
               ;
    proc.setArguments(argumentsCrontab);
    proc.start();
    proc.waitForFinished();

        QFile::setPermissions("C:/ProgramData/Offisave/offisave.exe", QFileDevice::Permissions(0x2|0x4|0x20|0x40|0x200|0x400|0x2000|0x4000));
        QFile::setPermissions("C:/ProgramData/Offisave/config.ini", QFileDevice::Permissions(0x2|0x4|0x20|0x40|0x200|0x400|0x2000|0x4000));
        QFile::setPermissions("C:/ProgramData/Offisave/config", QFileDevice::Permissions(0x2|0x4|0x20|0x40|0x200|0x400|0x2000|0x4000));

        QFile::remove("C:/ProgramData/Offisave/offisave.exe");
        QFile::remove("C:/ProgramData/Offisave/config.ini");
        QFile::remove("C:/ProgramData/Offisave/config");

    rep.rmdir(rep.path());
    removeLineToHosts();
    qDebug()<<"Désinstallation OK";
}
void PlanificationWindow::install()
{
    QDir rep;
    rep.setPath("C:/ProgramData/Offisave/");
    if (m_config[3].size() == 1) m_config[3]= "0"+m_config[3];
    if (m_config[4].size() == 1) m_config[4]="0"+m_config[4];
    QString time{m_config[3] + ":" + m_config[4]};
    qDebug()<< "time = " << time;
    //Tâche planifiée
    QProcess proc;
    proc.setProgram("SCHTASKS");
    QStringList argumentsCrontab;
    argumentsCrontab
            << "/Create"
            << "/SC"
            << "DAILY"
            << "/TN"
            << "Sauvegarde Offisave"
            << "/TR"
            << "c:\\ProgramData\\Offisave\\offisave.exe autosave"
            << "/ST"
            << time
            << "/F"
               ;
    proc.setArguments(argumentsCrontab);
    proc.start();

    //Copie des fichiers et du répertoire dans programData
    rep.mkpath(rep.path());
    QFile::copy("offisave.exe", "C:/ProgramData/Offisave/offisave.exe");
    QFile::copy("config.ini", "C:/ProgramData/Offisave/config.ini");
    //Tempo
    QTime dieTime= QTime::currentTime().addSecs(2);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    QFile configFile { "C:/programData/Offisave/config" };
    if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(NULL, "Erreur...", "Le fichier de config n'a pu être édité, annulation de l'installation ..");
        uninstall();
        return;
    }
    else
    {
        QString separateur{"@!;!@"};//Le séparateur pour séparer les données dans le fichier avant et après chiffrement, pas de pourcent pour pas géner le chiffrage
        CipherByteArray m_configCrypt;//Création d'un QByteArray chiffrable
        QTextStream in(&configFile);
        for(auto &line : m_config)
        {
            m_configCrypt += line.toLatin1() ;
            m_configCrypt += separateur.toLatin1();
        }
        m_configCrypt.encrypt();//Chiffrage
        in << m_configCrypt.toHex();//Ecriture des infos en crypté dans le fichier puis passage en hexdécimal
     }
    //On redonne uniquement les droits de lecture au proprietaire du fichier
    QFile::setPermissions("C:/ProgramData/Offisave/config.ini", QFileDevice::Permissions(0x4000));
    QFile::setPermissions("C:/ProgramData/Offisave/config", QFileDevice::Permissions(0x4000));

    qDebug()<<"Installation OK";

    QMessageBox::information(m_menu, "Installation sauvegarde", "La sauvegarde planifiée a été installée");
    emit backButton();
    delete this;

}
bool PlanificationWindow::addLineToHosts(QString emplacement)
{
        bool maj{true};
        emplacement.replace("\\", "/");
        QString ipEmplacement{ emplacement };
        ipEmplacement.remove(0, 2);
        ipEmplacement.truncate(ipEmplacement.indexOf(QChar ('/')));//Suppression des caractères après le premier slash trouvé
        qDebug()<< "ipEmplacement :" << ipEmplacement;
        QFile hosts{ "C:/Windows/System32/drivers/etc/hosts" };
        if( hosts.open(QIODevice::ReadOnly | QIODevice::Text) )
        {
            QTextStream in(&hosts);
            in.setCodec("UTF-8");
            while(!in.atEnd())
            {
                if ( in.readLine() == ipEmplacement+" nasAutosave #créé par Offisave, ne pas modifier" ) maj = false;
            }
            hosts.close();
        }
        else
        {
            maj = false;
        }
        if ( maj and hosts.open(QIODevice::Append | QIODevice::Text) )
        {
            qDebug()<< "On met à jour le fichier hosts du poste";
            QTextStream in(&hosts);
            in.setCodec("UTF-8");
            in << Qt::endl;
            in << ipEmplacement+" nasAutosave #créé par Offisave, ne pas modifier";
        }
        else
        {
            maj = false;
        }
        qDebug()<< "maj:"<< maj;
        return maj;
}
bool PlanificationWindow::removeLineToHosts()
{
        bool remove{false};
        QStringList fichierLock;
        QFile hosts{ "C:/Windows/System32/drivers/etc/hosts" };
        if( hosts.open(QIODevice::ReadOnly | QIODevice::Text) )
        {
            QTextStream out(&hosts);
            out.setCodec("UTF-8");
            while(!out.atEnd())
            {
                QString ligne;
                out.readLineInto(&ligne);
                fichierLock << ligne << "\n";
                //qDebug()<< "Valeur de ligne par défaut dans removeLineToHosts : " << ligne;
                ligne.remove(0, ligne.indexOf("#"));
                //qDebug()<< "Valeur de ligne après replace dans removeLineToHosts : " << ligne;
                if ( ligne == "#créé par Offisave, ne pas modifier" )
                {
                    fichierLock.pop_back();
                    fichierLock.pop_back();
                    remove = true;
                }
            }
            fichierLock.pop_back();
            hosts.close();
        }
        if( hosts.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream in(&hosts);
            in.setCodec("UTF-8");
            for (auto &ligne : fichierLock)
            {
                in << ligne;
            }
        }
        qDebug()<< "remove :"<< remove;
        return remove;
}
