import Foundation
import KFLogAPI
import KFService

/// KFLog module — implements ModuleProtocol for DAG startup.
///
///     try await Engine.run(graph: graph)
///     ServiceFactory.resolve(KFLogger.self).info("done")
public final class KFLogModule: ModuleProtocol {
    public static var dependencies: [ModuleID] { [] }

    private let mode: KFLogMode
    private let logDir: String?
    private let namePrefix: String
    private let publicKey: String?
    private let level: KFLogLevel
    private let consoleLog: Bool

    public init(
        mode: KFLogMode = .async,
        logDir: String? = nil,
        namePrefix: String = "App",
        publicKey: String? = nil,
        level: KFLogLevel = .verbose,
        consoleLog: Bool = false
    ) {
        self.mode = mode
        self.logDir = logDir
        self.namePrefix = namePrefix
        self.publicKey = publicKey
        self.level = level
        self.consoleLog = consoleLog
    }

    public func performInit() async {
        ServiceFactory.register(KFLogger.self) {
            let dir = self.logDir ?? NSSearchPathForDirectoriesInDomains(
                .cachesDirectory, .userDomainMask, true
            ).first ?? NSTemporaryDirectory()
            let logger = KFLogDefault()
            logger.open(mode: mode, logDir: dir, namePrefix: self.namePrefix, publicKey: self.publicKey)
            logger.level = self.level
            if consoleLog { self.consoleLog.setConsoleLog(true) }
            return logger
        }
    }
}
