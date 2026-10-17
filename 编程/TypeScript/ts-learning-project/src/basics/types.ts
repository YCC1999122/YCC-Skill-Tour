/**
 * ============================================================
 *  TypeScript 基础类型详解
 * ============================================================
 *
 * 本文件详细介绍 TypeScript 中的各种基础类型，包括：
 * 1. 原始类型（Primitive Types）
 * 2. 数组类型（Array Types）
 * 3. 元组类型（Tuple Types）
 * 4. 联合类型与交叉类型（Union & Intersection）
 * 5. 类型别名（Type Aliases）
 * 6. 接口（Interfaces）
 * 7. 类型断言（Type Assertions）
 * 8. 类型守卫（Type Guards）
 * 9. 字面量类型与枚举（Literal Types & Enums）
 * 10. any / unknown / never / void 类型
 * ============================================================
 */

// ============================================================
//  1. 原始类型（Primitive Types）
// ============================================================

// string 类型：表示字符串值
// 可以使用单引号、双引号或模板字符串（反引号）
let str: string = 'Hello TypeScript';          // 普通字符串
let str2: string = "你好，TypeScript";          // 双引号字符串
let str3: string = `模板字符串：${str}`;        // 模板字符串，支持变量插值

// number 类型：表示数字值
// TypeScript 中所有数字都是浮点数（没有 int 和 float 的区别）
// 支持十进制、二进制、八进制、十六进制表示法
let num: number = 42;                          // 十进制整数
let float: number = 3.14;                      // 浮点数
let binary: number = 0b1010;                   // 二进制（0b 前缀）
let octal: number = 0o744;                     // 八进制（0o 前缀）
let hex: number = 0xff;                        // 十六进制（0x 前缀）

// boolean 类型：表示布尔值（真/假）
let isDone: boolean = false;                   // 布尔值 false
let isActive: boolean = true;                  // 布尔值 true

// null 类型：表示空值
// null 是一个只有一个值的类型，即 null
// 在 strictNullChecks 模式下，null 只能赋值给 null 和 any 类型
let n: null = null;                            // null 值

// undefined 类型：表示未定义
// undefined 是一个只有一个值的类型，即 undefined
// 变量声明但未赋值时，默认值是 undefined
let u: undefined = undefined;                  // undefined 值

// symbol 类型：表示唯一的符号值
// Symbol 是 ES6 引入的新的原始数据类型
// 每个 Symbol 值都是唯一的，不会与其他 Symbol 相等
let sym1: symbol = Symbol('key');              // 创建一个 Symbol
let sym2: symbol = Symbol('key');              // 创建另一个 Symbol（与 sym1 不相等）
// sym1 === sym2 的结果为 false，因为每个 Symbol 都是唯一的

// bigint 类型：表示任意精度的整数
// BigInt 是 ES2020 引入的新的原始数据类型
// 可以表示超出 Number 安全范围的大整数
// 注意：需要 tsconfig.json 中 target 设置为 ES2020 或更高
let big: bigint = 100n;                        // 使用 n 后缀表示 bigint
let big2: bigint = BigInt(9007199254740991);   // 使用 BigInt 函数创建

// ============================================================
//  2. 数组类型（Array Types）
// ============================================================

// 数组类型有两种表示方式：类型[] 和 Array<类型>

// 方式一：类型[] 写法（更常用，更简洁）
// 在元素类型后面加上 []，表示由此类型元素组成的数组
let list1: number[] = [1, 2, 3];               // 数字数组
let names1: string[] = ['张三', '李四', '王五']; // 字符串数组

// 方式二：Array<类型> 写法（泛型写法）
// 使用数组泛型 Array<元素类型>
let list2: Array<number> = [1, 2, 3];          // 数字数组（泛型写法）
let names2: Array<string> = ['张三', '李四'];    // 字符串数组（泛型写法）

// 多维数组
let matrix: number[][] = [                     // 二维数组（矩阵）
  [1, 2, 3],
  [4, 5, 6],
  [7, 8, 9]
];
let matrix2: Array<Array<number>> = [          // 二维数组（泛型写法）
  [1, 2],
  [3, 4]
];

// 任意类型数组（不推荐，尽量使用具体类型）
let anyList: any[] = [1, 'hello', true, { name: 'test' }];

// ============================================================
//  3. 元组类型（Tuple Types）
// ============================================================

// 元组类型允许表示一个已知元素数量和类型的数组
// 各元素的类型不必相同，但顺序必须对应
// 元组是一种特殊的数组，长度固定且每个位置的类型已知

