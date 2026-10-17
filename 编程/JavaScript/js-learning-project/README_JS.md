# JavaScript 学习项目

## 项目简介

本项目是一套完整的 JavaScript 语言学习教程，基于 Node.js 14+ 运行时（ES6+ 语法），从数据类型与运算符、函数与闭包、数组高阶方法、ES6+ 新特性到原型链面向对象、异步编程（Promise / async-await）、数据结构、设计模式逐步进阶。每个 `.js` 文件都配有极其详细的逐行中文注释，从 `var/let/const` 区别到事件循环宏微任务、从 `this` 指向到柯里化都一一说明，适合零基础入门到进阶学习。

项目提供 `main.js` 交互式菜单主程序，运行 `node main.js`（或 `npm start`）后可按数字选择进入任意模块的演示，`package.json` 中还为每个模块预置了 `npm run` 快捷脚本。通过本项目的学习，你可以掌握 JavaScript 的核心知识体系：动态弱类型、原型与原型链、闭包与高阶函数、事件循环、Promise 异步模型、ES6+ 现代语法、模块化等，并能够独立编写结构清晰、符合现代规范的中型 JavaScript 程序。

## 目录结构

```
js-learning-project/
├── package.json                        # 项目配置文件（元信息、npm scripts 快捷脚本、engines 版本要求）
├── main.js                             # 主程序入口（readline 交互式菜单，require 导入各模块演示函数）
├── basics/                             # 第1章：基础语法
│   ├── 01_types_and_operators.js       # 类型与运算符（8 种数据类型、隐式/显式转换、== vs ===、var/let/const、解构、模板字符串）
│   └── 02_functions.js                 # 函数详解（三种定义方式、默认/rest/解构参数、闭包、高阶函数、柯里化、this 指向）
├── data/                               # 第2章：数据结构
│   └── structures.js                   # 数据结构与算法（链表、栈、队列、二叉搜索树、冒泡/选择/插入/快速/归并排序）
├── oop/                                # 第3章：面向对象编程
│   └── oop.js                          # 原型与类（原型链、构造函数、ES6 Class、extends/super、getter/setter、#私有字段）
├── async/                              # 第4章：异步编程
│   └── async_programming.js            # 异步详解（回调地狱、Promise 链式调用、async/await、Promise.all 并发、事件循环）
├── advanced/                           # 第5章：高级特性
│   ├── array_methods.js                # 数组高阶方法（map/filter/reduce/find/some/every/sort、链式调用）
│   └── es6_features.js                 # ES6+ 特性（Promise、Symbol、Iterator、Generator、Proxy/Reflect、Map/Set、模块化）
├── patterns/                           # 第6章：设计模式
│   └── design_patterns.js              # 设计模式（单例、工厂、观察者/发布订阅、策略、装饰器、代理）
└── README_JS.md                        # 本文件
```

## 学习路径建议

### 第一阶段：基础语法（basics）
- `01_types_and_operators.js` - 数据类型、类型转换、运算符、控制流、作用域、解构赋值
- `02_functions.js` - 函数定义、参数、闭包、高阶函数、柯里化、this 指向

核心知识点：
- JavaScript 是动态**弱类型**语言：变量类型运行时确定，运算时可能发生隐式转换
- 8 种数据类型：Number / String / Boolean / Undefined / Null / Symbol / BigInt（原始类型）+ Object
- `==`（宽松相等，会做类型转换）vs `===`（严格相等）——**永远优先使用 ===**
- `var`（函数作用域、有提升）vs `let`/`const`（块级作用域、暂时性死区）——现代代码禁用 var
- 解构赋值、模板字符串（反引号 + `${}`）、扩展运算符 `...` 与剩余参数
- 函数三种定义方式：函数声明（有提升）、函数表达式、箭头函数
- **闭包**：函数记住其定义时的词法作用域，是 JS 最重要的概念之一
- 高阶函数、柯里化（Currying）与偏函数应用
- `this` 指向规则：默认绑定、隐式绑定、显式绑定（call/apply/bind）、new 绑定；箭头函数无自己的 this

### 第二阶段：数据结构（data）
- `structures.js` - 链表、栈、队列、二叉搜索树、五种排序

核心知识点：
- 用 class 实现节点与链表的插入、删除、遍历
- 用数组实现栈（push/pop）与队列（push/shift）
- 二叉搜索树的插入、查找与中序遍历
- 冒泡、选择、插入、快速、归并排序的实现与复杂度对比
- 时间复杂度 O(n)、O(n²)、O(n log n) 的直观理解

### 第三阶段：面向对象编程（oop）
- `oop.js` - 原型、原型链、构造函数、ES6 Class、继承、私有字段

