#ifndef DATAWRITER_H
#define DATAWRITER_H

#include <QObject>
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>

class DataWriter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)

public:
    DataWriter(QObject* parent = nullptr)
        : QObject(parent)
    {
        QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

        if (!dataDir.exists())
            dataDir.mkpath(dataDir.absolutePath());

        QDateTime dt = QDateTime::currentDateTime();

        m_fileName = dataDir.absoluteFilePath("output-%1.txt").arg(dt.toString(Qt::ISODate));

        QFile f(m_fileName);
        f.open(QFile::WriteOnly);
        f.close();
    }

    Q_INVOKABLE void write(qreal lon,
                           qreal lat,
                           qreal alt,
                           qreal spd,
                           qreal avgspd,
                           qreal dist,
                           qreal gain,
                           qreal grad,
                           int hrm,
                           int avghrm,
                           int cad,
                           int avgcad)
    {
        if (!m_active)
            return;

        QFile f(m_fileName);

        if (f.open( QFile::WriteOnly | QFile::Text | QFile::Append))
        {
            QStringList entries;

            entries << QString("DT=%1").arg(QDateTime::currentMSecsSinceEpoch())
                    << QString("LON=%1").arg(lon, 0, 'f', 10)
                    << QString("LAT=%1").arg(lat, 0, 'f', 10)
                    << QString("ALT=%1").arg(alt)
                    << QString("SPD=%1").arg(spd)
                    << QString("AVGSPD=%1").arg(avgspd)
                    << QString("DIST=%1").arg(dist)
                    << QString("GAIN=%1").arg(gain)
                    << QString("GRAD=%1").arg(grad)
                    << QString("HRM=%1").arg(hrm)
                    << QString("AVGHRM=%1").arg(avghrm)
                    << QString("CAD=%1").arg(cad)
                    << QString("AVGCAD=%1").arg(avgcad);


            QString str = entries.join(';') + '\n';

            f.write(str.toUtf8());
            f.close();
        }
    }

    bool active() const { return m_active; }
    void setActive(bool active)
    {
        if (m_active != active)
            emit activeChanged(m_active = active);
    }

signals:
    void activeChanged(bool);

private:
    QString m_fileName;
    bool m_active = true;
};

#endif