// 定义一个元组类型：第一个元素是 string，第二个元素是 number
let tuple: [string, number];
// 正确赋值：类型和顺序都匹配
tuple = ['hello', 10];
// 错误示例：
// tuple = [10, 'hello'];  // 错误，类型顺序不匹配
// tuple = ['hello'];      // 错误，缺少第二个元素
// tuple = ['hello', 10, true];  // 错误，元素数量过多

// 访问元组元素
console.log(tuple[0]);                         // 访问第一个元素，类型为 string
console.log(tuple[1]);                         // 访问第二个元素，类型为 number

// 元组的越界访问（TypeScript 3.0+ 支持可选元素和剩余元素）
type PersonTuple = [string, number, boolean?]; // 第三个元素是可选的
let person1: PersonTuple = ['张三', 25, true];
let person2: PersonTuple = ['李四', 30];       // 可选元素可以省略

// 剩余元素的元组
type StringNumberTuple = [string, ...number[]]; // 第一个是 string，后面是任意数量的 number
let sn: StringNumberTuple = ['hello', 1, 2, 3];

// ============================================================
//  4. 联合类型与交叉类型（Union & Intersection）
// ============================================================

// ---------- 联合类型（Union Types）----------
// 使用 | 符号将多个类型联合起来，表示值可以是其中任意一种类型

// 基本联合类型：值可以是 string 或 number
let unionVar: string | number;
unionVar = 'hello';                            // 正确，可以是字符串
unionVar = 123;                                // 正确，可以是数字
// unionVar = true;                           // 错误，不能是布尔值

// 联合类型数组：数组元素可以是 string 或 number
let mixedArr: (string | number)[] = [1, 'two', 3, 'four'];

// 字符串字面量联合类型：限制值只能是特定的几个字符串之一
type Direction = 'up' | 'down' | 'left' | 'right';
let direction: Direction = 'up';               // 正确
// direction = 'forward';                     // 错误，不在联合类型中

// ---------- 交叉类型（Intersection Types）----------
// 使用 & 符号将多个类型组合在一起，表示新类型同时具有所有类型的属性

// 定义两个接口
interface Nameable {
  name: string;                                // 名称属性
}
interface Ageable {
  age: number;                                 // 年龄属性
}

// 使用交叉类型组合两个接口
// Person 类型同时具有 name 和 age 属性
type Person = Nameable & Ageable;

let person: Person = {
  name: '张三',
  age: 25
};

// 交叉类型常用于合并多个类型的特性
interface Loggable {
  log(message: string): void;                  // 日志方法
}
interface Serializable {
  serialize(): string;                         // 序列化方法
}

// 既是可记录日志的，又是可序列化的
type LoggerSerializer = Loggable & Serializable;

// ============================================================
//  5. 类型别名（Type Aliases）
// ============================================================

// 使用 type 关键字为类型创建别名
// 类型别名可以给任何类型起一个新名字，包括原始类型、联合类型、交叉类型、对象类型等

// 为原始类型创建别名（虽然意义不大，但可以提高代码可读性）
type ID = string;                              // ID 类型是 string 的别名
type UserID = number;                          // UserID 类型是 number 的别名

let userId: ID = 'user_001';                   // 使用类型别名
let orderId: UserID = 1001;                    // 使用类型别名

// 为联合类型创建别名（这是类型别名最常见的用法之一）
type StringOrNumber = string | number;         // 字符串或数字
let value: StringOrNumber = 'hello';
value = 123;

// 为对象类型创建别名
type UserType = {
  name: string;                                // 用户名
  age: number;                                 // 年龄
  email?: string;                              // 邮箱（可选属性）
};

let userA: UserType = {
  name: '张三',
  age: 25
};

// 类型别名支持泛型（后续泛型章节会详细讲解）
type Container<T> = {
  value: T;                                    // 泛型值
};

let stringContainer: Container<string> = {
  value: 'hello'
};

// type 与 interface 的区别（后面接口部分会详细对比）
// - type 可以表示任何类型，interface 只能表示对象类型
// - interface 可以被继承和实现，type 不行
// - interface 可以声明合并，type 不行

// ============================================================
//  6. 接口（Interfaces）
// ============================================================

// 接口是 TypeScript 的核心概念之一
// 用于定义对象的形状（shape），即对象应该有哪些属性和方法
// 接口是一种契约，规定了对象必须遵守的结构

// ---------- 基本接口定义 ----------
interface PersonInterface {
  name: string;                                // 必需属性：姓名
  age: number;                                 // 必需属性：年龄
}

