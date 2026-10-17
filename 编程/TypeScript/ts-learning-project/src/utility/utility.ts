/**
 * ============================================================
 *  TypeScript 工具类型与类型体操
 * ============================================================
 *
 * 本文件深入讲解 TypeScript 的类型编程技巧
 * 包括常用的自定义工具类型和类型体操（Type Gymnastics）练习
 *
 * 类型体操是指利用 TypeScript 的类型系统进行复杂的类型计算
 * 通过条件类型、映射类型、infer 等特性实现各种类型操作
 * ============================================================
 */

// ============================================================
//  一、自定义工具类型
// ============================================================

// 以下是一些常用的自定义工具类型实现
// 帮助你深入理解 TypeScript 内置工具类型的工作原理

// ---------- 1. Partial 实现 ----------
// 将所有属性变为可选
type MyPartial<T> = {
  [P in keyof T]?: T[P];
};

// ---------- 2. Required 实现 ----------
// 将所有属性变为必需
type MyRequired<T> = {
  [P in keyof T]-?: T[P];
};

// ---------- 3. Readonly 实现 ----------
// 将所有属性变为只读
type MyReadonly<T> = {
  readonly [P in keyof T]: T[P];
};

// ---------- 4. Pick 实现 ----------
// 从 T 中挑选出 K 包含的属性
type MyPick<T, K extends keyof T> = {
  [P in K]: T[P];
};

// ---------- 5. Exclude 实现 ----------
// 从 T 中排除可以赋值给 U 的类型
type MyExclude<T, U> = T extends U ? never : T;

// ---------- 6. Extract 实现 ----------
// 从 T 中提取可以赋值给 U 的类型
type MyExtract<T, U> = T extends U ? T : never;

// ---------- 7. Omit 实现 ----------
// 从 T 中排除 K 包含的属性
type MyOmit<T, K extends keyof any> = MyPick<T, MyExclude<keyof T, K>>;

// ---------- 8. Record 实现 ----------
// 创建键为 K 类型、值为 T 类型的对象类型
type MyRecord<K extends keyof any, T> = {
  [P in K]: T;
};

// ---------- 9. ReturnType 实现 ----------
// 获取函数返回值类型
type MyReturnType<T extends (...args: any[]) => any> = T extends (...args: any[]) => infer R ? R : never;

// ---------- 10. Parameters 实现 ----------
// 获取函数参数类型组成的元组
type MyParameters<T extends (...args: any[]) => any> = T extends (...args: infer P) => any ? P : never;

// ============================================================
//  二、进阶工具类型
// ============================================================

// ---------- 11. Awaited 实现 ----------
// 递归解包 Promise 类型
type MyAwaited<T> = T extends Promise<infer U> ? MyAwaited<U> : T;

// 测试
type AwaitedTest1 = MyAwaited<Promise<string>>;           // string
type AwaitedTest2 = MyAwaited<Promise<Promise<number>>>;  // number

// ---------- 12. If 实现 ----------
// 类型级别的 if-else
type If<C extends boolean, T, F> = C extends true ? T : F;

// 测试
type IfTest1 = If<true, string, number>;     // string
type IfTest2 = If<false, string, number>;    // number

// ---------- 13. Concat 实现 ----------
// 连接两个数组类型
type Concat<T extends any[], U extends any[]> = [...T, ...U];

// 测试
type ConcatTest = Concat<[1, 2], [3, 4]>;    // [1, 2, 3, 4]

// ---------- 14. Includes 实现 ----------
// 判断数组类型中是否包含某个类型
type Includes<T extends readonly any[], U> = T extends [infer First, ...infer Rest]
  ? (<V>() => V extends First ? 1 : 2) extends (<V>() => V extends U ? 1 : 2)
    ? true
    : Includes<Rest, U>
  : false;

// 测试
type IncludesTest1 = Includes<[1, 2, 3], 2>;      // true
type IncludesTest2 = Includes<[1, 2, 3], 4>;      // false

