/**
 * ============================================================
 *  TypeScript 高级特性详解
 * ============================================================
 *
 * 本文件详细介绍 TypeScript 中的高级特性，包括：
 * 1. 装饰器（类装饰器 / 方法装饰器 / 属性装饰器 / 参数装饰器）
 * 2. 命名空间 namespace
 * 3. 声明文件 .d.ts、declare
 * 4. 模块系统（ES Module / CommonJS）
 * 5. 类型兼容（结构类型系统）
 * 6. 字符串字面量类型 + 模板字面量类型
 * 7. keyof / typeof 操作符
 * 8. 索引访问类型 T[K]
 * 9. 映射类型修饰符（+/- readonly / +/- ?）
 * ============================================================
 */

// 注意：使用装饰器需要在 tsconfig.json 中启用 experimentalDecorators

// ============================================================
//  1. 装饰器（Decorators）
// ============================================================

// 装饰器是一种特殊的声明，可以附加到类声明、方法、属性或参数上
// 装饰器使用 @expression 形式，expression 是一个函数，会在运行时被调用
// 装饰器是实验性特性，需要在 tsconfig.json 中启用 experimentalDecorators

// ---------- 类装饰器（Class Decorators）----------
// 类装饰器应用于类的构造函数
// 可以用来观察、修改或替换类的定义

/**
 * 类装饰器：冻结类，防止被修改
 * @param constructor 目标类的构造函数
 */
function sealed(constructor: Function): void {
  // 冻结构造函数和原型，防止被修改
  Object.seal(constructor);
  Object.seal(constructor.prototype);
}

// 使用 @sealed 装饰器
@sealed
class Greeter {
  greeting: string;

  constructor(message: string) {
    this.greeting = message;
  }

  greet(): string {
    return `Hello, ${this.greeting}`;
  }
}

// ---------- 装饰器工厂（Decorator Factories）----------
// 如果我们想要定制装饰器如何应用于声明，可以写一个装饰器工厂
// 装饰器工厂是一个函数，返回真正的装饰器函数

/**
 * 装饰器工厂：记录类的创建时间
 * @param prefix 日志前缀
 */
function logClass(prefix: string): ClassDecorator {
  // 返回真正的类装饰器
  return function(target: Function): void {
    console.log(`[${prefix}] 类 ${target.name} 被创建了`);
  };
}

// 使用装饰器工厂
@logClass('用户模块')
class UserAdv {
  name: string;

  constructor(name: string) {
    this.name = name;
  }
}

// ---------- 方法装饰器（Method Decorators）----------
// 方法装饰器应用于类的方法
// 可以用来观察、修改或替换方法的定义

/**
 * 方法装饰器：记录方法执行时间
 * @param target 类的原型（对于静态方法是类本身）
 * @param propertyKey 方法名
 * @param descriptor 方法的属性描述符
 */
function logMethod(
  target: any,
  propertyKey: string,
  descriptor: PropertyDescriptor
): void {
  // 保存原始方法
  const originalMethod = descriptor.value;

  // 替换方法
  descriptor.value = function(...args: any[]) {
    console.log(`调用方法 ${propertyKey}，参数：`, args);
    const startTime = Date.now();
    const result = originalMethod.apply(this, args);
    const endTime = Date.now();
    console.log(`方法 ${propertyKey} 执行完成，耗时：${endTime - startTime}ms`);
    return result;
  };
}

class Calculator {
  @logMethod
  add(a: number, b: number): number {
    return a + b;
  }

  @logMethod
  multiply(a: number, b: number): number {
    // 模拟耗时操作
    let result = 0;
    for (let i = 0; i < b; i++) {
      result += a;
    }
    return result;
  }
}

// ---------- 属性装饰器（Property Decorators）----------
// 属性装饰器应用于类的属性

/**
 * 属性装饰器：标记属性为必填
 * @param target 类的原型
 * @param propertyKey 属性名
 */
function required(target: any, propertyKey: string): void {
  // 这里可以存储元数据，实际使用中通常配合 reflect-metadata 库
  console.log(`属性 ${propertyKey} 被标记为必填`);
}

