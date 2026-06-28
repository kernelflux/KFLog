import Foundation
import KFLogAPI
import KFService

/// Default KFLogger implementation backed by the xlog C/C++ engine (disk persistence).
/// Uses KFLogEngineBridge internally to avoid enum type conflicts with the ObjC KFLog module.
public final class KFLogDefault: KFLogger, KFSystemEventObserver {
    public var level: KFLogLevel {
        didSet { KFLogEngineBridge.setLevel(level.rawValue) }
    }

    private var _logDir: String = ""
    private var _namePrefix: String = ""

    public var logFileURLs: [URL] {
        guard !_logDir.isEmpty, !_namePrefix.isEmpty else { return [] }
        let dirURL = URL(fileURLWithPath: _logDir)
        guard let enumerator = FileManager.default.enumerator(at: dirURL,
                                                              includingPropertiesForKeys: nil) else {
            return []
        }
        var urls: [URL] = []
        for case let fileURL as URL in enumerator {
            let name = fileURL.lastPathComponent
            guard name.hasPrefix(_namePrefix) else { continue }
            guard name.hasSuffix(".xlog") || name.hasSuffix(".mmap3") else { continue }
            urls.append(fileURL)
        }
        return urls
    }

    public init() {
        self.level = .verbose
    }

    public func initialize(config: KFLogConfig) {
        _logDir = config.logDir
        _namePrefix = config.namePrefix
        self.level = config.level
        KFLogEngineBridge.open(
            mode: config.mode.rawValue,
            logDir: config.logDir,
            namePrefix: config.namePrefix,
            publicKey: config.publicKey
        )
        if !config.consoleLog { setConsoleLog(false) }
        if config.maxFileSize > 0 { setMaxFileSize(config.maxFileSize) }
    }

    public func unInit() {
        KFLogEngineBridge.close()
    }

    public func flush() {
        KFLogEngineBridge.flush()
    }

    public func setConsoleLog(_ enabled: Bool) {
        KFLogEngineBridge.setConsoleLog(enabled)
    }

    public func setMaxFileSize(_ size: UInt64) {
        KFLogEngineBridge.setMaxFileSize(size)
    }

    public func log(level: KFLogLevel, tag: String, message: String, metadata: KFLogMetadata?, file: String, function: String, line: Int) {
        guard isEnabled(for: level) else { return }
        let msg = message + formatMetadata(metadata)
        KFLogEngineBridge.log(level: level.rawValue, tag: tag,
                             file: file, function: function,
                             line: Int32(line), message: msg)
    }

    private func isEnabled(for level: KFLogLevel) -> Bool {
        level.rawValue >= self.level.rawValue
    }

    // MARK: - KFSystemEventObserver

    public func onEnterBackground() { flush() }
    public func onMemoryWarning() { flush() }
}
