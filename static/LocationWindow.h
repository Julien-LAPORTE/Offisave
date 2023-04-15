#ifndef LOCATIONWINDOW_H
#define LOCATIONWINDOW_H

#include <QPushButton>

class LocationWindow : public QPushButton
{
    Q_OBJECT
public:
    LocationWindow(int sizeX = 120, int sizeY = 30);
    ~LocationWindow();

private:
    int const m_sizeX;
    int const m_sizeY;
};

#endif // LOCATIONWINDOW_H
