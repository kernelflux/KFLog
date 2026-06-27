// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "KFLog",
    platforms: [
        .iOS(.v16),
    ],
    products: [
        .library(
            name: "KFLog",
            targets: ["KFLog"]
        ),
        .library(
            name: "KFLogAPI",
            targets: ["KFLogAPI"]
        ),
        .library(
            name: "KFLogSwift",
            targets: ["KFLogSwift"]
        ),
    ],
    dependencies: [
        .package(url: "https://github.com/kernelflux/kfservice.git", from: "1.0.0"),
    ],
    targets: [
        // C/C++ core
        .target(
            name: "KFLogCore",
            path: "Sources/Core",
            exclude: [
                "boost/libs/context",
                "boost/libs/coroutine",
                "boost/libs/thread",
                "boost/libs/date_time",
                "boost/thread",
                "boost/date_time",
                "comm/windows",
                "comm/unix/socket",
                "comm/kflogger/android_xlog.h",
                "comm/objc/objc_timer.h",
                "comm/objc/objc_timer.mm",
                "comm/objc/platform_comm.mm",
                "comm/objc/KFLogReachability.h",
                "comm/objc/KFLogReachability.mm",
                "comm/alarm.h",
            ],
            publicHeadersPath: "fakeinclude",
            cSettings: [
                .define("NDEBUG", .when(configuration: .release)),
            ],
            cxxSettings: [
                .headerSearchPath("."),
            ],
            linkerSettings: [
                .linkedLibrary("z"),
                .linkedLibrary("c++"),
            ]
        ),

        // ObjC wrapper
        .target(
            name: "KFLog",
            dependencies: ["KFLogCore"],
            path: "Sources/KFLog",
            publicHeadersPath: "fakeinclude",
            cxxSettings: [
                .headerSearchPath("."),
            ],
            linkerSettings: [
                .linkedLibrary("z"),
                .linkedLibrary("c++"),
            ]
        ),

        // Swift extensions
        .target(
            name: "KFLogSwift",
            dependencies: ["KFLog", "KFLogAPI", .product(name: "KFService", package: "KFService")],
            path: "Sources/KFLogSwift"
        ),

        // Protocol-only API (zero dependency)
        .target(
            name: "KFLogAPI",
            path: "Sources/KFLogAPI"
        ),
    ],
    cxxLanguageStandard: .cxx14
)
