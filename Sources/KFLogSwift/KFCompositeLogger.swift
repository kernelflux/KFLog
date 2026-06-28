// Copyright (c) 2026 KernelFlux. All rights reserved.

import Foundation
import KFLogAPI

/// KFLogger implementation that dispatches to multiple loggers simultaneously.
public final class KFCompositeLogger: KFLogger {
    private let loggers: [KFLogger]

    public var level: KFLogLevel {
        didSet {
            for logger in loggers { logger.level = level }
        }
    }

    public init(loggers: [KFLogger], level: KFLogLevel = .verbose) {
        self.loggers = loggers
        self.level = level
        for logger in loggers { logger.level = level }
    }

    public func initialize(config: KFLogConfig) {
        self.level = config.level
        for logger in loggers { logger.initialize(config: config) }
    }

    public func unInit() {
        for logger in loggers { logger.unInit() }
    }

    public func flush() {
        for logger in loggers { logger.flush() }
    }

    public func setConsoleLog(_ enabled: Bool) {
        for logger in loggers { logger.setConsoleLog(enabled) }
    }

    public func setMaxFileSize(_ size: UInt64) {
        for logger in loggers { logger.setMaxFileSize(size) }
    }

    public var logFileURLs: [URL] {
        loggers.flatMap { $0.logFileURLs }
    }

    public func log(level: KFLogLevel, tag: String, message: String, metadata: KFLogMetadata?, file: String, function: String, line: Int) {
        for logger in loggers {
            logger.log(level: level, tag: tag, message: message, metadata: metadata, file: file, function: function, line: line)
        }
    }
}
