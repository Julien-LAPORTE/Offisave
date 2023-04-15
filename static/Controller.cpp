#include "Controller.h"

Controller::Controller(QString argument, QString emplacement, QWidget *parent) : QWidget(parent), m_maintenance{ nullptr },
    m_mainWindow{nullptr}, m_advancedWindow{nullptr}, m_locationButton{nullptr},
    m_location { new QDir {emplacement} }, m_inventaire{nullptr} ,
    m_emplacement{ emplacement }, m_hostname{ "null" }, m_smbConnected{false}, m_nas{true}, m_serveurLGPI{true}, m_selectedLocation{false}
  ,m_argument{argument}
{
    if (!checkConfigFile()) exit(1);
    if (m_emplacement[m_emplacement.size() -1 ] == "/")
    {
        m_emplacement.chop(1);
    }
    else if( m_emplacement[m_emplacement.size() -1 ] == "\\" )
    {
        m_emplacement.chop(2);
    }
    m_labelStart = new QLabel {"Recherche NAS"};
    m_labelStart->setFixedSize(180, 60);
    m_labelStart->setWindowFlag(Qt::FramelessWindowHint);
    m_labelStart->setAlignment(Qt::AlignCenter);
    m_labelStart->setFont(QFont{"ARIAL", 8, 8 });
    m_labelStart->setWordWrap(true);
    m_labelStart->show();
    initHostName();

    m_testSmb = new QTcpSocket{this};
    connect( m_testSmb, &QTcpSocket::connected, this, &Controller::s_smbConnected );
    m_testSmb->connectToHost("192.168.0.210", 445);  
    QTimer::singleShot(0, this, &Controller::s_timeout);
}
Controller::~Controller()
{
    delete m_labelStart;
    delete m_testSmb;
    if (m_maintenance!=nullptr) delete m_maintenance;
    delete m_location;
    if (m_mainWindow!=nullptr) delete m_mainWindow;
    if (m_advancedWindow!=nullptr) delete m_advancedWindow;
    if (m_locationButton!=nullptr) delete m_locationButton;
    if (m_configWindow!=nullptr) delete m_configWindow;
}
void Controller::initHostName()
{
    QString localhostname =  QHostInfo::localHostName();
    QString localhostIP;
    QString localhostIPv6;
    QList<QHostAddress> hostList = QHostInfo::fromName(localhostname).addresses();
    foreach (const QHostAddress& address, hostList) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address.isLoopback() == false) {
             localhostIP = address.toString();
        }
        if (address.protocol() == QAbstractSocket::IPv6Protocol && address.isLoopback() == false) {
             localhostIPv6 = address.toString();
        }
    }
    QString localMacAddress;
    QString localNetmask;
    foreach (const QNetworkInterface& networkInterface, QNetworkInterface::allInterfaces()) {
        foreach (const QNetworkAddressEntry& entry, networkInterface.addressEntries()) {
            if (entry.ip().toString() == localhostIP) {
                localMacAddress = networkInterface.hardwareAddress();
                localNetmask = entry.netmask().toString();
                break;
            }
        }
    }
    qDebug() << "Localhost name: " << localhostname;
    qDebug() << "IP = " << localhostIP;
    qDebug() << "MAC = " << localMacAddress;
    qDebug() << "Netmask = " << localNetmask;
    QString hostname { "Poste" + localhostIP.remove(0, 10) };
    qDebug()<< "hostname à partir de l'adresse IP = " << hostname;
    m_hostname=hostname;
}

