#ifndef PLANIFICATIONWINDOW_H
#define PLANIFICATIONWINDOW_H

#include "static/MainWindow.h"
#include "static/NetUse.h"
#include "static/ConfigWindow.h"
#include "CipherByteArray.h"

#include <QProcess>
#include <QFile>
#include <QDir>
#include <QTime>
#include <QMessageBox>

class PlanificationWindow : public QWidget
{
    Q_OBJECT
public:
    PlanificationWindow(QString hostname, QWidget *parent = nullptr);
    ~PlanificationWindow();
    void setConfig(QStringList config);

public slots:
    void s_buttonClicked(QString name);

signals:
    void backButton();

private:
    MainWindow *m_menu;
    void uninstall();
    void install();//Création du fichier config et utilisation dans l'objet autosave
    QStringList m_config;//Path en [0], user en [1], password en [2], heure en [3] et minutes en [4]
    QString m_hostname;
    QString m_pathByName;
    bool addLineToHosts(QString emplacement);
    bool removeLineToHosts();
};

#endif // PLANIFICATIONWINDOW_H
