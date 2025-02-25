#pragma once

#include <QObject>

class StateManager : public QObject
{
    Q_OBJECT

public:
    enum class PluginState {
        Uninitialized,
        Initializing,
        Running,
        Error,
        Disabled
    };

    explicit StateManager(QObject* parent = nullptr);
    
    bool isEnabled() const;
    PluginState getCurrentState() const;
    QString getStateString() const;
    bool canTransitionTo(PluginState newState) const;
    
    void setEnabled(bool enabled);
    bool setState(PluginState newState);
    void setError(const QString& error);

signals:
    void stateChanged(PluginState newState);
    void enabledChanged(bool enabled);
    void errorOccurred(const QString& error);

private:
    bool validateStateTransition(PluginState newState) const;
    QString stateToString(PluginState state) const;

    PluginState current_state;
    bool enabled;
    QString last_error;
};