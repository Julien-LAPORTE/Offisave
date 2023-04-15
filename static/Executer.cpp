#include "Executer.h"

Executer::Executer(QString const &program, QStringList const &arguments, QStringList const &filesNameToCopy ,QObject *parent) : QObject(parent),
    m_tmp { new QDir(QDir::homePath() + "/AppData/Local/Temp") }, m_program{ new QProcess }, m_filesNameToCopy{ filesNameToCopy },
    m_load { new Loading("Chargement en cours...", arguments[0]+ "\\" + arguments[1]) },
    m_loadSizeX{ 0 }, m_loadSizeY{ 0 }, m_loadPosX{ 0 }, m_loadPosY{ 0 }
{
    connect (m_program, &QProcess::readyReadStandardOutput, this, &Executer::s_readStandardOutput);
    m_program->setProgram(m_tmp->path() + "/" + program);
    qDebug() << " Chemin du programme :" << m_program->program();
    m_program->setArguments(arguments);
    qDebug()<< "Liste des arguments :" << m_program->arguments();
    m_loadSizeX = m_load->size().width();
    m_loadSizeY = m_load->size().height();
    m_loadPosX = m_load->pos().x();
    m_loadPosY = m_load->pos().y();
}
Executer::~Executer()
{
    qDebug()<<"Destruction de l'instance Executer" << m_program->program();

    //Suppression des fichiers qui était neccessaire à l'execution du script
    for (auto &fileName : m_filesNameToCopy)
    {
    QFile::setPermissions(m_tmp->path() + "/" + fileName, QFileDevice::Permissions(0x2|0x4|0x20|0x40|0x200|0x400|0x2000|0x4000));
    QFile::remove(m_tmp->path() + "/" + fileName);
    }
    delete m_program;
    delete m_tmp;
    delete m_load;
}

void Executer::s_executer()
{
    //Suppression des fichiers necessaires à l'execution du script s'il existent par sécurité
    for (auto &fileName : m_filesNameToCopy)
    {
    QFile::setPermissions(m_tmp->path() + "/" + fileName, QFileDevice::Permissions(0x2|0x4|0x20|0x40|0x200|0x400|0x2000|0x4000));
    QFile::remove(m_tmp->path() + "/" + fileName);
    }

    //Copie de la liste des fichiers necessaires à l'execution du script
    for (auto &fileName : m_filesNameToCopy )
    {
    QFile::copy(":/" + fileName, m_tmp->path() + "/" + fileName);
    QFile::setPermissions(m_tmp->path() + "/" + fileName, QFileDevice::Permissions(0x2000|0x4000));
    qDebug()<<"Copie de :" << fileName << "dans :" << m_tmp->path() + "/" + fileName;
    }
    ConfigFile configFiles;
    //Execution du script
    m_program->setWorkingDirectory(m_tmp->path());
    m_program->start();
    qDebug()<<"Debut du script" << m_program->program() << m_program->arguments();
    waitForFinished();
    qDebug()<<"Fin du script" << m_program->program() << m_program->arguments();
}

void Executer::s_readStandardOutput()
{
    qDebug()<< "text = " << m_load->getText();
    QString allStandardOutputTemp { m_program->readAllStandardOutput() };
    qDebug()<< "allStandardOutput = " << allStandardOutputTemp;
    if (allStandardOutputTemp == "reduction copie off\r\n")
    {
            m_load->setFixedWidth(m_loadSizeX);
    }
    else if (allStandardOutputTemp == "reduction copie on\r\n")
    {
            m_load->setFixedWidth(m_loadSizeX/1.2);
    }
    else
    {
        m_load->setText( allStandardOutputTemp.remove("\n") );
    }
}
void Executer::s_systemTrayIcon(bool state)
{
    if(state)
    {
        m_load->autosaveStart();
    }
    else
    {
        m_load->autosaveEnd();
    }
}

void Executer::setArguments(const QStringList &arguments)
{
    m_program->setArguments(arguments);
    qDebug()<< "Nouveau arguments :" << m_program->arguments();
}
void Executer::waitForFinished()
{
    while (m_program->state()==QProcess::Running)
    {
        QCoreApplication::processEvents();
    }
}
void Executer::hideM_load()
{
    m_load->showMinimized();
}