核心知识点：
- JavaScript 是**基于原型**（prototype-based）的面向对象，而非基于类
- `prototype`（函数的原型对象）与 `__proto__`（对象的原型链指针）的关系
- **原型链**：属性查找沿 `__proto__` 逐级向上，直到 Object.prototype 或 null
- 构造函数 + new 的四个步骤（创建空对象、链接原型、绑定 this、返回对象）
- ES6 `class` 是原型机制的语法糖：`constructor`/`extends`/`super`/`static`
- getter/setter 访问器属性；`#` 前缀的真正私有字段（ES2022）
- 继承与多态在原型体系下的实现

### 第四阶段：异步编程（async）
- `async_programming.js` - 回调、Promise、async/await、并发方法、事件循环

核心知识点：
- JavaScript 单线程，异步依靠**事件循环**（Event Loop）调度
- 回调函数与"回调地狱"（Callback Hell）问题
- **Promise** 三种状态（pending/fulfilled/rejected）与链式调用（then/catch/finally）
- `async/await` 是 Promise 的语法糖，让异步代码看起来像同步
- 并发控制：`Promise.all`（全部成功）/`race`（竞速）/`allSettled`（全部 settle）/`any`（任一成功）
- **宏任务**（setTimeout、setInterval、I/O）与**微任务**（Promise.then、queueMicrotask）的执行顺序：每轮宏任务后清空微任务队列
- 错误处理：try/catch（同步与 async 内）vs `.catch()`（Promise 链）

### 第五阶段：高级特性（advanced）
- `array_methods.js` - 数组高阶方法与链式调用
- `es6_features.js` - Symbol、Iterator、Generator、Proxy/Reflect、Map/Set、模块化

核心知识点：
- 函数式数组方法：`map`（映射）、`filter`（过滤）、`reduce`（归约）、`find`/`some`/`every`、`sort`
- 链式调用组合多个方法表达复杂数据变换，是现代 JS 的标志性风格
- `Symbol` 创建唯一标识；`Iterator` 迭代协议与 `for...of` 的原理
- **Generator**（`function*` + `yield`）：可暂停恢复的函数，惰性序列与手动迭代
- `Proxy` 拦截对象操作（get/set/has 等 13 种陷阱），`Reflect` 提供对应默认行为——Vue3 响应式的底层
- `Map`/`Set`/`WeakMap`/`WeakSet` 与 Plain Object/Array 的适用场景对比
- CommonJS（require/module.exports）vs ES Module（import/export）两种模块系统

### 第六阶段：设计模式（patterns）
- `design_patterns.js` - 单例、工厂、观察者、策略、装饰器、代理

核心知识点：
- 单例模式：模块缓存天然单例，也可用闭包/类静态属性实现
- 工厂模式：函数封装对象创建，屏蔽 new 的细节
- 观察者模式（发布-订阅）：DOM 事件与 Node.js EventEmitter 的同型模式
- 策略模式：用对象字面量映射策略，替代冗长的 if-else 链
- 装饰器模式：高阶函数包装原函数增强功能
- 代理模式：用 ES6 Proxy 实现拦截与惰性求值

## 开发环境

### 1. Node.js 运行时
- **用途**：基于 Chrome V8 引擎的 JavaScript 服务端运行时，让 JS 脱离浏览器运行
- **下载地址**：https://nodejs.org/zh-cn（建议 LTS 长期支持版）
- **版本建议**：14.0.0 及以上（本项目 package.json 中 engines 要求 >=14.0.0，推荐 18+ LTS）
- **特点**：事件驱动、非阻塞 IO、与 npm 生态无缝集成；自带 V8 对 ES6+ 支持完善
- **验证安装**：命令行执行 `node -v` 与 `npm -v`
- **运行方式**：`node 文件名.js`；`node` 进入 REPL 交互式环境

### 2. npm 包管理器
- **用途**：Node.js 官方包管理器，管理 package.json 与 node_modules，访问全球最大包仓库 npmjs.com
- **常用命令**：
  - `npm install 包名` / `npm install` - 安装依赖 / 按 package.json 安装全部依赖
  - `npm run 脚本名` - 运行 package.json 中 scripts 定义的脚本（本项目预置了 start/basics/oop/advanced/async/patterns/data 七个脚本）
  - `npm init -y` - 快速初始化新项目
- **国内加速**：`npm config set registry https://registry.npmmirror.com`
- **说明**：本项目零依赖（dependencies 为空），clone 后无需 npm install 即可直接运行
- **替代品**：yarn（https://yarnpkg.com/）、pnpm（https://pnpm.io/zh/）按需了解

