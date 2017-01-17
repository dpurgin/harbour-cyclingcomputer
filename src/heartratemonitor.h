#ifndef HEARTRATEMONITOR_H
#define HEARTRATEMONITOR_H

#include "blesensor.h"

class HeartRateMonitor : public BLESensor
{
    Q_OBJECT

    Q_PROPERTY(bool attached READ attached NOTIFY attachedChanged)
    Q_PROPERTY(int value READ value NOTIFY valueChanged)
    Q_PROPERTY(int averageValue READ averageValue NOTIFY averageValueChanged)
    Q_PROPERTY(int maxValue READ maxValue NOTIFY maxValueChanged)

public:
    HeartRateMonitor(QObject* parent = nullptr);
    virtual ~HeartRateMonitor();

    bool attached() const { return m_attached; }
    int averageValue() const { return m_averageValue; }
    int maxValue() const { return m_maxValue; }
    int value() const { return m_value; }

signals:
    void attachedChanged(bool);
    void averageValueChanged(int);
    void maxValueChanged(int);
    void valueChanged(int);

private:
    void processCharacteristicChange(QLowEnergyService* service,
                                     const QLowEnergyCharacteristic& characteristic,
                                     QByteArray value) override;
    void processServices() override;
    void processServiceDetails(QLowEnergyService* service) override;

    void setAttached(bool attached)
    {
        if (m_attached != attached)
            emit attachedChanged(m_attached = attached);
    }

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
    bool m_attached = false;
    int m_averageValue = 0;
    int m_maxValue = 0;
    int m_value = 0;

    int m_measurementsSum = 0;
    int m_measurementsCount = 0;
};

#endif
