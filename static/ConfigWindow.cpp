#include "ConfigWindow.h"


ConfigWindow::ConfigWindow(QString hostname, QWidget *parent) : QWidget(parent), m_formLayout{new QFormLayout}, m_cancel{false}
{
    int largeurEcran = QApplication::desktop()->width();
    //int hauteurEcran = QApplication::desktop()->height();

    m_time = new QTimeEdit{ this };
    m_timeLabel = new QLabel{"Heure de sauvegarde", this};
    m_timeLabel->setStyleSheet("QLabel { color : white; }");

    setWindowFlag(Qt::FramelessWindowHint);
    setFixedWidth(largeurEcran / 4);
    QList<QLabel*> labels;
    QList<QLineEdit*> linesEdit;
    QPushButton *valider = new QPushButton {"Valider", this};
    QPushButton *annuler = new QPushButton {"Annuler", this};
    connect (annuler, &QPushButton::clicked, this, &ConfigWindow::s_annuler);
    connect (valider, &QPushButton::clicked, this, &ConfigWindow::s_valider);

    labels
            << new QLabel { "Chemin de sauvegarde", this }
            << new QLabel { "Utilisateur", this }
            << new QLabel { "Mot de passe",this }
              ;
    linesEdit
            << new QLineEdit {"//192.168.0.210/Public", this }
            << new QLineEdit { this }
            << new QLineEdit { this }
               ;
    assert(labels.size()==linesEdit.size() && "ConfigWindow error 0001" );

    for(int i{0}; i<labels.size(); i++)
    {
        m_formLayout->addRow(labels[i], linesEdit[i]);
        labels[i]->setStyleSheet("QLabel { color : white; }");
    }
    m_formLayout->addRow(m_timeLabel, m_time);
    m_formLayout->addRow(annuler, valider);
    setLayout(m_formLayout);
    show();
    qDebug()<< hostname;

    //On teste si le hostname commence par Poste
    QString poste;
        for(int i{0}; i<5; i++)
        {
            poste[i] = hostname[i];
        }
    if ( poste == "Poste" and ( hostname.remove(0, 5).toInt() > 0 ) )  //On récupère que le numéro du poste si c'est un numéro
    {
        QTime time{12, 00};
        m_time->setTime(time.addSecs( hostname.toInt() * 300 ));
        qDebug()<< "Numéro du poste : " << hostname;
    }
    else
    {
        m_time->setTime(QTime {12, 30});
    }
    while(isVisible()) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    linesEdit[0]->setText(linesEdit[0]->text().replace("/", "\\"));
    for(auto &line : linesEdit)
    {
        if (m_cancel) line->setText("");
            m_config << line->text();
    }
    m_config << QString::number( m_time->time().hour() );
    m_config << QString::number( m_time->time().minute() );
}
ConfigWindow::~ConfigWindow()
{
    qDebug()<< "Delete instance de ConfigWindows";
    delete m_formLayout;
    delete m_time;
}

QStringList ConfigWindow::getConfig()
{
    return m_config;
}
void ConfigWindow::s_valider()
{
    hide();
}
void ConfigWindow::s_annuler()
{
   m_cancel = true;
   hide();
}

void ConfigWindow::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);

  QBrush background(QColor(23, 23, 34));

  painter.setBrush(background);
  painter.setPen  (Qt::NoPen ); // No stroke

  painter.drawRect(0, 0, width(), height());
}