class PersonAdv {
  @required
  name: string = '';

  age: number = 0;
}

// ---------- 参数装饰器（Parameter Decorators）----------
// 参数装饰器应用于方法的参数

/**
 * 参数装饰器：标记参数为必填
 * @param target 类的原型
 * @param methodName 方法名
 * @param parameterIndex 参数索引
 */
function requiredParam(target: any, methodName: string, parameterIndex: number): void {
  console.log(`方法 ${methodName} 的第 ${parameterIndex} 个参数是必填的`);
}

class GreeterAdv {
  greet(@requiredParam name: string, greeting?: string): string {
    return `${greeting || '你好'}，${name}！`;
  }
}

// ============================================================
//  2. 命名空间 namespace
// ============================================================

// 命名空间是 TypeScript 早期的模块化方式
// 用于组织代码，避免全局作用域污染
// 注意：现代 TypeScript 更推荐使用 ES Module（import/export）
// 命名空间主要用于向后兼容和一些特殊场景

/**
 * 验证器命名空间
 * 包含各种验证相关的类型和函数
 */
namespace Validators {
  // 命名空间内的接口，默认是私有的，需要 export 才能在外部访问
  export interface StringValidator {
    isAcceptable(s: string): boolean;
  }

  // 常量
  const lettersRegexp = /^[A-Za-z]+$/;
  const numberRegexp = /^[0-9]+$/;

  // 导出的类
  export class LettersOnlyValidator implements StringValidator {
    isAcceptable(s: string): boolean {
      return lettersRegexp.test(s);
    }
  }

  // 导出的类
  export class ZipCodeValidator implements StringValidator {
    isAcceptable(s: string): boolean {
      return s.length === 5 && numberRegexp.test(s);
    }
  }

  // 导出的函数
  export function validateEmail(email: string): boolean {
    const emailRegexp = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    return emailRegexp.test(email);
  }
}

// 使用命名空间中的成员
const validators: Validators.StringValidator[] = [
  new Validators.LettersOnlyValidator(),
  new Validators.ZipCodeValidator()
];

const testStrings = ['Hello', '12345', '1234'];
for (const s of testStrings) {
  for (const validator of validators) {
    console.log(`"${s}" ${validator.isAcceptable(s) ? '满足' : '不满足'} 验证`);
  }
}

// ---------- 命名空间的拆分 ----------
// 命名空间可以拆分到多个文件中
// 即使在不同文件中，只要命名空间名称相同，就会被合并
// （这里因为在同一个文件中，就不演示了）

// ---------- 命名空间别名 ----------
// 可以使用 import 为命名空间创建别名
namespace Shapes {
  export namespace Polygons {
    export class Triangle {}
    export class Square {}
  }
}

// 为嵌套命名空间创建别名
import polygons = Shapes.Polygons;
const sq = new polygons.Square();

// ============================================================
//  3. 声明文件 .d.ts、declare
// ============================================================

// 声明文件（Declaration Files）以 .d.ts 结尾
// 用于描述 JavaScript 库的类型信息，让 TypeScript 能够使用这些库
// 使用 declare 关键字声明类型

// ---------- declare 的用法 ----------

// 声明全局变量
// 假设我们有一个全局变量 GLOBAL_VERSION
// declare const GLOBAL_VERSION: string;

// 声明全局函数
// declare function globalFunction(arg: string): number;

// 声明全局类
// declare class GlobalClass {
//   constructor(name: string);
//   name: string;
//   getName(): string;
// }

// 声明模块
// declare module 'some-library' {
//   export function doSomething(): void;
//   export const version: string;
// }

// ---------- 三斜线指令（Triple-Slash Directives）----------
// 三斜线指令是包含单个 XML 标签的单行注释
// 用于告诉编译器在编译过程中要引入的额外文件
// 注意：现代 TypeScript 更推荐使用 import 语句
// /// <reference path="path/to/declaration.d.ts" />
// /// <reference types="node" />

// 由于本文件是 .ts 文件，不是 .d.ts 文件
// 这里只做概念介绍，不实际编写声明文件

// ============================================================
//  4. 模块系统（ES Module / CommonJS）
// ============================================================