// 使用接口作为类型注解
let person11: PersonInterface = {
  name: '张三',
  age: 25
};

// ---------- 可选属性 ----------
// 在属性名后加上 ? 表示该属性是可选的
// 可选属性在使用时可以省略
interface PersonWithOptional {
  name: string;                                // 必需属性
  age?: number;                                // 可选属性：年龄
  email?: string;                              // 可选属性：邮箱
}

let person22: PersonWithOptional = {
  name: '李四'                                  // 可以不提供可选属性
};

let person33: PersonWithOptional = {
  name: '王五',
  age: 30,
  email: 'wangwu@example.com'
};

// ---------- 只读属性 ----------
// 使用 readonly 关键字定义只读属性
// 只读属性只能在对象创建时赋值，之后不能修改
interface Point {
  readonly x: number;                          // 只读属性：x 坐标
  readonly y: number;                          // 只读属性：y 坐标
}

let point: Point = { x: 10, y: 20 };
// point.x = 30;                               // 错误，x 是只读属性，不能修改

// ReadonlyArray<T>：只读数组
// 创建后不能修改的数组
let readonlyArr: ReadonlyArray<number> = [1, 2, 3];
// readonlyArr[0] = 10;                        // 错误，不能修改元素
// readonlyArr.push(4);                        // 错误，不能添加元素
// readonlyArr.length = 10;                    // 错误，不能修改长度

// ---------- 索引签名（Index Signatures）----------
// 索引签名用于描述那些可以通过索引访问的类型
// 比如数组、字典等

// 字符串索引签名：可以用任意字符串作为键来访问
interface StringDictionary {
  [key: string]: string;                       // 字符串索引，值为 string 类型
}

let dict: StringDictionary = {
  'name': '张三',
  'age': '25',                                  // 注意：值必须是 string 类型
  // 'score': 100                              // 错误，值必须是 string
};

// 数字索引签名：可以用任意数字作为索引来访问
interface NumberArray {
  [index: number]: number;                     // 数字索引，值为 number 类型
}

let numArr: NumberArray = [1, 2, 3];
console.log(numArr[0]);                         // 通过数字索引访问

// 同时支持两种索引签名
// 注意：数字索引的返回值必须是字符串索引返回值类型的子类型
interface MixedArray {
  [index: number]: number;                     // 数字索引
  [key: string]: number | string;              // 字符串索引（必须包含数字索引的类型）
}

// ---------- 接口的继承 ----------
// 接口可以继承另一个接口，继承其所有属性和方法
// 使用 extends 关键字

interface AnimalInterface {
  name: string;                                // 动物名称
  eat(): void;                                 // 吃的方法
}

// Dog 接口继承自 Animal，拥有 Animal 的所有属性和方法
interface Dog extends AnimalInterface {
  breed: string;                               // 品种
  bark(): void;                                // 吠叫的方法
}

let myDog: Dog = {
  name: '旺财',
  breed: '金毛',
  eat() { console.log('狗在吃东西'); },
  bark() { console.log('汪汪汪！'); }
};

// 接口可以多继承（继承多个接口）
interface Flyable {
  fly(): void;                                 // 飞的方法
}
interface Swimmable {
  swim(): void;                                // 游泳的方法
}

// Duck 接口同时继承 Flyable 和 Swimmable
interface Duck extends Flyable, Swimmable {
  quack(): void;                               // 嘎嘎叫的方法
}

// ---------- 接口与类型别名的区别 ----------
// 1. 类型别名可以表示任何类型（原始类型、联合类型、交叉类型、元组等）
//    接口只能表示对象类型
// 2. 接口可以被继承（extends）和实现（implements）
//    类型别名不能被继承和实现
// 3. 接口支持声明合并（多个同名接口会自动合并）
//    类型别名不支持声明合并
// 4. 一般来说，定义对象结构时优先使用接口
//    定义联合类型、交叉类型、元组等时使用类型别名

// ============================================================
//  7. 类型断言（Type Assertions）
// ============================================================

// 类型断言允许你告诉 TypeScript 编译器："相信我，我知道这个值的确切类型"
// 类型断言不会进行运行时检查，只是编译时的类型提示
// 两种语法：as 语法 和 尖括号语法

// ---------- as 语法（推荐）----------
let someValue: unknown = 'this is a string';
// 使用 as 关键字断言 someValue 是 string 类型
let strLength: number = (someValue as string).length;

