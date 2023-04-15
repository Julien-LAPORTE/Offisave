#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QTimeEdit>

class ConfigWindow : public QWidget
{
    Q_OBJECT
public:
    ConfigWindow(QString hostname, QWidget *parent = nullptr);
    ~ConfigWindow();
    QStringList getConfig();

private slots:
    void s_valider();
    void s_annuler();

private:
    QTimeEdit *m_time;
    QLabel *m_timeLabel;
    QFormLayout *m_formLayout;
    QStringList m_config;
    bool m_cancel;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // CONFIGWINDOW_H
