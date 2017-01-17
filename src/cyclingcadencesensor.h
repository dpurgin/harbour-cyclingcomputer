#ifndef CYCLYNGCADENCESENSOR_H
#define CYCLINGCADENCESENSOR_H

#include "blesensor.h"

class CyclingCadenceSensor : public BLESensor
{
    Q_OBJECT

    Q_PROPERTY(int averageValue READ averageValue NOTIFY averageValueChanged)
    Q_PROPERTY(int maxValue READ maxValue NOTIFY maxValueChanged)
    Q_PROPERTY(int value READ value NOTIFY valueChanged)

public:
    CyclingCadenceSensor(QObject* parent = nullptr);
    virtual ~CyclingCadenceSensor();

    int averageValue() const { return m_averageValue; }
    int maxValue() const { return m_maxValue; }
    int value() const { return m_value; }

signals:
    void averageValueChanged(int);
    void maxValueChanged(int);
    void valueChanged(int);

private:
    void processCharacteristicChange(QLowEnergyService* service,
                                     const QLowEnergyCharacteristic& characteristic,
                                     QByteArray value) override;
    void processServices() override;
    void processServiceDetails(QLowEnergyService* service) override;

private:
    void setAverageValue(int averageValue)
    {
        if (m_averageValue != averageValue)
            emit averageValueChanged(m_averageValue = averageValue);
    }

    void setMaxValue(int maxValue)
    {
        if (m_maxValue != maxValue)
            emit maxValueChanged(m_maxValue = maxValue);
    }

    void setValue(int value)
    {
        if (m_value != value)
            emit valueChanged(m_value = value);
    }

private:
    int m_averageValue = 0;
    int m_maxValue = 0;
    int m_value = 0;    

    quint32 m_measurement = 0;
    quint16 m_time = 0;
    qint32 m_hold = 3;

    quint32 m_valuesSum = 0;
    quint32 m_valuesCount = 0;
};

#endif
