#ifndef BACKSIDE_H
#define BACKSIDE_H

#include <QWidget>
#include <QTimer>
#include <string>
#include <QPalette>

class BackSide : public QObject
{
    Q_OBJECT

public:
    BackSide(int nb_image, std::string filesDirectory, QWidget *cible);
    void startTimer(int interval);

public slots:
    void changePlan();

private:
    int m_nbImage;
    std::string m_directory;
    std::vector<QPalette> m_fonds;

    int m_index;
    QTimer *m_timer;
    QWidget *m_cible;

};

#endif // BACKSIDE_H
