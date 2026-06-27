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

    public func open(mode: KFLogMode, logDir: String, namePrefix: String, publicKey: String?) {}
    public func close() {}
    public func flush() {}

    public func log(level: KFLogLevel, tag: String, message: String, file: String, function: String, line: Int) {
        guard isEnabled(for: level) else { return }
        NSLog("[%@] %@", tag, message)
    }

    private func isEnabled(for level: KFLogLevel) -> Bool {
        level.rawValue >= self.level.rawValue
    }
}
