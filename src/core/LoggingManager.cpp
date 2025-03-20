/*---------------------------------------------------------*\
| Lightscape Plugin for OpenRGB                             |
|                                                           |
| LoggingManager.cpp                                        |
|                                                           |
| Manages logging verbosity                                 |
\*---------------------------------------------------------*/

#include "core/LoggingManager.h"
#include <QDebug>
#include <QDateTime>
#include <QMutexLocker>
#include <QTextStream>
#include <QFileInfo>

// Convenience for module usage
using namespace Lightscape;

// Global handler that redirects to the singleton
namespace Lightscape {
    void logRedirectHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        LoggingManager::getInstance().handleMessage(type, context, msg);
    }
}
// File logging methods
void LoggingManager::enableFileLogging(bool enable) {
    QMutexLocker locker(&_mutex);
    
    _fileLoggingEnabled = enable;
    
    if (enable) {
        ensureLogFileOpen();
    } else if (_logFile.isOpen()) {
        _logFile.close();
    }
    
    // Log the state change
    if (enable) {
        fprintf(stdout, "[INFO] File logging enabled: %s\n", qPrintable(_logFilePath));
    } else {
        fprintf(stdout, "[INFO] File logging disabled\n");
    }
    fflush(stdout);
}

bool LoggingManager::isFileLoggingEnabled() const {
    QMutexLocker locker(&_mutex);
    return _fileLoggingEnabled;
}

void LoggingManager::setLogFilePath(const QString& path) {
    QMutexLocker locker(&_mutex);
    
    // Close existing file if open
    if (_logFile.isOpen()) {
        _logFile.close();
    }
    
    _logFilePath = path;
    
    // Reopen if file logging is enabled
    if (_fileLoggingEnabled) {
        ensureLogFileOpen();
    }
}

QString LoggingManager::getLogFilePath() const {
    QMutexLocker locker(&_mutex);
    return _logFilePath;
}

void LoggingManager::setMaxLogFiles(int count) {
    QMutexLocker locker(&_mutex);
    _maxLogFiles = count;
}

int LoggingManager::getMaxLogFiles() const {
    QMutexLocker locker(&_mutex);
    return _maxLogFiles;
}

void LoggingManager::setMaxLogSize(int sizeInMB) {
    QMutexLocker locker(&_mutex);
    _maxLogSizeInMB = sizeInMB;
}

int LoggingManager::getMaxLogSize() const {
    QMutexLocker locker(&_mutex);
    return _maxLogSizeInMB;
}

// Message storage methods
QVector<LogMessage> LoggingManager::getRecentMessages(int count) const {
    QMutexLocker locker(&_mutex);
    
    if (count <= 0 || count >= _recentMessages.size()) {
        return _recentMessages;
    }
    
    // Return the most recent 'count' messages
    return _recentMessages.mid(_recentMessages.size() - count);
}

void LoggingManager::clearRecentMessages() {
    QMutexLocker locker(&_mutex);
    _recentMessages.clear();
}

// Internal helper methods
QString LoggingManager::getFormattedMessage(LogLevel level, const QString& message) const {
    // Use a compact timestamp format (without milliseconds)
    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString("MM-dd hh:mm:ss");
    
    return QString("%1 %2%3").arg(timestamp).arg(getLogPrefix(level)).arg(message);
}

QString LoggingManager::getLogPrefix(LogLevel level) const {
    switch (level) {
        case LogLevel::Error:
            return "[ERROR] ";
        case LogLevel::Warning:
            return "[WARNING] ";
        case LogLevel::Info:
            return "[INFO] ";
        case LogLevel::Debug:
            return "[DEBUG] ";
        case LogLevel::Verbose:
            return "[VERBOSE] ";
        default:
            return "";
    }
}

void LoggingManager::writeToLogFile(const QString& formattedMessage) {
    ensureLogFileOpen();
    
    if (_logFile.isOpen()) {
        // Check if we need to rotate logs based on file size
        if (_logFile.size() > _maxLogSizeInMB * 1024 * 1024) {
            rotateLogFiles();
        }
        
        // Write the message to the file
        QTextStream stream(&_logFile);
        stream << formattedMessage << "\n";
        stream.flush();
    }
}

