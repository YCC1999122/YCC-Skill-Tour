# TypeScript 学习项目

## 项目简介

本项目是一套完整的 TypeScript 学习教程，基于 Node.js 运行环境，使用 TypeScript 5.3 编译器与 ts-node 即时执行工具，从基础类型、函数类型到面向对象、泛型、装饰器、工具类型与设计模式逐步进阶。每个 `.ts` 文件都配有极其详细的逐行中文注释，从类型注解到类型守卫，从泛型约束到 `infer` 推断都一一说明，适合有 JavaScript 基础、希望系统掌握 TypeScript 的学习者。

通过本项目的学习，你可以掌握 TypeScript 的核心知识体系：结构化类型系统、接口与类型别名、联合/交叉类型、泛型编程、条件类型与映射类型、装饰器、声明文件，以及 TypeScript 特有的工具类型与"类型体操"，并能够独立编写类型安全、可维护性强的大型前端/Node.js 项目。

## 目录结构

```
ts-learning-project/
├── package.json                  # npm 项目配置（脚本命令、devDependencies：typescript/ts-node/@types/node）
├── tsconfig.json                 # TypeScript 编译配置（target/module/strict/装饰器/声明文件等，含逐行注释）
├── src/
│   ├── main.ts                   # 主程序入口（readline 交互菜单，选择运行各模块演示）
│   ├── basics/                   # 第1部分：基础语法
│   │   ├── types.ts              # 基础类型详解（原始类型、数组、元组、联合/交叉、接口、断言、守卫、字面量、枚举、any/unknown/never/void）
│   │   └── functions.ts          # 函数类型详解（参数/返回值类型、可选/默认/剩余参数、函数重载、箭头函数、this 类型、高阶函数）
│   ├── oop/
│   │   └── oop.ts                # 面向对象（class、构造器、参数属性、继承、修饰符、static、抽象类、implements、getter/setter、多态）
│   ├── generics/
│   │   └── generics.ts           # 泛型详解（泛型函数/接口/类、extends 约束、默认类型、条件类型、映射类型、infer 推断）
│   ├── advanced/
│   │   └── advanced.ts           # 高级特性（装饰器、namespace、.d.ts 声明、模块系统、结构类型兼容、模板字面量、keyof/typeof、索引访问、映射修饰符）
│   ├── utility/
│   │   └── utility.ts            # 工具类型与类型体操（手写 Partial/Required/Readonly/Pick 等内置工具类型的实现原理与练习）
│   └── patterns/
│       └── patterns.ts           # 设计模式（单例、工厂、观察者、策略、装饰器、代理，结合 TS 类型系统实现）
└── README_TS.md                  # 本文件
```

> 说明：`node_modules/`（npm 依赖目录）与 `dist/`（tsc 编译输出目录）为自动生成，已在上面的目录树中省略。

## 学习路径建议

### 第一阶段：基础类型（src/basics/types.ts）
- 运行菜单选项 1，配合源码逐行阅读

核心知识点：
- 原始类型：`string` / `number` / `boolean`，TS 中所有数字都是浮点数，支持二进制/八进制/十六进制字面量
- 数组类型两种写法：`number[]` 与 `Array<number>`
- 元组（Tuple）：固定长度与固定类型的数组，如 `[string, number]`
- 联合类型 `A | B` 与交叉类型 `A & B` 的区别与使用场景
- 类型别名 `type` 与接口 `interface` 的异同（接口可合并声明、别名可表示联合/元组等任意类型）
- 类型断言 `as` 与类型守卫（`typeof` / `instanceof` / `in` / 自定义守卫函数 `x is T`）
- 字面量类型、枚举 `enum`，以及 `any` / `unknown` / `never` / `void` 四个特殊类型的边界

### 第二阶段：函数类型（src/basics/functions.ts）
- 运行菜单选项 2，理解函数在 TS 中的完整类型描述

核心知识点：
- 参数类型与返回值类型的显式标注（返回值可由 `return` 自动推断）
- 可选参数 `name?: string`、默认参数 `name: string = 'x'`、剩余参数 `...args: T[]`
- 函数重载：多个函数签名对应一个实现，编译器按签名匹配
- 箭头函数的类型推断与 `this` 参数类型的显式声明
- 高阶函数类型：函数作为参数与返回值的类型表达（回调函数类型）

### 第三阶段：面向对象（src/oop/oop.ts）
- 运行菜单选项 3，对比 ES6 class 理解 TS 的增强

核心知识点：
- `class` 定义、属性类型注解、构造函数 `constructor`
- 参数属性语法糖：`constructor(private name: string)` 一步完成属性声明与赋值
- 继承 `extends` 与 `super`，方法重写 `override`
- 四种访问修饰符：`public` / `private` / `protected` / `readonly`
- 静态成员 `static`、抽象类 `abstract`、实现接口 `implements`
- `getter` / `setter` 访问器与多态

### 第四阶段：泛型（src/generics/generics.ts）
- 运行菜单选项 4，泛型是 TS 最重要的进阶门槛

