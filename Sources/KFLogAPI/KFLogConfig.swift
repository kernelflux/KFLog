import Foundation

/// Configuration for the KFLogger service.
public struct KFLogConfig: Sendable {
    /// Directory for log files.
    public var logDir: String

    /// Prefix prepended to log file names.
    public var namePrefix: String

    /// Minimum log level. Default `.verbose`.
    public var level: KFLogLevel

    /// Log appender mode. Default `.async`.
    public var mode: KFLogMode

    /// Optional public key for log encryption.
    public var publicKey: String?

    /// Enable console output. Default true.
    public var consoleLog: Bool

    /// Maximum per-file size in bytes. 0 means use default.
    public var maxFileSize: UInt64

    public init(
        logDir: String,
        namePrefix: String,
        level: KFLogLevel = .verbose,
        mode: KFLogMode = .async,
        publicKey: String? = nil,
        consoleLog: Bool = true,
        maxFileSize: UInt64 = 0
    ) {
        self.logDir = logDir
        self.namePrefix = namePrefix
        self.level = level
        self.mode = mode
        self.publicKey = publicKey
        self.consoleLog = consoleLog
        self.maxFileSize = maxFileSize
    }
}
