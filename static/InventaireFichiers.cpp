#include "InventaireFichiers.h"

InventaireFichiers::InventaireFichiers(QStringList sources, bool showLoad, QObject *parent)
    :QObject{parent},  m_load{ new QLabel }, m_sourceIteration{0}, m_tailleTotaleOctects{0}, m_tailleTotaleMOctects{0},
      m_tailleTotaleGOctects{0}, m_sources{sources}, m_infos{"Erreur"}, m_total{"0Gigas"}
{
    if (showLoad)
    {
    m_load->setWindowFlag(Qt::FramelessWindowHint);
    m_load->setFixedSize(300, 100);
    m_load->setText("Calcul en cours ...");
    m_load->setFont(QFont{"ARIAL", 16, 16 });
    m_load->setAlignment(Qt::AlignCenter);
    m_load->show();
    }
    for (QString &source : sources)
    {
        m_dirsIterations << new QDirIterator (source , QDir::NoDotAndDotDot | QDir::Files | QDir::AllDirs | QDir::System | QDir::Hidden, QDirIterator::Subdirectories);
    }
}

InventaireFichiers::~InventaireFichiers()
{
   delete m_load;
   while (!m_fileInfoList.empty()) { delete m_fileInfoList.back(); m_fileInfoList.pop_back(); }
   while (!m_fileList.empty()) { delete m_fileList.back(); m_fileList.pop_back(); }
   while (!m_dirList.empty()) { delete m_dirList.back(); m_dirList.pop_back(); }
   while (!m_dirsIterations.empty()) { delete m_dirsIterations.back(); m_dirsIterations.pop_back(); }
   qDebug()<<"Destruction de l'instance" << this->objectName() << "de la classe InventaireFichier OK";
}

void InventaireFichiers::nextSource()
{
    if (m_dirsIterations.size()+1 > m_sourceIteration)
    {
        m_sourceIteration++;
        inventaireFichiers();
    }
}

void InventaireFichiers::showBox()
{
    QMessageBox msgBox;
    msgBox.setText(m_infos);
    msgBox.setInformativeText(m_total);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();

}

QString InventaireFichiers::getTotal()
{
    QString total{m_total};
    total.remove(0,18);
    qDebug()<< "totalTemp = " << total ;
    return total;
}


