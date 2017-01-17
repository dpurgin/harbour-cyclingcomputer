#include "blesensor.h"

#include <QStringBuilder>

QString _write_bytes(void* ptr, int length)
{
    QStringList repr;

    QStringList binaryRepr;
    QString printRepr;

    for (int i = 0; i < length; ++i)
    {
        char ch = reinterpret_cast< char* >(ptr)[i];

        binaryRepr.push_back(QString("%1").arg(ch, 2, 16, QLatin1Char('0')));
        printRepr.push_back(QChar::isPrint(ch)? QChar(ch): '.');

        if ((i + 1) % 8 == 0)
        {
            repr.push_back(binaryRepr.join(QLatin1Char(' ')) % QString("    ") % printRepr);

            binaryRepr.clear();
            printRepr.clear();
        }
    }

    if (binaryRepr.size() > 0)
    {
        while (binaryRepr.size() < 8)
            binaryRepr.push_back("  ");

        repr.push_back(binaryRepr.join(QLatin1Char(' ')) % QString("    ") % printRepr);
    }

    QString data = repr.join(QChar('\n'));

    if (repr.size() > 1)
        data.prepend(QLatin1Char('\n'));

    return data;
}

BLESensor::BLESensor(QObject* parent)
    : QObject(parent)
{
}

BLESensor::~BLESensor()
{
    deleteController();
}

void BLESensor::connectSensor()
{
    qDebug();

    Q_ASSERT(m_controller != nullptr);

    setConnectionState(ConnectionState::Connecting);
    m_controller->connectToDevice();

}

void BLESensor::createController()
{
    deleteController();

    m_controller = new QLowEnergyController(QBluetoothAddress(m_address));
    m_controller->setRemoteAddressType(QLowEnergyController::RandomAddress);

    connect(m_controller, &QLowEnergyController::connected,
            this, &BLESensor::onSensorConnected);
    connect(m_controller, &QLowEnergyController::disconnected,
            this, &BLESensor::onSensorDisconnected);
    connect(m_controller, SIGNAL(error(QLowEnergyController::Error)),
            this, SLOT(onSensorError(QLowEnergyController::Error)));
    connect(m_controller, &QLowEnergyController::discoveryFinished,
            this, &BLESensor::onDiscoveryFinished);
}

void BLESensor::deleteController()
{
    if (m_controller)
    {
        if (m_controller->state() == QLowEnergyController::ConnectedState)
        {
            connect(m_controller, &QLowEnergyController::disconnected,
                    m_controller, &QLowEnergyController::deleteLater);

            m_controller->disconnectFromDevice();
        }
        else
        {
            delete m_controller;
        }
    }
}

void BLESensor::disconnectSensor()
{
    qDebug();

    Q_ASSERT(m_controller != nullptr);

    setConnectionState(ConnectionState::Disconnecting);
    m_controller->disconnectFromDevice();
}

void BLESensor::discoverServiceDetails(QBluetoothUuid::ServiceClassUuid serviceClassUuid)
{
    QBluetoothUuid serviceUuid(serviceClassUuid);

    if (m_controller->services().contains(serviceUuid))
    {
        auto* service = m_controller->createServiceObject(serviceUuid);

        connect(service, &QLowEnergyService::characteristicChanged,
                this, &BLESensor::onCharacteristicChanged);
        connect(service, &QLowEnergyService::descriptorWritten,
                this, &BLESensor::onDescriptorWritten);
        connect(service, &QLowEnergyService::stateChanged,
                this, &BLESensor::onServiceStateChanged);

        service->discoverDetails();

        m_services.push_back(service);
    }
}

void BLESensor::onCharacteristicChanged(const QLowEnergyCharacteristic& characteristic,
                                        QByteArray value)
{
    qDebug() << characteristic.name() << _write_bytes(value.data(), value.size());

    processCharacteristicChange(qobject_cast< QLowEnergyService* >(sender()),
                                characteristic,
                                value);
}

void BLESensor::onDescriptorWritten(const QLowEnergyDescriptor& d,
                                    const QByteArray&)
{
    qDebug() << d.name();
}

void BLESensor::onDiscoveryFinished()
{
    setConnectionState(ConnectionState::Connected);

    qDebug() << "Connected and services discovered on " << m_controller->remoteAddress();

    processServices();
}

void BLESensor::onSensorConnected()
{
    qDebug() << "Discovering services on " << m_controller->remoteAddress();

    m_controller->discoverServices();
}

void BLESensor::onSensorDisconnected()
{
    setConnectionState(ConnectionState::Disconnected);
    qDebug() << "Disconnected from " << m_controller->remoteAddress();

    if (reconnect())
        connectSensor();
}

void BLESensor::onSensorError(QLowEnergyController::Error error)
{
    qDebug() << "Controller error: " << error << m_controller->errorString();

    if (m_controller->state() == QLowEnergyController::ConnectedState)
        setConnectionState(ConnectionState::Connected);
    else
        setConnectionState(ConnectionState::Disconnected);
}

void BLESensor::onServiceStateChanged(QLowEnergyService::ServiceState state)
{
    if (state != QLowEnergyService::ServiceDiscovered)
        return;

    auto* service = qobject_cast< QLowEnergyService* >(sender());

    if (service)
        processServiceDetails(service);
}

void BLESensor::processCharacteristicChange(QLowEnergyService*,
                                            const QLowEnergyCharacteristic& characteristic,
                                            QByteArray value)
{
    if (characteristic.uuid() == QBluetoothUuid::BatteryLevel)
    {
        setBatterySupported(true);
        setBatteryLevel(static_cast< int >(value[0]));
    }
}

void BLESensor::processServiceDetails(QLowEnergyService *service)
{
    if (service->serviceUuid() == QBluetoothUuid::GenericAccess)
    {
        auto characteristic = service->characteristic(QBluetoothUuid::DeviceName);

        if (characteristic.isValid())
            setName(QString::fromUtf8(characteristic.value()));
    }
    else if (service->serviceUuid() == QBluetoothUuid::BatteryService)
    {
        auto characteristic = service->characteristic(QBluetoothUuid::BatteryLevel);

        qDebug() << "Battery level properties: " << characteristic.properties();

        setBatterySupported(true);

        if (characteristic.properties().testFlag(QLowEnergyCharacteristic::Read))
            setBatteryLevel(static_cast< int >(characteristic.value()[0]));

        if (characteristic.properties().testFlag(QLowEnergyCharacteristic::Notify))
        {
            service->writeDescriptor(
                characteristic.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration),
                QByteArray::fromHex("0100"));
        }

    }
}

void BLESensor::processServices()
{
    discoverServiceDetails(QBluetoothUuid::GenericAccess);
    discoverServiceDetails(QBluetoothUuid::BatteryService);
}
