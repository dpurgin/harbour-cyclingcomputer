#include "facilities.h"

#include <QDebug>
#include <QtQml>

#include <QGeoPositionInfoSource>

namespace
{
    void initQml()
    {
        qmlRegisterSingletonType< BluetoothFacility >(
            "harbour.cyclingcomputer.Facilities",
            1, 0,
            "Bluetooth",
            [](QQmlEngine*, QJSEngine*) -> QObject*
            {
                return new BluetoothFacility();
            });
    }

    Q_CONSTRUCTOR_FUNCTION(initQml)
}

BluetoothFacility::BluetoothFacility(QObject* parent)
    : Facility(parent)
{
    m_localDevice = new QBluetoothLocalDevice;

    if (m_localDevice->isValid())
    {
        connect(m_localDevice, &QBluetoothLocalDevice::hostModeStateChanged,
                this, &BluetoothFacility::onHostModeStateChanged);
        connect(m_localDevice, &QBluetoothLocalDevice::error,
                this, &BluetoothFacility::onError);

        setPowered(m_localDevice->hostMode() != QBluetoothLocalDevice::HostPoweredOff);
    }
}

BluetoothFacility::~BluetoothFacility()
{
    delete m_localDevice;
}

void BluetoothFacility::powerOff()
{
    if (powered())
    {
        setBusy(true);
        m_localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    }
}

void BluetoothFacility::powerOn()
{
    if (!powered())
    {
        setBusy(true);
        m_localDevice->powerOn();
    }
}

void BluetoothFacility::onError(QBluetoothLocalDevice::Error error)
{
    qDebug() << error;

    setBusy(false);
}

void BluetoothFacility::onHostModeStateChanged(QBluetoothLocalDevice::HostMode state)
{
    qDebug() << state;

    setBusy(false);
    setPowered(state != QBluetoothLocalDevice::HostPoweredOff);
}

GpsFacility::GpsFacility(QObject *parent)
    : Facility(parent)
{
    qDebug() << QGeoPositionInfoSource::availableSources();

    QScopedPointer< QGeoPositionInfoSource > source(QGeoPositionInfoSource::createDefaultSource(this));
    qDebug() << source->supportedPositioningMethods();
}

GpsFacility::~GpsFacility()
{

}


