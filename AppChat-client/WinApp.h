#ifndef WINAPP_H
#define WINAPP_H

#include <QObject>
#include <SFML/Network.hpp>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QKeyEvent>
#include <QString>
#include <QThread>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QDialog>

#include <string>

#include "ThreadManager.h"
#include "DataType.h"

class WinApp : public QWidget
{
    Q_OBJECT

public:
    WinApp(sf::IpAddress Host);
    ~WinApp();

public slots:
    void receiveData(QString);
    void showGui();
    bool  connectToHost();
    void changeHost_GUI();
    void changeHost();

private:

    sf::TcpSocket m_socket;
    sf::IpAddress m_host;

    QPushButton *m_configButton;
    QDialog *m_dialogChangeIp;
    bool dialogIsOpen;
    QLineEdit *m_changeIpEdit;

    QFormLayout *m_connectLay;
    QLineEdit *m_editpseudo;
    QLineEdit *m_editcode;

    QPushButton *m_pseudoButton;

    QHBoxLayout *m_errorCLay;
    QLabel *m_errorConnection;
    QPushButton *m_retryConnection;
    QString m_pseudo;

    QVBoxLayout *m_mainLay;
    QTextEdit *m_textRMessage;

    QFormLayout *m_enterMLay;
    QLineEdit *m_enterMessage;

    bool updateMessage;
    QString m_txtContenue;
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    void tryUpdateMessage();
    void sendMessage(const QString &text);

    ThreadManager *m_ThreadManager;

    long long unsigned int p;
    long long unsigned int q;
    long long unsigned int n;
    long long unsigned int e;
    long long unsigned int d;

    DataType type;
};

#endif // WINAPP_H