// TypeScript 支持多种模块系统
// 最常用的是 ES Module（import/export）和 CommonJS（require/module.exports）
// 由 tsconfig.json 中的 module 选项决定编译后的模块格式

// ---------- ES Module ----------
// 使用 export 导出，使用 import 导入
// 这是 ES6 引入的标准模块系统，推荐使用

// 导出方式 1：命名导出
export function utilFunction(): string {
  return '这是一个工具函数';
}

export const utilConstant = 42;

export interface UtilInterface {
  id: number;
  name: string;
}

// 导出方式 2：默认导出
// 每个模块只能有一个默认导出
export default class DefaultExportClass {
  static sayHello(): void {
    console.log('你好，我是默认导出的类');
  }
}

// 导出方式 3：重命名导出
// export { utilFunction as renamedFunction };

// 对应的导入方式（在其他文件中）：
// import { utilFunction, utilConstant } from './module';  // 命名导入
// import DefaultExportClass from './module';               // 默认导入
// import * as utils from './module';                       // 命名空间导入
// import { utilFunction as renamed } from './module';      // 重命名导入

// ---------- CommonJS ----------
// CommonJS 是 Node.js 的传统模块系统
// 使用 require 导入，module.exports 导出
// TypeScript 可以编译为 CommonJS 格式

// TypeScript 中可以直接使用 ES Module 语法
// 编译时会自动转换为 CommonJS（如果 module 选项是 commonjs）

// 在 TypeScript 中导入 CommonJS 模块
// 需要使用 esModuleInterop 选项来确保兼容性
// import * as fs from 'fs';  // 导入 Node.js 的 fs 模块

// ============================================================
//  5. 类型兼容（Type Compatibility）
// ============================================================

// TypeScript 使用结构类型系统（Structural Type System）
// 也叫鸭子类型（Duck Typing）
// 只要两个类型的结构兼容，它们就是兼容的，不需要显式声明关系

// ---------- 基本兼容性 ----------
interface Named {
  name: string;
}

class PersonNamedAdv {
  name: string;

  constructor(name: string) {
    this.name = name;
  }
}

// 虽然 PersonNamedAdv 没有显式实现 Named 接口
// 但它有 name 属性，所以结构上是兼容的
let named: Named;
named = new PersonNamedAdv('张三');               // 正确，结构兼容

// 对象字面量的兼容性
const obj = { name: '李四', age: 25 };
named = obj;                                      // 正确，有 name 属性即可

// 注意：直接赋值对象字面量时会进行额外的检查（ excess property checking ）
// named = { name: '王五', age: 30 };             // 错误，对象字面量不能有多余属性

// ---------- 函数的类型兼容 ----------
// 函数的兼容性比较特殊

// 参数数量：目标函数的参数可以更少
type Func1 = (a: number, b: number) => number;
type Func2 = (a: number) => number;

let f1: Func1 = (a, b) => a + b;
let f2: Func2 = (a) => a * 2;

f1 = f2;                                          // 正确，f2 的参数更少
// f2 = f1;                                      // 错误，f1 的参数更多

// 参数类型：必须对应兼容
type FuncWithString = (s: string) => void;
type FuncWithNumber = (n: number) => void;

let fs: FuncWithString = (s) => console.log(s);
let fn: FuncWithNumber = (n) => console.log(n);
// fs = fn;                                      // 错误，参数类型不兼容

// 返回值类型：必须对应兼容
type FuncReturnString = () => string;
type FuncReturnNumber = () => number;

let fStr: FuncReturnString = () => 'hello';
let fNum: FuncReturnNumber = () => 42;
// fStr = fNum;                                  // 错误，返回值类型不兼容

// ---------- 类的类型兼容 ----------
// 类的兼容性只比较实例成员，不比较静态成员和构造函数
// private 和 protected 成员会影响兼容性

class AnimalCompat {
  name: string;
  constructor(name: string) {
    this.name = name;
  }
}

class DogCompat {
  name: string;
  breed: string;
  constructor(name: string, breed: string) {
    this.name = name;
    this.breed = breed;
  }
}

