#ifndef QRES_H
#define QRES_H

#include <qt_windows.h>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QProcess>
#include <QMessageBox>

class QRes : public QObject
{
public:
    QRes();
    ~QRes();
    static void backup(QString backupFilePath);
    static void restore(QString backupFilePath);
};

#endif // QRES_H
