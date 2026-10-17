/**
 * ============================================================
 *  TypeScript 泛型（Generics）详解
 * ============================================================
 *
 * 本文件详细介绍 TypeScript 中的泛型特性，包括：
 * 1. 泛型函数
 * 2. 泛型接口
 * 3. 泛型类
 * 4. 泛型约束（extends 约束）
 * 5. 泛型默认类型
 * 6. 条件类型 T extends U ? X : Y
 * 7. 泛型工具类型
 * 8. 映射类型、infer 推断类型
 * ============================================================
 */

// ============================================================
//  1. 泛型函数
// ============================================================

// 泛型是 TypeScript 中最强大的特性之一
// 它允许我们创建可重用的组件，这些组件可以支持多种类型而不是单一类型
// 泛型使用 <T> 语法，其中 T 是类型参数（Type Parameter）

// ---------- 为什么需要泛型 ----------
// 假设我们需要一个函数，返回传入的参数本身
// 如果不用泛型，可能会这样写：

function identityAny(arg: any): any {
  return arg;
}
// 问题：丢失了类型信息，返回值是 any 类型，失去了类型安全

// 使用泛型后：
/**
 * 恒等函数：返回传入的参数本身
 * @param arg 任意类型的参数
 * @returns 与输入相同类型的值
 * @template T 类型参数，表示输入和输出的类型
 */
function identity<T>(arg: T): T {
  return arg;
}

// 使用泛型函数
// 方式一：显式指定类型参数
const output1 = identity<string>('hello');      // output1 的类型是 string
const output2 = identity<number>(42);           // output2 的类型是 number

// 方式二：让 TypeScript 自动推断类型参数
const output3 = identity('world');              // TypeScript 推断 T 为 string
const output4 = identity(100);                  // TypeScript 推断 T 为 number

// ---------- 多个类型参数 ----------
// 泛型函数可以有多个类型参数

/**
 * 交换元组中两个元素的位置
 * @param tuple 包含两个元素的元组
 * @returns 交换后的元组
 * @template T 第一个元素的类型
 * @template U 第二个元素的类型
 */
function swap<T, U>(tuple: [T, U]): [U, T] {
  return [tuple[1], tuple[0]];
}

const swapped = swap(['hello', 42]);
// swapped 的类型是 [number, string]
console.log(swapped[0].toFixed(2));              // 42.00（TypeScript 知道是 number）
console.log(swapped[1].toUpperCase());           // HELLO（TypeScript 知道是 string）

// ---------- 泛型函数类型 ----------
// 泛型函数本身也是一种类型

// 定义泛型函数类型
type GenericIdentityFn = <T>(arg: T) => T;

// 使用泛型函数类型
let myIdentity: GenericIdentityFn = identity;

// ============================================================
//  2. 泛型接口
// ============================================================

// 接口也可以使用泛型，使接口更加灵活和可复用

// ---------- 基本泛型接口 ----------
/**
 * 泛型容器接口
 * 用于包装任意类型的值
 * @template T 被包装值的类型
 */
interface Container<T> {
  value: T;                                      // 被包装的值
  getValue(): T;                                 // 获取值的方法
  setValue(newValue: T): void;                   // 设置值的方法
}

// 实现泛型接口
class SimpleContainer<T> implements Container<T> {
  value: T;

  constructor(initialValue: T) {
    this.value = initialValue;
  }

  getValue(): T {
    return this.value;
  }

  setValue(newValue: T): void {
    this.value = newValue;
  }
}

const stringContainer = new SimpleContainer<string>('hello');
console.log(stringContainer.getValue());         // hello

const numberContainer = new SimpleContainer<number>(42);
console.log(numberContainer.getValue());         // 42

// ---------- 泛型接口的多种用法 ----------

// 泛型数组接口
interface GenericArray<T> {
  [index: number]: T;                            // 索引签名
  length: number;                                // 长度
  push(...items: T[]): number;                   // 添加元素
  pop(): T | undefined;                          // 弹出元素
}

