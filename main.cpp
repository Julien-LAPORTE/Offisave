#include "static/Controller.h"

#include <QApplication>
#include <QSharedMemory>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSharedMemory sharedMemory { "{69B55631-A712-4d8c-8852-A8ED297EF439}" };
    if(sharedMemory.create( sizeof(int)) == false )
        {
            // Utilisation d'une QMessageBox pour prévenir l'utilisateur que le programme est déjà lancé
            QMessageBox msgBox;
            msgBox.setText("L'application est déjà lancée");
            msgBox.exec();
            return 1;
        }

        QStringList arguments;
        qDebug()<< "argument.size :" << QApplication::arguments().size();
        for(auto &argument : QApplication::arguments())
        {
            qDebug()<< argument << Qt::endl;
            arguments << argument;
        }

        if (arguments.size() < 2)
        {
           Controller w;
           return a.exec();
        }
        else
        {
           Controller w{ arguments[1] };
           return a.exec();
        }


}
