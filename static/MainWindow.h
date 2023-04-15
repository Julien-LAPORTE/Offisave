#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QDebug>
#include <cassert>
#include <QMainWindow>
#include <QPushButton>
#include <QList>
#include <QWidget>
#include <QMessageBox>
#include <QLineEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
     MainWindow(QStringList &texts, QStringList &tooltips, QStringList &windowsConfirmationName, QStringList &windowsConfirmationText,
                QString saveName = "null", int const sizeX = 180, int const sizeY = 60, int const sizeQLineEdit = 20 );
    ~MainWindow();
     void changeButtons(int buttonNumber, QString text, QString tooltip, QString confirmationName, QString confirmationText);
     QString getQlineText();

public slots:
    void s_showConfirmation(int buttonNumber);
    void s_qlineTextChanged();

signals:
    void buttonClicked(QString);//emet le signal du bouton valeur int == case du bouton dans le tableau
    void qlineTextChanged();

private:
    QList<QPushButton *> m_buttons;
    QLineEdit *m_qline;
    QStringList m_windowsConfirmationName;
    QStringList m_windowsConfirmationText;
    void createButtons(int sizeX, int sizeY,  int sizeQLineEdit, QStringList &texts, QStringList &tooltips );//renseigner "null" en tooltips pour ne pas en mettre
};
#endif // MAINWINDOW_H