// 泛型函数接口（调用签名）
interface GenericFn {
  <T>(arg: T): T;                                // 调用签名，类型参数在调用签名上
}

// 另一种写法：类型参数在接口名上
interface GenericFn2<T> {
  (arg: T): T;                                   // 调用签名
}

// ---------- 常见泛型接口示例 ----------
// 键值对接口
interface KeyValuePair<K, V> {
  key: K;
  value: V;
}

const pair1: KeyValuePair<string, number> = {
  key: 'age',
  value: 25
};

const pair2: KeyValuePair<number, boolean> = {
  key: 1,
  value: true
};

// ============================================================
//  3. 泛型类
// ============================================================

// 类也可以使用泛型，使类的属性和方法能够支持多种类型

/**
 * 泛型栈（Stack）类
 * 后进先出（LIFO）的数据结构
 * @template T 栈中元素的类型
 */
class Stack<T> {
  private items: T[] = [];                       // 存储栈元素的数组

  /**
   * 入栈：向栈顶添加元素
   * @param item 要添加的元素
   */
  push(item: T): void {
    this.items.push(item);
  }

  /**
   * 出栈：从栈顶移除并返回元素
   * @returns 栈顶元素，如果栈为空则返回 undefined
   */
  pop(): T | undefined {
    return this.items.pop();
  }

  /**
   * 查看栈顶元素（不移除）
   * @returns 栈顶元素，如果栈为空则返回 undefined
   */
  peek(): T | undefined {
    return this.items[this.items.length - 1];
  }

  /**
   * 判断栈是否为空
   */
  isEmpty(): boolean {
    return this.items.length === 0;
  }

  /**
   * 获取栈的大小
   */
  size(): number {
    return this.items.length;
  }
}

// 使用泛型类
const numberStack = new Stack<number>();
numberStack.push(1);
numberStack.push(2);
numberStack.push(3);
console.log(numberStack.pop());                  // 3
console.log(numberStack.peek());                 // 2

const stringStack = new Stack<string>();
stringStack.push('a');
stringStack.push('b');
console.log(stringStack.pop());                  // 'b'

// ---------- 泛型类的多个类型参数 ----------
/**
 * 泛型键值存储类
 * @template K 键的类型
 * @template V 值的类型
 */
class KeyValueStore<K, V> {
  private data: Map<K, V> = new Map();

  set(key: K, value: V): void {
    this.data.set(key, value);
  }

  get(key: K): V | undefined {
    return this.data.get(key);
  }

  has(key: K): boolean {
    return this.data.has(key);
  }

  delete(key: K): boolean {
    return this.data.delete(key);
  }
}

const store = new KeyValueStore<string, number>();
store.set('age', 25);
store.set('score', 95);
console.log(store.get('age'));                   // 25

// ============================================================
//  4. 泛型约束（Generic Constraints）
// ============================================================

// 有时候我们希望限制类型参数的范围，而不是接受任意类型
// 使用 extends 关键字来定义泛型约束
// 被约束的类型参数必须满足约束条件（具有指定的属性或方法）

// ---------- 基本约束 ----------

// 定义一个接口作为约束条件
interface Lengthwise {
  length: number;                                // 必须有 length 属性
}

/**
 * 记录并返回具有 length 属性的值
 * @param arg 具有 length 属性的参数
 * @returns 与输入相同类型的值
 * @template T 类型参数，必须具有 length 属性
 */
function logWithLength<T extends Lengthwise>(arg: T): T {
  console.log('长度:', arg.length);              // 现在可以安全地访问 .length
  return arg;
}

logWithLength('hello');                          // 正确，字符串有 length 属性
logWithLength([1, 2, 3]);                        // 正确，数组有 length 属性
logWithLength({ length: 10, name: 'test' });     // 正确，对象有 length 属性
// logWithLength(123);                            // 错误，数字没有 length 属性

// ---------- 多约束 ----------
// 类型参数可以有多个约束，使用交叉类型 & 连接

