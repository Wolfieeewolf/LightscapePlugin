#include "core/StateManager.h"

StateManager::StateManager(QObject* parent)
    : QObject(parent)
    , current_state(PluginState::Uninitialized)
    , enabled(false)
{
}

bool StateManager::isEnabled() const
{
    return enabled;
}

StateManager::PluginState StateManager::getCurrentState() const
{
    return current_state;
}

QString StateManager::getStateString() const
{
    return stateToString(current_state);
}

bool StateManager::canTransitionTo(PluginState newState) const
{
    return validateStateTransition(newState);
}

void StateManager::setEnabled(bool enable)
{
    if (enabled != enable)
    {
        enabled = enable;
        
        // Update state based on enabled status
        if (enabled)
        {
            if (current_state == PluginState::Disabled)
            {
                setState(PluginState::Initializing);
            }
        }
        else
        {
            setState(PluginState::Disabled);
        }
        
        emit enabledChanged(enabled);
    }
}

bool StateManager::setState(PluginState newState)
{
    if (!validateStateTransition(newState))
    {
        setError(QString("Invalid state transition from %1 to %2")
                .arg(stateToString(current_state))
                .arg(stateToString(newState)));
        return false;
    }

    if (current_state != newState)
    {
        current_state = newState;
        emit stateChanged(current_state);
    }
    
    return true;
}

void StateManager::setError(const QString& error)
{
    last_error = error;
    current_state = PluginState::Error;
    emit errorOccurred(error);
    emit stateChanged(current_state);
}

bool StateManager::validateStateTransition(PluginState newState) const
{
    switch (current_state)
    {
        case PluginState::Uninitialized:
            return newState == PluginState::Initializing || 
                   newState == PluginState::Error ||
                   newState == PluginState::Disabled;
            
        case PluginState::Initializing:
            return newState == PluginState::Running || 
                   newState == PluginState::Error ||
                   newState == PluginState::Disabled;
            
        case PluginState::Running:
            return newState == PluginState::Error ||
                   newState == PluginState::Disabled;
            
        case PluginState::Error:
            return newState == PluginState::Initializing ||
                   newState == PluginState::Disabled;
            
        case PluginState::Disabled:
            return newState == PluginState::Initializing;
            
        default:
            return false;
    }
}

QString StateManager::stateToString(PluginState state) const
{
    switch (state)
    {
        case PluginState::Uninitialized:
            return "Uninitialized";
        case PluginState::Initializing:
            return "Initializing";
        case PluginState::Running:
            return "Running";
        case PluginState::Error:
            return "Error";
        case PluginState::Disabled:
            return "Disabled";
        default:
            return "Unknown";
    }
}