void InventaireFichiers::inventaireFichiers()
{
    m_fileInfoList << new QFileInfoList;
    m_fileList << new QStringList;
    m_dirList << new QStringList;
    m_tailleOctets << 0;
    m_tailleMOctets << 0;
    m_tailleGOctets << 0;
    m_nombreFichiers << 0;
    m_nombreRepertoires << 0;

    if (m_dirsIterations.size() > 0)
    {
        QTimer::singleShot(0, this, &InventaireFichiers::s_iteratorInventaireFichiers);
    }
}
void InventaireFichiers::s_iteratorInventaireFichiers()
{

        if (m_dirsIterations[m_sourceIteration]->hasNext())
        {
            QTimer::singleShot( 0, this, &InventaireFichiers::s_stepInventaireFichiers );
        }
        else//On affiche les info en qDebug une fois l'inventaire effectué sur la source
        {
            //conversion en Go, Mo, et octets
            conversion(m_tailleOctets[m_sourceIteration], m_tailleMOctets[m_sourceIteration], m_tailleGOctets[m_sourceIteration]);
            m_tailleTotaleOctects += m_tailleOctets[m_sourceIteration];
            m_tailleTotaleMOctects += m_tailleMOctets[m_sourceIteration];
            m_tailleTotaleGOctects += m_tailleGOctets[m_sourceIteration];
            conversion(m_tailleTotaleOctects, m_tailleTotaleMOctects, m_tailleTotaleGOctects);
            qDebug()<< Qt::endl;
            qDebug()<< m_dirsIterations[m_sourceIteration]->path();
            qDebug()<< "Nombre de fichiers :" << m_nombreFichiers[m_sourceIteration];
            qDebug()<< "Nombre de repertoires : " << m_nombreRepertoires[m_sourceIteration];
            //Si autre source, on relance l'inventaire sur le source suivante
            if (m_dirsIterations.size() > m_sourceIteration+1)
            {
                qDebug()<< "Source suivante...";
                nextSource();
            }
            else
            {
                m_sourceIteration = 0;
                m_load->hide();
                QString infos;
                for (int i { 0 }; i< m_dirsIterations.size(); i++)
                {
                        QDir dir { m_sources[i] };
                        if (m_tailleGOctets[i] > 0)
                        {
                                infos += dir.path() + " : " + (QString::number(m_tailleGOctets[i]) + "," + QString::number(m_tailleMOctets[i]) + "Go");
                                infos += "\n";
                        }
                        else if (m_tailleMOctets[i] > 0 )
                        {
                                infos += dir.path() + " : " + (QString::number(m_tailleMOctets[i]) + "Mo");
                                infos += "\n";
                        }
                }
                QString total;
                int tailleTotaleMO = m_tailleTotaleMOctects * 0.9765625;
                qDebug()<< "tailleTotalMOtemp :" << tailleTotaleMO;
                qDebug()<< "m_tailleTotalMOctects :" << tailleTotaleMO;
                if(m_tailleTotaleGOctects > 0)
                {
                    total = "\n\tTaille totale : " + QString::number(m_tailleTotaleGOctects) + "," + QString::number(tailleTotaleMO) + "Go";
                }
                else if ( m_tailleTotaleMOctects > 0 )
                {
                    total = "\n\tTaille totale : " + QString::number(tailleTotaleMO) + "Mo";
                }
                m_infos = infos;
                m_total = total;

                emit inventaireTermine();
            }
        }
}
void InventaireFichiers::s_stepInventaireFichiers()
{
    //qDebug()<< "\nAvant iteration" << m_dirsIterations[m_sourceIteration].filePath();
    m_dirsIterations[m_sourceIteration]->next();
    //qDebug()<< "Après iteration" << m_dirsIterations[m_sourceIteration].filePath();
    if ( m_dirsIterations[m_sourceIteration]->fileInfo().isFile() || m_dirsIterations[m_sourceIteration]->fileInfo().isSymLink() )
    {
        //On vérifie que la taille du fichier en octets ne dépasse pas la valeur d'un type int
        verifMax(m_dirsIterations[m_sourceIteration]->fileInfo(), m_tailleOctets[m_sourceIteration], m_tailleMOctets[m_sourceIteration], m_tailleGOctets[m_sourceIteration], INT_MAX);
        *m_fileInfoList[m_sourceIteration] << m_dirsIterations[m_sourceIteration]->fileInfo() ;
        *m_fileList[m_sourceIteration] << m_dirsIterations[m_sourceIteration]->filePath();
        m_nombreFichiers[m_sourceIteration]++;
    }
    else
    {
        *m_dirList[m_sourceIteration] << m_dirsIterations[m_sourceIteration]->filePath();
        m_nombreRepertoires[m_sourceIteration]++;
    }
    QTimer::singleShot( 0, this, &InventaireFichiers::s_iteratorInventaireFichiers );
}

QList<QStringList *> InventaireFichiers::getArbo() const&
{
    return m_dirList;
}
QList<QStringList *> InventaireFichiers::getCheminsFichiers() const&
{
    return m_fileList;
}
QList<int> InventaireFichiers::getNombreFichiers() const&
{
    return m_nombreFichiers;
}
QList<int> InventaireFichiers::getNombreRepertoires() const&
{
    return m_nombreRepertoires;
}
QList<int> InventaireFichiers::getTailleMOctets() const&
{
    return m_tailleMOctets;
}
QList<int> InventaireFichiers::getTailleGOctets() const&
{
    return m_tailleGOctets;
}