interface Named {
  name: string;
}

interface Aged {
  age: number;
}

// 同时具有 name 和 age 属性
function greetPerson<T extends Named & Aged>(person: T): string {
  return `你好，${person.name}，你今年${person.age}岁`;
}

const person = { name: '张三', age: 25, email: 'zhangsan@example.com' };
console.log(greetPerson(person));                // 正确，有 name 和 age

// ---------- keyof 约束 ----------
// 使用 keyof 操作符约束类型参数为另一个类型的键

/**
 * 安全地获取对象的属性值
 * @param obj 目标对象
 * @param key 属性名（必须是 obj 的有效属性）
 * @returns 属性值
 * @template T 对象类型
 * @template K 键类型，必须是 T 的键
 */
function getProperty<T, K extends keyof T>(obj: T, key: K): T[K] {
  return obj[key];
}

const userG = { name: '张三', age: 25, email: 'zhangsan@example.com' };

const userName = getProperty(userG, 'name');     // 类型为 string
const userAge = getProperty(userG, 'age');       // 类型为 number
// const userFoo = getProperty(userG, 'foo');     // 错误，'foo' 不是 userG 的属性

// ---------- 类类型约束 ----------
// 使用 new () => T 约束类型参数为可构造的类

interface AnimalGeneric {
  name: string;
}

interface AnimalConstructor<T extends AnimalGeneric> {
  new (name: string): T;                         // 构造函数签名
}

/**
 * 创建动物实例的工厂函数
 * @param ctor 动物类的构造函数
 * @param name 动物名称
 * @returns 动物实例
 * @template T 动物类型，必须是 Animal 的子类
 */
function createAnimal<T extends AnimalGeneric>(
  ctor: AnimalConstructor<T>,
  name: string
): T {
  return new ctor(name);
}

class DogGeneric implements AnimalGeneric {
  name: string;
  constructor(name: string) {
    this.name = name;
  }
  bark(): void {
    console.log('汪汪！');
  }
}

const dogGeneric = createAnimal(DogGeneric, '旺财');
dogGeneric.bark();

// ============================================================
//  5. 泛型默认类型
// ============================================================

// 泛型参数可以指定默认类型
// 当使用时不指定类型参数时，会使用默认类型
// 使用 = 语法指定默认类型

/**
 * 泛型函数，带默认类型
 * @template T 类型参数，默认为 string
 */
function defaultValueFunc<T = string>(value: T): T {
  return value;
}

// 使用默认类型（T 为 string）
const defaultStr = defaultValueFunc('hello');    // string 类型
// 显式指定类型
const defaultNum = defaultValueFunc<number>(42); // number 类型

// ---------- 泛型类的默认类型 ----------
/**
 * 泛型字典类，带默认类型
 * @template K 键的类型，默认为 string
 * @template V 值的类型，默认为 any
 */
class Dictionary<K = string, V = any> {
  private data: Record<string, V> = {};

  set(key: K extends string ? K : string, value: V): void {
    this.data[key as string] = value;
  }

  get(key: K extends string ? K : string): V | undefined {
    return this.data[key as string];
  }
}

// 使用默认类型
const dict1 = new Dictionary();                  // Dictionary<string, any>
dict1.set('name', '张三');

// 显式指定类型
const dict2 = new Dictionary<string, number>();  // Dictionary<string, number>
dict2.set('age', 25);

// ============================================================
//  6. 条件类型（Conditional Types）
// ============================================================

// 条件类型是 TypeScript 中最强大的类型编程工具之一
// 语法：T extends U ? X : Y
// 含义：如果 T 可以赋值给 U，则类型为 X，否则为 Y

// ---------- 基本条件类型 ----------

// 判断类型是否为字符串
type IsString<T> = T extends string ? true : false;

type A = IsString<string>;                       // true
type B = IsString<number>;                       // false
type C = IsString<'hello'>;                      // true（字符串字面量也是 string）

