#ifndef COPYPROGRESS_H
#define COPYPROGRESS_H

#include "InventaireFichiers.h"
#include "InventaireRepertoire.h"

#include <QTime>
#include <QCoreApplication>


class CopyProgress : public QObject
{
    Q_OBJECT
public:
    CopyProgress(QString emplacement, bool isBackup = true, QObject *parent = nullptr);
    ~CopyProgress();
    QStringList getInfosProgress();

public slots:
    void s_finInventaire();

signals:
    void nombreFichierInventaire(int nombreFichiersInventaire);
    void nombreFichiersChanged(int nombreFichiers);
    void finished();

private:
    InventaireFichiers *m_inventaire;
    InventaireRepertoire *m_inventaireBackup;
    int m_nombreInventaire;
    int m_nombreFichiersInventaire;
    int m_nombreFichiersBackup;
    QString m_emplacement;
    bool m_isBackup;
private slots:
    void s_updateInventaire();
    void s_checkIfFinished();
};

#endif // COPYPROGRESS_H
