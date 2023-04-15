#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "ConfigFile.h"

#include <QFileInfo>
#include <QProcess>
#include <QDebug>
#include <QDir>

void conversion(int &valeurO, int &valeurMo, int &valeurGo);
void verifMax( QFileInfo const &infoFichier, int &valeurO, int &valeurMo,int &valeurGo , int const max = INT_MAX );
void explorerWindows(QString state = "on");
QStringList listDirToSave(bool copyFiles = true);//Envoyer false si pas besoin de copier les fichiers
bool checkConfigFile();

#endif // FUNCTIONS_H
