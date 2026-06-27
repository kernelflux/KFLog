# KFLog

高性能 iOS 日志组件 —— 异步 mmap 写引擎、AES+ECDH 加密、支持 `@autoclosure` 的 Swift 友好 API。

Fork 自 [Tencent/mars](https://github.com/Tencent/mars) 1.3.0，剥离 Android/Win32 代码，适配 KernelFlux 组件库。

[English](README.md)

## 特性

- **异步日志写入** — 基于 mmap 的环形缓冲区，日志线程无阻塞 I/O
- **AES + ECDH 加密** — 日志静态加密，可选公钥进行密钥协商
- **控制台输出** — 独立于文件日志的控制台开关
- **`@autoclosure` Swift API** — 消息求值延迟到日志级别检查之后
- **文件分割** — 可配置最大文件大小和按时间轮转
- **模块级日志筛选** — `logFileURLs(matching:)` 按模块前缀收集日志文件
- **系统事件集成** — 通过 `KFSystemEventObserver` 在进后台/低内存时自动刷盘

## 安装

**Swift Package Manager**

```
https://github.com/kernelflux/kflog.git
```

或在 `Package.swift` 中：

```swift
.package(url: "https://github.com/kernelflux/kflog.git", from: "1.0.0")
```

按需添加 target：

| Product | 说明 | 依赖 |
|---------|------|------|
| `KFLog` | ObjC 封装 | `KFLogCore` |
| `KFLogAPI` | Swift 纯协议（零依赖） | 无 |
| `KFLogSwift` | Swift 扩展 + KFService 集成 | `KFLog`、`KFLogAPI`、`KFService` |

## 架构

```
KFLog
├── KFLogCore/           ← C/C++（mars xlog 核心，剥离 Android/Win32）
│   ├── comm/            ← 平台抽象、线程、mmap
│   └── log/             ← 日志引擎：缓冲区、加密、文件管理
├── KFLog/               ← ObjC 封装（KFLog.h, KFLog.mm）
├── KFLogAPI/            ← Swift 协议（KFLogger）+ KFLogLevel、KFLogMode
└── KFLogSwift/          ← KFLogDefault、KFConsoleLogger、KFCompositeLogger、KFLogModule
```

## 快速开始

### 配合 KFService 使用（推荐）

```swift
import KFService
import KFLogSwift

// App 启动时注册模块
KFServiceManager.register(module: KFLogModule(
    logDir: documentsPath,
    namePrefix: "MyApp",
    level: .info
))

// 获取实例并记录日志
let logger = KFServiceManager.resolve(KFLogger.self)
logger.info("用户已登录")
logger.error("网络超时", tag: "Networking")
```

### 独立使用（无需 KFService）

```swift
import KFLogSwift

let logger = KFLogDefault(
    mode: .async,
    logDir: documentsPath,
    namePrefix: "MyApp",
    level: .info
)

logger.info("App 已启动")
logger.debug("键 \(key) 缓存未命中")  // level > debug 时跳过
logger.flush()  // 强制写入磁盘

// 在文件日志之外同时输出到控制台
logger.setConsoleLog(true)
```

## 日志级别

```swift
public enum KFLogLevel: Int {
    case verbose = 0   // 全部日志
    case debug   = 1
    case info    = 2   // 发布版建议级别
    case warn    = 3
    case error   = 4
    case fatal   = 5
    case none    = 6   // 禁用所有日志
}
```

## API 参考

### KFLogger 协议

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

### 便捷方法（协议扩展）

```swift
logger.verbose("...")   // @autoclosure — level > verbose 时不求值
logger.debug("...")
logger.info("...")
logger.warn("...")
logger.error("...")
logger.fatal("...")
```

## 日志文件管理

```swift
// 所有日志文件
let urls = logger.logFileURLs

// 按模块前缀筛选
let networkLogs = logger.logFileURLs(matching: "MyApp_Networking")
```

## KFLogModule

注册到 KFService，处理生命周期：

```swift
KFLogModule(
    mode: .async,
    logDir: documentsPath,
    namePrefix: "MyApp",
    publicKey: nil,
    level: .info,
    priority: 200   // 在 KV 存储之后启动
)
```

模块遵循 `KFSystemEventObserver` —— 在 `onEnterBackground()` 和 `onMemoryWarning()` 时自动刷盘。

## 源文件结构

```
Sources/
├── Core/                 ← C/C++ 核心（fork 自 Tencent/mars xlog）
│   ├── comm/             ← 线程、mmap、平台抽象
│   └── log/              ← 日志缓冲区、AES/ECDH 加密、文件写入
├── KFLog/                ← ObjC 封装（KFLog.h, KFLog.mm）
├── KFLogAPI/             ← KFLogger 协议、KFLogLevel、KFLogMode
└── KFLogSwift/           ← KFLogDefault、KFConsoleLogger、KFLogModule
```

## 许可证

[MIT](LICENSE) — Copyright (c) 2016 THL A29 Limited (Tencent)，Copyright (c) 2026 KernelFlux

本项目 fork 自 [Tencent/mars](https://github.com/Tencent/mars)，继承其 MIT 许可证。源文件头部许可声明已统一合并到根目录 [LICENSE](LICENSE) 文件中。
