#include "locationsensor.h"

#include <QtQml>

namespace
{
    void initQml()
    {
        qmlRegisterSingletonType< LocationSensor >(
            "harbour.cyclingcomputer.Sensors",
            1, 0,
            "LocationSensor",
            [](QQmlEngine*, QJSEngine*) -> QObject*
            {
                return new LocationSensor();
            }
        );
    }

    Q_CONSTRUCTOR_FUNCTION(initQml)
}

LocationSensor::LocationSensor(QObject* parent)
    : QObject(parent)
{
    m_source = QGeoPositionInfoSource::createDefaultSource(this);

    if (m_source)
    {
        connect(m_source, &QGeoPositionInfoSource::positionUpdated,
                this, &LocationSensor::onPositionUpdated);
        connect(m_source, SIGNAL(error(QGeoPositionInfoSource::Error)),
                this, SLOT(onPositionError(QGeoPositionInfoSource::Error)));
        connect(m_source, &QGeoPositionInfoSource::updateTimeout,
                this, &LocationSensor::onPositionTimeout);

        m_source->setUpdateInterval(1000);
        m_source->startUpdates();        
    }
}

LocationSensor::~LocationSensor()
{
    delete m_source;
}

void LocationSensor::onPositionError(QGeoPositionInfoSource::Error error)
{
    setAvailable(false);
    setReliable(false);
    qDebug() << error;
}

void LocationSensor::onPositionTimeout()
{
    setAvailable(false);
    setReliable(false);

    m_gradientQueue.clear();

    setGradient(0);
    setCoordinate(QGeoCoordinate());

    qDebug() << "position timeout";
}

void LocationSensor::onPositionUpdated(const QGeoPositionInfo& info)
{
    setAvailable(true);
    setReliable(info.attribute(QGeoPositionInfo::HorizontalAccuracy) < 10 &&
                info.attribute(QGeoPositionInfo::VerticalAccuracy) < 10);

    if (reliable())
    {
        while (m_gradientQueue.size() > 15)
            m_gradientQueue.pop_front();

        m_gradientQueue.push_back(info.coordinate());

        qreal gradientDistance = 0;

        if (m_gradientQueue.size() == 15)
            for (int i = 1; i < m_gradientQueue.size(); ++i)
                gradientDistance += m_gradientQueue[i].distanceTo(m_gradientQueue[i - 1]);

        if (gradientDistance > 0)
        {
            qreal meanStartAltitude = 0;
            qreal meanEndAltitude = 0;

            for (int i = 0; i < 5; ++i)
            {
                meanStartAltitude += m_gradientQueue[i].altitude();
                meanEndAltitude += m_gradientQueue[i + 10].altitude();
            }

            meanStartAltitude /= 5;
            meanEndAltitude /= 5;

            setGradient((meanEndAltitude - meanStartAltitude) / gradientDistance);
        }
        else
            setGradient(0);

        if (m_coordinate.isValid())
            setDistance(distance() + info.coordinate().distanceTo(m_coordinate));

        if (info.coordinate().altitude() > m_coordinate.altitude())
            setElevationGain(elevationGain() +
                             info.coordinate().altitude() - m_coordinate.altitude());
    }

    setCoordinate(info.coordinate());
    setSpeed(info.attribute(QGeoPositionInfo::GroundSpeed));

    if (reliable())
    {
        setMaxSpeed(std::max< qreal >(maxSpeed(), m_speed));

        if (m_speed >= 2)
        {
            m_speedMeasurementsSum += m_speed;
            ++m_speedMeasurementsCount;
            setAverageSpeed(m_speedMeasurementsSum / m_speedMeasurementsCount);
        }
    }
}


