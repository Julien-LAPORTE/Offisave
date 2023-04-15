#ifndef LOADING_H
#define LOADING_H

#include "Maintenance.h"
#include "CopyProgress.h"

#include <QInputDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDesktopWidget>
#include <QProgressBar>
#include <QScreen>

class Loading : public QProgressBar
{
    Q_OBJECT
public:
    Loading(QString text, QString const &emplacement);
    ~Loading();
    void setText(QString text);
    QString getText();
    void autosaveStart();
    void autosaveEnd();

private:
    Maintenance *m_maintenance;
    QLabel *m_label;
    CopyProgress *m_progress;
    QString m_emplacement;
    QList<QLabel *> m_infos;
    QStringList m_textInfos;
    QScreen *m_screen;
    int m_largeurEcran;
    int m_hauteurEcran;
    double m_hauteurWindow;
    QSystemTrayIcon *m_tray;
    void initInfos();

private slots:
    void s_SetNombreFichiersInventaire(int nombreFichiers);
    void s_SetNombreFichiersBackup(int nombreFichiers);
    void s_finished();

};

#endif // LOADING_H
