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

    public init(level: KFLogLevel = .verbose) {
        self.level = level
    }

    public func open(mode: KFLogMode, logDir: String, namePrefix: String, publicKey: String?) {
        _logDir = logDir
        _namePrefix = namePrefix
        KFLogEngineBridge.open(mode: mode.rawValue, logDir: logDir,
                               namePrefix: namePrefix, publicKey: publicKey)
    }

    public func close() {
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

    public func log(level: KFLogLevel, tag: String, message: String, file: String, function: String, line: Int) {
        guard isEnabled(for: level) else { return }
        KFLogEngineBridge.log(level: level.rawValue, tag: tag,
                             file: file, function: function,
                             line: Int32(line), message: message)
    }

    private func isEnabled(for level: KFLogLevel) -> Bool {
        level.rawValue >= self.level.rawValue
    }

    // MARK: - KFSystemEventObserver

    public func onEnterBackground() { flush() }
    public func onMemoryWarning() { flush() }
}
