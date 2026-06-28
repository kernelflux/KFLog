import KFService
import KFLogAPI

public struct KFLogAssembly: ServiceAssembly {
    public init() {}
    public func assemble(container: ServiceContainer) {
        container.register(KFLogger.self) { KFLogDefault() }
        container.register(KFLogger.self, name: "console") { KFConsoleLogger() }
    }
}
