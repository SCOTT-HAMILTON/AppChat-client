#include "WinApp.h"
#include <iostream>

#include <QIcon>
#include <QApplication>
#include <RSA-Crypt.h>
#include <QPalette>
#include <QBrush>
#include <QImage>

WinApp::WinApp(sf::IpAddress Host): QWidget(),
  m_host(Host), updateMessage(false), type(), dialogIsOpen(false), p(509), q(9403)
{
    setFixedWidth(340);
    setFixedHeight(300);
    //config button
    QIcon configIcon(QApplication::applicationDirPath()+"/config-button.png");
    m_configButton = new QPushButton;
    m_configButton->setIcon(configIcon);
    QMenu *menu = new QMenu("config button");
    menu->addAction("modifier l'ip du serveur", this, &changeHost_GUI);
    m_configButton->setMenu(menu);
    m_configButton->setFixedWidth(25);
    m_configButton->setFixedHeight(25);

    //pseudo GUI
    m_connectLay = new QFormLayout;
    QFont font;
    font.setBold(true);
    font.setPixelSize(14);
    m_pseudo = QString::fromStdString(sf::IpAddress::getPublicAddress().toString());
    m_editpseudo = new QLineEdit;
    m_editcode = new QLineEdit;
    m_pseudoButton = new QPushButton("valider");
    m_pseudotxt = new QLabel("Entrer votre pseudo : ");
    m_codetxt = new QLabel("Entrer votre code : ");
    m_pseudotxt->setFont(font);
    m_codetxt->setFont(font);
    m_connectLay->addRow(m_pseudotxt, m_editpseudo);
    m_connectLay->addRow(m_codetxt, m_editcode);

    QFont font_error;
    font_error.setPixelSize(11);
    m_errorCLay = new QHBoxLayout;
    m_errorConnection = new QLabel("le serveur n'est pas disponible pour le moment.");
    m_errorConnection->setFont(font_error);
    m_retryConnection = new QPushButton("réessayer");
    m_errorCLay->addWidget(m_errorConnection);
    m_errorCLay->addWidget(m_retryConnection);
    m_errorConnection->hide();
    m_retryConnection->hide();

    //set Interface
    m_mainLay = new QVBoxLayout;

    m_textRMessage = new QTextEdit;
    m_textRMessage->hide();
    m_textRMessage->setReadOnly(true);

    m_enterMLay = new QFormLayout;
    m_enterMessage = new QLineEdit;
    m_enterMessage->hide();
    m_enterMLay->addRow("Message", m_enterMessage);

    m_mainLay->addWidget(m_configButton, 0, Qt::AlignTop|Qt::AlignRight);
    m_mainLay->addLayout(m_errorCLay);
    m_mainLay->addLayout(m_connectLay);
    m_mainLay->addWidget(m_pseudoButton);
    m_mainLay->addWidget(m_textRMessage);

    setLayout(m_mainLay);

    getPUKey(p, q, n, e);
    getPRKey(p, q, n, d);

    QPalette fond;
    fond.setBrush(backgroundRole(),QBrush(QImage(QApplication::applicationDirPath()+"/main-window.jpg")));
    setPalette(fond);

    //setConnection
    QObject::connect(m_pseudoButton, SIGNAL(clicked(bool)), this, SLOT(showGui()));
    QObject::connect(m_retryConnection, SIGNAL(clicked(bool)), this, SLOT(connectToHost()));
}
void WinApp::keyPressEvent(QKeyEvent *event){
    if (event->key() == 16777220){
        updateMessage = true;
        tryUpdateMessage();
    }
}
void WinApp::keyReleaseEvent(QKeyEvent *event){
    if (event->key() == 16777220){
        updateMessage = false;
    }
}

void WinApp::tryUpdateMessage(){
    if (updateMessage && m_enterMessage->hasFocus() && m_enterMessage->text() != ""){
        m_txtContenue += QString("moi : "+m_enterMessage->text()+"\n");
        m_textRMessage->append(QString("moi : "+m_enterMessage->text()));
        m_textRMessage->moveCursor(QTextCursor::End);
        sendMessage(QString(m_pseudo+" : "+m_enterMessage->text()));
        m_enterMessage->setText("");
    }
}