void LoggingManager::ensureLogFileOpen() {
    if (!_logFile.isOpen()) {
        // Create directory if it doesn't exist
        QFileInfo fileInfo(_logFilePath);
        QDir().mkpath(fileInfo.absolutePath());
        
        _logFile.setFileName(_logFilePath);
        if (!_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            fprintf(stderr, "Failed to open log file: %s\n", qPrintable(_logFilePath));
            fflush(stderr);
        }
    }
}

void LoggingManager::rotateLogFiles() {
    // Close current log file
    if (_logFile.isOpen()) {
        _logFile.close();
    }
    
    // Get the base name and extension
    QFileInfo fileInfo(_logFilePath);
    QString baseName = fileInfo.completeBaseName();
    QString extension = fileInfo.suffix();
    QString dirPath = fileInfo.absolutePath();
    
    // Move existing log files to make room for the new one
    for (int i = _maxLogFiles - 1; i > 0; --i) {
        QString oldFile = QString("%1/%2.%3.%4").arg(dirPath).arg(baseName).arg(i).arg(extension);
        QString newFile = QString("%1/%2.%3.%4").arg(dirPath).arg(baseName).arg(i + 1).arg(extension);
        
        // Remove the oldest log file if it exists
        if (i == _maxLogFiles - 1) {
            QFile::remove(newFile);
        }
        
        // Rename existing log files
        if (QFile::exists(oldFile)) {
            QFile::rename(oldFile, newFile);
        }
    }
    
    // Rename the current log file
    QString newFile = QString("%1/%2.1.%3").arg(dirPath).arg(baseName).arg(extension);
    QFile::rename(_logFilePath, newFile);
    
    // Open a new log file
    ensureLogFileOpen();
}

