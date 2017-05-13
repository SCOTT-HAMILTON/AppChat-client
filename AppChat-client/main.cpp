#include <QApplication>
#include "Manager.h"

using namespace sf;

int main (int argc, char *argv[]){

    QApplication app(argc, argv);

    Manager manager;
    manager.manage();

    app.exec();

    return 0;
}