// ---------- 尖括号语法（不推荐，在 JSX 中会有冲突）----------
let someValue2: unknown = 'hello world';
// 使用 <类型> 语法进行类型断言
let strLength2: number = (<string>someValue2).length;

// 注意：类型断言要谨慎使用，因为它会绕过类型检查
// 只有当你确定值的类型时才使用类型断言
// 错误的类型断言可能导致运行时错误

// 常见使用场景：
// 1. 从 DOM 获取元素时，断言为具体的元素类型
// const input = document.getElementById('input') as HTMLInputElement;

// 2. 处理 JSON.parse 等返回 any 类型的函数
// const data = JSON.parse(jsonStr) as MyDataType;

// 3. 当你比 TypeScript 更了解类型时

// 双重断言（Double Assertion）
// 当两个类型之间没有直接关系时，可以先断言为 unknown 再断言为目标类型
let x: string = 'hello';
// 先断言为 unknown，再断言为 number
// 注意：这通常是不安全的，应该尽量避免
let y = (x as unknown) as number;

// ============================================================
//  8. 类型守卫（Type Guards）
// ============================================================

// 类型守卫是一种在运行时检查类型的机制
// 它可以帮助 TypeScript 在特定代码块中缩小变量的类型范围
// 类型守卫的特点是：返回类型为 "参数 is 类型" 的函数

// ---------- typeof 类型守卫 ----------
// 使用 typeof 操作符检查原始类型

// 定义一个函数，接受 string 或 number 类型的参数
function printValue(value: string | number): void {
  // 使用 typeof 检查 value 是否为 string 类型
  if (typeof value === 'string') {
    // 在这个代码块中，TypeScript 知道 value 是 string 类型
    // 因此可以安全地调用字符串的方法
    console.log('字符串类型：', value.toUpperCase());
  } else {
    // 在这个代码块中，TypeScript 知道 value 是 number 类型
    // 因此可以安全地调用数字的方法
    console.log('数字类型：', value.toFixed(2));
  }
}

// typeof 只能检查原始类型：string, number, boolean, symbol, bigint, undefined, function, object
// 注意：typeof null 返回 'object'，这是 JavaScript 的历史遗留问题

// ---------- instanceof 类型守卫 ----------
// 使用 instanceof 操作符检查对象是否是某个类的实例

class Dogg {
  bark(): void {
    console.log('汪汪汪！');
  }
}

class Cat {
  meow(): void {
    console.log('喵喵喵！');
  }
}

// 函数参数是 Dog 或 Cat 类型
function makeSound(animal: Dogg | Cat): void {
  // 使用 instanceof 检查 animal 是否是 Dog 的实例
  if (animal instanceof Dogg) {
    // 在这个代码块中，TypeScript 知道 animal 是 Dog 类型
    animal.bark();
  } else {
    // 在这个代码块中，TypeScript 知道 animal 是 Cat 类型
    animal.meow();
  }
}

// ---------- in 类型守卫 ----------
// 使用 in 操作符检查对象是否具有某个属性

interface Bird {
  fly(): void;                                 // 鸟会飞
  layEggs(): void;                             // 鸟会下蛋
}

interface Fish {
  swim(): void;                                // 鱼会游泳
  layEggs(): void;                             // 鱼会下蛋
}

// 函数参数是 Bird 或 Fish 类型
function move(animal: Bird | Fish): void {
  // 使用 in 检查 animal 是否有 fly 属性
  if ('fly' in animal) {
    // 在这个代码块中，TypeScript 知道 animal 是 Bird 类型
    animal.fly();
  } else {
    // 在这个代码块中，TypeScript 知道 animal 是 Fish 类型
    animal.swim();
  }
}

// ---------- 自定义类型守卫 ----------
// 你可以定义自己的类型守卫函数
// 函数返回值类型为 "参数名 is 类型" 的形式

// 定义一个接口
interface AdminUser {
  name: string;
  role: 'admin';                               // 管理员角色
  manageUsers(): void;                         // 管理用户的方法
}

interface RegularUser {
  name: string;
  role: 'user';                                // 普通用户角色
  viewContent(): void;                         // 查看内容的方法
}

// 自定义类型守卫函数：判断是否是管理员用户
function isAdmin(user: AdminUser | RegularUser): user is AdminUser {
  // 返回一个布尔值，表示 user 是否是 AdminUser 类型
  return user.role === 'admin';
}

// 使用自定义类型守卫
function handleUser(user: AdminUser | RegularUser): void {
  if (isAdmin(user)) {
    // TypeScript 知道 user 是 AdminUser 类型
    user.manageUsers();
  } else {
    // TypeScript 知道 user 是 RegularUser 类型
    user.viewContent();
  }
}

