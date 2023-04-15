#ifndef CHECKLOCATION_H
#define CHECKLOCATION_H

#include <QRunnable>
#include <QFile>

class CheckLocation : public QRunnable
{
public:
    explicit CheckLocation();
    virtual void run();
};

#endif // CHECKLOCATION_H
