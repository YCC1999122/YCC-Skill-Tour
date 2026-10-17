# Go 学习项目

## 项目简介

本项目是一套完整的 Go 语言学习教程，基于 Go 1.21 工具链，从基础语法、数据结构、面向对象（struct + method + interface）到 defer/panic/recover、反射、泛型、goroutine 与 channel 并发编程、设计模式逐步进阶。每个 `.go` 文件都配有极其详细的逐行中文注释，从包声明到接口隐式实现、从 CSP 并发哲学到 sync 原语都一一说明，适合零基础入门到进阶学习。

项目提供 `main.go` 交互式菜单主程序，运行 `go run main.go` 后可按数字选择进入任意模块的演示。通过本项目的学习，你可以掌握 Go 的核心知识体系：静态强类型、包管理与 go mod、结构体与嵌入、接口隐式实现、错误处理惯例、goroutine 轻量线程、channel 通信、select 多路复用、context 取消传播等，并能够独立编写简洁、高效、可并发的中型 Go 程序。

## 目录结构

```
go-learning-project/
├── go.mod                          # 模块定义文件（声明模块名与 Go 版本，本项目仅使用标准库）
├── main.go                         # 主程序入口（main 包，交互式菜单，bufio 读取用户输入）
├── basics/                         # 第1章：基础语法
│   └── basics.go                   # 基础语法（数据类型、运算符、控制流、函数、切片与 map）
├── structures/                     # 第2章：数据结构
│   └── structures.go               # 数据结构与算法（链表、栈、队列、二叉搜索树、堆、排序）
├── oop/                            # 第3章：面向对象编程
│   └── oop.go                      # struct+method+interface（方法、接口、类型断言、嵌入、error）
├── concurrency/                    # 第4章：并发编程
│   └── concurrency.go              # goroutine、channel、select、sync、context、atomic
├── advanced/                       # 第5章：高级特性
│   └── advanced.go                 # defer/panic/recover、自定义错误、反射、闭包、泛型、embed
├── patterns/                       # 第6章：设计模式
│   └── patterns.go                 # 设计模式（单例、工厂、观察者、策略、装饰器、选项模式）
└── README_GO.md                    # 本文件
```

## 学习路径建议

### 第一阶段：基础语法（basics）
- `basics.go` - 数据类型、运算符、控制流、函数、切片与 map

核心知识点：
- Go 是静态强类型语言，编译时检查类型；`:=` 短变量声明自动推导类型
- 包（package）是代码组织单元，目录名即包名；可执行程序必须有 `main` 包和 `main` 函数
- 访问控制机制：首字母大写导出（public），首字母小写包内私有，没有 public/private 关键字
- 切片（slice）是对数组的引用视图，len/cap 双属性，append 动态扩容
- map 是内置哈希表，必须用 `make` 初始化；遍历时顺序随机
- `for` 是唯一的循环关键字（无 while），`for range` 遍历切片/map/字符串
- 函数支持多返回值，惯用 `(result, error)` 返回错误

### 第二阶段：数据结构（structures）
- `structures.go` - 链表、栈、队列、二叉搜索树、堆、排序

核心知识点：
- 用 struct + 指针实现链表节点的插入、删除、遍历
- 用切片实现栈与队列（append 入栈、切片弹出）
- 二叉搜索树的递归插入与中序遍历
- 标准库 `container/heap` 实现堆/优先队列（需实现 heap.Interface 接口）
- 冒泡、选择、插入、快速排序的实现与复杂度对比

### 第三阶段：面向对象编程（oop）
- `oop.go` - struct、方法、接口、类型断言、嵌入、error

核心知识点：
- Go 没有 class 与继承，用 struct + method + interface 实现面向对象
- 方法 = 带接收者（receiver）的函数；值接收者 vs 指针接收者的区别
- **接口隐式实现**：类型只要实现了接口的全部方法即满足接口，无需 `implements` 关键字
- 空接口 `interface{}`（或 `any`）可持有任意类型，配合类型断言（type assertion）还原具体类型
- 结构体嵌入（embedding）实现"组合优于继承"的代码复用
- `fmt.Stringer` 接口（String() 方法）定制打印格式
- `error` 是内置接口（Error() string），Go 错误处理的核心

