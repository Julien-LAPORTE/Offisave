#ifndef CIPHERBYTEARRAY_H
#define CIPHERBYTEARRAY_H


#include <QDebug>
#include <QRandomGenerator>

class CipherByteArray : public QByteArray
{
public:
    CipherByteArray();
    ~CipherByteArray();
    void encrypt();
    void uncrypt();

private:
};

#endif // CIPHERBYTEARRAY_H