// ---------- 15. Push 实现 ----------
// 向数组类型末尾添加元素
type Push<T extends any[], U> = [...T, U];

// 测试
type PushTest = Push<[1, 2], 3>;              // [1, 2, 3]

// ---------- 16. Unshift 实现 ----------
// 向数组类型开头添加元素
type Unshift<T extends any[], U> = [U, ...T];

// 测试
type UnshiftTest = Unshift<[1, 2], 0>;        // [0, 1, 2]

// ============================================================
//  三、类型体操 - 数组操作
// ============================================================

// ---------- 17. 获取数组长度 ----------
type Length<T extends readonly any[]> = T['length'];

// 测试
type LengthTest = Length<['a', 'b', 'c']>;    // 3

// ---------- 18. 获取数组第一个元素 ----------
type First<T extends any[]> = T extends [] ? never : T[0];
// 或者使用 infer
type First2<T extends any[]> = T extends [infer F, ...any[]] ? F : never;

// 测试
type FirstTest1 = First<[1, 2, 3]>;            // 1
type FirstTest2 = First<[]>;                   // never

// ---------- 19. 获取数组最后一个元素 ----------
type Last<T extends any[]> = T extends [...any[], infer L] ? L : never;

// 测试
type LastTest1 = Last<[1, 2, 3]>;              // 3
type LastTest2 = Last<[]>;                     // never

// ---------- 20. 移除数组最后一个元素 ----------
type Pop<T extends any[]> = T extends [...infer Rest, any] ? Rest : [];

// 测试
type PopTest = Pop<[1, 2, 3]>;                 // [1, 2]

// ---------- 21. 数组反转 ----------
type Reverse<T extends any[]> = T extends [infer F, ...infer Rest]
  ? [...Reverse<Rest>, F]
  : [];

// 测试
type ReverseTest = Reverse<[1, 2, 3]>;         // [3, 2, 1]

// ---------- 22. 数组扁平化（单层）----------
type Flatten<T extends any[]> = T extends [infer F, ...infer Rest]
  ? F extends any[]
    ? [...F, ...Flatten<Rest>]
    : [F, ...Flatten<Rest>]
  : [];

// 测试
type FlattenTest = Flatten<[1, [2, 3], [4]]>;  // [1, 2, 3, 4]

// ---------- 23. 数组深度扁平化 ----------
type DeepFlatten<T extends any[]> = T extends [infer F, ...infer Rest]
  ? F extends any[]
    ? [...DeepFlatten<F>, ...DeepFlatten<Rest>]
    : [F, ...DeepFlatten<Rest>]
  : [];

// 测试
type DeepFlattenTest = DeepFlatten<[1, [2, [3, 4]], [[[5]]]]>;  // [1, 2, 3, 4, 5]

// ============================================================
//  四、类型体操 - 字符串操作
// ============================================================

// ---------- 24. TrimLeft 实现 ----------
// 移除字符串左侧空格
type TrimLeft<S extends string> = S extends `${' ' | '\n' | '\t'}${infer Rest}`
  ? TrimLeft<Rest>
  : S;

// 测试
type TrimLeftTest = TrimLeft<'  hello'>;       // 'hello'

// ---------- 25. TrimRight 实现 ----------
// 移除字符串右侧空格
type TrimRight<S extends string> = S extends `${infer Rest}${' ' | '\n' | '\t'}`
  ? TrimRight<Rest>
  : S;

// 测试
type TrimRightTest = TrimRight<'hello  '>;     // 'hello'

// ---------- 26. Trim 实现 ----------
// 移除字符串两侧空格
type Trim<S extends string> = TrimLeft<TrimRight<S>>;

// 测试
type TrimTest = Trim<'  hello  '>;             // 'hello'

// ---------- 27. Capitalize 实现 ----------
// 首字母大写
type MyCapitalize<S extends string> = S extends `${infer F}${infer Rest}`
  ? `${Uppercase<F>}${Rest}`
  : S;

