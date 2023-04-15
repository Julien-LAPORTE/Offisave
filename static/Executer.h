#ifndef EXECUTER_H
#define EXECUTER_H

#include "Loading.h"
#include "ConfigFile.h"


#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>

class Executer : public QObject
{
    Q_OBJECT
public:
    //Pour executer un script en source, renseigner le nom du program en source(nom.bat), les arguments
    //et une QStringList de nom de fichier en source si besoin de leurs présences à l'execution du script
    //Fonctionne que avec un nom de fichier et pas un chemin complet (:/fichier.ini) comme pour le script
    explicit Executer(QString const &program, QStringList const &arguments, QStringList const &filesNameToCopy, QObject *parent = nullptr);
    ~Executer();
    void setArguments(QStringList const &arguments);
    void waitForFinished();
    void hideM_load();

public slots:
    void s_executer();
    void s_readStandardOutput();
    void s_systemTrayIcon(bool state = false);

signals:


private:
    QDir *m_tmp;
    QProcess *m_program;
    QStringList m_filesNameToCopy;
    Loading *m_load;
    double m_loadSizeX;
    double m_loadSizeY;
    double m_loadPosX;
    double m_loadPosY;
};

#endif // EXECUTER_H
