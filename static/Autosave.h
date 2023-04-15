#ifndef AUTOSAVE_H
#define AUTOSAVE_H

#include "static/Executer.h"
#include "static/QRes.h"
#include "static/NetUse.h"
#include "CipherByteArray.h"

#include <QObject>
#include <QDebug>
#include <QDir>

class Autosave : public QObject
{
    Q_OBJECT
public:
    Autosave(QString hostname, QObject *parent = nullptr);
    ~Autosave();
    void backup();

signals:
    void autosaveStart(bool state = true);//false s'il se termine

private:
    QString m_hostname;
    QDir m_emplacement;
    QString m_path;
    Executer *m_backup;
    QStringList getEmplacement();//[0]path, [1]user, [2]paswword (Utilisation du fichier config créer par PlanificationWindows)
    bool createQdirLocation();
};

#endif // AUTOSAVE_H