QString LoggingManager::getDefaultLogFilePath() const {
    // Get OpenRGB plugins directory
    QString configPath;
    
    #ifdef _WIN32
        configPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        configPath = configPath.replace("/Lightscape", "/OpenRGB/plugins/Lightscape");
    #elif __APPLE__
        configPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        configPath = configPath.replace("/Lightscape", "/OpenRGB/plugins/Lightscape");
    #else // Linux
        configPath = QDir::homePath() + "/.config/OpenRGB/plugins/Lightscape";
    #endif
    
    // Make sure the directory exists
    QDir dir(configPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    // Create logs subdirectory
    dir.mkpath("logs");
    
    // Get current date/time for log filename
    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString("yyyyMMdd_hhmmss");
    
    return QString("%1/logs/Lightscape_%2.log").arg(configPath).arg(timestamp);
}

LoggingManager& LoggingManager::getInstance() {
    static LoggingManager instance;
    return instance;
}

LoggingManager::LoggingManager() 
    : _logLevel(LogLevel::Error)  // Default to Error level only
    , _originalHandler(nullptr)
    , _fileLoggingEnabled(false)
    , _maxLogFiles(3)  // Keep only 3 old log files
    , _maxLogSizeInMB(1)  // Rotate logs when they reach 1MB
    , _maxRecentMessages(100)  // Store only 100 recent messages
{
    // Set default log file path
    _logFilePath = getDefaultLogFilePath();
}

LoggingManager::~LoggingManager() {
    // Restore original handler if we installed one
    if (_originalHandler) {
        qInstallMessageHandler(_originalHandler);
    }
    
    // Close log file if open
    if (_logFile.isOpen()) {
        _logFile.close();
    }
}

void LoggingManager::installHandler() {
    // Store the original handler and install our own
    _originalHandler = qInstallMessageHandler(logRedirectHandler);
}

void LoggingManager::setLogLevel(LogLevel level) {
    QMutexLocker locker(&_mutex); // Non-const reference
    _logLevel = level;
}

LogLevel LoggingManager::getLogLevel() const {
    // Can't use QMutexLocker with const method, so manually lock/unlock
    _mutex.lock();
    LogLevel level = _logLevel;
    _mutex.unlock();
    return level;
}

bool LoggingManager::isEnabled(LogLevel level) const {
    // Can't use QMutexLocker with const method, so manually lock/unlock
    _mutex.lock();
    bool enabled = static_cast<int>(level) <= static_cast<int>(_logLevel);
    _mutex.unlock();
    return enabled;
}

void LoggingManager::error(const QString& message) {
    logMessage(LogLevel::Error, message);
}

void LoggingManager::warning(const QString& message) {
    logMessage(LogLevel::Warning, message);
}

void LoggingManager::info(const QString& message) {
    logMessage(LogLevel::Info, message);
}

void LoggingManager::debug(const QString& message) {
    logMessage(LogLevel::Debug, message);
}

void LoggingManager::verbose(const QString& message) {
    logMessage(LogLevel::Verbose, message);
}

void LoggingManager::logMessage(LogLevel level, const QString& message) {
    // Special case: Always allow the plugin loaded message through
    bool isLoadedMessage = message.contains("Plugin loaded successfully");
    
    // Check if this log level is enabled (or if it's the special message)
    if (!isLoadedMessage && !isEnabled(level)) {
        return;
    }
    
    QMutexLocker locker(&_mutex);
    
    // Format the message with timestamp and level prefix
    QString formattedMessage = getFormattedMessage(level, message);
    
    // Output to console
    fprintf(stdout, "%s\n", qPrintable(formattedMessage));
    fflush(stdout);
    
    // Write to log file if enabled
    if (_fileLoggingEnabled) {
        writeToLogFile(formattedMessage);
    }
    
    // Store in recent messages (for UI display)
    LogMessage logMsg;
    logMsg.message = message;
    logMsg.level = level;
    logMsg.timestamp = QDateTime::currentDateTime();
    
    _recentMessages.append(logMsg);
    
    // Trim message list if it exceeds the maximum
    while (_recentMessages.size() > _maxRecentMessages) {
        _recentMessages.removeFirst();
    }
}

void LoggingManager::handleMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    Q_UNUSED(context);
    
    // Lock for thread safety
    QMutexLocker locker(&_mutex);
    
    // Immediately filter out messages we don't want to see
    
    // Filter all standard informational messages
    if (msg.contains("SettingsManager:") ||
        msg.contains("[ResourceManager]") ||
        msg.contains("[ResourceHandler]") ||
        msg.contains("Populating") ||
        msg.contains("Found device") ||
        msg.contains("Added device") ||
        msg.contains("Device assignment") ||
        msg.contains("Unassigned devices") ||
        msg.contains("Registering") ||
        msg.contains("Detected") ||
        msg.contains("State ")) {
        return;  // Filter out all these types of messages
    }
    
    // Map Qt message types to our log levels
    LogLevel level;
    switch (type) {
        case QtFatalMsg:
        case QtCriticalMsg:
            level = LogLevel::Error;
            break;
        case QtWarningMsg:
            level = LogLevel::Warning;
            break;
        case QtInfoMsg:
            level = LogLevel::Info;
            break;
        case QtDebugMsg:
        default:
            level = LogLevel::Debug;
            break;
    }
    
    // Only proceed if level is appropriate
    if (static_cast<int>(level) <= static_cast<int>(_logLevel)) {
        // Format the message with timestamp and level prefix
        QString formattedMessage = getFormattedMessage(level, msg);
        
        // Output to console
        fprintf(stdout, "%s\n", qPrintable(formattedMessage));
        fflush(stdout);
        
        // Write to log file if enabled
        if (_fileLoggingEnabled) {
            writeToLogFile(formattedMessage);
        }
        
        // Store in recent messages (for UI display)
        LogMessage logMsg;
        logMsg.message = msg;
        logMsg.level = level;
        logMsg.timestamp = QDateTime::currentDateTime();
        
        _recentMessages.append(logMsg);
        
        // Trim message list if it exceeds the maximum
        while (_recentMessages.size() > _maxRecentMessages) {
            _recentMessages.removeFirst();
        }
    }
    
    // If it's a fatal message, also forward to the original handler for Qt to handle it
    if (type == QtFatalMsg && _originalHandler) {
        _originalHandler(type, context, msg);
    }
}