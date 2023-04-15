#ifndef INVENTAIREREPERTOIRE_H
#define INVENTAIREREPERTOIRE_H

#include <QObject>
#include <QDir>
#include <QDirIterator>
#include <QTimer>

#include "functions.h"

class InventaireRepertoire : public QObject
{
    Q_OBJECT
public:
    InventaireRepertoire(QString chemin, QObject *parent = nullptr);
    ~InventaireRepertoire();

    void inventaireFichiers();
    QString getTaille();

    QStringList getArbo() const&;
    QStringList getCheminsFichiers() const&;
    int getNombreFichiers() const&;
    int getNombreRepertoires() const&;
    int getTailleMOctets() const&;
    int getTailleGOctets() const&;
signals:
    void inventaireTermine();

private slots:
    void s_iteratorInventaireFichiers();
    void s_stepInventaireFichiers();

private:
    QFileInfoList m_fileInfoList;
    QDirIterator m_dirsIterations;
    QStringList m_fileList;
    QStringList m_dirList;
    int m_tailleOctets;
    int m_tailleMOctets;
    int m_tailleGOctets;
    int m_nombreFichiers;
    int m_nombreRepertoires;
};

#endif // INVENTAIREREPERTOIRE_H
