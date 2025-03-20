/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| EffectTabHeader.h                                         |
|                                                           |
| Header that appears in each effect tab with controls      |
\*---------------------------------------------------------*/

#pragma once

#include <QWidget>

namespace Lightscape {
    class BaseEffect;
}

namespace Ui {
class EffectTabHeader;
}

class EffectTabHeader : public QWidget
{
    Q_OBJECT
    
public:
    explicit EffectTabHeader(QWidget *parent = nullptr, Lightscape::BaseEffect* effect = nullptr);
    ~EffectTabHeader();
    
    void setEffectName(const QString& name);
    QString getEffectName() const;
    
    void toggleRunningIndicator(bool running);
    void setEffect(Lightscape::BaseEffect* effect);
    Lightscape::BaseEffect* getEffect() const;
    
signals:
    void closeRequested();
    void startStopRequested(bool start);
    void renamed(const QString& newName);
    
private slots:
    void onCloseClicked();
    void onStartStopClicked(bool checked);
    void onRenameClicked();
    
private:
    Ui::EffectTabHeader *ui;
    Lightscape::BaseEffect* _effect;
    QString _effectName;
    bool _isRunning = false;
};
