#ifndef FACILITIES_H
#define FACILITIES_H

#include <QObject>
#include <QBluetoothLocalDevice>

class Facility : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)

public:
    Facility(QObject* parent = nullptr)
        : QObject(parent)
    {
    }

    virtual ~Facility()
    {
    }

    bool busy() const { return m_busy; }

signals:
    void busyChanged(bool);

protected:
    void setBusy(bool busy)
    {
        if (m_busy != busy)
            emit busyChanged(m_busy = busy);
    }

private:
    bool m_busy = false;
};

class BluetoothFacility : public Facility
{
    Q_OBJECT

    Q_PROPERTY(bool powered READ powered NOTIFY poweredChanged)

public:
    explicit BluetoothFacility(QObject* parent = nullptr);
    virtual ~BluetoothFacility();

    bool powered() const { return m_powered; }

    Q_INVOKABLE void powerOn();
    Q_INVOKABLE void powerOff();

signals:
    void poweredChanged(bool);

protected:
    void setPowered(bool powered)
    {
        if (m_powered != powered)
            emit poweredChanged(m_powered = powered);
    }

private slots:
    void onError(QBluetoothLocalDevice::Error error);
    void onHostModeStateChanged(QBluetoothLocalDevice::HostMode state);

private:
    bool m_powered = false;

    QBluetoothLocalDevice* m_localDevice = nullptr;
};

class GpsFacility : public Facility
{
    Q_OBJECT

public:
    explicit GpsFacility(QObject* parent = nullptr);
    virtual ~GpsFacility();

};

#endif
