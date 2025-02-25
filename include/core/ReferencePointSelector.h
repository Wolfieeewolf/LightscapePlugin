#pragma once

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QMap>
#include <QDataStream>
#include "grid/GridTypes.h"
#include "devices/NonRGBDevice.h"

class ReferencePointSelector : public QWidget {
    Q_OBJECT

public:
    struct ReferencePoint {
        GridPosition position;
        bool isRGBDevice;
        QString name;
        bool enabled;
        QString deviceId;

        ReferencePoint() : isRGBDevice(true), enabled(false) {}
        ReferencePoint(const GridPosition& pos, bool isRGB, const QString& n, const QString& id) 
            : position(pos), isRGBDevice(isRGB), name(n), enabled(false), deviceId(id) {}
    };
    
    friend QDataStream& operator<<(QDataStream& stream, const ReferencePoint& point) {
        return stream << point.position.x << point.position.y << point.position.z 
                     << point.isRGBDevice << point.name << point.enabled << point.deviceId;
    }
    
    friend QDataStream& operator>>(QDataStream& stream, ReferencePoint& point) {
        return stream >> point.position.x >> point.position.y >> point.position.z 
                     >> point.isRGBDevice >> point.name >> point.enabled >> point.deviceId;
    }

    explicit ReferencePointSelector(QWidget* parent = nullptr);
    ~ReferencePointSelector();

    void addReferencePoint(const GridPosition& pos, bool isRGBDevice, const QString& name, const QString& deviceId);
    void removeReferencePoint(const QString& deviceId);
    void clearReferencePoints();
    QList<ReferencePoint> getEnabledReferencePoints() const;
    bool hasEnabledReferencePoints() const;
    void updateReferencePointPosition(const QString& deviceId, const GridPosition& newPos);
    void setReferencePointEnabled(const QString& deviceId, bool enabled);
    QWidget* getWidget() const { return mainWidget; }
    void setTitle(const QString& title);
    QByteArray saveState() const;
    bool restoreState(const QByteArray& state);

signals:
    void referencePointsChanged();
    void referencePointEnabled(const QString& deviceId, bool enabled);
    void selectionChanged(const QString& deviceId);

private slots:
    void onCheckBoxStateChanged(int state);
    void onComboBoxSelectionChanged(int index);
    void updateUI();

private:
    struct ReferencePointUI {
        QComboBox* comboBox;
        QCheckBox* checkBox;
        QString currentId;
    };

    QWidget* mainWidget;
    QVBoxLayout* mainLayout;
    QMap<QString, ReferencePoint> referencePoints;
    QList<ReferencePointUI> uiElements;

    void setupUI();
    void cleanupUI();
    void rebuildUI();
};