// ---------- 条件类型与联合类型 ----------
// 当条件类型的类型参数是联合类型时，会分配（distributive）到每个成员

// 条件类型在联合类型上的分配行为
type ToArray<T> = T extends any ? T[] : never;

type StrArrOrNumArr = ToArray<string | number>;
// 等价于：string[] | number[]
// 因为：ToArray<string> | ToArray<number> = string[] | number[]

// 如果不想要分配行为，可以用 [] 包裹类型参数
type ToArrayNonDist<T> = [T] extends [any] ? T[] : never;

type ArrOfStrOrNum = ToArrayNonDist<string | number>;
// 等价于：(string | number)[]
// 因为整个联合类型作为一个整体进行判断

// ---------- 常用条件类型示例 ----------

// 从类型中排除 null 和 undefined
type NonNull<T> = T extends null | undefined ? never : T;

type MaybeString = string | null | undefined;
type DefinitelyString = NonNull<MaybeString>;   // string

// 判断是否是函数类型
type IsFunction<T> = T extends (...args: any[]) => any ? true : false;

type FuncCheck = IsFunction<() => void>;         // true
type NonFuncCheck = IsFunction<string>;          // false

// ============================================================
//  7. 泛型工具类型（Utility Types）
// ============================================================

// TypeScript 内置了许多实用的泛型工具类型
// 这些工具类型都是基于条件类型和映射类型实现的

// 先定义一个示例接口
interface UserUtil {
  id: number;
  name: string;
  age: number;
  email: string;
  address?: string;
}

// ---------- Partial<T> ----------
// 将 T 的所有属性变为可选的
// 实现原理：type Partial<T> = { [P in keyof T]?: T[P]; }

type PartialUser = Partial<UserUtil>;
// 等价于：
// {
//   id?: number;
//   name?: string;
//   age?: number;
//   email?: string;
//   address?: string;
// }

// 使用场景：更新用户时，不需要提供所有属性
function updateUser(user: UserUtil, updates: Partial<UserUtil>): UserUtil {
  return { ...user, ...updates };
}

const userUtil: UserUtil = { id: 1, name: '张三', age: 25, email: 'zhang@example.com' };
const updated = updateUser(userUtil, { age: 26, email: 'new@example.com' });

// ---------- Required<T> ----------
// 将 T 的所有属性变为必需的（去掉可选 ?）
// 实现原理：type Required<T> = { [P in keyof T]-?: T[P]; }

type RequiredUser = Required<UserUtil>;
// 所有属性都是必需的，包括 address

// ---------- Readonly<T> ----------
// 将 T 的所有属性变为只读的
// 实现原理：type Readonly<T> = { readonly [P in keyof T]: T[P]; }

type ReadonlyUser = Readonly<UserUtil>;
// 所有属性都是 readonly 的

// 使用场景：创建不可变对象
const readonlyUser: ReadonlyUser = { id: 1, name: '张三', age: 25, email: 'zhang@example.com' };
// readonlyUser.name = '李四';                    // 错误，只读属性

// ---------- Pick<T, K> ----------
// 从 T 中挑选出指定的属性 K 组成新类型
// 实现原理：type Pick<T, K extends keyof T> = { [P in K]: T[P]; }

type UserNameAndEmail = Pick<UserUtil, 'name' | 'email'>;
// 等价于：
// {
//   name: string;
//   email: string;
// }

// 使用场景：只需要对象的部分属性
function displayUserInfo(user: Pick<UserUtil, 'name' | 'email'>): void {
  console.log(`姓名: ${user.name}, 邮箱: ${user.email}`);
}

// ---------- Omit<T, K> ----------
// 从 T 中排除指定的属性 K，返回剩余属性组成的类型
// 实现原理：type Omit<T, K extends keyof any> = Pick<T, Exclude<keyof T, K>>

type UserWithoutPassword = Omit<UserUtil, 'age'>;
// 等价于 UserUtil 去掉 age 属性

