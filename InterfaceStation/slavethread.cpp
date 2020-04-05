#include "slavethread.h"
#include <QSerialPort>
#include <QTime>

SlaveThread::SlaveThread(QObject *parent)
{

}

SlaveThread::~SlaveThread()
{

}


void SlaveThread::startSlave(const QString &portName, int waitTimeout, const QString &response)
{
    const QMutexLocker locker(&m_mutex);
    m_portName = portName;
    m_waitTimeout = waitTimeout;
    m_response = response;
    if (!isRunning())
        start();
}

void SlaveThread::run()
{
    bool currentPortNameChanged = false;
    QByteArray requestData;
    QString request;
    m_mutex.lock();
    QString currentPortName;
    if (currentPortName != m_portName) {
        currentPortName = m_portName;
        currentPortNameChanged = true;
    }

    int currentWaitTimeout = m_waitTimeout;
    QString currentRespone = m_response;
    m_mutex.unlock();

    QSerialPort serial;

    while (!m_quit) {
        if (currentPortNameChanged) {
            serial.close();
            serial.setPortName(currentPortName);

            if (!serial.open(QIODevice::ReadWrite)) {
                emit error(tr("Can't open %1, error code %2")
                           .arg(m_portName).arg(serial.error()));
                return;
            }
        }

        if (serial.waitForReadyRead(currentWaitTimeout)) {
            // read request
            requestData = serial.readAll();
            while (serial.waitForReadyRead(10))
                requestData += serial.readAll();
            printf(requestData);
        } else {
            emit timeout(tr("Wait read request timeout %1").arg(QTime::currentTime().toString()));
        }
        // write response
        const QByteArray responseData = currentRespone.toUtf8();
        serial.write(responseData);
        if (serial.waitForBytesWritten(m_waitTimeout)) {
            request = QString::fromUtf8(requestData);
            emit this->request(request);
        } else {
            emit timeout(tr("Wait write response timeout %1").arg(QTime::currentTime().toString()));
        }
            emit this->request(request);
        m_mutex.lock();
        if (currentPortName != m_portName) {
            currentPortName = m_portName;
            currentPortNameChanged = true;
        } else {
            currentPortNameChanged = false;
        }
        currentWaitTimeout = m_waitTimeout;
        currentRespone = m_response;
        m_mutex.unlock();
    }
}