// ============================================================
//  9. 字面量类型与枚举（Literal Types & Enums）
// ============================================================

// ---------- 字面量类型 ----------
// 字面量类型将值限制为特定的字面量值
// 可以是字符串字面量、数字字面量、布尔字面量

// 字符串字面量类型
type Status = 'pending' | 'approved' | 'rejected';
let orderStatus: Status = 'pending';          // 正确
orderStatus = 'approved';                      // 正确
// orderStatus = 'cancelled';                   // 错误，不在允许的值中

// 数字字面量类型
type DiceValue = 1 | 2 | 3 | 4 | 5 | 6;
let diceRoll: DiceValue = 3;                   // 正确
// diceRoll = 7;                               // 错误，超过范围

// 布尔字面量类型
type IsValid = true | false;                   // 实际上就是 boolean 类型
let valid: IsValid = true;

// ---------- 枚举（Enums）----------
// 枚举是 TypeScript 特有的类型，用于定义一组命名的常量
// 使用 enum 关键字定义

// 数字枚举：默认从 0 开始，依次递增
enum DirectionEnum {
  Up,                                          // 0（默认值）
  Down,                                        // 1
  Left,                                        // 2
  Right                                        // 3
}

let dir: DirectionEnum = DirectionEnum.Up;
console.log(dir);                              // 输出：0
console.log(DirectionEnum.Down);               // 输出：1

// 数字枚举可以手动指定初始值
enum Color {
  Red = 1,                                     // 从 1 开始
  Green,                                       // 2（自动递增）
  Blue                                         // 3
}

let c: Color = Color.Green;
console.log(c);                                // 输出：2

// 数字枚举也可以全部手动指定
enum ErrorCode {
  NotFound = 404,
  ServerError = 500,
  BadRequest = 400
}

// 字符串枚举：每个成员都必须用字符串字面量初始化
enum MessageType {
  Success = 'SUCCESS',
  Error = 'ERROR',
  Warning = 'WARNING',
  Info = 'INFO'
}

let msg: MessageType = MessageType.Success;
console.log(msg);                              // 输出：'SUCCESS'

// 异构枚举：同时包含数字和字符串（不推荐，容易造成混淆）
enum Mixed {
  No = 0,
  Yes = 'YES'
}

// ---------- const 枚举 ----------
// 使用 const enum 定义的枚举在编译时会被完全移除
// 所有使用枚举的地方会直接替换为对应的值
// 好处是可以减少编译后的代码体积
// 缺点是不能在运行时访问枚举对象（比如不能枚举所有成员）

const enum Weekday {
  Monday = 1,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday,
  Sunday
}

let day: Weekday = Weekday.Monday;
// 编译后会变成：let day = 1 /* Monday */;
// Weekday 对象在运行时不存在

// ---------- 枚举反向映射 ----------
// 数字枚举支持反向映射：可以通过值获取名称
// 字符串枚举不支持反向映射

enum NumEnum {
  A = 1,
  B = 2
}

console.log(NumEnum.A);                        // 输出：1
console.log(NumEnum[1]);                       // 输出：'A'（反向映射）
console.log(NumEnum[2]);                       // 输出：'B'

// ============================================================
//  10. any / unknown / never / void 类型
// ============================================================

// ---------- any 类型 ----------
// any 类型表示任意类型，完全绕过类型检查
// 可以赋值为任何值，也可以赋值给任何类型
// 不建议频繁使用，会失去 TypeScript 的类型安全优势

let anyVar: any = 10;                          // 初始化为数字
anyVar = 'hello';                              // 可以改为字符串
anyVar = true;                                 // 可以改为布尔值
anyVar = { name: 'test' };                     // 可以改为对象
anyVar();                                      // 甚至可以当作函数调用（编译时不报错，运行时可能出错）

// 当你不确定一个值的类型，或者从第三方库获取数据时，可能会用到 any
// 但应该尽量避免使用 any，优先考虑使用 unknown 或具体类型

// ---------- unknown 类型 ----------
// unknown 类型是 TypeScript 3.0 引入的安全版 any
// unknown 类型的值可以是任何类型，但不能直接使用
// 必须先进行类型检查（类型守卫或类型断言）才能使用

let unknownVar: unknown = 10;
unknownVar = 'hello';
unknownVar = { name: 'test' };