// ---------- Record<K, T> ----------
// 创建一个对象类型，键的类型为 K，值的类型为 T
// 实现原理：type Record<K extends keyof any, T> = { [P in K]: T; }

type StringRecord = Record<string, number>;      // { [key: string]: number }

type RolePermissions = Record<'admin' | 'user' | 'guest', string[]>;
// 等价于：
// {
//   admin: string[];
//   user: string[];
//   guest: string[];
// }

const permissions: RolePermissions = {
  admin: ['read', 'write', 'delete', 'manage'],
  user: ['read', 'write'],
  guest: ['read']
};

// ---------- ReturnType<T> ----------
// 获取函数类型 T 的返回值类型
// 实现原理：type ReturnType<T extends (...args: any) => any> = T extends (...args: any) => infer R ? R : any;

function createUser(name: string, age: number): UserUtil {
  return { id: 1, name, age, email: `${name}@example.com` };
}

type UserReturnType = ReturnType<typeof createUser>;  // UserUtil
type MathReturnType = ReturnType<typeof Math.max>;    // number

// ---------- Parameters<T> ----------
// 获取函数类型 T 的参数类型组成的元组
// 实现原理：type Parameters<T extends (...args: any) => any> = T extends (...args: infer P) => any ? P : never;

type CreateUserParams = Parameters<typeof createUser>;
// 等价于：[string, number]

const params: CreateUserParams = ['张三', 25];
const newUser = createUser(...params);

// ---------- Exclude<T, U> ----------
// 从联合类型 T 中排除可以赋值给 U 的类型
// 实现原理：type Exclude<T, U> = T extends U ? never : T;

type AllTypes = string | number | boolean | null;
type NonNullTypes = Exclude<AllTypes, null>;     // string | number | boolean

type Letters = 'a' | 'b' | 'c' | 'd';
type WithoutAB = Exclude<Letters, 'a' | 'b'>;    // 'c' | 'd'

// ---------- Extract<T, U> ----------
// 从联合类型 T 中提取可以赋值给 U 的类型（与 Exclude 相反）
// 实现原理：type Extract<T, U> = T extends U ? T : never;

type Extracted = Extract<AllTypes, string | number>;  // string | number
type ExtractedLetters = Extract<Letters, 'a' | 'b' | 'e'>;  // 'a' | 'b'

// ============================================================
//  8. 映射类型、infer 推断类型
// ============================================================

// ---------- 映射类型（Mapped Types）----------
// 映射类型允许我们基于已有类型创建新类型
// 通过遍历已有类型的属性，对每个属性进行转换

// 基本映射类型语法：{ [P in K]: T }
// 其中 K 通常是 keyof 某个类型得到的联合类型

// 自定义映射类型：将所有属性变为 string 类型
type AllString<T> = {
  [P in keyof T]: string;
};

type UserAllString = AllString<UserUtil>;
// 所有属性类型都变为 string

// 自定义映射类型：深度可选
type DeepPartial<T> = {
  [P in keyof T]?: T[P] extends object ? DeepPartial<T[P]> : T[P];
};

interface NestedUser {
  name: string;
  profile: {
    age: number;
    address: {
      city: string;
      street: string;
    };
  };
}

type DeepPartialUser = DeepPartial<NestedUser>;
// 所有嵌套属性也都是可选的

// ---------- 映射类型修饰符 ----------
// 在映射类型中，可以使用 + 和 - 来添加或移除修饰符
// +? 添加可选，-? 移除可选
// +readonly 添加只读，-readonly 移除只读

// 将所有属性变为必需的（移除可选修饰符）
type MyRequired<T> = {
  [P in keyof T]-?: T[P];
};

// 将所有属性变为可变的（移除只读修饰符）
type Mutable<T> = {
  -readonly [P in keyof T]: T[P];
};

const readonlyObj: Readonly<UserUtil> = { id: 1, name: '张三', age: 25, email: 'z@e.com' };
const mutableObj: Mutable<Readonly<UserUtil>> = readonlyObj;
mutableObj.name = '李四';                         // 现在可以修改了

