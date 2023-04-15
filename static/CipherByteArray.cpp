#include "CipherByteArray.h"
namespace
{
    const QByteArray printables()
    {
        QByteArray list;
        for(int i = 0; i < 256; ++i)
        {
            if (QChar(char(i)).isPrint())
                list+= char(i);
        }
        return list;
    }
}

CipherByteArray::CipherByteArray() : QByteArray()
{

}
CipherByteArray::~CipherByteArray()
{

}
void CipherByteArray::encrypt()
{

            qDebug()<< "Debut :" << QString::fromLatin1(*this);
            // Pour éviter de décoder en "%xx" les caractères affichables
            static const QByteArray exclude = printables();

            QByteArray texteEnOctet = QByteArray::fromPercentEncoding(*this);
            static const QByteArray clef = "tç_àçèdhjscbi\"*ùjkndcècfgbfhjkukxs";
            QByteArray codeFinal;
            int tailleClef = clef.size();

            for (int i = 0; i < texteEnOctet.size(); ++i)
            {
                codeFinal += char(texteEnOctet[i] ^ clef[i % tailleClef]);
            }

            this->clear();

            *this += codeFinal.toPercentEncoding(exclude);
            qDebug()<< "Fin   :" << QString::fromLatin1(*this);
}
void CipherByteArray::uncrypt()
{
    encrypt();
}
