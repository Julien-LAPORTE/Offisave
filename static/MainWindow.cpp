 #include "MainWindow.h"

MainWindow::MainWindow(QStringList &texts, QStringList &tooltips, QStringList &windowsConfirmationName, QStringList &windowsConfirmationText,
                       QString saveName, int const sizeX, int const sizeY, int const sizeQLineEdit) : m_qline { nullptr },
    m_windowsConfirmationName{windowsConfirmationName}, m_windowsConfirmationText(windowsConfirmationText)//Mettre "null" pour aucune messagebox
{
    assert( windowsConfirmationName.size() == m_windowsConfirmationText.size() && "Constructor Error : Number of windowsConfirmationName is not equal to number of windowsConfirmationText" );
    assert( windowsConfirmationName.size() == texts.size() && "Constructor Error : Number of Button is not equal to number of windowsConfirmationText" );
    setWindowFlag(Qt::FramelessWindowHint);
    setCursor(Qt::PointingHandCursor);
    if(saveName != "null")
    {
    m_qline = new QLineEdit { saveName, this };
    m_qline->setGeometry(0, 0, 180, 20);
    m_qline->setAlignment(Qt::AlignCenter);
    connect ( m_qline, &QLineEdit::textChanged, this, &MainWindow::s_qlineTextChanged );
    }
    createButtons(sizeX, sizeY,sizeQLineEdit, texts, tooltips);
}

MainWindow::~MainWindow()
{
    for ( QPushButton *&button : m_buttons )
    {
        qDebug() << "Destructions m_boutons" << button->text();
        delete button;
    }
    if (m_qline != nullptr) delete m_qline;
}

void MainWindow::s_showConfirmation(int bouttonNumber)
{
       if ( m_windowsConfirmationName[bouttonNumber] != "null" or m_windowsConfirmationText[bouttonNumber] != "null" )
       {
           QMessageBox::information(this, m_windowsConfirmationName[bouttonNumber], m_windowsConfirmationText[bouttonNumber]);
           if(bouttonNumber != 1)//si c'est pas une restauration on quitte immediatement
           {
           qApp->exit();
           }
       }
       else
       {
           qApp->exit();
       }
}

void MainWindow::s_qlineTextChanged()
{
    if(m_qline != nullptr) m_qline->setText(m_qline->text().remove("(").remove(")").remove("^").remove("&").remove("/").remove("\\").remove(":").remove("*").remove("?").remove("\"").remove("<").remove(">").remove("|").remove(" "));
    emit qlineTextChanged();
}

void MainWindow::createButtons(int sizeX, int sizeY, int sizeQLineEdit, QStringList &texts, QStringList &tooltips )
{
    for (int i{0}; i<texts.size(); i++)
    {
        m_buttons << new QPushButton { texts[i], this };
        if(m_qline != nullptr)
        {
        m_buttons[i]->setGeometry(0, sizeY*i+sizeQLineEdit, sizeX, sizeY);
        setFixedSize(sizeX, sizeY*(i+1)+sizeQLineEdit);
        }
        else
        {
            m_buttons[i]->setGeometry(0, sizeY*i, sizeX, sizeY);
            setFixedSize(sizeX, sizeY*(i+1));
        }
        if( tooltips.size() > i and tooltips[i] != "null" ) m_buttons[i]->setToolTip(tooltips[i]);
        //On connecte le bouton
        connect ( m_buttons[i], &QPushButton::clicked, [this, i]()-> void { emit buttonClicked(m_buttons[i]->text()) ; } );
    }
}

void MainWindow::changeButtons(int buttonNumber, QString text, QString tooltip, QString confirmationName, QString confirmationText)
{
    assert( buttonNumber <= m_buttons.size() -1 && "numero du bouton et plus grand que le tableau" );
    qDebug() << "Changement du texte sur le bouton : " << buttonNumber;
    qDebug() << ": m_buttons.size =" << m_buttons.size();
    m_buttons[buttonNumber]->setText(text);
    m_buttons[buttonNumber]->setToolTip(tooltip);
    m_windowsConfirmationName[buttonNumber] = confirmationName;
    m_windowsConfirmationText[buttonNumber] = confirmationText;
}

QString MainWindow::getQlineText()
{
    if (m_qline!= nullptr)
    {
        return m_qline->text();
    }
    else
    {
        return "null";
    }
}
