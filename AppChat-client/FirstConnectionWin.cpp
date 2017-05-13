#include "FirstConnectionWin.h"
#include "DataType.h"

#include <string>
#include <RSA-Crypt.h>
#include <iostream>

FirstConnectionWin::FirstConnectionWin(sf::IpAddress Host) : QWidget(),
    m_host(Host), connected(false)
{
    m_mainLay = new QVBoxLayout;

    m_pseudotxt = new QLabel("Entrer votre pseudo : ");
    m_pseudoedit = new QLineEdit;
    m_codetxt = new QLabel("Entrer votre code : ");
    m_codeedit = new QLineEdit;
    m_validateButton = new QPushButton("valider");
    m_errortxt = new QLabel("");
    m_errortxt->hide();

    m_mainLay->addWidget(m_pseudotxt);
    m_mainLay->addWidget(m_pseudoedit);
    m_mainLay->addWidget(m_codetxt);
    m_mainLay->addWidget(m_codeedit);
    m_mainLay->addWidget(m_validateButton);
    m_mainLay->addWidget(m_errortxt);

    setLayout(m_mainLay);

    sf::Packet receive, send;

    std::string dataS("You're connected with "+sf::IpAddress::getLocalAddress().toString());
    send << dataS;
    m_errortxt->hide();

    QObject::connect(m_validateButton, SIGNAL(clicked(bool)), this, SLOT(tryConnect()));
}

int FirstConnectionWin::tryConnect(){
    sf::Packet receive, send;
    std::string typeR, dataR;

    if (!connected){
        if (m_socket.connect(m_host, 53000) != sf::Socket::Done){
            m_errortxt->setText("impossible de se connecter au serveur");
            m_errortxt->show();
            return 1;
        }
        std::cout << "connected" << std::endl;
        if (m_socket.receive(receive) != sf::Socket::Done){
            m_errortxt->setText("erreur de connexion au serveur");
            m_errortxt->show();
            return 1;
        }
        std::cout << "receiving" << std::endl;
        if (m_socket.send(send) != sf::Socket::Done){
            m_errortxt->setText("erreur de connexion au serveur");
            m_errortxt->show();
            return 1;
        }
        connected = true;
    }

    send << DataType::create_client << m_pseudoedit->text().toStdString();
    if (m_socket.send(send) != sf::Socket::Done){
        m_errortxt->setText("erreur de connexion au serveur");
        m_errortxt->show();
        return 1;
    }else{
        receive.clear();

        if (m_socket.receive(receive) != sf::Socket::Done){
            m_errortxt->setText("erreur de connexion au serveur");
            m_errortxt->show();
            return 1;
        }
        std::cout << "oui 1" << std::endl;
        receive >> typeR >> dataR;
        if (typeR == DataType::pseudoVailable){
            send.clear();
            send << DataType::code << m_codeedit->text().toStdString();
            if (m_socket.send(send) != sf::Socket::Done){
                m_errortxt->setText("erreur de connexion au serveur");
                m_errortxt->show();
                return 1;
            }
            close();
            emit finished();
        }else{
            m_errortxt->setText("ce pseudo est déjà pris, prenez en un autre");
            m_errortxt->show();
        }
    }

    return 0;

}
