#include "BackSide.h"
#include <QString>
#include <QBrush>
#include <QImage>
#include <QObject>

#include <iostream>

BackSide::BackSide(int nb_image, std::string filesDirectory, QWidget *cible) :
    m_nbImage(nb_image), m_directory(filesDirectory), m_cible(cible), m_index(0)
{
    int i = 0;
    QPalette fond;
    for (i = 0; i < m_nbImage; i++){
        fond.setBrush(m_cible->backgroundRole(),QBrush(QImage(QString::fromStdString(m_directory)+"/PLAN"+QString::number(i+1)+".jpg")));
        m_fonds.push_back(fond);
    }

    m_cible->setPalette(m_fonds[m_index]);
   //QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(changePlan()));


}

void BackSide::startTimer(int interval){

   m_timer->start(interval);

}

void BackSide::changePlan(){
    m_index++;
    m_cible->setPalette(m_fonds[m_index]);
}



