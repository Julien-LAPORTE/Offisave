#include "LocationWindow.h"

LocationWindow::LocationWindow(int sizeX, int sizeY) : m_sizeX{sizeX}, m_sizeY{sizeY}
{
    setText("Emplacement");
    setGeometry(0, 0, sizeX, sizeY);
    setWindowFlag(Qt::FramelessWindowHint);
    setToolTip("Permet de choisir un autre emplacement d'enregistrement du dossier client avant recup");
}
LocationWindow::~LocationWindow()
{

}
