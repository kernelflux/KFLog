# KFLog

High-performance iOS logging component — async mmap-backed writer, AES+ECDH encryption, Swift-friendly API with `@autoclosure` support.

Forked from [Tencent/mars](https://github.com/Tencent/mars) 1.3.0, stripped of Android/Win32 code, adapted for the KernelFlux component library.

[中文文档](README_CN.md)

## Features

- **Async log writing** — mmap-based ring buffer, no blocking I/O on the logging thread
- **AES + ECDH encryption** — logs encrypted at rest, optional public key for key agreement
- **Console output** — toggle console logging independently from file logging
- **`@autoclosure` Swift API** — message evaluation deferred until log level is checked
- **File splitting** — configurable max file size and time-based rotation
- **Module-level log filtering** — `logFileURLs(matching:)` to collect logs by module prefix
- **System event integration** — auto-flush on background / memory warning via `KFSystemEventObserver`

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
| `KFLogSwift` | Swift extensions + KFService integration | `KFLog`, `KFLogAPI`, `KFService` |

## Architecture

```
KFLog
├── KFLogCore/           ← C/C++ (mars xlog core, stripped of Android/Win32)
│   ├── comm/            ← Platform abstraction, thread, mmap
│   └── log/             ← Log engine: buffer, encryption, file management
├── KFLog/               ← ObjC wrapper (KFLog.h, KFLog.mm)
├── KFLogAPI/            ← Swift protocol (KFLogger) + KFLogLevel, KFLogMode
└── KFLogSwift/          ← KFLogDefault, KFConsoleLogger, KFCompositeLogger, KFLogModule
```

## Quick Start

### Using KFService (recommended)

```swift
import KFService
import KFLogSwift

// Register module at app launch
KFServiceManager.register(module: KFLogModule(
    logDir: documentsPath,
    namePrefix: "MyApp",
    level: .info
))

// Resolve and log
let logger = KFServiceManager.resolve(KFLogger.self)
logger.info("User logged in")
logger.error("Network timeout", tag: "Networking")
```

### Standalone (no KFService)

```swift
import KFLogSwift

let logger = KFLogDefault(
    mode: .async,
    logDir: documentsPath,
    namePrefix: "MyApp",
    level: .info
)

logger.info("App started")
logger.debug("Cache miss for key: \(key)")  // skipped when level > debug
logger.flush()  // force write to disk

// Enable console output alongside file logging
logger.setConsoleLog(true)
```

## Log Levels

```swift
public enum KFLogLevel: Int {
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
@objc public protocol KFLogger: AnyObject {
    var level: KFLogLevel { get set }

    func open(mode: KFLogMode, logDir: String, namePrefix: String, publicKey: String?)
    func close()
    func flush()
    func setConsoleLog(_ enabled: Bool)
    func setMaxFileSize(_ size: UInt64)

    var logFileURLs: [URL] { get }

    func log(level: KFLogLevel, tag: String, message: String,
             file: String, function: String, line: Int)
}
```

### Convenience Methods (protocol extension)

```swift
logger.verbose("...")   // @autoclosure — not evaluated if level > verbose
logger.debug("...")
logger.info("...")
logger.warn("...")
logger.error("...")
logger.fatal("...")
```

## Log File Management

```swift
// All log files
let urls = logger.logFileURLs

// Filter by module prefix
let networkLogs = logger.logFileURLs(matching: "MyApp_Networking")
```

## KFLogModule

Registers with KFService, handles lifecycle:

```swift
KFLogModule(
    mode: .async,
    logDir: documentsPath,
    namePrefix: "MyApp",
    publicKey: nil,
    level: .info,
    priority: 200   // start after KV store
)
```

The module conforms to `KFSystemEventObserver` — auto-flushes on `onEnterBackground()` and `onMemoryWarning()`.

## Source Layout

```
Sources/
├── Core/                 ← C/C++ core (forked from Tencent/mars xlog)
│   ├── comm/             ← Thread, mmap, platform abstraction
│   └── log/              ← Log buffer, AES/ECDH encryption, file writer
├── KFLog/                ← ObjC wrapper (KFLog.h, KFLog.mm)
├── KFLogAPI/             ← KFLogger protocol, KFLogLevel, KFLogMode
└── KFLogSwift/           ← KFLogDefault, KFConsoleLogger, KFLogModule
```

## License

[MIT](LICENSE) — Copyright (c) 2016 THL A29 Limited (Tencent), Copyright (c) 2026 KernelFlux

This project is forked from [Tencent/mars](https://github.com/Tencent/mars) and inherits its MIT license. Per-file license headers have been consolidated into the root [LICENSE](LICENSE).