// unknownVar.toUpperCase();                     // 错误，不能直接调用方法
// unknownVar();                                // 错误，不能直接调用

// 必须先进行类型检查
if (typeof unknownVar === 'string') {
  console.log(unknownVar.toUpperCase());       // 正确，TypeScript 知道这是 string
}

// unknown 与 any 的区别：
// - any：完全自由，可以做任何操作，不需要类型检查
// - unknown：类型安全，使用前必须进行类型检查
// - 建议优先使用 unknown 而不是 any

// ---------- never 类型 ----------
// never 类型表示永远不会有值的类型
// 常用于表示：
//   1. 函数永远不会返回（比如抛出异常或无限循环）
//   2. 类型守卫中不可能的情况（穷尽检查）

// 函数抛出异常，永远不会正常返回
function throwError(message: string): never {
  throw new Error(message);
}

// 无限循环函数，永远不会返回
function infiniteLoop(): never {
  while (true) {
    // 无限循环...
  }
}

// never 类型的一个重要用途：穷尽性检查（Exhaustiveness checking）
// 确保 switch 语句覆盖了所有可能的情况

type Fruit = 'apple' | 'banana' | 'orange';

function getFruitColor(fruit: Fruit): string {
  switch (fruit) {
    case 'apple':
      return 'red';
    case 'banana':
      return 'yellow';
    case 'orange':
      return 'orange';
    default:
      // 如果 Fruit 类型新增了成员，但忘记在这里处理
      // 这里就会报错，因为新的成员不能赋值给 never 类型
      // 这是一种确保代码完整性的好方法
      const _exhaustiveCheck: never = fruit;
      return _exhaustiveCheck;
  }
}

// ---------- void 类型 ----------
// void 类型表示没有任何类型
// 常用于表示函数没有返回值

// 函数没有返回值，返回类型为 void
function sayHello(name: string): void {
  console.log(`你好，${name}！`);
  // 不需要 return 语句，或者 return 后面没有值
  // return;  // 这样也可以，返回 undefined
}

// void 类型的变量只能赋值为 undefined 或 null（非严格模式下）
// 在 strictNullChecks 模式下，void 类型的变量只能赋值为 undefined
let voidVar: void = undefined;
// voidVar = null;                              // 严格模式下错误

// void 与 never 的区别：
// - void：函数执行完了，但没有返回值（返回 undefined）
// - never：函数根本就不会执行完（抛出异常或无限循环）

// ============================================================
//  演示函数：运行所有基础类型示例
// ============================================================

export function runTypesDemo(): void {
  console.log('========== 基础类型演示 ==========');

  // 原始类型
  console.log('1. 原始类型:');
  console.log('   string:', str);
  console.log('   number:', num);
  console.log('   boolean:', isDone);
  console.log('   null:', n);
  console.log('   undefined:', u);
  console.log('   symbol:', sym1.toString());
  console.log('   bigint:', big.toString());

  // 数组
  console.log('\n2. 数组类型:');
  console.log('   number[]:', list1);
  console.log('   string[]:', names1);

  // 元组
  console.log('\n3. 元组类型:');
  console.log('   tuple:', tuple);
  console.log('   tuple[0]:', tuple[0]);

  // 联合类型
  console.log('\n4. 联合类型与交叉类型:');
  console.log('   unionVar (string):', unionVar);
  console.log('   Person (交叉类型):', person);

  // 类型别名
  console.log('\n5. 类型别名:');
  console.log('   userId (ID 类型):', userId);
  console.log('   userA (UserType):', userA);

  // 接口
  console.log('\n6. 接口:');
  console.log('   person11:', person11);
  console.log('   person22 (可选属性):', person22);
  console.log('   point (只读属性):', point);

  // 类型断言
  console.log('\n7. 类型断言:');
  console.log('   strLength:', strLength);

  // 类型守卫
  console.log('\n8. 类型守卫:');
  printValue('hello world');
  printValue(3.14159);

  // 枚举
  console.log('\n9. 枚举:');
  console.log('   DirectionEnum.Up:', DirectionEnum.Up);
  console.log('   Color.Green:', Color.Green);
  console.log('   MessageType.Success:', MessageType.Success);

  // any / unknown / never / void
  console.log('\n10. any / unknown / never / void:');
  console.log('    anyVar:', anyVar);
  console.log('    unknownVar (作为 string 使用前需要类型检查):', typeof unknownVar === 'string' ? unknownVar : '非字符串');
  sayHello('TypeScript 学习者');

  console.log('\n========== 基础类型演示结束 ==========\n');
}
