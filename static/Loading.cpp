#include "Loading.h"

Loading::Loading(QString text, QString const &emplacement) : m_maintenance {nullptr}, m_label {new QLabel( text, this) },
    m_progress{nullptr}, m_emplacement{emplacement}, m_screen{ QGuiApplication::primaryScreen() },
    m_tray{ nullptr }
{    
    if (QApplication::keyboardModifiers().testFlag(Qt::AltModifier) and !QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
    {
        m_maintenance = new Maintenance { QInputDialog::getText(this, "Message perso", "Saisir un texte" ) };
    }
    else if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) and !QApplication::keyboardModifiers().testFlag(Qt::AltModifier))
    {
        m_maintenance = new Maintenance { "POSTE EN MAINTENANCE" };
    }
    else
    {

    }
    if (m_maintenance!=nullptr)
    {
        setParent(m_maintenance);
        move(Qt::AlignLeft, Qt::AlignTop);
    }

    m_largeurEcran = m_screen->size().width();
    m_hauteurEcran = m_screen->size().height();
    m_hauteurWindow = m_hauteurEcran / 36.48;
    qDebug()<< "m_hauteurEcran :" << m_hauteurEcran;
    qDebug()<< "m_largeurEcran :" << m_largeurEcran;
    qDebug()<< "m_hauteurWindow :" << m_hauteurWindow;
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    move(Qt::AlignLeft, 0 );
    m_label->setFixedSize(m_largeurEcran, m_hauteurWindow);
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setFont(QFont{"ARIAL", 10, 10 });
    m_label->setText(text);
    m_label->setWordWrap(true);

    setFixedSize(m_largeurEcran, m_hauteurWindow);
    show();
}
Loading::~Loading()
{
    if ( m_maintenance != nullptr ) delete m_maintenance;
    delete m_label;
    if (m_progress != nullptr) delete m_progress;
    for (auto *&info: m_infos)
    {
        qDebug()<< "Destructions attribut info :" << info;
        delete info;
    }
    delete m_screen;
    if(m_tray != nullptr) delete m_tray;
}

void Loading::setText(QString text)
{
    m_label->setText(text);
    qDebug()<< "m_label->text : " << m_label->text();
    if( m_label->text() == "Sauvegarde registre Outlook\r" )
    {
        m_textInfos << "Répertoires sauvegardés\n"
                    << "Fichiers sauvegardés\n"
                    << "Taille sauvegardée\n"
                       ;
      initInfos();
      m_progress = new CopyProgress { m_emplacement };
      connect(m_progress, &CopyProgress::nombreFichierInventaire, this, &Loading::s_SetNombreFichiersInventaire);
      connect(m_progress, &CopyProgress::nombreFichiersChanged, this, &Loading::s_SetNombreFichiersBackup);
      connect(m_progress, &CopyProgress::finished, this, &Loading::s_finished);
    }
    else if( m_label->text() == "téléchargement firefox\r" or m_label->text() == "fichier corrompu : nouveau téléchargement de firefox\r" )
    {
        QString date { QDate::currentDate().toString("yyyy_MM_dd") };
        QString emplacement{m_emplacement};
        emplacement.truncate(emplacement.lastIndexOf(QChar ('\\'))); //Suppression des caractères après le backslash

        int largeurEcran = QApplication::desktop()->width();
        int hauteurEcran = QApplication::desktop()->height();
        double hauteur{ hauteurEcran / 36.48};
        QLabel *download{ new QLabel("0 Mo", this) };
        download->setFixedSize(largeurEcran / 8, hauteur);
        download->setAlignment(Qt::AlignCenter);
        download->move(0, 0);
        download->setFont(QFont{"ARIAL", 8, 8 });
        download->setText(text);
        download->setWordWrap(true);
        download->setStyleSheet("QLabel { color : red; }");
        download->show();
        while(m_label->text() == "téléchargement firefox\r" or m_label->text() == "fichier corrompu : nouveau téléchargement de firefox\r" )
        {
            QCoreApplication::processEvents();
            int sizeO{0};
            int sizeMO{0};
            int sizeGO{0};
            QString size;
            QFileInfo tFirefox{emplacement + "\\" + "firefox_"+ date + ".exe" };
            sizeO = tFirefox.size();
            conversion(sizeO, sizeMO, sizeGO);
            sizeMO = sizeMO*0.9765625;
            if(sizeGO > 0)
            {
                size = QString::number(sizeGO) + "," + QString::number(sizeMO) + " Go";
            }
            else if (sizeMO > 0)
            {
                size = QString::number(sizeMO) + "," + QString::number(sizeO) + " Mo";
            }
            else
            {
                size = QString::number(sizeO) + " Octets";
            }
            download->setText(size);
        }
        delete download;
    }
    else if( m_label->text() == "Restauration registre Outlook\r" )
    {
        m_textInfos << "Nombre répertoires restaurés\n"
                    << "Nombre fichiers restaurés\n"
                    << "Taille restaurée\n"
                       ;
        initInfos();
        m_progress = new CopyProgress { m_emplacement, false };
        connect(m_progress, &CopyProgress::nombreFichierInventaire, this, &Loading::s_SetNombreFichiersInventaire);
        connect(m_progress, &CopyProgress::nombreFichiersChanged, this, &Loading::s_SetNombreFichiersBackup);
        connect(m_progress, &CopyProgress::finished, this, &Loading::s_finished);
    }
}
QString Loading::getText()
{
    return m_label->text();
}