// DogCompat 有 name 属性，所以可以赋值给 AnimalCompat 类型
let animalCompat: AnimalCompat;
animalCompat = new DogCompat('旺财', '金毛');     // 正确

// ---------- 泛型的类型兼容 ----------
// 泛型类型的兼容性取决于其使用方式

interface Empty<T> {
  // 空接口，没有使用 T
}

let e1: Empty<number>;
let e2: Empty<string>;
// e1 = e2;  // 正确，因为接口是空的，T 没有被使用

interface NotEmpty<T> {
  data: T;                                       // 使用了 T
}

let ne1: NotEmpty<number>;
let ne2: NotEmpty<string>;
// ne1 = ne2;  // 错误，因为 data 的类型不同

// ============================================================
//  6. 字符串字面量类型 + 模板字面量类型
// ============================================================

// ---------- 字符串字面量类型 ----------
// 字符串字面量类型将值限制为特定的字符串

type EventName = 'click' | 'hover' | 'submit';

function handleEvent(event: EventName): void {
  console.log(`处理事件：${event}`);
}

handleEvent('click');                            // 正确
// handleEvent('scroll');                         // 错误，不在允许的值中

// ---------- 模板字面量类型 ----------
// TypeScript 4.1 引入的新特性
// 使用模板字符串语法创建新的字符串字面量类型

type Greeting = `hello ${string}`;
// 表示以 "hello " 开头的字符串

const g1: Greeting = 'hello world';              // 正确
const g2: Greeting = 'hello TypeScript';         // 正确
// const g3: Greeting = 'hi world';              // 错误，不是以 "hello " 开头

// ---------- 模板字面量类型的应用 ----------

// 事件名称类型
type EventType = 'click' | 'focus' | 'blur';
type HandlerName = `on${Capitalize<EventType>}`;
// 结果：'onClick' | 'onFocus' | 'onBlur'

// CSS 属性类型
type CSSProperty = 'margin' | 'padding';
type CSSDirection = 'top' | 'right' | 'bottom' | 'left';
type CSSPropertyDirection = `${CSSProperty}-${CSSDirection}`;
// 结果：'margin-top' | 'margin-right' | 'margin-bottom' | 'margin-left' |
//       'padding-top' | 'padding-right' | 'padding-bottom' | 'padding-left'

// 路由路径类型
type Route = `/users/${string}/posts/${string}`;
const userPostRoute: Route = '/users/123/posts/456';  // 正确

// ---------- 内置字符串操作类型 ----------
// TypeScript 内置了几个用于操作字符串字面量类型的工具类型

// Uppercase<T>：将字符串转为大写
type Upper = Uppercase<'hello'>;               // 'HELLO'

// Lowercase<T>：将字符串转为小写
type Lower = Lowercase<'HELLO'>;               // 'hello'

// Capitalize<T>：将首字母大写
type Cap = Capitalize<'hello'>;                // 'Hello'

// Uncapitalize<T>：将首字母小写
type Uncap = Uncapitalize<'Hello'>;            // 'hello'

// ---------- 更复杂的模板字面量类型 ----------

// 构建属性访问器类型
type PropEventSource<T> = {
  on(event: `${string & keyof T}Changed`, callback: () => void): void;
};

// ============================================================
//  7. keyof / typeof 操作符
// ============================================================

// ---------- keyof 操作符 ----------
// keyof 操作符用于获取一个类型的所有键（属性名），返回一个联合类型

interface PersonKeyof {
  name: string;
  age: number;
  address: string;
}

type PersonKeys = keyof PersonKeyof;              // 'name' | 'age' | 'address'

// 使用 keyof 约束函数参数
function getPropertyKeyof<T, K extends keyof T>(obj: T, key: K): T[K] {
  return obj[key];
}

const personKeyof: PersonKeyof = {
  name: '张三',
  age: 25,
  address: '北京市'
};

const nameKeyof = getPropertyKeyof(personKeyof, 'name');    // string 类型
const ageKeyof = getPropertyKeyof(personKeyof, 'age');      // number 类型

// keyof 与索引签名
interface StringArray {
  [index: number]: string;
}