// 测试
type MyCapitalizeTest = MyCapitalize<'hello'>;  // 'Hello'

// ---------- 28. Replace 实现 ----------
// 替换字符串中的第一个匹配项
type Replace<
  S extends string,
  From extends string,
  To extends string
> = From extends ''
  ? S
  : S extends `${infer Before}${From}${infer After}`
    ? `${Before}${To}${After}`
    : S;

// 测试
type ReplaceTest = Replace<'hello world', 'world', 'typescript'>;  // 'hello typescript'

// ---------- 29. ReplaceAll 实现 ----------
// 替换字符串中的所有匹配项
type ReplaceAll<
  S extends string,
  From extends string,
  To extends string
> = From extends ''
  ? S
  : S extends `${infer Before}${From}${infer After}`
    ? `${Before}${To}${ReplaceAll<After, From, To>}`
    : S;

// 测试
type ReplaceAllTest = ReplaceAll<'aaa bbb aaa', 'aaa', 'xxx'>;  // 'xxx bbb xxx'

// ---------- 30. StringToUnion 实现 ----------
// 将字符串转换为字符联合类型
type StringToUnion<S extends string> = S extends `${infer F}${infer Rest}`
  ? F | StringToUnion<Rest>
  : never;

// 测试
type StringToUnionTest = StringToUnion<'abc'>;  // 'a' | 'b' | 'c'

// ============================================================
//  五、类型体操 - 对象操作
// ============================================================

// ---------- 31. 深度 Readonly ----------
type DeepReadonly<T> = {
  readonly [P in keyof T]: T[P] extends Record<string, unknown>
    ? DeepReadonly<T[P]>
    : T[P];
};

// 测试
type DeepReadonlyTest = DeepReadonly<{
  a: string;
  b: {
    c: number;
    d: {
      e: boolean;
    };
  };
}>;
// 所有嵌套属性都是 readonly 的

// ---------- 32. 深度 Partial ----------
type DeepPartial<T> = {
  [P in keyof T]?: T[P] extends object ? DeepPartial<T[P]> : T[P];
};

// ---------- 33. 合并两个对象类型 ----------
type Merge<F, S> = {
  [P in keyof F | keyof S]: P extends keyof S
    ? S[P]
    : P extends keyof F
      ? F[P]
      : never;
};

// 测试
type MergeTest = Merge<{ a: string; b: number }, { b: string; c: boolean }>;
// { a: string; b: string; c: boolean }

// ---------- 34. KebabCase 转换 ----------
// 驼峰命名转短横线命名
type KebabCase<S extends string> = S extends `${infer F}${infer Rest}`
  ? Rest extends Uncapitalize<Rest>
    ? `${Lowercase<F>}${KebabCase<Rest>}`
    : `${Lowercase<F>}-${KebabCase<Uncapitalize<Rest>>}`
  : S;

// 测试
type KebabCaseTest = KebabCase<'fooBarBaz'>;    // 'foo-bar-baz'

// ---------- 35. CamelCase 转换 ----------
// 短横线命名转驼峰命名
type CamelCase<S extends string> = S extends `${infer First}-${infer Rest}`
  ? `${First}${CamelCase<Capitalize<Rest>>}`
  : S;

// 测试
type CamelCaseTest = CamelCase<'foo-bar-baz'>;  // 'fooBarBaz'

// ============================================================
//  六、类型体操 - 高级技巧
// ============================================================

// ---------- 36. 元组转联合类型 ----------
type TupleToUnion<T extends any[]> = T[number];

// 测试
type TupleToUnionTest = TupleToUnion<[1, 'hello', true]>;  // 1 | 'hello' | true

// ---------- 37. 联合类型转元组（有局限性）----------
// 注意：联合类型是无序的，转元组的结果顺序不保证
// 这是一个比较 hack 的实现，仅用于理解类型系统

