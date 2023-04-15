#include "Maintenance.h"

#include <QMessageBox>
Maintenance::Maintenance(QString text, QString backColor, QString color, int textSizeX, int textSizeY )
    : m_timer {nullptr}, m_leave {nullptr}, m_secondes{0}, m_minutes{0}, m_heures{0}
{
       if ( !isVisible() ) showFullScreen();
       setText(text);
       setWordWrap(true);
       setStyleSheet("QLabel { background-color :" +backColor+ "; color :" +color+ "; }");
       setAlignment(Qt::AlignCenter);
       if (textSizeX !=0 and textSizeY != 0) setFont(QFont{"ARIAL", textSizeX, textSizeY});
       setCursor(Qt::ForbiddenCursor);
}

Maintenance::~Maintenance()
{
   if(m_timer!=nullptr) delete m_timer;
}

void Maintenance::showMaintenance()
{
    explorerWindows("off");
    m_timer=new QLabel {"00:00:00"};
    m_timer->setParent(this);
    m_timer->setWindowFlag(Qt::FramelessWindowHint);
    m_timer->setFixedSize(600, 60);
    m_timer->setAlignment(Qt::AlignLeft);
    m_timer->setFont(QFont{"ARIAL", 10, 10 });
    m_timer->setWordWrap(true);
    m_timer->show();
    this->show();
    QTimer::singleShot(0, this, &Maintenance::s_iteratorTimer);

    m_leave=new QPushButton { "Fermer", this };
    m_leave->move(Qt::AlignLeft, Qt::AlignBottom);
    m_leave->setStyleSheet(QString::fromUtf8("background-color: red; "));
    m_leave->setCursor(Qt::PointingHandCursor);
    connect( m_leave, &QPushButton::clicked, this , &Maintenance::s_endMaintenance );
    m_leave->show();
}

void Maintenance::s_endMaintenance()
{
    explorerWindows();
    qApp->exit();
}

void Maintenance::s_iteratorTimer()
{
        QTimer::singleShot(1000, this, &Maintenance::s_stepTimer);
        m_secondes++;
        if (m_secondes >= 60)
        {
            m_minutes++;
            m_secondes-=60;
            if (m_minutes >= 60)
            {
                m_heures++;
                m_minutes-=60;
            }
        }
}

void Maintenance::s_stepTimer()
{
    QTimer::singleShot(0, this, &Maintenance::s_iteratorTimer);
    m_timer->setText(QString::number(m_heures) + " Heures " + QString::number(m_minutes) + " Minutes " + QString::number(m_secondes) + " Secondes" );
}
