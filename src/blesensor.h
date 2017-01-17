#ifndef BLESENSOR_H
#define BLESENSOR_H

#include <QLowEnergyController>

class BLESensor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(bool reconnect READ reconnect WRITE setReconnect NOTIFY reconnectChanged)

    Q_PROPERTY(bool batterySupported READ batterySupported NOTIFY batterySupportedChanged)
    Q_PROPERTY(int batteryLevel READ batteryLevel NOTIFY batteryLevelChanged)

public:
    enum class ConnectionState
    {
        Disconnected,
        Connecting,
        Disconnecting,
        Connected
    };

public:
    BLESensor(QObject* parent = nullptr);
    virtual ~BLESensor();

    QString address() const { return m_address; }
    void setAddress(QString address)
    {
        if (m_address != address)
        {
            emit addressChanged(m_address = address);

            createController();

            if (connectionState() == ConnectionState::Connected)
                disconnectSensor();
        }
    }

    int batteryLevel() const { return m_batteryLevel; }
    bool batterySupported() const { return m_batterySupported; }

    bool connected() const { return connectionState() == ConnectionState::Connected; }
    ConnectionState connectionState() const { return m_connectionState; }
    QString name() const { return m_name; }

    bool reconnect() const { return m_reconnect; }
    void setReconnect(bool reconnect)
    {
        if (m_reconnect != reconnect)
            emit reconnectChanged(m_reconnect = reconnect);
    }

public slots:
    void connectSensor();
    void disconnectSensor();

signals:
    void addressChanged(QString);
    void batteryLevelChanged(int);
    void batterySupportedChanged(bool);
    void connectedChanged(bool);
    void connectionStateChanged(ConnectionState);
    void nameChanged(QString);
    void reconnectChanged(bool);

private slots:
    void onCharacteristicChanged(const QLowEnergyCharacteristic& characteristic, QByteArray value);
    void onDescriptorWritten(const QLowEnergyDescriptor& descriptor, const QByteArray& value);
    void onDiscoveryFinished();
    void onSensorConnected();
    void onSensorDisconnected();
    void onSensorError(QLowEnergyController::Error);
    void onServiceStateChanged(QLowEnergyService::ServiceState state);

    void setBatteryLevel(int batteryLevel)
    {
        if (m_batteryLevel != batteryLevel)
            emit batteryLevelChanged(m_batteryLevel = batteryLevel);
    }

    void setBatterySupported(bool batterySupported)
    {
        if (m_batterySupported != batterySupported)
            emit batterySupportedChanged(m_batterySupported = batterySupported);
    }

    void setConnectionState(ConnectionState state)
    {
        if (m_connectionState != state)
        {
            bool _connected = connected();

            emit connectionStateChanged(m_connectionState = state);

            if (_connected != connected())
                emit connectedChanged(connected());
        }
    }

    void setName(QString name)
    {
        if (m_name != name)
            emit nameChanged(m_name = name);
    }

protected:
    void discoverServiceDetails(QBluetoothUuid::ServiceClassUuid serviceUuid);

    virtual void processCharacteristicChange(QLowEnergyService*,
                                             const QLowEnergyCharacteristic&,
                                             QByteArray value);
    virtual void processServices();
    virtual void processServiceDetails(QLowEnergyService* service);

private:
    void createController();
    void deleteController();

private:
    QLowEnergyController* m_controller = nullptr;

    QList< QLowEnergyService* > m_services;

    QString m_address;
    ConnectionState m_connectionState = ConnectionState::Disconnected;
    QString m_name;
    bool m_reconnect = true;

    bool m_batterySupported = false;
    int m_batteryLevel = 0;
};

#endif