### 第四阶段：并发编程（concurrency）
- `concurrency.go` - goroutine、channel、select、sync、context、atomic

核心知识点：
- goroutine 是 Go 运行时调度的轻量级线程，`go func()` 一行启动，初始栈仅几 KB
- Go 并发哲学："不要通过共享内存来通信，而要通过通信来共享内存"（CSP 模型）
- channel（通道）是 goroutine 间的类型安全管道；无缓冲 channel 同步，带缓冲 channel 异步
- `select` 多路复用多个 channel 操作，配合 `default` 实现非阻塞收发
- `sync.WaitGroup` 等待一组 goroutine 完成；`sync.Mutex` 互斥锁保护共享状态
- `sync.Once` 保证只执行一次（单例模式基础）；`atomic` 包提供原子操作
- `context.Context` 跨 goroutine 传递取消信号与超时控制

### 第五阶段：高级特性（advanced）
- `advanced.go` - defer/panic/recover、自定义错误、反射、闭包、泛型、embed

核心知识点：
- `defer` 延迟执行（LIFO 栈序），常用于资源释放；`panic`/`recover` 处理不可恢复错误
- 自定义错误：`errors.New`、`fmt.Errorf("%w")` 包装错误链、`errors.Is/As` 判断错误
- 反射（reflect 包）在运行时检查类型与值，ORM、序列化库的底层机制，应谨慎使用
- 闭包捕获外部变量，函数作为一等公民可赋值、传参、返回
- 泛型（Go 1.18+）用类型参数编写通用容器与算法，类型约束（constraint）限定可用类型
- `embed` 包将静态文件嵌入二进制，单文件部署利器

### 第六阶段：设计模式（patterns）
- `patterns.go` - 单例、工厂、观察者、策略、装饰器、选项模式

核心知识点：
- 单例模式：`sync.Once` 是 Go 中最可靠的单例实现方式
- 工厂模式：返回接口而非具体类型，调用方只依赖抽象
- 观察者模式：channel 与回调函数实现发布-订阅
- 策略模式：用函数类型替代策略接口，更简洁
- 装饰器模式：高阶函数包装处理逻辑（中间件的思想）
- 函数选项模式（Functional Options）：Go 社区处理可选配置的经典惯用法

## 开发环境

### 1. Go 工具链
- **用途**：Go 官方工具链，集编译器（go build）、运行器（go run）、测试器（go test）、包管理（go get）于一体
- **下载地址**：https://go.dev/dl/（国内镜像 https://golang.google.cn/dl/）
- **版本建议**：1.21 及以上（本项目 go.mod 声明 go 1.21，泛型需 1.18+）
- **特点**：编译为原生机器码、单二进制部署、交叉编译简单（`GOOS=linux GOARCH=amd64 go build`）
- **验证安装**：命令行执行 `go version`
- **国内加速**：`go env -w GOPROXY=https://goproxy.cn,direct`（七牛云代理）

### 2. go mod 模块管理
- **用途**：Go 1.11 引入的官方依赖管理系统，取代旧的 GOPATH 模式
- **常用命令**：
  - `go mod init 模块名` - 初始化模块（生成 go.mod）
  - `go mod tidy` - 自动添加缺失依赖、移除无用依赖
  - `go get 包路径@版本` - 添加/升级依赖
  - `go build` / `go run main.go` - 编译 / 直接运行
- **说明**：本项目只使用标准库，go.mod 中无任何外部依赖

### 3. gofmt / go vet 代码工具
- **gofmt**：官方代码格式化工具，`gofmt -w .` 一键统一风格（Tab 缩进），Go 社区"无格式之争"的根基
- **go vet**：官方静态检查工具，`go vet ./...` 发现常见错误（如 Printf 参数不匹配、复制锁）
- **golangci-lint**（可选）：聚合多种 linter 的增强检查工具，团队项目推荐

