import Foundation

/// Log level. rawValue matches the underlying C engine's level enum.
@objc public enum KFLogLevel: Int {
    case verbose = 0
    case debug = 1
    case info = 2
    case warn = 3
    case error = 4
    case fatal = 5
    case none = 6
}

/// Log appender mode.
@objc public enum KFLogMode: Int {
    case async = 0
    case sync = 1
}

/// Abstract logger protocol.
/// Implement this to provide custom logging backends (console, file, network, etc.).
@objc public protocol KFLogger: AnyObject {
    var level: KFLogLevel { get set }

    @objc func open(mode: KFLogMode, logDir: String, namePrefix: String, publicKey: String?)
    @objc func close()
    @objc func flush()
    @objc optional func setConsoleLog(_ enabled: Bool)
    @objc optional func setMaxFileSize(_ size: UInt64)

    /// URLs of all current log files on disk (.xlog + .mmap3).
    /// Returns an empty array for loggers without disk persistence (e.g. console).
    var logFileURLs: [URL] { get }

    func log(level: KFLogLevel,
             tag: String,
             message: String,
             file: String,
             function: String,
             line: Int)
}

// MARK: - Log Files

public extension KFLogger {
    /// Default implementation — returns empty array.
    /// Override in disk-based loggers to return actual log file URLs.
    var logFileURLs: [URL] { [] }

    /// Filtered subset of log files whose name starts with the given prefix.
    /// Pass `nil` to get all log files (equivalent to `logFileURLs`).
    func logFileURLs(matching prefix: String?) -> [URL] {
        guard let prefix else { return logFileURLs }
        return logFileURLs.filter { $0.lastPathComponent.hasPrefix(prefix) }
    }
}

// MARK: - Convenience

public extension KFLogger {
    func verbose(_ message: @autoclosure () -> String,
                 tag: String = "KFLog",
                 file: String = #file,
                 function: String = #function,
                 line: Int = #line) {
        guard level.rawValue <= KFLogLevel.verbose.rawValue else { return }
        log(level: .verbose, tag: tag, message: message(), file: file, function: function, line: line)
    }

    func debug(_ message: @autoclosure () -> String,
               tag: String = "KFLog",
               file: String = #file,
               function: String = #function,
               line: Int = #line) {
        guard level.rawValue <= KFLogLevel.debug.rawValue else { return }
        log(level: .debug, tag: tag, message: message(), file: file, function: function, line: line)
    }

    func info(_ message: @autoclosure () -> String,
              tag: String = "KFLog",
              file: String = #file,
              function: String = #function,
              line: Int = #line) {
        guard level.rawValue <= KFLogLevel.info.rawValue else { return }
        log(level: .info, tag: tag, message: message(), file: file, function: function, line: line)
    }

    func warn(_ message: @autoclosure () -> String,
              tag: String = "KFLog",
              file: String = #file,
              function: String = #function,
              line: Int = #line) {
        guard level.rawValue <= KFLogLevel.warn.rawValue else { return }
        log(level: .warn, tag: tag, message: message(), file: file, function: function, line: line)
    }

    func error(_ message: @autoclosure () -> String,
               tag: String = "KFLog",
               file: String = #file,
               function: String = #function,
               line: Int = #line) {
        guard level.rawValue <= KFLogLevel.error.rawValue else { return }
        log(level: .error, tag: tag, message: message(), file: file, function: function, line: line)
    }

    func fatal(_ message: @autoclosure () -> String,
               tag: String = "KFLog",
               file: String = #file,
               function: String = #function,
               line: Int = #line) {
        guard level.rawValue <= KFLogLevel.fatal.rawValue else { return }
        log(level: .fatal, tag: tag, message: message(), file: file, function: function, line: line)
    }
}
