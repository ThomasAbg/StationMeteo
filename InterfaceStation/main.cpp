#include "mainwindow.h"
#include <QApplication>
#include <QSerialPort>
#include <QDebug>
#include "slavethread.h"


#define PORT "/dev/ttyUSB0"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
/*
    QSerialPort *port;

    // instanciation du port
    port = new QSerialPort(QLatin1String(PORT));

    // TODO : paramètrer le port (débit, ...)

    // ouverture du port
    port->open(QIODevice::ReadWrite);
    qDebug("<debug> etat ouverture port : %d", port->isOpen());

    // TODO : réceptionner et/ou envoyer des données

    // fermeture du port
    port->close();
    qDebug("<debug> etat ouverture port : %d", port->isOpen());

    delete port;
*/
    SlaveThread *comm = new SlaveThread();
    comm->startSlave(PORT,10000,"coucou");

    MainWindow w;
    w.show();

    return a.exec();
}
