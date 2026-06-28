import KFService
import KFLogAPI

final class KFLogStartupTask: BaseStartupTask {
    override var identifier: String { "com.kernelflux.log" }
    override var actorRequirement: ActorRequirement { .mainActor }

    private let config: KFLogConfig

    init(config: KFLogConfig) { self.config = config }

    override func run() async throws {
        let logger = try ServiceContainer.shared.resolve(KFLogger.self)
        logger.initialize(config: config)
    }
}

public struct KFLogStartupModule: StartupModule {
    private let config: KFLogConfig
    public var tasks: [any StartupTask] { [KFLogStartupTask(config: config)] }
    public init(config: KFLogConfig) { self.config = config }
}
