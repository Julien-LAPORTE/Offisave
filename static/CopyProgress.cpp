#include "CopyProgress.h"

CopyProgress::CopyProgress(QString emplacement, bool isBackup, QObject *parent) : QObject(parent),
    m_inventaire{ new InventaireFichiers(listDirToSave(false), false) },
    m_inventaireBackup{ new InventaireRepertoire( emplacement ) }, m_nombreInventaire{0},
    m_emplacement{emplacement}, m_isBackup{isBackup}
{
    qDebug()<< "Emplacement pour l'instance de l'objet CopyProcess : " << emplacement;
    connect(m_inventaire, &InventaireFichiers::inventaireTermine, this, &CopyProgress::s_finInventaire);
    connect(m_inventaireBackup, &InventaireRepertoire::inventaireTermine, this, &CopyProgress::s_finInventaire);
    m_inventaire->inventaireFichiers();
    m_inventaireBackup->inventaireFichiers();
}
CopyProgress::~CopyProgress()
{
    delete m_inventaire;
    delete m_inventaireBackup;
}
QStringList CopyProgress::getInfosProgress()
{
    QStringList infos;
    if(m_isBackup)
    {
        infos << QString::number( m_inventaireBackup->getNombreRepertoires() )
              << QString::number( m_inventaireBackup->getNombreFichiers() )
              << m_inventaireBackup->getTaille()
             ;
    }
    else
    {
        int nombreRepertoires{0};
        for( auto &nombre: m_inventaire->getNombreRepertoires() )
        {
            nombreRepertoires+=nombre;
        }

        int nombreFichiers{0};
        for ( auto &nombre: m_inventaire->getNombreFichiers() )
        {
            nombreFichiers+=nombre;
        }

        infos << QString::number( nombreRepertoires )
              << QString::number( nombreFichiers )
              << m_inventaire->getTotal()
             ;
    }
    return infos;
}

void CopyProgress::s_finInventaire()
{
    if (m_nombreInventaire < 2) m_nombreInventaire++;
    if(m_nombreInventaire == 2)
    {
               qDebug()<< "m_nombreInventaire :" << m_nombreInventaire <<
                          "\nTaille inventaire :" << m_inventaire->getTotal() <<
                          "\nTaille backup :" << m_inventaireBackup->getTaille();
               int nombreFichiersInventaire{0}; for (auto &nombre : m_inventaire->getNombreFichiers()) nombreFichiersInventaire+=nombre;
               m_nombreFichiersInventaire=nombreFichiersInventaire;
               m_nombreFichiersBackup=m_inventaireBackup->getNombreFichiers()+4;

               qDebug()<< "Nombre fichiers inventaire :" << m_nombreFichiersInventaire <<
                          "\nNombre fichiers backup :" << m_nombreFichiersBackup;
               if(m_isBackup)
               {
                   emit nombreFichierInventaire(m_nombreFichiersInventaire);
               }
               else
               {
                   emit nombreFichierInventaire(m_nombreFichiersBackup);
               }
               QTimer::singleShot( 0, this, &CopyProgress::s_updateInventaire );
    }
    else if (m_nombreInventaire > 2)
    {
        if(m_isBackup)
        {
            m_nombreFichiersBackup = m_inventaireBackup->getNombreFichiers() + 4;
            emit nombreFichiersChanged(m_nombreFichiersBackup);
        }
        else
        {
            int nombreFichiersInventaire{0}; for (auto &nombre : m_inventaire->getNombreFichiers()) nombreFichiersInventaire+=nombre;
            m_nombreFichiersInventaire=nombreFichiersInventaire;
            emit nombreFichiersChanged(m_nombreFichiersInventaire);
        }
        QTimer::singleShot( 0, this, &CopyProgress::s_checkIfFinished );
    }
}

void CopyProgress::s_updateInventaire()
{
    if (m_isBackup)
    {
        delete m_inventaireBackup;
        m_inventaireBackup = new InventaireRepertoire( m_emplacement );
        connect(m_inventaireBackup, &InventaireRepertoire::inventaireTermine, this, &CopyProgress::s_finInventaire);

        m_nombreInventaire++;
        //qDebug()<< "Update inventaire backup numéro : " << m_nombreInventaire - 2 ;
        m_inventaireBackup->inventaireFichiers();
    }
    else
    {
        delete m_inventaire;
        m_inventaire = new InventaireFichiers(listDirToSave(false), false) ;
        connect(m_inventaire, &InventaireFichiers::inventaireTermine, this, &CopyProgress::s_finInventaire);        

        m_nombreInventaire++;
        //qDebug()<< "Update inventaire restauration numéro : " << m_nombreInventaire - 2 ;
        m_inventaire->inventaireFichiers();
    }
}

void CopyProgress::s_checkIfFinished()
{
        qDebug()<< "Nombre fichier Inventaire :" << m_nombreFichiersInventaire;
        qDebug()<< "Nombre fichiers backup :" << m_nombreFichiersBackup;
        if(m_isBackup)
        {
            if ( m_nombreFichiersBackup < m_nombreFichiersInventaire )
            {
                QTimer::singleShot( 0, this, &CopyProgress::s_updateInventaire );
            }
            else
            {
                emit finished();
            }
        }
        else
        {
            if ( m_nombreFichiersBackup > m_nombreFichiersInventaire )
            {
                QTimer::singleShot( 0, this, &CopyProgress::s_updateInventaire );
            }
            else
            {
                emit finished();
            }
        }
}
