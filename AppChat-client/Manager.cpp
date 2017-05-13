#include "Manager.h"

#include <fstream>
#include <iostream>
#include <QWidget>
#include <QObject>

#include <QApplication>

#include <ServerManager.h>

#define STATUS_PATH "/config/status.txt"

Manager::Manager() :
    m_host("5.51.51.245")
{
    if (sf::IpAddress::getPublicAddress() == sf::IpAddress("5.51.51.245")){
        m_host = "192.168.1.99";
    };

    fenetre = new WinApp(m_host);
}

void Manager::manage(){

    if (!hostExist(m_host.toString(), QApplication::applicationDirPath().toStdString()+STATUS_PATH)){
        createStatus(m_host.toString(), 0, QApplication::applicationDirPath().toStdString()+STATUS_PATH);
    }
    if (getStatus(m_host.toString(), QApplication::applicationDirPath().toStdString()+STATUS_PATH) == 48){
        setupConnection = new FirstConnectionWin(m_host);
        setupConnection->show();
        std::cout << "first window launch !" << std::endl;
        QObject::connect(setupConnection, SIGNAL(finished()), this, SLOT(done()));
    }else{
        std::cout << "main window launch !" << std::endl;
       fenetre->show();
    }

    std::cout << "finished" << std::endl;
}

void Manager::done(){
     setStatus(m_host.toString(), 1, QApplication::applicationDirPath().toStdString()+STATUS_PATH);
     fenetre->show();
}

