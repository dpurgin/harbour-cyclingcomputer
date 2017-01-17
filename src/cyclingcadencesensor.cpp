#include "cyclingcadencesensor.h"

#include <QtQml>

#include <limits>

namespace
{
    void initQml()
    {
        qmlRegisterSingletonType< CyclingCadenceSensor >(
            "harbour.cyclingcomputer.Sensors",
            1, 0,
            "CyclingCadenceSensor",
            [](QQmlEngine*, QJSEngine*) -> QObject*
            {
                return new CyclingCadenceSensor();
            }
        );
    }

    Q_CONSTRUCTOR_FUNCTION(initQml)
}

CyclingCadenceSensor::CyclingCadenceSensor(QObject* parent)
    : BLESensor(parent)
{
//    for (int i = 0; i < 6; ++i)
//        m_measurements.push_back(Measurement(QDateTime::currentMSecsSinceEpoch(), 0));
}

CyclingCadenceSensor::~CyclingCadenceSensor()
{
}

void CyclingCadenceSensor::processCharacteristicChange(QLowEnergyService* service,
                                                   const QLowEnergyCharacteristic& characteristic,
                                                   QByteArray value)
{
    BLESensor::processCharacteristicChange(service, characteristic, value);

    if (characteristic.uuid() == QBluetoothUuid(QBluetoothUuid::CSCMeasurement))
    {
        quint8 flags = value[0];
        quint32 wheelRevolutions = 0;
        quint16 crankRevolutions = 0;
        quint16 lastEventTime = 0;

        if (flags & 0x01) // wheel revolutions
        {
            // no idea how to process this

            memcpy(&wheelRevolutions, value.mid(1, 4).data(), sizeof(wheelRevolutions));
            memcpy(&lastEventTime, value.mid(5, 2).data(), sizeof(lastEventTime));

            wheelRevolutions = qFromLittleEndian(wheelRevolutions);
        }
        else if (flags & 0x02) // crank revolutions
        {
            memcpy(&crankRevolutions, value.mid(1, 2).data(), sizeof(crankRevolutions));
            memcpy(&lastEventTime, value.mid(3, 2).data(), sizeof(lastEventTime));

            crankRevolutions = qFromLittleEndian(crankRevolutions);
        }

        lastEventTime = qFromLittleEndian(lastEventTime);

        qDebug() << "Wheel rev:" << wheelRevolutions;
        qDebug() << "Crank rev:" << crankRevolutions;
        qDebug() << "Last measurement:" << lastEventTime;

        quint32 measurement = std::max< quint32 >(wheelRevolutions, crankRevolutions);

        qint32 revDiff = measurement - m_measurement;
        qint32 timeDiff = 0;

        // overflow in sensor
        if (lastEventTime < m_time)
            timeDiff = (std::numeric_limits< quint16 >::max() - m_time) + lastEventTime;
        else
            timeDiff = lastEventTime - m_time;

        if (timeDiff > 0)
        {
            setValue(revDiff * (60000.0 / timeDiff));
            m_hold = 3;
        }
        else if (--m_hold <= 0)
            setValue(0);

        if (timeDiff > 0 || m_hold == 0)
        {
            m_measurement = measurement;
            m_time = lastEventTime;
        }

        if (m_value > 0)
        {
            m_valuesSum += m_value;
            ++m_valuesCount;
            setAverageValue(m_valuesSum / m_valuesCount);
        }

        setMaxValue(std::max< int >(m_maxValue, m_value));
    }
}

void CyclingCadenceSensor::processServiceDetails(QLowEnergyService* service)
{
    BLESensor::processServiceDetails(service);

    if (service->serviceUuid() == QBluetoothUuid(QBluetoothUuid::CyclingSpeedAndCadence))
    {
        auto scControlPoint = service->characteristic(QBluetoothUuid::SCControlPoint);

        if (scControlPoint.isValid())
        {
            qDebug() << "SC Control point props: " << scControlPoint.properties();

            auto descriptor = scControlPoint.descriptor(
                        QBluetoothUuid::ClientCharacteristicConfiguration);
            service->writeDescriptor(descriptor, QByteArray::fromHex("0200"));
        }


        auto measurement = service->characteristic(QBluetoothUuid::CSCMeasurement);

        qDebug() << "Measurement props: " << measurement.properties();

        if (measurement.properties().testFlag(QLowEnergyCharacteristic::Notify))
        {
            auto descriptor = measurement.descriptor(
                        QBluetoothUuid(QBluetoothUuid::ClientCharacteristicConfiguration));

            service->writeDescriptor(descriptor, QByteArray::fromHex("0100"));
        }
    }
}

void CyclingCadenceSensor::processServices()
{
    BLESensor::processServices();

    discoverServiceDetails(QBluetoothUuid::CyclingSpeedAndCadence);
}
