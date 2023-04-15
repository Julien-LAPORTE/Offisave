#include "Autosave.h"

Autosave::Autosave(QString hostname, QObject *parent) : QObject(parent),
    m_hostname{hostname}, m_emplacement{}, m_path{}
{
    QStringList scriptArguments;
    QStringList emplacement{ getEmplacement() };
    m_emplacement.setPath( emplacement[0] );
    if (m_path[0].isLetter())
    {
        m_path = emplacement[0] + "sauvegardes";
    }
    else
    {
        m_path = emplacement[0] + "\\sauvegardes";
    }
    qDebug()<< "-----------------------------------------------";
    qDebug()<< "-----------------------------------------------";
    qDebug()<< "Constructeur Autosave--------------------------";
    qDebug()<< "-----------------------------------------------";
    qDebug()<< "-----------------------------------------------";
    qDebug()<< "m_emplacement par défaut:" << m_emplacement.path();
    qDebug()<< "m_path par défaut:" << m_path;
    qDebug()<< "emplacement[0]:" << emplacement[0];
    qDebug()<< "hostname:" << hostname;

    scriptArguments
            << m_path
            << hostname
            ;
    QStringList files{"autosave.bat", "autoBureau.ini", "noUser.ini", "autoTaches.ini", "user.ini", "userLocal.ini", "userRoaming.ini"};
    m_backup = new Executer{ "autosave.bat", scriptArguments, files };
    connect(this, &Autosave::autosaveStart, m_backup, &Executer::s_systemTrayIcon);//à l'envoi du signal true le message de démarrage s'afiche
                                                                                   //et à l'envoi de false le message de fin s'affiche
    m_path.replace("\\", "/");
    qDebug()<< "m_path après replace:" << m_path;
}
Autosave::~Autosave()
{
    qDebug()<< "***********************************************";
    qDebug()<< "Destructeur Autosave--------------------------";
    qDebug()<< "***********************************************";
    delete m_backup;
}

void Autosave::backup()
{
    emit autosaveStart();
    qDebug()<<"Début de autosave backup m_path vaut :" << m_path;
        if ( m_path[0].isLetter() )//Se le path commence par une lettre on copie direct
        {
            if( createQdirLocation() )
            {
                QRes::backup(m_path + "/résolutions/"+m_hostname+".res");//Sauvegarde de la résolution dans un fichier

                qDebug()<< "Démarrage sauvegarde sur : " << m_path;
                qDebug()<< "Copie de icon.exe et icon32.exe dans :" << m_path+"/icon/ReIcon.exe ";
                QFile::copy(":/icon.exe", m_path + "/icon/ReIcon_x64.exe");
                QFile::copy(":/icon32.exe", m_path + "/icon/ReIcon.exe");
                m_backup->s_executer();
            }
        }
        else //Sinon on utilise net use pour monter le partage réseau en S:\ et on lance la copie
        {
            NetUse repBackup(getEmplacement());
            repBackup.mount();
            if ( createQdirLocation() )
            {
                QRes::backup(m_path + "/résolutions/"+m_hostname+".res");//Sauvegarde de la résolution dans un fichier

                qDebug()<< "Démarrage sauvegarde sur : " << m_path;
                qDebug()<< "Copie de icon.exe et icon32.exe dans :" << m_path +"/icon/ReIcon.exe ";
                QFile::copy(":/icon.exe", m_path + "/icon/ReIcon_x64.exe");
                QFile::copy(":/icon32.exe", m_path + "/icon/ReIcon.exe");
                m_backup->s_executer();
            }
            repBackup.unmount();
        }
        emit autosaveStart(false);
}

QStringList Autosave::getEmplacement()
{
    QStringList emplacement;//Chemin de sauvegarde en 0, user en 1 et mot de passe en 2 dans le tableau
    CipherByteArray cipherEmplacement;
    QFile config { "C:/programData/Offisave/config" };
    if (config.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString separateur{"@!;!@"};//Le séparateur pour séparer les données dans le fichier avant et après chiffrement, pas de pourcent pour pas géner le chiffrage
        int t{separateur.toLatin1().size()};
        QTextStream in(&config);
        while (!in.atEnd())
        {
                in >> cipherEmplacement;
        }
        QByteArray emplacementTemp = QByteArray::fromHex(cipherEmplacement);

        cipherEmplacement.clear();
        cipherEmplacement += emplacementTemp;
        cipherEmplacement.uncrypt();
        QString temp{cipherEmplacement};

        emplacement << temp;
        qDebug()<< emplacement;
        emplacement[0].truncate(emplacement[0].indexOf(QString(separateur.toLatin1())));
        qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
        qDebug()<<"Chemin :" << emplacement [0];

        emplacement << temp;
        emplacement[1].remove(0, emplacement[1].indexOf(QString(separateur.toLatin1()))+t).truncate(emplacement[1].indexOf(QString(separateur.toLatin1())));
        qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
        qDebug()<<"Login :" << emplacement [1];

        emplacement << temp;
        emplacement[2].remove(0, emplacement[2].indexOf(QString(separateur.toLatin1()))+t).remove(0, emplacement[2].indexOf(QString(separateur.toLatin1()))+t).truncate(emplacement[2].indexOf(QString(separateur.toLatin1())));;
        qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
        qDebug()<<"Password :" << emplacement [2];

        return emplacement;
    }
    else
    {
        QMessageBox::warning(NULL, "Attention","Problème de configuration\n pas de sauvegarde des données de ce poste aujourd'hui)");
        return emplacement;
    }
}
bool Autosave::createQdirLocation()
{
    qDebug()<< "createQdirLocation m_emplacement path :" << m_emplacement.path();
    qDebug()<< "createQdirLocation m_path :" << m_path;
    if (!QFile::exists(m_path))
    {
        qDebug()<<"m_emplacement existe pas";
        m_emplacement.mkpath("sauvegardes");
        QProcess attrib;
        attrib.setProgram("attrib");
        QStringList arguments;
        arguments
                << "+S"
                << "+H"
                << m_emplacement.path().replace("/", "\\") + "\\sauvegardes";
                   ;
        attrib.setArguments(arguments);
        attrib.start();
        attrib.waitForFinished();
    }
    m_emplacement.cd("sauvegardes");
    m_emplacement.mkpath("icon");
    m_emplacement.mkpath("résolutions");
    qDebug()<< "m_emplacement après cd :" << m_emplacement.path();
    return QFile::exists(m_emplacement.path());
}
