#include "InventaireRepertoire.h"

InventaireRepertoire::InventaireRepertoire(QString chemin, QObject *parent ) : QObject(parent),
    m_dirsIterations {chemin , QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs | QDir::System | QDir::Hidden, QDirIterator::Subdirectories},
    m_tailleOctets{0}, m_tailleMOctets{0}, m_tailleGOctets{0}, m_nombreFichiers{0}, m_nombreRepertoires{0}
{
}
InventaireRepertoire::~InventaireRepertoire()
{
    qDebug()<<"Destruction de l'instance" << this->objectName() << "de la classe InventaireRepertoire OK";
}

QString InventaireRepertoire::getTaille()
{
    qDebug()<< "m_tailleGOctets = " << m_tailleGOctets ;
    qDebug()<< "m_tailleMOctets = " << m_tailleMOctets ;
    QString total;
    int tailleMO = m_tailleMOctets * 0.9765625;
    if(m_tailleGOctets == 0)
    {
        total = QString::number(tailleMO) + "Mo";
    }
    else
    {
        if (tailleMO >= 0 and tailleMO < 10)
        {
            total = QString::number( m_tailleGOctets ) + "," + "00" + QString::number(tailleMO) + "Go";
        }
        else if(tailleMO >= 10 and tailleMO < 100)
        {
            total = QString::number( m_tailleGOctets ) + "," + "0" + QString::number(tailleMO) + "Go";
        }
        else
        {
            total = QString::number(m_tailleGOctets) + "," + QString::number(tailleMO) + "Go";
        }
        qDebug()<< "total = " << total ;
    }
    return total;
}

void InventaireRepertoire::inventaireFichiers()
{
        QTimer::singleShot(0, this, &InventaireRepertoire::s_iteratorInventaireFichiers);
}
void InventaireRepertoire::s_iteratorInventaireFichiers()
{

        if (m_dirsIterations.hasNext())
        {
            QTimer::singleShot( 0, this, &InventaireRepertoire::s_stepInventaireFichiers );
        }
        else
        {
            qDebug()<< Qt::endl;
            qDebug()<< m_dirsIterations.path();
            qDebug()<< "Nombre de fichiers :" << m_nombreFichiers;
            qDebug()<< "Nombre de repertoires : " << m_nombreRepertoires;
            qDebug()<< "m_tailleGOctets iterator = " << m_tailleGOctets ;
            qDebug()<< "m_tailleMOctets iterator = " << m_tailleMOctets ;
            emit inventaireTermine();

        }
}
void InventaireRepertoire::s_stepInventaireFichiers()
{
    m_dirsIterations.next();
    if ( m_dirsIterations.fileInfo().isFile() || m_dirsIterations.fileInfo().isSymLink() )
    {
        //On vérifie que la taille du fichier en octets ne dépasse pas la valeur d'un type int
        verifMax(m_dirsIterations.fileInfo(), m_tailleOctets, m_tailleMOctets, m_tailleGOctets, INT_MAX);
        m_fileInfoList << m_dirsIterations.fileInfo() ;
        m_fileList << m_dirsIterations.filePath();
        m_nombreFichiers++;
    }
    else
    {
        m_dirList << m_dirsIterations.filePath();
        m_nombreRepertoires++;
    }
    QTimer::singleShot( 0, this, &InventaireRepertoire::s_iteratorInventaireFichiers );
}

QStringList InventaireRepertoire::getArbo() const&
{
    return m_dirList;
}
QStringList InventaireRepertoire::getCheminsFichiers() const&
{
    return m_fileList;
}
int InventaireRepertoire::getNombreFichiers() const&
{
    return m_nombreFichiers;
}
int InventaireRepertoire::getNombreRepertoires() const&
{
    return m_nombreRepertoires;
}
int InventaireRepertoire::getTailleMOctets() const&
{
    return m_tailleMOctets;
}
int InventaireRepertoire::getTailleGOctets() const&
{
    return m_tailleGOctets;
}
