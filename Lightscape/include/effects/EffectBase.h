#pragma once
#include <QString>
#include <QColor>
#include <QVariantMap>
#include <QVector>

class EffectBase {
public:
    virtual ~EffectBase() = default;
    virtual void update(qint64 deltaTime) = 0;
    virtual QVector<QColor> getColors() = 0;
    
    QString getName() const { return name; }
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }

protected:
    QString name;
    bool active = false;
    QVariantMap parameters;
};