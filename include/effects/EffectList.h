#pragma once

#include <QObject>
#include <QMap>
#include <QList>
#include <QSet>
#include <QStringList>
#include "effects/EffectInfo.h"

namespace Lightscape {

class EffectList : public QObject
{
    Q_OBJECT

public:
    static EffectList& getInstance();

    // Effect list management
    void addEffect(const EffectInfo& info);
    void removeEffect(const QString& effectId);
    bool hasEffect(const QString& effectId) const;

    // Effect access
    EffectInfo getEffect(const QString& effectId) const;
    QList<EffectInfo> getEffects() const;
    QList<EffectInfo> getEffectsByCategory(EffectCategory category) const;
    QStringList getCategories() const;

signals:
    void effectAdded(const EffectInfo& info);
    void effectRemoved(const QString& effectId);
    void effectsCleared();

private:
    EffectList() = default;
    ~EffectList() = default;
    
    // Prevent copying
    EffectList(const EffectList&) = delete;
    EffectList& operator=(const EffectList&) = delete;

    QMap<QString, EffectInfo> effects;
};

} // namespace Lightscape