void Loading::s_SetNombreFichiersInventaire(int nombreFichiers)
{
    setRange(0, nombreFichiers);
}
void Loading::s_SetNombreFichiersBackup(int nombreFichiers)
{
    setValue(nombreFichiers);
    QStringList infosProgress{ m_progress->getInfosProgress() };
    for (int i{0}; i < m_textInfos.size(); i++)
    {
        m_infos[i]->setText( m_textInfos[i] + infosProgress[i] );
    }
}
void Loading::s_finished()
{
    setValue(maximum());
}

void Loading::initInfos()
{
    qDebug()<< "m_textInfos : " << m_textInfos ;
    m_infos << new QLabel { m_textInfos[0], this }
            << new QLabel { m_textInfos[1], this }
            << new QLabel { m_textInfos[2], this }
               ;
    int i{0};
    for(auto &info : m_infos)
    {
        info->setFixedSize(m_largeurEcran / 8, m_hauteurWindow);
        info->setAlignment(Qt::AlignCenter);
        info->setFont(QFont{"ARIAL", 7, 7 });
        info->setWordWrap(true);
        info->setStyleSheet("QLabel { color : red; }");
        info->move(i, +0);
        i+=m_largeurEcran/8;
        info->show();
    }
}
void Loading::autosaveStart()
{
    hide();
    qDebug()<< "autosaveStart()";
    m_tray = new QSystemTrayIcon{this};
    QIcon icon(":/icone.ico");
    m_tray->setIcon(icon);
    m_tray->setToolTip("Pharmagest : sauvegarde Offisave");
    m_tray->show();
    m_tray->showMessage("Sauvegarde", "La sauvegarde des données du poste vient de se lancer");

    // Création du menu contextuel de notre icône
    QMenu* stmenu = new QMenu(this);
    QAction* actTexte1 = new QAction("Afficher", this);
    QAction* actTexte2 = new QAction("Masquer", this);
    stmenu->addAction(actTexte1);
    stmenu->addAction(actTexte2);
    m_tray->setContextMenu(stmenu); // On assigne le menu contextuel à l'icône de notification
    connect(actTexte1, &QAction::triggered, this, &Loading::show );
    connect(actTexte2, &QAction::triggered , this, &Loading::hide );
}
void Loading::autosaveEnd()
{
    qDebug()<< "autosaveEnd()";
    m_tray->showMessage("Sauvegarde", "La sauvegarde est terminée");
}
