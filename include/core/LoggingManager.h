/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| LoggingManager.h                                          |
|                                                           |
| Manages logging verbosity                                 |
\*---------------------------------------------------------*/

#pragma once

#include <QString>
#include <QMutex>
#include <QFile>
#include <QDateTime>
#include <QVector>
#include <QStandardPaths>
#include <QDir>

namespace Lightscape {

// Enum for log levels
enum class LogLevel {
    None = 0,    // No logging
    Error = 1,   // Critical errors only
    Warning = 2, // Warnings and errors
    Info = 3,    // Important information, warnings, and errors
    Debug = 4,   // Detailed debug information
    Verbose = 5  // Extremely detailed information
};

// Structure to hold log messages
struct LogMessage {
    QString message;
    LogLevel level;
    QDateTime timestamp;
};

/**
 * @brief LoggingManager provides centralized logging functionality for the Lightscape plugin.
 *
 * Key features:
 * - Multiple log levels (Error, Warning, Info, Debug, Verbose)
 * - File logging with rotation support
 * - Thread-safe operation
 * - Message storage for UI display
 * - Integration with Qt's logging system
 *
 * Usage example:
 * ```cpp
 * // Initialize logging
 * LoggingManager::getInstance().setLogLevel(LogLevel::Info);
 * LoggingManager::getInstance().enableFileLogging(true);
 *
 * // Log messages
 * LOG_INFO("Application started");
 * LOG_WARNING("Configuration file not found, using defaults");
 * LOG_ERROR("Failed to connect to device");
 *
 * // Get recent logs for display
 * QVector<LogMessage> messages = LoggingManager::getInstance().getRecentMessages();
 * ```
 */

// Forward declaration for QMessageHandler
class LoggingManager;
void logRedirectHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

class LoggingManager {
public:
    // Get singleton instance
    static LoggingManager& getInstance();
    
    // Set global log level
    void setLogLevel(LogLevel level);
    LogLevel getLogLevel() const;
    
    // Log messages at different levels
    void error(const QString& message);
    void warning(const QString& message);
    void info(const QString& message);
    void debug(const QString& message);
    void verbose(const QString& message);
    
    // Check if a given log level is enabled
    bool isEnabled(LogLevel level) const;
    
    // Handle Qt logging messages - accessed by the global handler
    void handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    
    // Install the handler to capture Qt's built-in logging
    void installHandler();
    
    // File logging configuration
    void enableFileLogging(bool enable);
    bool isFileLoggingEnabled() const;
    void setLogFilePath(const QString& path);
    QString getLogFilePath() const;
    void setMaxLogFiles(int count);
    int getMaxLogFiles() const;
    void setMaxLogSize(int sizeInMB);
    int getMaxLogSize() const;
    
    // Get recent log messages (for UI display)
    QVector<LogMessage> getRecentMessages(int count = 100) const;
    void clearRecentMessages();
    
private:
    LoggingManager();
    ~LoggingManager();
    
    // Prevent copying
    LoggingManager(const LoggingManager&) = delete;
    LoggingManager& operator=(const LoggingManager&) = delete;
    
    // Core properties
    LogLevel _logLevel;
    mutable QMutex _mutex;  // Mutable for thread safety in const methods
    QtMessageHandler _originalHandler;
    
    // File logging properties
    bool _fileLoggingEnabled;
    QString _logFilePath;
    QFile _logFile;
    int _maxLogFiles;
    int _maxLogSizeInMB;
    
    // Message storage (for UI)
    QVector<LogMessage> _recentMessages;
    int _maxRecentMessages;
    
    // Internal methods
    void logMessage(LogLevel level, const QString& message);
    void writeToLogFile(const QString& formattedMessage);
    void rotateLogFiles();
    QString getFormattedMessage(LogLevel level, const QString& message) const;
    QString getLogPrefix(LogLevel level) const;
    QString getDefaultLogFilePath() const;
    void ensureLogFileOpen();
    
    // Friend the global handler function
    friend void logRedirectHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

// Convenience macros for logging
#define LOG_ERROR(msg) Lightscape::LoggingManager::getInstance().error(msg)
#define LOG_WARNING(msg) Lightscape::LoggingManager::getInstance().warning(msg)
#define LOG_INFO(msg) Lightscape::LoggingManager::getInstance().info(msg)
#define LOG_DEBUG(msg) Lightscape::LoggingManager::getInstance().debug(msg)
#define LOG_VERBOSE(msg) Lightscape::LoggingManager::getInstance().verbose(msg)

// Macros for conditional logging checks
#define IS_LOG_ENABLED(level) Lightscape::LoggingManager::getInstance().isEnabled(level)

} // namespace Lightscape