void Controller::s_buttonClicked(QString name)
{
    QStringList scriptArguments;
    QString emplacement{ m_emplacement };
    scriptArguments
            << emplacement.replace("/", "\\")
            << m_hostname;
            ;
    if (name == "Sauvegarder")
    {
            if(m_mainWindow->isVisible())
            {
                m_mainWindow->hide();
            }
            else
            {
                m_advancedWindow->hide();
                m_locationButton->hide();
            }

        if ( createQdirLocation() )
        {
            m_location->mkpath("résolutions");
            QRes::backup(m_emplacement + "/résolutions/"+m_hostname+".res");//Sauvegarde de la résolution dans un fichier
            qDebug()<< "Démarrage sauvegarde sur : " << emplacement.replace("/", "\\");
            QStringList files{"sauvegarde.bat", "bureau.ini", "noUser.ini", "taches.ini", "user.ini", "userLocal.ini", "userRoaming.ini"};
            Executer backup{ "sauvegarde.bat", scriptArguments, files };
            m_location->mkpath("icon");
            qDebug()<< "Copie de icon.exe et icon32.exe dans :" << m_emplacement + "/icon/ReIcon.exe";
            QFile::copy(":/icon.exe", m_emplacement + "/icon/ReIcon_x64.exe");
            QFile::copy(":/icon32.exe", m_emplacement + "/icon/ReIcon.exe");
            backup.s_executer();
            m_advancedWindow->s_showConfirmation(0);
        }
        else
        {
            // Utilisation d'une QMessageBox pour prévenir l'utilisateur que le repertoire n'existe pas
            QMessageBox msgBox;
            msgBox.setText("Sauvegarde Impossible\nPas de nas ou de serveur LGPI");
            msgBox.setInformativeText("Merci de choisir un emplacement via le menu avancé");
            msgBox.exec();
            m_mainWindow->show();
        }
    }
    else if ( name == "Restaurer")
    {
        if(m_mainWindow->isVisible())
        {
            m_mainWindow->hide();
        }
        else
        {
            m_advancedWindow->hide();
            m_locationButton->hide();
        }

        if ( createQdirLocation() )
        {
        qDebug()<< "Démarrage restauration sur : " << emplacement.replace("/", "\\") ;
        QStringList files{"restauration.bat", "bureau.ini", "noUser.ini", "taches.ini", "user.ini", "userLocal.ini", "userRoaming.ini"};
        Executer restauration{ "restauration.bat", scriptArguments, files };
        restauration.s_executer();
        qDebug() << "Controler, fin restauration";
        restauration.hideM_load();
        m_advancedWindow->s_showConfirmation(1);
        QRes::restore(m_emplacement + "/résolutions/"+m_hostname+".res");//Restauration de la résolution à partir du fichier
        }
        else
        {
            // Utilisation d'une QMessageBox pour prévenir l'utilisateur que le repertoire n'existe pas
            QMessageBox msgBox;
            msgBox.setText("Restauration impossible\nPas de nas ou de serveur LGPI");
            msgBox.setInformativeText("Merci de choisir un emplacement via le menu avancé");
            msgBox.exec();
            m_mainWindow->show();
        }
    }
    else if ( name == "Avancé")
    {
        m_mainWindow->hide();
        m_locationButton->show();
        m_advancedWindow->show();
    }
    else if ( name == "Retour")
    {
        m_advancedWindow->hide();
        m_locationButton->hide();
        m_mainWindow->show();

    }
    else if ( name == "Inventaire données" )
    {
        m_advancedWindow->hide();
        m_locationButton->hide();
        QStringList sources{ listDirToSave() };
        m_inventaire = new InventaireFichiers {sources};
        connect(m_inventaire, &InventaireFichiers::inventaireTermine, this, &Controller::s_finInventaire);
        m_inventaire->inventaireFichiers();
    }
    else if (name == "Configuration")
    {
        m_configWindow = new ConfigMainWindow{this};
        if(m_mainWindow->isVisible())
        {
            m_mainWindow->hide();
        }
        else
        {
            m_advancedWindow->hide();
            m_locationButton->hide();
        }
        connect (m_configWindow, &ConfigMainWindow::finConfig, this, &Controller::s_showMenu );
        m_configWindow->show();
    }
    else if ( name == "Maintenance" )
    {
        if(m_mainWindow->isVisible())
        {
            m_mainWindow->hide();
        }
        else
        {
            m_advancedWindow->hide();
            m_locationButton->hide();
        }
        if (QApplication::keyboardModifiers().testFlag(Qt::AltModifier) and !QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
        {
            m_maintenance = new Maintenance { QInputDialog::getText(m_advancedWindow ,"Message perso", "Saisir un texte" ) };
        }
        else
        {
            m_maintenance = new Maintenance { "POSTE EN MAINTENANCE" };
        }
        m_maintenance->showMaintenance();
    }
    else if ( name == "Sauvegarde AUTO")
    {
        m_advancedWindow->hide();
        m_locationButton->hide();
        PlanificationWindow* menu = new PlanificationWindow {m_hostname, this} ;
        connect (menu, &PlanificationWindow::backButton, this, &Controller::s_showMenu );
    }
    else if ( name == "Quitter")
    {
        qApp->exit();
    }
    else
    {
        assert(false && "Bouton n'existe pas !!!");
    }
}
bool Controller::backupOrRestauration()
{
    if (m_hostname == "null")
    {
    initHostName();
    }
    else
    {
        QString hostname;
        hostname=m_mainWindow->getQlineText();
        m_hostname=hostname;
    }
    qDebug()<< "Fin de isBackup";
    return QFile::exists(m_emplacement +"/"+ m_hostname);
}
void Controller::s_setLocation()
{
    QString emplacement { m_emplacement };
    qDebug()<< "emplacement :" << emplacement;

    m_emplacement = ( QFileDialog::getExistingDirectory(m_advancedWindow, "Emplacement sauvegardes", "C:\\" , QFileDialog::ShowDirsOnly) );
    if (m_emplacement == "")
    {
        m_emplacement = emplacement;
        return;
    }

    QString dirName { m_emplacement };
    dirName.remove(0, dirName.lastIndexOf( QChar('/') ) ).remove(0,1);
    qDebug()<< "dirName :" << dirName;
    if (dirName == "sauvegardes") m_emplacement.chop(12);

    if ( m_emplacement == "C:/" or m_emplacement =="D:/" or m_emplacement == "E:/" or m_emplacement == "F:/" or m_emplacement == "G:/" )
    {
         m_emplacement = (m_emplacement+"sauvegardes");
    }
    else
    {
         m_emplacement = (m_emplacement+"/sauvegardes");
    }
    if (backupOrRestauration())
    {
        m_mainWindow->changeButtons(0, "Restaurer", "Permet la restauration des fichiers du poste", "Restauration des fichiers", "La restauration de tous les fichiers à utiliser par cette session : " + QDir::homePath().remove(0, 9) + " est terminée");
    }
    else
    {
        m_mainWindow->changeButtons(0, "Sauvegarder", "Permet la sauvegarde des fichiers du poste", "Sauvegarde des fichiers", "La sauvegarde de tous les fichiers à utiliser par cette session : " + QDir::homePath().remove(0, 9) + " est terminée");
    }
    qDebug()<< "Nouvel emplacement :"<< m_emplacement;
    m_location->setPath(m_emplacement);
    m_selectedLocation=true;

}

void Controller::s_finInventaire()
{
    m_inventaire->showBox();
    delete m_inventaire;
    m_advancedWindow->show();
    m_locationButton->show();
}
void Controller::s_saveNameChanged()
{
    if (m_mainWindow->getQlineText().isEmpty()) m_hostname="null";
    if (backupOrRestauration())
    {
        m_mainWindow->changeButtons(0, "Restaurer", "Permet la restauration des fichiers du poste", "Restauration des fichiers", "La restauration de tous les fichiers à utiliser par cette session : " + QDir::homePath().remove(0, 9) + " est terminée");
    }
    else
    {
        m_mainWindow->changeButtons(0, "Sauvegarder", "Permet la sauvegarde des fichiers du poste", "Sauvegarde des fichiers", "La sauvegarde de tous les fichiers à utiliser par cette session : " + QDir::homePath().remove(0, 9) + " est terminée");
    }
        qDebug()<< "Changement du nom du repertoire de sauvegarde de cette session par : " << m_hostname;
}
void Controller::s_showMenu()
{
    m_mainWindow->show();
}
bool Controller::createQdirLocation()
{
    if ( m_nas == false and m_serveurLGPI == false and m_selectedLocation == false )
    {
        qDebug()<< "CreateQdirLocation return false";
        return false;
    }

    qDebug()<< "CreateQdirLocation m_location path :" << m_emplacement;
    if (!QFile::exists(m_emplacement))
    {
        qDebug()<<"m_emplacement existe pas !!!";
        m_location->mkdir(m_location->path().replace("/", "\\"));
        QProcess attrib;
        attrib.setProgram("attrib");
        QStringList arguments;
        arguments
                << "+S"
                << "+H"
                << m_location->path().replace("/", "\\")
                   ;
        attrib.setArguments(arguments);
        attrib.start();
        attrib.waitForFinished();
    }
    qDebug()<< "CreateQdirLocation return true";
    return true;
}

void Controller::s_initMainWindow()
{
    m_locationButton = new LocationWindow;
    QStringList textsMainWindow, tooltipsMainWindow, confirmationNameMainWindow, confirmationTextMainWindow;
    textsMainWindow
            << "Sauvegarder"
            << "Avancé"
            << "Quitter"
               ;
    tooltipsMainWindow
            << "Maintenir CTRL avant de cliquer pour avoir une fenêtre de maintenance pendant la sauvegarde des fichiers\n(utiliser à la place ALT pour personnaliser le message)"
            << "Permet d'afficher le menu avancé"
            << "Fermer immediatement l'application"
               ;
    confirmationNameMainWindow
            << "Sauvegarde des fichiers"
            << "null"
            << "null"
               ;
    confirmationTextMainWindow
            << "La sauvegarde de tous les fichiers utilisés par cette session : " + QDir::homePath().remove(0, 9) + " est terminée"
            << "null"
            << "null"
               ;
    m_mainWindow = new MainWindow { textsMainWindow, tooltipsMainWindow, confirmationNameMainWindow, confirmationTextMainWindow, m_hostname };
    m_mainWindow->show();
    //initialisation de l'interface
    m_locationButton = new LocationWindow;
    QStringList textsAdvancedWindow, tooltipsAdvancedWindow, confirmationNameAdvancedWindow, confirmationTextAdvancedWindow;
    textsAdvancedWindow
            << "Sauvegarder"
            << "Restaurer"
            << "Inventaire données"
            << "Configuration"
            << "Maintenance"
            << "Sauvegarde AUTO"
            << "Retour"
               ;
    tooltipsAdvancedWindow
            << "Maintenir CTRL avant de cliquer pour avoir une fenêtre de maintenance pendant la sauvegarde des fichiers\n(utiliser à la place ALT pour personnaliser le message)"
            << "Maintenir CTRL avant de cliquer pour avoir une fenêtre de maintenance pendant la restauration des fichiers\n(utiliser à la place ALT pour personnaliser le message)"
            << "Permet d'afficher la taille des repertoires qui seront sauvegardés\nLes répertoires de moins de 1mo sont ignorés"
            << "Permet d'afficher la fenêtre de configuration"
            << "Permet d'afficher une fenêtre de maintenance\n(maintenir à la place ALT pour personnaliser le message)"
            << "Pour planifier des sauvegardes automatique du poste"
            << "retour"
               ;
    confirmationNameAdvancedWindow
            << "Sauvegarde des fichiers"
            << "Restauration des fichiers"
            << "null"
            << "null"
            << "null"
            << "null"
            << "null"
               ;
    confirmationTextAdvancedWindow
            << "La sauvegarde de tous les fichiers utilisés par cette session : " + QDir::homePath().remove(0, 9) + " est terminée"
            << "La restauration de tous les fichiers à utiliser par cette session : " + QDir::homePath().remove(0, 9) + " est terminée"
            << "null"
            << "null"
            << "null"
            << "null"
            << "null"
               ;
    m_advancedWindow = new MainWindow {textsAdvancedWindow, tooltipsAdvancedWindow, confirmationNameAdvancedWindow, confirmationTextAdvancedWindow };
    connect ( m_mainWindow, &MainWindow::buttonClicked, this, &Controller::s_buttonClicked );
    connect ( m_advancedWindow, &MainWindow::buttonClicked, this, &Controller::s_buttonClicked );
    connect ( m_locationButton, &LocationWindow::clicked, this, &Controller::s_setLocation );
    connect ( m_mainWindow, &MainWindow::qlineTextChanged, this, &Controller::s_saveNameChanged);
}
void Controller::s_timeout()
{
    QTimer::singleShot(5000, this, &Controller::s_stepTimeout);
}
void Controller::s_stepTimeout()
{
    qDebug()<<"s_stepTimeout m_emplacement = " << m_emplacement;
    if ( m_mainWindow == nullptr and m_emplacement == "//192.168.0.210/Public/sauvegardes" and m_testSmb->state() != QAbstractSocket::ConnectedState and m_smbConnected==false )
    {
        m_testSmb->abort();
        m_nas = false;
        m_emplacement="//192.168.0.100/tmp/sauvegardes";
        m_labelStart->setText("Pas de NAS\nRecherche serveur");
        m_testSmb->connectToHost("192.168.0.100", 445);
        QTimer::singleShot(0, this, &Controller::s_timeout);
    }
    else if ( m_mainWindow == nullptr and m_emplacement == "//192.168.0.100/tmp/sauvegardes" and m_testSmb->state() != QAbstractSocket::ConnectedState and m_smbConnected==false )
    {
        m_serveurLGPI = false;
        m_emplacement="C:/akjbkjdsbfuierbvj dfjvgbijb-jdbfjhbe12DG";
        s_initMainWindow();
        //s_setLocation();
        QTimer::singleShot( 0, this, &Controller::s_smbConnected );
    }
    else
    {
        if ( m_mainWindow == nullptr )QTimer::singleShot( 0, this, &Controller::s_smbConnected );
    }
}
void Controller::s_smbConnected()
{
    if ( m_testSmb->state() == QAbstractSocket::ConnectingState )
    {
        m_testSmb->abort();
    }
    else if ( m_testSmb->state() == QAbstractSocket::ConnectedState )
    {
        m_testSmb->disconnect();
    }
    m_labelStart->hide();

    if (m_smbConnected == false)
    {
        if ( m_mainWindow == nullptr ) s_initMainWindow();
        m_mainWindow->s_qlineTextChanged();
        backupOrRestauration();
        if (m_emplacement == "C:/akjbkjdsbfuierbvj dfjvgbijb-jdbfjhbe12DG")
        {
            m_emplacement="//192.168.0.210/Public/sauvegardes";
        }
        m_location->setPath(m_emplacement);
        m_smbConnected = true;
    }
    if (m_argument == "autosave" )
    {
        if(m_mainWindow->isVisible())
        {
            m_mainWindow->hide();
        }
        else
        {
            m_advancedWindow->hide();
            m_locationButton->hide();
        }
            Autosave autosave { m_hostname, this };
            autosave.backup();
            exit(EXIT_SUCCESS);
    }
    else if(m_argument != NULL)
    {
        qApp->exit();
    }
}