// ---------- infer 推断类型 ----------
// infer 关键字用于在条件类型中推断类型
// 只能在条件类型的 extends 子句中使用
// 可以理解为：声明一个类型变量，让 TypeScript 自动推断

// 示例 1：推断函数返回类型（ReturnType 的简化实现）
type MyReturnType<T> = T extends (...args: any[]) => infer R ? R : any;

type Ret1 = MyReturnType<() => string>;          // string
type Ret2 = MyReturnType<(a: number, b: number) => number>;  // number

// 示例 2：推断函数参数类型（Parameters 的简化实现）
type MyParameters<T> = T extends (...args: infer P) => any ? P : never;

type Params1 = MyParameters<(a: string, b: number) => void>;  // [string, number]

// 示例 3：推断 Promise 的值类型
type UnpackPromise<T> = T extends Promise<infer U> ? U : T;

type PromiseValue = UnpackPromise<Promise<string>>;  // string
type NonPromiseValue = UnpackPromise<number>;        // number

// 示例 4：推断数组元素类型
type ElementType<T> = T extends (infer U)[] ? U : T;

type NumArrElement = ElementType<number[]>;      // number
type StrArrElement = ElementType<string[]>;      // string

// 示例 5：深度解包 Promise
type DeepUnpackPromise<T> = T extends Promise<infer U> ? DeepUnpackPromise<U> : T;

type DeepPromiseValue = DeepUnpackPromise<Promise<Promise<number>>>;  // number

// ============================================================
//  演示函数：运行所有泛型示例
// ============================================================

export function runGenericsDemo(): void {
  console.log('========== 泛型演示 ==========');

  // 泛型函数
  console.log('1. 泛型函数:');
  console.log('   identity<string>("hello") =', identity<string>('hello'));
  console.log('   identity(42) =', identity(42));
  console.log('   swap(["hello", 42]) =', swap(['hello', 42]));

  // 泛型接口
  console.log('\n2. 泛型接口:');
  const sc = new SimpleContainer<string>('hello');
  console.log('   SimpleContainer<string> =', sc.getValue());

  // 泛型类
  console.log('\n3. 泛型类:');
  const stack = new Stack<number>();
  stack.push(1);
  stack.push(2);
  stack.push(3);
  console.log('   Stack 栈顶 =', stack.peek());
  console.log('   Stack 大小 =', stack.size());

  // 泛型约束
  console.log('\n4. 泛型约束:');
  logWithLength('hello world');
  logWithLength([1, 2, 3]);

  const userG = { name: '张三', age: 25, email: 'zhang@example.com' };
  console.log('   getProperty(user, "name") =', getProperty(userG, 'name'));

  // 泛型默认类型
  console.log('\n5. 泛型默认类型:');
  const defaultVal = defaultValueFunc('默认字符串');
  console.log('   defaultValueFunc("默认字符串") =', defaultVal);

  // 条件类型
  console.log('\n6. 条件类型:');
  type TestString = IsString<string>;
  type TestNumber = IsString<number>;
  console.log('   IsString<string> = true（编译时类型）');
  console.log('   IsString<number> = false（编译时类型）');

  // 工具类型
  console.log('\n7. 泛型工具类型:');
  const partialUser: Partial<UserUtil> = { name: '张三' };
  console.log('   Partial<UserUtil>:', partialUser);

  const record: Record<string, number> = { a: 1, b: 2 };
  console.log('   Record<string, number>:', record);

  type RT = ReturnType<typeof createUser>;
  console.log('   ReturnType<createUser> = UserUtil（编译时类型）');

  // 映射类型与 infer
  console.log('\n8. 映射类型与 infer:');
  type Unpacked = UnpackPromise<Promise<string>>;
  console.log('   UnpackPromise<Promise<string>> = string（编译时类型）');
  console.log('   ElementType<number[]> = number（编译时类型）');

  console.log('\n========== 泛型演示结束 ==========\n');
}
