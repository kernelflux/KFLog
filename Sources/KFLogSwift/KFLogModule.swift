import Foundation
import KFLogAPI
import KFService

/// KFLog module — provides DAG startup hook after registration.
///
/// Host registers KFLogger in init():
///     ServiceFactory.register(KFLogger.self) { KFLogDefault() }
///
/// Engine calls performInit() after dependencies are ready.
public final class KFLogModule: ModuleProtocol {
    public static var dependencies: [ModuleID] { [] }
    public init() {}

    public func performInit() async {
        // Any async setup after logger is registered.
    }
}