type ArrayKeys = keyof StringArray;              // number | 数组的方法名
// 实际上是 number | "length" | "toString" | ... 等所有数组属性

// ---------- typeof 操作符 ----------
// typeof 操作符用于获取一个值的类型
// 在类型上下文中使用 typeof，可以获取变量或属性的类型

// 基本用法
const userTypeof = { name: '张三', age: 25 };
type UserTypeof = typeof userTypeof;
// 等价于：{ name: string; age: number; }

// 对函数使用 typeof
function addTypeof(a: number, b: number): number {
  return a + b;
}

type AddType = typeof addTypeof;
// 等价于：(a: number, b: number) => number

// 结合 keyof 使用
type UserKeys = keyof typeof userTypeof;         // 'name' | 'age'

// 对枚举使用 typeof
enum ColorTypeof {
  Red = 'red',
  Green = 'green',
  Blue = 'blue'
}

type ColorType = typeof ColorTypeof;             // 枚举对象的类型
type ColorKeys = keyof typeof ColorTypeof;       // 'Red' | 'Green' | 'Blue'

// ---------- 类型查询的应用 ----------
// 当值的类型很复杂时，使用 typeof 可以避免重复定义类型

// 例如，从配置对象中提取类型
const config = {
  apiUrl: 'https://api.example.com',
  timeout: 5000,
  retries: 3,
  features: {
    darkMode: true,
    multiLanguage: false
  }
};

// 从值中提取类型
type Config = typeof config;
// 这样就不需要手动定义 Config 接口了

// ============================================================
//  8. 索引访问类型（Indexed Access Types）
// ============================================================

// 索引访问类型用于访问类型的某个属性的类型
// 语法：T[K]，其中 T 是对象类型，K 是键类型

interface PersonIndex {
  name: string;
  age: number;
  address: {
    city: string;
    street: string;
  };
}

// 访问单个属性的类型
type NameType = PersonIndex['name'];              // string
type AgeType = PersonIndex['age'];                // number
type AddressType = PersonIndex['address'];        // { city: string; street: string }

// 访问嵌套属性的类型
type CityType = PersonIndex['address']['city'];   // string

// 使用联合类型作为索引
type NameOrAge = PersonIndex['name' | 'age'];     // string | number

// 使用 keyof 作为索引
type AllTypes = PersonIndex[keyof PersonIndex];   // string | number | { city: string; street: string }

// ---------- 索引访问类型的应用 ----------

// 获取数组元素的类型
const fruits = ['apple', 'banana', 'orange'];
type Fruit = typeof fruits[number];              // string

// 获取函数参数的类型
function createPerson(name: string, age: number, address: string): PersonIndex {
  return { name, age, address: { city: address, street: '' } };
}

type CreatePersonParams = Parameters<typeof createPerson>;
type FirstParam = CreatePersonParams[0];         // string
type SecondParam = CreatePersonParams[1];        // number

// 与泛型结合使用
function getPropertyIndexed<T, K extends keyof T>(obj: T, key: K): T[K] {
  return obj[key];
}

// ============================================================
//  9. 映射类型修饰符（+/- readonly / +/- ?）
// ============================================================

// 在映射类型中，可以使用 + 和 - 修饰符来添加或移除属性修饰符
// + 添加修饰符（默认）
// - 移除修饰符

// 定义一个示例类型
type MappedExample = {
  readonly a: string;
  readonly b: number;
  c: boolean;
  d?: string;
};

// ---------- 移除只读修饰符（-readonly）----------
type MutableExample = {
  -readonly [P in keyof MappedExample]: MappedExample[P];
};
// 结果：{ a: string; b: number; c: boolean; d?: string; }
// 所有 readonly 修饰符都被移除了

// ---------- 添加只读修饰符（+readonly）----------
type ReadonlyExample = {
  +readonly [P in keyof MappedExample]: MappedExample[P];
};
// 结果：{ readonly a: string; readonly b: number; readonly c: boolean; readonly d?: string; }
// 所有属性都添加了 readonly（已经有的不受影响）

// ---------- 移除可选修饰符（-?）----------
type RequiredExample = {
  [P in keyof MappedExample]-?: MappedExample[P];
};
// 结果：{ readonly a: string; readonly b: number; c: boolean; d: string; }
// 所有可选属性都变为必需的