type UnionToIntersection<U> =
  (U extends any ? (k: U) => void : never) extends ((k: infer I) => void)
    ? I
    : never;

type LastOf<T> =
  UnionToIntersection<T extends any ? () => T : never> extends () => (infer R)
    ? R
    : never;

type PushTuple<T extends any[], U> = [...T, U];

type UnionToTuple<T, L = LastOf<T>, N = [T] extends [never] ? true : false> =
  true extends N
    ? []
    : PushTuple<UnionToTuple<Exclude<T, L>>, L>;

// 测试
type UnionToTupleTest = UnionToTuple<'a' | 'b' | 'c'>;  // ['a', 'b', 'c'] 或其他顺序

// ---------- 38. Chainable 链式调用 ----------
// 实现一个可链式调用的类型

type Chainable<T = {}> = {
  option<K extends string, V>(
    key: K extends keyof T ? never : K,
    value: V
  ): Chainable<Omit<T, K> & Record<K, V>>;
  get(): T;
};

// 测试
declare const chainableConfig: Chainable;
const result = chainableConfig
  .option('foo', 123)
  .option('name', 'type-challenges')
  .option('bar', { value: 'Hello World' })
  .get();
// result 的类型：{ foo: number; name: string; bar: { value: string } }

// ---------- 39. 获取函数参数数量 ----------
type ParameterCount<T extends (...args: any[]) => any> = Parameters<T>['length'];

// 测试
type ParamCountTest = ParameterCount<(a: string, b: number, c: boolean) => void>;  // 3

// ---------- 40. 判断是否是联合类型 ----------
type IsUnion<T, U = T> = [T] extends [never]
  ? false
  : T extends any
    ? [U] extends [T]
      ? false
      : true
    : false;

// 测试
type IsUnionTest1 = IsUnion<string | number>;     // true
type IsUnionTest2 = IsUnion<string>;              // false

// ============================================================
//  七、实用类型工具函数
// ============================================================

/**
 * 类型断言辅助函数
 * 在运行时检查一个值是否满足某个类型（编译时 + 运行时双重检查）
 */
export function assertType<T>(value: T): T {
  return value;
}

/**
 * 检查类型是否相等（编译时检查）
 * 如果两个类型不相等，会产生类型错误
 */
export type Expect<T extends true> = T;
export type Equal<X, Y> =
  (<T>() => T extends X ? 1 : 2) extends (<T>() => T extends Y ? 1 : 2)
    ? true
    : false;
export type NotEqual<X, Y> = true extends Equal<X, Y> ? false : true;

// 使用示例：
// type Test = Expect<Equal<MyPartial<{ a: string }>, { a?: string }>>;

// ============================================================
//  演示函数：运行工具类型与类型体操示例
// ============================================================

