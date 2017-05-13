#ifndef FIRSTCONNECTIONWIN_H
#define FIRSTCONNECTIONWIN_H

#include <QWidget>

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <SFML/Network.hpp>

class FirstConnectionWin : public QWidget
{
    Q_OBJECT

public:
    FirstConnectionWin(sf::IpAddress Host);

signals:
    void finished();

public slots:
    int tryConnect();

private:
    QVBoxLayout *m_mainLay;

    QLabel *m_pseudotxt;
    QLineEdit *m_pseudoedit;
    QLabel *m_codetxt;
    QLineEdit *m_codeedit;
    QPushButton *m_validateButton;
    QLabel *m_errortxt;

    sf::IpAddress m_host;
    sf::TcpSocket m_socket;

    bool connected;

};

#endif // FIRSTCONNECTIONWIN_H
