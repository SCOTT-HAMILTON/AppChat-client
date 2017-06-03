#include "FirstConnectionWin.h"
#include "DataType.h"
#include "BackSide.h"

#include <string>
#include <RSA-Crypt.h>
#include <iostream>
#include <QPalette>
#include <QBrush>
#include <QImage>
#include <QApplication>
#include <QMenu>
#include <QIcon>
#include <string>
#include <vector>
#include <QTimer>

#include <iostream>

FirstConnectionWin::FirstConnectionWin(sf::IpAddress Host) : QWidget(),
    m_host(Host), connected(false), dialogIsOpen(false), m_indexPlan(0)
{

    setFixedWidth(200);
    setFixedHeight(200);

    QIcon configIcon(QApplication::applicationDirPath()+"/config-button.png");
    m_configButton = new QPushButton;
    m_configButton->setIcon(configIcon);
    QMenu *menu = new QMenu("config button");
    menu->addAction("modifier l'ip du serveur", this, &changeHost_GUI);
    m_configButton->setMenu(menu);
    m_configButton->setFixedWidth(25);
    m_configButton->setFixedHeight(25);

    m_mainLay = new QVBoxLayout;

    m_pseudotxt = new QLabel("Entrer votre pseudo : ");
    QFont font;
    font.setBold(true);
    font.setPixelSize(14);
    m_pseudotxt->setFont(font);
    m_pseudoedit = new QLineEdit;
    m_codetxt = new QLabel("Entrer votre code : ");
    m_codetxt->setFont(font);
    m_codeedit = new QLineEdit;
    m_validateButton = new QPushButton("valider");
    m_errortxt = new QLabel("");
    m_errortxt->hide();

    m_mainLay->addWidget(m_configButton, 0, Qt::AlignRight|Qt::AlignTop);
    m_mainLay->addWidget(m_pseudotxt);
    m_mainLay->addWidget(m_pseudoedit);
    m_mainLay->addWidget(m_codetxt);
    m_mainLay->addWidget(m_codeedit);
    m_mainLay->addWidget(m_validateButton);
    m_mainLay->addWidget(m_errortxt);

    setLayout(m_mainLay);

    m_errortxt->hide();

    QObject::connect(m_validateButton, SIGNAL(clicked(bool)), this, SLOT(tryConnect()));

    QPalette fond;
    int i = 0;
    for (i = 0; i < 5; i++){
        fond.setBrush(backgroundRole(),QBrush(QImage(QApplication::applicationDirPath()+"/PLAN"+QString::number(i+1)+".jpg")));
        m_fonds.push_back(fond);
    }

    setPalette(m_fonds[m_indexPlan]);
    QTimer *timer = new QTimer;
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(changePlan()));
    timer->start(5000);

}

void FirstConnectionWin::changePlan(){
    m_indexPlan++;
    if (m_indexPlan>4)m_indexPlan=0;
    setPalette(m_fonds[m_indexPlan]);
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
            if (dialogIsOpen){
                m_dialogChangeIp->close();
                dialogIsOpen = false;
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

void FirstConnectionWin::changeHost_GUI(){
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

void FirstConnectionWin::changeHost(){
    m_dialogChangeIp->close();
    dialogIsOpen = false;
    std::cout << "host changed to: " << (m_changeIpEdit->text()).toStdString() << std::endl;
    m_host = sf::IpAddress((m_changeIpEdit->text()).toStdString());
}