### 4. IDE 推荐
- **VS Code + Go 插件**（https://code.visualstudio.com/）：安装 Go 扩展后会提示安装 gopls（官方语言服务器），提供智能补全、跳转定义、自动导入、调试，轻量高效，Go 官方推荐
- **GoLand**（https://www.jetbrains.com/go/）：JetBrains 出品的 Go 专业 IDE，功能全面（重构、调试、测试覆盖、数据库工具），收费软件，适合重度 Go 开发
- **说明**：保存时自动 gofmt 是基本配置，请在 IDE 中开启

### 5. Go Playground（在线试验）
- **网址**：https://go.dev/play/
- 无需安装环境即可在浏览器中编写、运行、分享 Go 代码，学习语法时随写随试的好帮手

## Go 基础知识

### Go 语言特点速览
| 特性 | 说明 |
|------|------|
| 类型系统 | 静态类型、强类型，编译时类型检查 |
| 编译执行 | 编译为原生机器码，启动快、部署简单（单二进制） |
| 垃圾回收 | 内置 GC，自动内存管理（低延迟 STW） |
| 并发模型 | CSP 模型：goroutine + channel，语言级原生支持 |
| 代码组织 | package 包机制，import 路径导入，go mod 管理依赖 |
| 命名规范 | 驼峰命名；首字母大小写决定导出性（唯一访问控制机制） |
| 设计哲学 | 简单、明确、组合优于继承、"少即是多"（Less is more） |

### 基本数据类型
| 分类 | 类型 | 说明 |
|------|------|------|
| 布尔 | bool | true / false，不参与数值运算 |
| 有符号整数 | int / int8 / int16 / int32 / int64 | int 宽度随平台（32 或 64 位） |
| 无符号整数 | uint / uint8(byte) / uint16 / uint32 / uint64 | byte 是 uint8 别名 |
| 浮点 | float32 / float64 | 默认 float64 |
| 复数 | complex64 / complex128 | 科学计算用 |
| 字符串 | string | 不可变，UTF-8 编码；rune（int32）表示 Unicode 码点 |
| 引用类型 | slice / map / channel / 指针 / 接口 | make 初始化，零值多为 nil |

### 接口隐式实现
- Go 接口是**隐式满足**的：类型实现了接口的所有方法即自动满足该接口，无需声明
- 优点：解耦定义与实现，可为已有类型"事后"添加接口抽象
- 惯用法：接口应该小（1~3 个方法），"接口越大，抽象越弱"；`io.Reader`、`io.Writer` 是单方法接口的典范
- 类型断言 `v, ok := i.(T)` 与类型开关 `switch v := i.(type)` 用于接口值还原

### defer 执行规则
- `defer` 注册的函数在当前函数返回前执行，多个 defer 按 **LIFO（后进先出）** 顺序执行
- defer 的参数在**注册时**立即求值，而非执行时
- 典型用途：关闭文件、释放锁、记录耗时、recover 捕获 panic
- defer 有一定的性能开销，热路径中慎用（Go 1.14 后开销已大幅降低）

### goroutine 与 channel 要点
- goroutine 由 Go 运行时调度（GMP 模型），而非操作系统，可轻松创建数十万协程
- 无缓冲 channel：发送方阻塞直到接收方就绪（同步）；带缓冲 channel：缓冲满才阻塞
- 向已关闭的 channel 发送会 panic；从已关闭 channel 接收返回零值且 ok=false
- channel 应由**发送方**关闭，接收方通过 `v, ok := <-ch` 或 `for range ch` 感知关闭
- goroutine 泄漏是常见生产事故：确保每个 goroutine 都有退出路径（context 取消、关闭 channel）

## 学习方法建议

