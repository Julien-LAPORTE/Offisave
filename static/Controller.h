#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "LocationWindow.h"
#include "Executer.h"
#include "MainWindow.h"
#include "InventaireFichiers.h"
#include "PlanificationWindow.h"
#include "functions.h"
#include "Autosave.h"
#include "QRes.h"
#include "../../Users/Sam/Documents/configOffisave/ConfigMainWindow.h"

#include <QFileDialog>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QDir>
#include <QTcpSocket>


class Controller : public QWidget
{
    Q_OBJECT
public:
    Controller(QString argument = NULL, QString emplacement = ("//192.168.0.210/Public/sauvegardes"), QWidget *parent = nullptr);
    ~Controller();
    void initHostName();

public slots:
    void s_buttonClicked(QString name);
        void s_setLocation();
        void s_finInventaire();
        void s_saveNameChanged();
        void s_showMenu();
        void s_initMainWindow();
        void s_timeout();
        void s_stepTimeout();
        void s_smbConnected();

private:
    Maintenance *m_maintenance;
    MainWindow *m_mainWindow;
    MainWindow *m_advancedWindow;
    LocationWindow *m_locationButton;
    QDir *m_location;
    InventaireFichiers *m_inventaire;
    QTcpSocket *m_testSmb;
    QLabel *m_labelStart;
    QString m_emplacement;
    QString m_hostname;
    bool backupOrRestauration();
    bool createQdirLocation();
    bool m_smbConnected;
    bool m_nas;
    bool m_serveurLGPI;
    bool m_selectedLocation;
    QString m_argument;
    ConfigMainWindow *m_configWindow;
};

#endif // CONTROLLER_H
