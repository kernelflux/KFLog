# KFLog

High-performance iOS logging component — async mmap-backed writer, AES+ECDH encryption, Swift-friendly API with `@autoclosure` support and structured metadata.

Forked from [Tencent/mars](https://github.com/Tencent/mars) 1.3.0, stripped of Android/Win32 code, adapted for the KernelFlux component library.

## Features

- **Async log writing** — mmap-based ring buffer, no blocking I/O on the logging thread
- **AES + ECDH encryption** — logs encrypted at rest, optional public key for key agreement
- **Console output** — toggle console logging independently from file logging
- **`@autoclosure` Swift API** — message evaluation deferred until log level is checked
- **Structured metadata** — `KFLogMetadata` key-value pairs attached to every log entry
- **File splitting** — configurable max file size and time-based rotation
- **Module-level log filtering** — `logFileURLs(matching:)` to collect logs by module prefix

## Installation

**Swift Package Manager**

```
https://github.com/kernelflux/kflog.git
```

Or in `Package.swift`:

```swift
.package(url: "https://github.com/kernelflux/kflog.git", from: "1.0.0")
```

Then add the target you need:

| Product | Description | Depends on |
|---------|-------------|------------|
| `KFLog` | ObjC wrapper | `KFLogCore` |
| `KFLogAPI` | Swift protocol-only (zero deps) | nothing |
| `KFLogSwift` | Swift impl + KFService integration | `KFLog`, `KFLogAPI`, `KFService` |

## Architecture

```
KFLog
├── KFLogCore/           ← C/C++ (mars xlog core, stripped of Android/Win32)
│   ├── comm/            ← Platform abstraction, thread, mmap
│   └── log/             ← Log engine: buffer, encryption, file management
├── KFLog/               ← ObjC wrapper (KFLog.h, KFLog.mm)
├── KFLogAPI/            ← Swift protocol (KFLogger) + KFLogLevel, KFLogMode, KFLogConfig
└── KFLogSwift/          ← KFLogDefault, KFConsoleLogger, KFCompositeLogger, KFLogAssembly, KFLogStartupModule
```

## Quick Start

### Using KFService (recommended)

```swift
import KFService
import KFLogSwift

// In App init — register via assembly (also registers named "console" logger)
ServiceContainer.shared.install(KFLogAssembly())

// In App.task — run startup
try await Engine.run(modules: [
    KFLogStartupModule(config: KFLogConfig(
        logDir: documentsPath,
        namePrefix: "MyApp",
        consoleLog: true
    )),
])
```

Resolve and use anywhere:

```swift
let logger = try ServiceContainer.shared.resolve(KFLogger.self)
logger.info("User logged in")
logger.error("Network timeout", metadata: ["host": "api.example.com"], tag: "Networking")

// Console-only logger (named registration)
let console = try ServiceContainer.shared.resolve(KFLogger.self, name: "console")
```

### Standalone (no KFService)

```swift
import KFLogSwift

let logger = KFLogDefault()
logger.initialize(config: KFLogConfig(
    mode: .async,
    logDir: documentsPath,
    namePrefix: "MyApp"
))
logger.info("App started")
logger.debug("Cache miss for key: \(key)")  // skipped when level > debug
logger.flush()  // force write to disk
logger.setConsoleLog(true)
```

## Log Levels

```swift
public enum KFLogLevel: Int, Sendable {
    case verbose = 0   // all logs
    case debug   = 1
    case info    = 2   // default for release
    case warn    = 3
    case error   = 4
    case fatal   = 5
    case none    = 6   // disable all logging
}
```

## API Reference

### KFLogger Protocol

```swift
public protocol KFLogger: AnyObject {
    var level: KFLogLevel { get set }

    func initialize(config: KFLogConfig)
    func unInit()
    func flush()
    func setConsoleLog(_ enabled: Bool)
    func setMaxFileSize(_ size: UInt64)

    var logFileURLs: [URL] { get }

    func log(level: KFLogLevel, tag: String, message: String,
             metadata: KFLogMetadata?, file: String, function: String, line: Int)
}
```

### KFLogConfig

```swift
public struct KFLogConfig {
    public var logDir: String
    public var namePrefix: String
    public var publicKey: String?
    public var mode: KFLogMode
    public var level: KFLogLevel
    public var maxFileSize: UInt64
    public var consoleLog: Bool

    public init(logDir: String, namePrefix: String,
                publicKey: String? = nil, mode: KFLogMode = .async,
                level: KFLogLevel = .info, maxFileSize: UInt64 = 5_242_880,
                consoleLog: Bool = false)
}
```

### Convenience Methods (protocol extension)

```swift
logger.verbose("...")   // @autoclosure — not evaluated if level > verbose
logger.debug("...")
logger.info("...")
logger.warn("...")
logger.error("...")     // supports metadata: ["key": "value"]
logger.fatal("...")
```

### Structured Metadata

```swift
logger.info("Request completed", metadata: [
    "status": "200",
    "latency_ms": "42",
    "endpoint": "/api/users"
])
// Output: Request completed {endpoint=/api/users latency_ms=42 status=200}
```

## Log File Management

```swift
// All log files
let urls = logger.logFileURLs

// Filter by module prefix
let networkLogs = logger.logFileURLs(matching: "MyApp_Networking")
```

## KFService Integration

| Type | Role |
|------|------|
| `KFLogAssembly` | Implements `ServiceAssembly` — registers `KFLogger` → `KFLogDefault`, registers named `"console"` → `KFConsoleLogger` |
| `KFLogStartupModule` | Implements `StartupModule` — provides `KFLogStartupTask` with config |

```swift
// Install (sync, in App init)
ServiceContainer.shared.install(KFLogAssembly())

// Override with custom impl — last write wins
ServiceContainer.shared.register(KFLogger.self) { MyCustomLogger() }

// Run (async, in App.task)
try await Engine.run(modules: [
    KFLogStartupModule(config: KFLogConfig(logDir: dir, namePrefix: "MyApp")),
])
```

## Source Layout

```
Sources/
├── Core/                 ← C/C++ core (forked from Tencent/mars xlog)
│   ├── comm/             ← Thread, mmap, platform abstraction
│   └── log/              ← Log buffer, AES/ECDH encryption, file writer
├── KFLog/                ← ObjC wrapper (KFLog.h, KFLog.mm)
├── KFLogAPI/             ← KFLogger protocol, KFLogLevel, KFLogMode, KFLogConfig
└── KFLogSwift/           ← KFLogDefault, KFConsoleLogger, KFLogAssembly, KFLogStartupModule
```

## License

[MIT](LICENSE) — Copyright (c) 2016 THL A29 Limited (Tencent), Copyright (c) 2026 KernelFlux

This project is forked from [Tencent/mars](https://github.com/Tencent/mars) and inherits its MIT license.
