#ifndef NETUSE_H
#define NETUSE_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QMessageBox>

class NetUse : public QProcess
{
public:
    NetUse(QStringList config, QObject *parent = nullptr);//config[0] path, config[1]User, config[2]Password, config[3] Heure, config[4] Minute
    ~NetUse();
    bool mount();
    bool unmount();
    bool checkPath();

private:
    QString m_path;
    QString m_user;
    QString m_password;
};

#endif // NETUSE_H
