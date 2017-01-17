#ifndef LOCATIONSENSOR_H
#define LOCATIONSENSOR_H

#include <QObject>

#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>

class LocationSensor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qreal altitude READ altitude NOTIFY altitudeChanged)
    Q_PROPERTY(bool available READ available NOTIFY availableChanged)
    Q_PROPERTY(qreal averageSpeed READ averageSpeed NOTIFY averageSpeedChanged)
    Q_PROPERTY(qreal distance READ distance NOTIFY distanceChanged)
    Q_PROPERTY(qreal elevationGain READ elevationGain NOTIFY elevationGainChanged)
    Q_PROPERTY(qreal gradient READ gradient NOTIFY gradientChanged)
    Q_PROPERTY(qreal longitude READ longitude NOTIFY longitudeChanged)
    Q_PROPERTY(qreal latitude READ latitude NOTIFY latitudeChanged)
    Q_PROPERTY(qreal maxSpeed READ maxSpeed NOTIFY maxSpeedChanged)
    Q_PROPERTY(bool reliable READ reliable NOTIFY reliableChanged)
    Q_PROPERTY(qreal speed READ speed NOTIFY speedChanged)

public:
    LocationSensor(QObject* parent = nullptr);
    virtual ~LocationSensor();

    qreal altitude() const { return m_coordinate.altitude(); }
    bool available() const { return m_available; }
    qreal averageSpeed() const { return m_averageSpeed; }
    qreal distance() const { return m_distance; }
    qreal elevationGain() const { return m_elevationGain; }
    qreal gradient() const { return m_gradient; }
    qreal latitude() const { return m_coordinate.latitude(); }
    qreal longitude() const { return m_coordinate.longitude(); }
    qreal maxSpeed() const { return m_maxSpeed; }
    bool reliable() const { return m_reliable; }
    qreal speed() const { return m_speed; }

signals:
    void altitudeChanged(qreal);
    void availableChanged(bool);
    void averageSpeedChanged(qreal);
    void distanceChanged(qreal);
    void elevationGainChanged(qreal);
    void gradientChanged(qreal);
    void latitudeChanged(qreal);
    void longitudeChanged(qreal);
    void maxSpeedChanged(qreal);
    void reliableChanged(bool);
    void speedChanged(qreal);

private:
    void setAvailable(bool available)
    {
        if (m_available != available)
            emit availableChanged(m_available = available);
    }

    void setAverageSpeed(qreal averageSpeed)
    {
        if (m_averageSpeed != averageSpeed)
            emit averageSpeedChanged(m_averageSpeed = averageSpeed);
    }

    void setCoordinate(const QGeoCoordinate& coordinate)
    {
        bool lonChanged, latChanged, altChanged;

        lonChanged = coordinate.longitude() != m_coordinate.longitude();
        latChanged = coordinate.latitude() != m_coordinate.latitude();
        altChanged = coordinate.altitude() != m_coordinate.altitude();

        m_coordinate = coordinate;

        if (lonChanged)
            emit longitudeChanged(m_coordinate.longitude());

        if (latChanged)
            emit latitudeChanged(m_coordinate.latitude());

        if (altChanged)
            emit altitudeChanged(m_coordinate.altitude());
    }

    void setDistance(qreal distance)
    {
        if (m_distance != distance)
            emit distanceChanged(m_distance = distance);
    }

    void setElevationGain(qreal elevationGain)
    {
        if (m_elevationGain != elevationGain)
            emit elevationGainChanged(m_elevationGain = elevationGain);
    }

    void setGradient(qreal gradient)
    {
        if (m_gradient != gradient)
            emit gradientChanged(m_gradient = gradient);
    }

    void setMaxSpeed(qreal maxSpeed)
    {
        if (m_maxSpeed != maxSpeed)
            emit maxSpeedChanged(m_maxSpeed = maxSpeed);
    }

    void setReliable(bool reliable)
    {
        if (m_reliable != reliable)
            emit reliableChanged(m_reliable = reliable);
    }

    void setSpeed(qreal speed)
    {
        if (m_speed != speed)
            emit speedChanged(m_speed = speed);
    }

private slots:
    void onPositionUpdated(const QGeoPositionInfo& info);
    void onPositionError(QGeoPositionInfoSource::Error);
    void onPositionTimeout();

private:
    bool m_available = false;
    qreal m_averageSpeed = 0;
    qreal m_distance = 0;
    qreal m_elevationGain = 0;
    qreal m_gradient = 0;
    qreal m_maxSpeed = 0;
    bool m_reliable = false;
    qreal m_speed = 0;

    QGeoCoordinate m_coordinate;
    qreal m_speedMeasurementsSum = 0;
    int m_speedMeasurementsCount = 0;

    QList< QGeoCoordinate > m_gradientQueue;

    QGeoPositionInfoSource* m_source = nullptr;    
};

#endif
