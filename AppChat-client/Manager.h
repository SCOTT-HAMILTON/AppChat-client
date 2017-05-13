#ifndef MANAGER_H
#define MANAGER_H

#include <SFML/Network.hpp>
#include "WinApp.h"
#include "FirstConnectionWin.h"

class Manager : public QObject
{
    Q_OBJECT

public:
    Manager();
    void manage();

public slots:
    void done();

private:
    sf::IpAddress m_host;
    WinApp *fenetre;
    FirstConnectionWin *setupConnection;
};

#endif // MANAGER_H
