#ifndef INVENTAIREFICHIERS_H
#define INVENTAIREFICHIERS_H

#include <QDebug>
#include <cassert>
#include <QFileInfoList>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QTimer>
#include <QLabel>
#include <QMessageBox>
#include <QSpacerItem>
#include <QGridLayout>

#include "functions.h"

class InventaireFichiers : public QObject
{
    Q_OBJECT
public:
    InventaireFichiers(QStringList sources, bool showLoad = true, QObject *parent = nullptr);
    ~InventaireFichiers();

    void inventaireFichiers();
    void nextSource();
    void showBox();
    QString getTotal();

    QList<QStringList *> getArbo() const&;
    QList<QStringList *> getCheminsFichiers() const&;
    QList<int> getNombreFichiers() const&;
    QList<int> getNombreRepertoires() const&;
    QList<int> getTailleMOctets() const&;
    QList<int> getTailleGOctets() const&;
signals:
    void inventaireTermine();

private slots:
    void s_iteratorInventaireFichiers();
    void s_stepInventaireFichiers();

private:
    QLabel *m_load;
    QList<QFileInfoList *> m_fileInfoList;
    QList<QStringList *> m_fileList;
    QList<QStringList *> m_dirList;
    QList<QDirIterator *> m_dirsIterations;
    int m_sourceIteration;
    QList<int> m_tailleOctets;
    QList<int> m_tailleMOctets;
    QList<int> m_tailleGOctets;
    QList<int> m_nombreFichiers;
    QList<int> m_nombreRepertoires;
    int m_tailleTotaleOctects;
    int m_tailleTotaleMOctects;
    int m_tailleTotaleGOctects;
    QStringList m_sources;
    QString m_infos;
    QString m_total;
};

#endif // INVENTAIREFICHIERS_H