核心知识点：
- 为什么需要泛型：`any` 会丢失类型信息，泛型保留输入输出类型关联
- 泛型函数、泛型接口、泛型类的定义与调用
- 泛型约束 `T extends U`：限制类型参数必须满足的形状
- 泛型默认类型 `T = string`
- 条件类型 `T extends U ? X : Y`：类型层面的三元表达式
- 映射类型 `{ [P in keyof T]: T[P] }` 与 `infer` 类型推断

### 第五阶段：高级特性（src/advanced/advanced.ts）
- 运行菜单选项 5，需在 tsconfig.json 中开启 `experimentalDecorators`

核心知识点：
- 装饰器：类装饰器、方法装饰器、属性装饰器、参数装饰器（`@expression` 语法）
- 命名空间 `namespace` 与声明文件 `.d.ts`、`declare` 关键字
- 模块系统：ES Module 与 CommonJS 的差异与互操作（`esModuleInterop`）
- 结构化类型系统：TS 的类型兼容看"形状"而非"名字"（鸭子类型）
- 模板字面量类型、`keyof` / `typeof` 操作符、索引访问类型 `T[K]`
- 映射类型修饰符：`+/- readonly`、`+/- ?`

### 第六阶段：设计模式与类型体操（src/patterns + src/utility）
- 运行菜单选项 6 与 7，将类型系统能力综合应用

核心知识点：
- 单例模式：私有构造函数 + 静态实例（如 ConfigurationManager 配置管理器）
- 工厂模式、观察者模式、策略模式、装饰器模式、代理模式的 TS 实现
- 手写内置工具类型：`MyPartial` / `MyRequired` / `MyReadonly` / `MyPick`，理解其映射类型原理
- 常用内置工具类型：`Partial` / `Required` / `Readonly` / `Pick` / `Omit` / `Record` / `Exclude` / `Extract` / `ReturnType`
- 类型体操：组合条件类型、映射类型、`infer` 完成复杂类型计算

## 开发环境

### 1. Node.js（必需）
- **用途**：JavaScript/TypeScript 的运行时环境，自带 npm 包管理器
- **下载地址**：https://nodejs.org/（建议 LTS 长期支持版）
- **验证安装**：`node -v` 与 `npm -v`
- **特点**：本项目使用 Node.js 内置的 `readline` 模块实现命令行交互菜单

### 2. TypeScript 编译器 tsc（项目依赖）
- **用途**：将 `.ts` 源码编译为 `.js`（本项目中已作为 devDependency 安装）
- **安装方式**：`npm install` 自动安装；也可全局安装 `npm install -g typescript`
- **常用命令**：`tsc`（编译）、`tsc --watch`（监听模式）、`tsc --init`（生成 tsconfig）
- **特点**：编译时进行完整的静态类型检查，类型错误在运行前即可发现

### 3. ts-node（项目依赖）
- **用途**：不经过编译直接运行 `.ts` 文件，适合学习与快速验证
- **使用方式**：`npm run dev`（等价于 `ts-node src/main.ts`）
- **特点**：省去"编译 → 运行"两步，改完立即执行

### 4. VS Code（推荐 IDE）
- **用途**：微软官方编辑器，对 TypeScript 的支持业内最佳（TS 同为微软出品）
- **下载地址**：https://code.visualstudio.com/
- **推荐扩展**：ESLint、Prettier、Error Lens（行内显示类型错误）
- **特点**：内置 IntelliSense 智能提示、类型悬停查看、跳转定义、自动重构，无需额外配置即可识别 tsconfig.json

### 5. 项目常用命令
| 命令 | 作用 |
|------|------|
| `npm install` | 安装 package.json 中声明的依赖 |
| `npm run dev` | 用 ts-node 直接运行交互菜单（推荐学习方式） |
| `npm run build` | 用 tsc 编译到 dist/ 目录 |
| `npm start` | 编译并运行 dist/main.js |
| `npm run watch` | 监听模式编译，保存即重新编译 |

## TypeScript 基础知识

### 与 JavaScript 的关系
- TypeScript 是 JavaScript 的**超集**：所有合法的 JS 代码都是合法的 TS 代码
- TS 在 JS 基础上添加了**静态类型系统**与部分未来语法（装饰器等），编译后还原为纯 JS
- 学习路径建议：先掌握 ES6+ 的 JS 基础（let/const、箭头函数、class、模块、Promise），再学 TS 事半功倍
- 类型信息在编译后被完全擦除（Type Erasure），运行时类型检查需借助类型守卫

### 类型系统核心
- **结构化类型（Structural Typing）**：类型兼容取决于结构形状，而非声明的名字——只要对象包含所需的属性和方法，就认为兼容
- **类型推断**：多数情况下 TS 能自动推断类型，无需处处标注；函数返回值、变量初始化通常可省略注解
- **严格模式**：`strict: true` 开启全部严格检查，其中最重要的是 `strictNullChecks`（null/undefined 不能随意赋值）与 `noImplicitAny`（禁止隐式 any）

