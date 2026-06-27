import Foundation
import KFLogAPI
import KFService
@_exported import KFLogAPI

/// KFLog service module — registers the default xlog-backed KFLogger with KFServiceManager.
///
///     KFServiceManager.register(module: KFLogModule(namePrefix: "Demo"))
///     KFServiceManager.resolve(KFLogger.self).info("done")
public struct KFLogModule: KFModule {
    public let priority: Int

    private let mode: KFLogMode
    private let logDir: String?
    private let namePrefix: String
    private let publicKey: String?
    private let level: KFLogLevel
    private let consoleLog: Bool

    public init(
        priority: Int = 200,
        mode: KFLogMode = .async,
        logDir: String? = nil,
        namePrefix: String = "App",
        publicKey: String? = nil,
        level: KFLogLevel = .verbose,
        consoleLog: Bool = false
    ) {
        self.priority = priority
        self.mode = mode
        self.logDir = logDir
        self.namePrefix = namePrefix
        self.publicKey = publicKey
        self.level = level
        self.consoleLog = consoleLog
    }

    public func register() {
        KFServiceManager.register(KFLogger.self) {
            let dir = logDir ?? NSSearchPathForDirectoriesInDomains(
                .cachesDirectory, .userDomainMask, true
            ).first ?? NSTemporaryDirectory()
            let logger = KFLogDefault()
            logger.open(mode: mode, logDir: dir, namePrefix: namePrefix, publicKey: publicKey)
            logger.level = level
            if consoleLog { logger.setConsoleLog(true) }
            return logger
        }
    }

    public func unregister() {
        guard let logger = KFServiceManager.resolveOptional(KFLogger.self) else { return }
        logger.flush()
        logger.close()
    }
}