### 3. 浏览器开发者工具（控制台）
- **打开方式**：Chrome/Edge/Firefox 中按 F12 或 Ctrl+Shift+I，切到 Console 面板
- **用途**：随写随试 JS 代码、查看对象结构、调试断点（Sources 面板）、分析网络请求
- **特点**：浏览器环境独占 DOM/BOM API（document、window），是学习前端 JS 的必备工具
- **小技巧**：`console.table()` 表格化输出数组、`console.dir()` 查看对象原型链

### 4. IDE 推荐
- **VS Code**（https://code.visualstudio.com/）：JavaScript 开发的事实标准编辑器，内置 JS/TS 语言服务（智能补全、跳转、重构），插件生态丰富（ESLint、Prettier、Debugger for Node），免费轻量
- **WebStorm**（https://www.jetbrains.com/webstorm/）：JetBrains 出品的前端专业 IDE，功能全面，收费软件
- **配置建议**：安装 ESLint（代码检查）+ Prettier（格式化）并开启"保存时自动修复/格式化"

### 5. 在线试验场
- MDN Playground：https://developer.mozilla.org/zh-CN/play
- CodePen：https://codepen.io/（前端 HTML/CSS/JS 即时预览）
- JSFiddle / StackBlitz：免安装在线运行与分享代码

## JavaScript 基础知识

### JavaScript 语言特点速览
| 特性 | 说明 |
|------|------|
| 类型系统 | 动态类型、**弱类型**（存在隐式类型转换，如 `"1" + 2 === "12"`） |
| 执行方式 | JIT 编译执行（V8 引擎：解释 + 即时编译热点代码） |
| 运行环境 | 浏览器（DOM/BOM）与 Node.js（fs/http 等）双宿主 |
| 并发模型 | 单线程 + 事件循环（Event Loop），异步非阻塞 |
| 面向对象 | 基于原型（prototype），ES6 class 为语法糖 |
| 函数地位 | 函数是一等公民：可赋值、传参、返回、闭包 |
| 标准演进 | ECMAScript 每年一版：ES6(2015) 是分水岭，之后按年份命名 ES2016~ES2024 |
| 模块系统 | CommonJS（Node 传统，require）与 ES Module（标准，import/export）并存 |

### var / let / const 对比
| 维度 | var | let | const |
|------|-----|-----|-------|
| 作用域 | 函数作用域 | 块级作用域 | 块级作用域 |
| 变量提升 | 有（初始化为 undefined） | 有但未初始化（暂时性死区） | 同 let |
| 重复声明 | 允许 | 禁止 | 禁止 |
| 重新赋值 | 允许 | 允许 | **禁止**（但对象属性可改） |
| 全局挂载 | 挂到 window | 不挂载 | 不挂载 |

- 现代规范：**默认 const，需要重新赋值才用 let，禁用 var**

### 闭包（Closure）
- 定义：函数 + 其定义时的词法作用域的组合；内层函数即使在外层函数返回后，仍能访问外层变量
- 用途：数据私有化（模块模式）、函数工厂、柯里化、记忆化缓存
- 代价：被闭包引用的变量无法被垃圾回收，滥用导致内存占用升高
- 经典面试题：循环中 var 捕获共享变量 vs let 每轮新绑定

### 事件循环（Event Loop）
- JS 引擎单线程执行，异步任务由宿主环境（浏览器/Node）的线程池处理，完成后回调进入任务队列
- 执行顺序：**同步代码 → 清空微任务队列 → 取一个宏任务执行 → 再清空微任务 → 循环**
- 微任务（Microtask）：Promise.then/catch/finally、queueMicrotask、MutationObserver
- 宏任务（Macrotask）：setTimeout/setInterval、setImmediate（Node）、I/O、UI 渲染
- 理解事件循环是预判 `setTimeout(fn, 0)` 与 `Promise.resolve().then(fn)` 输出顺序的关键

### ES6+ 核心特性速查
| 特性 | 版本 | 说明 |
|------|------|------|
| let / const | ES2015 | 块级作用域变量声明 |
| 箭头函数 | ES2015 | 简洁语法，词法 this |
| 解构赋值 | ES2015 | 从数组/对象批量提取值 |
| 模板字符串 | ES2015 | 反引号 + `${}` 插值、多行字符串 |
| class | ES2015 | 原型继承的语法糖 |
| Promise | ES2015 | 异步编程标准方案 |
| 模块化 import/export | ES2015 | 官方模块系统 |
| async / await | ES2017 | Promise 语法糖，同步式写异步 |
| 扩展运算符 ... | ES2015/ES2018 | 数组/对象的展开与收集 |
| 可选链 ?. / 空值合并 ?? | ES2020 | 安全访问嵌套属性、默认值 |
| #私有字段 | ES2022 | class 内真正的私有成员 |

## 学习方法建议

