#ifndef FIRSTCONNECTIONWIN_H
#define FIRSTCONNECTIONWIN_H

#include <QWidget>

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>

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
    void changeHost_GUI();
    void changeHost();
    void changePlan();

private:
    QVBoxLayout *m_mainLay;

    QPushButton *m_configButton;
    QDialog *m_dialogChangeIp;
    QLineEdit *m_changeIpEdit;

    bool dialogIsOpen;

    QLabel *m_pseudotxt;
    QLineEdit *m_pseudoedit;
    QLabel *m_codetxt;
    QLineEdit *m_codeedit;
    QPushButton *m_validateButton;
    QLabel *m_errortxt;

    sf::IpAddress m_host;
    sf::TcpSocket m_socket;

    int m_indexPlan;
    std::vector<QPalette> m_fonds;

    bool connected;

};

#endif // FIRSTCONNECTIONWIN_H