### 常用工具类型速查
| 工具类型 | 作用 | 示例 |
|----------|------|------|
| `Partial<T>` | 所有属性变可选 | 更新部分字段的 DTO |
| `Required<T>` | 所有属性变必填 | 消除可选属性 |
| `Readonly<T>` | 所有属性只读 | 防止配置被修改 |
| `Pick<T, K>` | 挑选指定属性 | `Pick<User, 'id' \| 'name'>` |
| `Omit<T, K>` | 排除指定属性 | `Omit<User, 'password'>` |
| `Record<K, V>` | 构造键值对类型 | `Record<string, number>` |
| `Exclude<T, U>` | 从联合中排除 | `Exclude<'a'\|'b', 'a'>` |
| `ReturnType<F>` | 获取函数返回类型 | 配合 `typeof fn` 使用 |

### tsconfig.json 关键选项
| 选项 | 本项目取值 | 说明 |
|------|-----------|------|
| `target` | ES2020 | 编译后的 JS 版本 |
| `module` | commonjs | 模块系统（Node.js 默认） |
| `strict` | true | 开启全部严格类型检查（强烈推荐） |
| `outDir` / `rootDir` | ./dist / ./src | 输出/源码目录 |
| `esModuleInterop` | true | ES 模块与 CommonJS 互操作 |
| `experimentalDecorators` | true | 启用装饰器（高级特性章节必需） |
| `declaration` | true | 生成 .d.ts 声明文件 |
| `sourceMap` | true | 生成 source map 便于调试 |

## 学习方法建议

1. **逐行阅读注释**：本项目的核心价值在于每个 `.ts` 文件的逐行中文注释，注释中同时给出了"正确写法"与"错误写法"的对照，不要跳过被注释掉的错误示例。
2. **先跑起来再读**：先执行 `npm install && npm run dev`，通过菜单运行每个模块看输出，再回头读源码，理解会更立体。
3. **故意写错看报错**：把注释掉的错误代码取消注释，观察编译器报错信息，学会"读错误提示"是用好 TS 的关键技能。
4. **悬停看推断**：在 VS Code 中将鼠标悬停在变量/函数上，观察 TS 推断出的类型，培养对类型系统的直觉。
5. **循序渐进**：按六个阶段的顺序学习，泛型之前的章节是泛型的基础，泛型是工具类型与类型体操的基础，切忌跳跃。
6. **查官方手册**：遇到类型行为疑惑时查阅 TypeScript Handbook（有官方中文版），它比多数二手教程更准确。

## 注意事项

### 1. 不要为了通过检查滥用 any 与断言
- `any` 会关闭类型检查，让 TS 退化为 JS；`as` 断言是"告诉编译器相信我"，断言错了编译器不会救你
- 优先使用 `unknown` 代替 `any`（使用前必须收窄类型），优先用类型守卫代替断言
- 实在无法确定的类型，先标注 `unknown` 再逐步收窄，比一上来就 `any` 安全得多

### 2. type 与 interface 的选择
- 定义对象/类的契约优先用 `interface`（支持声明合并、被 `implements`，报错信息更友好）
- 表示联合类型、元组、函数类型、映射类型等只能用 `type`
- 团队项目中应统一约定，避免两者混用造成风格割裂

### 3. 类型在运行时不存在
- TS 类型在编译后被完全擦除，运行时无法获取接口、类型别名
- 需要运行时校验外部数据（如 API 返回）时，应配合类型守卫或 Zod 等校验库，不能依赖编译期类型
- `instanceof` 只能用于类，不能用于接口/类型别名

### 4. strict 模式不要关
- `strictNullChecks` 会在编译期拦截大量"undefined 不是函数"类的线上错误
- 历史项目迁移可临时放宽，但新项目应始终开启 `strict: true`
- 本项目 tsconfig.json 已开启全部严格选项，示例代码均在严格模式下通过编译

### 5. ts-node 仅用于开发学习
- ts-node 在运行时现场编译，启动慢、无缓存，不适合生产部署
- 生产环境应先 `tsc` 编译为 JS 再用 Node.js 运行（对应本项目 `npm start`）

## 学习资源推荐

### 推荐书籍
1. 《TypeScript 编程》- Boris Cherny（O'Reilly，系统深入）
2. 《Effective TypeScript》- Dan Vanderkam（62 条进阶建议）
3. 《TypeScript 入门教程》- 阮一峰（开源中文教程，适合入门）
4. 《TypeScript Deep Dive》- Basarat Ali Syed（开源电子书）
5. 《重构：改善既有代码的设计》（配合设计模式章节阅读）

### 在线教程网址
- TypeScript 官方文档（中文）：https://www.typescriptlang.org/zh/docs/
- TypeScript Playground（在线练习类型）：https://www.typescriptlang.org/play
- Type Challenges（类型体操题库）：https://github.com/type-challenges/type-challenges
- 阮一峰 TypeScript 教程：https://wangdoc.com/typescript/
- MDN JavaScript 参考（补 JS 基础）：https://developer.mozilla.org/zh-CN/docs/Web/JavaScript
- Node.js 官方文档：https://nodejs.org/docs/

---

**祝学习愉快！如有问题请参考各文件中的详细注释。**
