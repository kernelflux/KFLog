// Copyright (c) 2026 KernelFlux. All rights reserved.

import Foundation
import KFLogAPI

/// KFLogger implementation that logs to NSLog (console only, no disk).
public final class KFConsoleLogger: KFLogger {
    public var level: KFLogLevel

    public init(level: KFLogLevel = .verbose) {
        self.level = level
    }

    public var logFileURLs: [URL] { [] }

    public func initialize(config: KFLogConfig) { self.level = config.level }
    public func unInit() {}
    public func flush() {}

    public func log(level: KFLogLevel, tag: String, message: String, metadata: KFLogMetadata?, file: String, function: String, line: Int) {
        guard isEnabled(for: level) else { return }
        let msg = message + formatMetadata(metadata)
        NSLog("[%@] %@", tag, msg)
    }

    private func isEnabled(for level: KFLogLevel) -> Bool {
        level.rawValue >= self.level.rawValue
    }
}