void WinApp::sendMessage(const QString &text){
    sf::Packet send;
    if (text != ""){
       send << DataType::message << ToStr(crypt(n, e, (text.toStdString())));
       m_socket.send(send);
    }
}

void WinApp::receiveData(QString dataR){
    std::string tmp = dataR.toStdString();
    QString message =  QString::fromStdString(decrypt(d, n, FromStr(tmp)))+QString("\n");
    m_txtContenue += message;
    m_textRMessage->append(message);
    m_textRMessage->moveCursor(QTextCursor::End);
}

void WinApp::showGui(){
    if (!connectToHost()){

        if (m_editpseudo->text().toStdString() != ""){
            m_pseudo = m_editpseudo->text();
        }

        sf::Packet send, receive;
        std::string dataR, typeR;
        send << DataType::connection_client << m_pseudo.toStdString();
        m_socket.send(send);
        send.clear();
        send << DataType::code << m_editcode->text().toStdString();
        m_socket.send(send);
        typeR = "";
        m_socket.receive(receive);
        receive >> typeR >> dataR;

        if (typeR == DataType::valid_connection){
            send.clear();
            send << DataType::getInitMessages << "";
            m_socket.send(send);
            receive.clear();
            m_socket.receive(receive);
            receive >> typeR >> dataR;
            m_txtContenue = QString::fromStdString(decrypt(d, n, FromStr(dataR)));
            m_textRMessage->setPlainText(m_txtContenue);

            m_errorConnection->hide();
            std::cout << "pseudo available" << std::endl;
            m_editpseudo->hide();
            m_editcode->hide();
            m_connectLay->removeRow(m_editpseudo);
            m_connectLay->removeRow(m_editcode);
            m_pseudoButton->hide();
            m_configButton->hide();
            if (dialogIsOpen){
                m_dialogChangeIp->close();
                dialogIsOpen = false;
            }
            m_mainLay->addLayout(m_enterMLay);
            m_textRMessage->show();
            m_enterMessage->show();
            setWindowTitle("Client : "+m_pseudo);
            setFixedSize(300, 300);
            m_ThreadManager = new ThreadManager(&m_socket);
            QObject::connect(m_ThreadManager, SIGNAL(workerRData(QString)), this, SLOT(receiveData(QString)));
            m_ThreadManager->startWorker();
        }
        else {
            m_errorConnection->hide();
            m_errorConnection->setText("ce pseudo ou ce code est incompatible");
            m_errorConnection->show();
        }

    }else std::cout << "connection error, retry" << std::endl;

}

void WinApp::changeHost_GUI(){
    m_dialogChangeIp = new QDialog(this);
    QLabel *txt = new QLabel("Entrer l'addresse ip du serveur : ");
    m_changeIpEdit = new QLineEdit;
    QPushButton *button = new QPushButton("modifier");

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(txt);
    lay->addWidget(m_changeIpEdit);
    lay->addWidget(button);
    m_dialogChangeIp->setLayout(lay);

    QObject::connect(button, SIGNAL(clicked()), this, SLOT(changeHost()));

    m_dialogChangeIp->show();
    dialogIsOpen = true;
}

void WinApp::changeHost(){

    m_dialogChangeIp->close();
    dialogIsOpen = false;
    std::cout << "host changed to: " << (m_changeIpEdit->text()).toStdString() << std::endl;
    m_host = sf::IpAddress((m_changeIpEdit->text()).toStdString());
}

bool WinApp::connectToHost(){
    sf::Packet receive, send;
    std::string dataS("You're connected with "+sf::IpAddress::getLocalAddress().toString());
    send << dataS;
    m_errorConnection->hide();
    m_socket.disconnect();
    if (m_socket.connect(m_host, 53000) != sf::Socket::Done){
        m_errorConnection->show();
        m_retryConnection->show();
        return 1;
    }
    if (m_socket.receive(receive) != sf::Socket::Done){
        m_errorConnection->show();
        m_retryConnection->show();
        return 1;
    }
    if (m_socket.send(send) != sf::Socket::Done){
        m_errorConnection->show();
        m_retryConnection->show();
        return 1;
    }
    m_errorConnection->hide();
    m_retryConnection->hide();

    return 0;
}

WinApp::~WinApp(){
    delete m_ThreadManager;
    m_socket.disconnect();
}