// ---------- 添加可选修饰符（+?）----------
type OptionalExample = {
  [P in keyof MappedExample]+?: MappedExample[P];
};
// 结果：{ readonly a?: string; readonly b?: number; c?: boolean; d?: string; }
// 所有属性都变为可选的

// ---------- 组合使用 ----------
// 可以同时使用多种修饰符

// 移除只读和可选（所有属性变为可变且必需）
type MutableRequiredExample = {
  -readonly [P in keyof MappedExample]-?: MappedExample[P];
};

// 添加只读和可选（所有属性变为只读且可选）
type ReadonlyOptionalExample = {
  +readonly [P in keyof MappedExample]+?: MappedExample[P];
};

// ---------- 实际应用示例 ----------

// 自定义工具类型：将所有属性变为可变
type Mutable<T> = {
  -readonly [P in keyof T]: T[P];
};

// 自定义工具类型：将所有属性变为必需
type MyRequired<T> = {
  [P in keyof T]-?: T[P];
};

// 自定义工具类型：深度只读
type DeepReadonly<T> = {
  readonly [P in keyof T]: T[P] extends object ? DeepReadonly<T[P]> : T[P];
};

// 使用示例
interface PersonMutable {
  name: string;
  age: number;
}

const personMutable: Mutable<Readonly<PersonMutable>> = {
  name: '张三',
  age: 25
};
personMutable.name = '李四';                     // 现在可以修改了

// ============================================================
//  演示函数：运行所有高级特性示例
// ============================================================

export function runAdvancedDemo(): void {
  console.log('========== 高级特性演示 ==========');

  // 装饰器
  console.log('1. 装饰器:');
  console.log('   类装饰器 @sealed 已应用于 Greeter 类');
  const greeter = new Greeter('World');
  console.log('   greeter.greet():', greeter.greet());

  console.log('   @logClass 装饰器已应用于 UserAdv 类');
  const calc = new Calculator();
  console.log('   calculator.add(1, 2) =', calc.add(1, 2));
  console.log('   calculator.multiply(2, 1000) =', calc.multiply(2, 1000));

  // 命名空间
  console.log('\n2. 命名空间:');
  console.log('   Validators.validateEmail("test@example.com") =',
    Validators.validateEmail('test@example.com'));
  console.log('   LettersOnlyValidator.isAcceptable("Hello") =',
    new Validators.LettersOnlyValidator().isAcceptable('Hello'));

  // 类型兼容
  console.log('\n3. 类型兼容（结构类型系统）:');
  const personCompat: Named = new PersonNamedAdv('张三');
  console.log('   PersonNamedAdv 可赋值给 Named（结构兼容）:', personCompat.name);

  // 模板字面量类型
  console.log('\n4. 模板字面量类型:');
  const greeting: Greeting = 'hello TypeScript';
  console.log('   Greeting 类型:', greeting);

  type CSSProp = CSSPropertyDirection;
  console.log('   CSSPropertyDirection 类型（编译时类型）: margin-top | margin-right | ...');

  // keyof / typeof
  console.log('\n5. keyof / typeof 操作符:');
  type Keys = keyof PersonKeyof;
  console.log('   keyof PersonKeyof = "name" | "age" | "address"（编译时类型）');

  const userT = { name: '张三', age: 25 };
  type UserT = typeof userT;
  console.log('   typeof userT = { name: string; age: number }（编译时类型）');

  // 索引访问类型
  console.log('\n6. 索引访问类型:');
  type N = PersonIndex['name'];
  console.log('   PersonIndex["name"] = string（编译时类型）');
  console.log('   PersonIndex["address"]["city"] = string（编译时类型）');

  // 映射类型修饰符
  console.log('\n7. 映射类型修饰符:');
  console.log('   -readonly 移除只读修饰符');
  console.log('   -? 移除可选修饰符');
  console.log('   +readonly 添加只读修饰符');
  console.log('   +? 添加可选修饰符');

  console.log('\n========== 高级特性演示结束 ==========\n');
}
