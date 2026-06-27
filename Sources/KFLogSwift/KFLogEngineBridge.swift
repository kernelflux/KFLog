import KFLog

/// Thin bridge to KFLogEngine that avoids enum name conflicts with KFLogAPI.
/// Only imports the ObjC KFLog module; KFLogAPI is not imported here.
enum KFLogEngineBridge {
    static func setLevel(_ rawValue: Int) {
        KFLogEngine.setLevel(KFLogLevel(rawValue: rawValue)!)
    }

    static func open(mode: Int, logDir: String, namePrefix: String, publicKey: String?) {
        KFLogEngine.open(KFLogMode(rawValue: mode)!, logDir: logDir, namePrefix: namePrefix, publicKey: publicKey)
    }

    static func close() { KFLogEngine.close() }
    static func flush() { KFLogEngine.flush() }
    static func setConsoleLog(_ enabled: Bool) { KFLogEngine.setConsoleLog(enabled) }
    static func setMaxFileSize(_ size: UInt64) { KFLogEngine.setMaxFileSize(size) }

    static func log(level: Int, tag: String, file: String, function: String, line: Int32, message: String) {
        KFLogEngine.log(level: KFLogLevel(rawValue: level)!,
                        tag: tag, file: file, function: function,
                        line: line, message: message)
    }
}