1. **逐行阅读注释**：本项目的核心价值在于每个 `.js` 文件的逐行中文注释，不要跳过，特别是文件头说明各知识点的 JSDoc 块注释。
2. **先跑起来再改**：先 `node main.js` 通过菜单运行各模块观察输出，再修改参数（如 setTimeout 延时、Promise 顺序）重新运行，预判结果再验证——这是掌握事件循环的最佳练习。
3. **双环境对照**：同一段代码分别在 Node 和浏览器控制台运行，体会两个宿主的差异（如全局对象 global vs window）。
4. **动手默写**：看完一个模块后，合上书把核心代码（如手写 Promise、闭包计数器、柯里化 add、发布订阅）默写一遍，面试高频题尽在其中。
5. **循序渐进**：按 6 个阶段的顺序学习；Promise 依赖函数与回调知识、class 依赖原型知识，跳跃学习容易断层。
6. **查 MDN 文档**：MDN Web Docs 是 JS 最权威的参考（有完整中文），遇到 API 用法问题养成查 MDN 而非博客的习惯。

## 注意事项

### 1. 隐式类型转换的坑
- `"1" + 2` 得 `"12"`（字符串拼接），`"1" - 2` 得 `-1`（数值减法）——运算符决定转换方向
- `[] + []` 得 `""`、`[] + {}` 得 `"[object Object]"` 等怪异结果都源于隐式转换
- 防御方法：始终使用 `===` 严格比较；显式转换用 `Number()`/`String()`/`Boolean()`
- `NaN !== NaN`，判断 NaN 用 `Number.isNaN()`

### 2. this 指向问题
- 普通函数的 this 在**调用时**动态决定：方法调用指向对象、独立调用指向 undefined（严格模式）或全局对象
- 箭头函数没有自己的 this，捕获定义时外层作用域的 this——回调函数优先用箭头函数
- `call`/`apply`/`bind` 显式指定 this；bind 返回绑定后的新函数
- 事件处理器、setTimeout 回调、解构提取方法是 this 丢失的三大高发场景

### 3. 异步代码的执行顺序
- `setTimeout(fn, 0)` 不是立即执行，而是等当前同步代码与微任务全部完成后才执行
- `Promise` 的 executor 是**同步**执行的，只有 then 回调进微任务队列
- async 函数中 await 之后的代码相当于微任务
- 不要假设异步回调的执行顺序与代码书写顺序一致，用 await 或 Promise.all 显式编排

### 4. 引用类型与拷贝
- Object、Array 是引用类型，赋值/传参复制的是引用，修改会互相影响
- 浅拷贝：`{...obj}`、`Object.assign({}, obj)`、`[...arr]`、`arr.slice()`——只复制第一层
- 深拷贝：`structuredClone(obj)`（Node 17+/现代浏览器）或 `JSON.parse(JSON.stringify(obj))`（有局限：丢失函数、undefined、循环引用报错）
- `const` 只保证引用不变，不保证对象内容不变

### 5. Node 与浏览器环境差异
- 浏览器独有：window、document、DOM API、alert/fetch（Node 18+ 也内置了 fetch）
- Node 独有：process、global、Buffer、fs/path/os 等内置模块、CommonJS require
- 本项目模块使用 CommonJS（require/module.exports）；浏览器与现代工具链使用 ES Module
- `'use strict'` 严格模式能暴露潜在错误（如未声明变量赋值），本项目所有模块均已开启，建议养成习惯

## 学习资源推荐

### 推荐书籍
1. 《JavaScript 高级程序设计（第 4 版）》- Matt Frisbie（"红宝书"，全面权威）
2. 《JavaScript 权威指南（第 7 版）》- David Flanagan（"犀牛书"，案头参考）
3. 《你不知道的 JavaScript》（上/中/下）- Kyle Simpson（深入作用域、this、原型、异步）
4. 《JavaScript 语言精粹（修订版）》- Douglas Crockford（"蝴蝶书"，取其精华）
5. 《ES6 标准入门（第 3 版）》- 阮一峰（中文 ES6 最佳教程，有免费在线版）

### 在线教程网址
- MDN Web Docs（最权威参考）：https://developer.mozilla.org/zh-CN/docs/Web/JavaScript
- 现代 JavaScript 教程（The Modern JavaScript Tutorial，高质量免费）：https://zh.javascript.info/
- 阮一峰 ES6 入门教程（在线免费）：https://es6.ruanyifeng.com/
- 菜鸟教程 JavaScript：https://www.runoob.com/js/js-tutorial.html
- Node.js 官方文档：https://nodejs.org/zh-cn/docs
- ECMAScript 官方规范（进阶）：https://tc39.es/ecma262/

---

**祝学习愉快！如有问题请参考各文件中的详细注释。**
