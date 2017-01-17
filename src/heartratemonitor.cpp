#include "heartratemonitor.h"

#include <QtQml>

namespace
{
    void initQml()
    {
        qmlRegisterSingletonType< HeartRateMonitor >(
            "harbour.cyclingcomputer.Sensors",
            1, 0,
            "HeartRateMonitor",
            [](QQmlEngine*, QJSEngine*) -> QObject*
            {
                return new HeartRateMonitor();
            }
        );
    }

    Q_CONSTRUCTOR_FUNCTION(initQml)
}

HeartRateMonitor::HeartRateMonitor(QObject* parent)
    : BLESensor(parent)
{
}

HeartRateMonitor::~HeartRateMonitor()
{
}

void HeartRateMonitor::processCharacteristicChange(QLowEnergyService* service,
                                                   const QLowEnergyCharacteristic& characteristic,
                                                   QByteArray value)
{
    BLESensor::processCharacteristicChange(service, characteristic, value);

    if (characteristic.uuid() == QBluetoothUuid(QBluetoothUuid::HeartRateMeasurement))
    {
        quint8 flags = value[0];
        quint16 bpm = 0;

        if ((flags & 0x01) == 0) // 8 bit measurement
        {
            bpm = value[1];
        }
        else // 16-bit measurement
        {
            memcpy(&bpm, value.mid(1, 2).data(), sizeof(bpm));

            bpm = qFromLittleEndian(bpm);
        }

        setAttached((flags & 0x06) != 0x04);
        setMaxValue(std::max< int >(maxValue(), bpm));
        setValue(bpm);

        if (bpm > 0)
        {
            m_measurementsSum += bpm;
            ++m_measurementsCount;

            setAverageValue(m_measurementsSum / m_measurementsCount);
        }
    }
}

void HeartRateMonitor::processServiceDetails(QLowEnergyService* service)
{
    BLESensor::processServiceDetails(service);

    if (service->serviceUuid() == QBluetoothUuid(QBluetoothUuid::HeartRate))
    {
        auto characteristic = service->characteristic(QBluetoothUuid::HeartRateMeasurement);

        qDebug() << "HRM char props: " << characteristic.properties();

        if (characteristic.properties().testFlag(QLowEnergyCharacteristic::Notify))
        {
            auto descriptor = characteristic.descriptor(
                        QBluetoothUuid(QBluetoothUuid::ClientCharacteristicConfiguration));

            service->writeDescriptor(descriptor, QByteArray::fromHex("0100"));
        }
    }
}

void HeartRateMonitor::processServices()
{
    BLESensor::processServices();

    discoverServiceDetails(QBluetoothUuid::HeartRate);
}