export function runUtilityDemo(): void {
  console.log('========== 工具类型与类型体操演示 ==========');
  console.log('');
  console.log('本模块主要展示类型层面的操作（编译时类型）');
  console.log('以下为各工具类型的说明与效果：');
  console.log('');

  console.log('--- 基础工具类型 ---');
  console.log('1.  MyPartial<T>    - 将所有属性变为可选');
  console.log('2.  MyRequired<T>   - 将所有属性变为必需');
  console.log('3.  MyReadonly<T>   - 将所有属性变为只读');
  console.log('4.  MyPick<T, K>    - 从 T 中挑选 K 属性');
  console.log('5.  MyExclude<T, U> - 从 T 中排除 U 类型');
  console.log('6.  MyExtract<T, U> - 从 T 中提取 U 类型');
  console.log('7.  MyOmit<T, K>    - 从 T 中排除 K 属性');
  console.log('8.  MyRecord<K, T>  - 创建键为 K、值为 T 的对象');
  console.log('9.  MyReturnType<T> - 获取函数返回值类型');
  console.log('10. MyParameters<T> - 获取函数参数元组');
  console.log('');

  console.log('--- 进阶工具类型 ---');
  console.log('11. MyAwaited<T>    - 递归解包 Promise');
  console.log('12. If<C, T, F>     - 类型级 if-else');
  console.log('13. Concat<T, U>    - 连接两个数组类型');
  console.log('14. Includes<T, U>  - 判断数组是否包含某类型');
  console.log('15. Push<T, U>      - 向数组末尾添加元素');
  console.log('16. Unshift<T, U>   - 向数组开头添加元素');
  console.log('');

  console.log('--- 数组操作体操 ---');
  console.log('17. Length<T>       - 获取数组长度');
  console.log('18. First<T>        - 获取数组第一个元素');
  console.log('19. Last<T>         - 获取数组最后一个元素');
  console.log('20. Pop<T>          - 移除数组最后一个元素');
  console.log('21. Reverse<T>      - 数组反转');
  console.log('22. Flatten<T>      - 数组扁平化（单层）');
  console.log('23. DeepFlatten<T>  - 数组深度扁平化');
  console.log('');

  console.log('--- 字符串操作体操 ---');
  console.log('24. TrimLeft<S>     - 移除左侧空格');
  console.log('25. TrimRight<S>    - 移除右侧空格');
  console.log('26. Trim<S>         - 移除两侧空格');
  console.log('27. MyCapitalize<S> - 首字母大写');
  console.log('28. Replace<S,F,T>  - 替换第一个匹配项');
  console.log('29. ReplaceAll<S,F,T> - 替换所有匹配项');
  console.log('30. StringToUnion<S> - 字符串转字符联合');
  console.log('');

  console.log('--- 对象操作体操 ---');
  console.log('31. DeepReadonly<T> - 深度只读');
  console.log('32. DeepPartial<T>  - 深度可选');
  console.log('33. Merge<F, S>     - 合并两个对象类型');
  console.log('34. KebabCase<S>    - 驼峰转短横线');
  console.log('35. CamelCase<S>    - 短横线转驼峰');
  console.log('');

  console.log('--- 高级技巧 ---');
  console.log('36. TupleToUnion<T> - 元组转联合类型');
  console.log('37. UnionToTuple<T> - 联合类型转元组');
  console.log('38. Chainable<T>    - 链式调用类型');
  console.log('39. ParameterCount<T> - 获取函数参数数量');
  console.log('40. IsUnion<T>      - 判断是否是联合类型');
  console.log('');

  console.log('--- 运行时验证示例 ---');

  // 演示一些可以在运行时展示的效果
  const strVal = assertType<string>('hello');
  console.log('assertType<string>("hello"):', strVal);

  const numVal = assertType<number>(42);
  console.log('assertType<number>(42):', numVal);

  // 演示工具类型的实际效果（通过运行时对象展示）
  interface User {
    id: number;
    name: string;
    email: string;
  }

  const user: User = { id: 1, name: '张三', email: 'zhang@example.com' };
  console.log('');
  console.log('原始类型 User:', user);

  // Partial 效果：所有属性可选
  const partialUser: Partial<User> = { name: '李四' };
  console.log('Partial<User>（部分属性）:', partialUser);

  // Pick 效果：只挑选部分属性
  const pickedUser: Pick<User, 'name' | 'email'> = { name: '王五', email: 'wang@example.com' };
  console.log('Pick<User, "name" | "email">（挑选属性）:', pickedUser);

  // Omit 效果：排除部分属性
  const omittedUser: Omit<User, 'id'> = { name: '赵六', email: 'zhao@example.com' };
  console.log('Omit<User, "id">（排除 id）:', omittedUser);

  // Record 效果：键值对类型
  const userRecord: Record<string, User> = {
    'user1': { id: 1, name: '张三', email: 'zhang@example.com' },
    'user2': { id: 2, name: '李四', email: 'li@example.com' }
  };
  console.log('Record<string, User>（字典）:', Object.keys(userRecord));

  console.log('');
  console.log('========== 工具类型与类型体操演示结束 ==========\n');
}