1. **逐行阅读注释**：本项目的核心价值在于每个 `.go` 文件的逐行中文注释，不要跳过，特别是文件开头说明包职责与设计理念的块注释。
2. **先跑起来再改**：先 `go run main.go` 通过菜单运行各模块观察输出，再修改参数（如 channel 缓冲大小、goroutine 数量）重新运行，对比结果差异。
3. **用竞态检测器验证并发代码**：`go run -race main.go` 启用数据竞争检测，是排查并发 bug 的神器。
4. **动手默写**：看完一个模块后，合上书把核心代码（如接口定义、worker pool、选项模式）默写一遍。
5. **循序渐进**：按 6 个阶段的顺序学习；接口依赖 struct 与 method、并发依赖基础语法，跳跃学习容易断层。
6. **阅读标准库源码**：Go 标准库是最好的学习材料（如 fmt、sort、net/http），代码简洁、注释规范，遇到用法问题先查标准库文档。

## 注意事项

### 1. 值语义与指针语义
- Go 中函数传参总是**值拷贝**；传结构体拷贝整个结构体，传指针（或 slice/map/channel）才能共享修改
- 方法定义时：需要修改接收者或结构体较大时用指针接收者；一致原则：一个类型的方法集不要混用值/指针接收者
- 指针接收者的方法同时属于值与指针的方法集，值接收者的方法只属于值的方法集（影响接口赋值）

### 2. 错误处理惯例
- Go 没有 try/catch，错误作为普通值返回：`if err != nil { return err }` 是标准模式
- 不要忽略错误返回值（`_` 丢弃 err 是代码异味），也不要滥用 panic
- panic 只用于程序无法继续的致命错误；可预期错误一律返回 error
- Go 1.13+ 用 `fmt.Errorf("...: %w", err)` 包装错误链，`errors.Is/As` 向上追溯

### 3. 切片 append 的扩容陷阱
- `append` 容量不足时会重新分配底层数组，原切片与新切片从此互不影响
- 多个切片共享底层数组时，一处修改可能影响另一处，函数间传递切片子区间需警惕
- 预分配容量 `make([]T, 0, n)` 可避免反复扩容，提升性能

### 4. map 使用注意
- map 不是并发安全的：多 goroutine 同时读写会触发 fatal error，需用 sync.Mutex 或 sync.Map 保护
- 读取不存在的 key 返回零值，用 `v, ok := m[key]` 区分"不存在"与"零值"
- map 遍历顺序随机，不要依赖遍历顺序

### 5. goroutine 泄漏与关闭
- goroutine 不会因父函数返回而自动结束，必须有明确退出机制
- 向无接收者的 channel 发送、从无发送者的 channel 接收会导致 goroutine 永久阻塞（泄漏）
- 使用 `context.WithCancel` 传播取消信号、close(channel) 广播关闭，是两种主流退出模式
- main 函数返回时所有 goroutine 被强制终止，注意用 WaitGroup 等待任务完成

## 学习资源推荐

### 推荐书籍
1. 《Go 程序设计语言》（The Go Programming Language）- Alan Donovan & Brian Kernighan（Go 圣经，必读）
2. 《Go 语言实战》- William Kennedy（入门到进阶平滑过渡）
3. 《Go 并发编程实战（第 2 版）》- 郝林
4. 《Go 语言高级编程》- 柴树杉、曹春晖（进阶：CGO、汇编、RPC）
5. 《Effective Go》（官方文档，免费，必读）

### 在线教程网址
- Go 官方网站：https://go.dev/（国内镜像 https://golang.google.cn/）
- Go 官方教程 A Tour of Go：https://go.dev/tour/
- Go 标准库文档：https://pkg.go.dev/std
- Effective Go 中文翻译：https://go.dev/doc/effective_go
- 菜鸟教程 Go 语言：https://www.runoob.com/go/go-tutorial.html
- Go 语言圣经中文版（开源书）：https://gopl-zh.github.io/
- Go by Example（英文，示例驱动）：https://gobyexample.com/

---

**祝学习愉快！如有问题请参考各文件中的详细注释。**
