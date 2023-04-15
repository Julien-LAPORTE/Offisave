#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include "functions.h"

#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

class Maintenance : public QLabel
{

    Q_OBJECT
public:
     Maintenance(QString test, QString backColor = "black", QString color = "white", int textSizeX=50, int textSizeY=50);
    ~Maintenance();
     void showMaintenance();

public slots:
     void s_iteratorTimer();
     void s_stepTimer();
     void s_endMaintenance();

private:
     QLabel *m_timer;
     QPushButton *m_leave;
     int m_secondes;
     int m_minutes;
     int m_heures;

};

#endif // MAINTENANCE_H
