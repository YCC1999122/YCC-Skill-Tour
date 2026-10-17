/**
 * ============================================================
 *  TypeScript 函数类型详解
 * ============================================================
 *
 * 本文件详细介绍 TypeScript 中的函数类型，包括：
 * 1. 函数类型定义（参数类型 / 返回值类型）
 * 2. 可选参数、默认参数、剩余参数
 * 3. 函数重载
 * 4. 箭头函数与类型推断
 * 5. this 参数类型
 * 6. 高阶函数类型
 * ============================================================
 */

// ============================================================
//  1. 函数类型定义（参数类型 / 返回值类型）
// ============================================================

// ---------- 函数声明的类型定义 ----------
// 在函数声明中，可以为每个参数指定类型，也可以为返回值指定类型
// TypeScript 会根据 return 语句自动推断返回值类型，但显式指定更清晰

/**
 * 计算两个数字的和
 * @param a 第一个数字
 * @param b 第二个数字
 * @returns 两个数字的和
 */
function add(a: number, b: number): number {
  // 参数 a 和 b 都是 number 类型
  // 返回值类型也是 number
  return a + b;
}

// 调用函数时，参数类型必须匹配
const sum = add(1, 2);                         // 正确
// add('1', 2);                                 // 错误，第一个参数类型不匹配
// add(1);                                      // 错误，缺少参数
// add(1, 2, 3);                                // 错误，参数过多

// ---------- 函数表达式的类型定义 ----------
// 函数表达式可以单独定义函数类型

// 方式一：直接在变量上定义函数类型
let multiply: (x: number, y: number) => number;
// 赋值一个符合类型的函数
multiply = function(x: number, y: number): number {
  return x * y;
};

// 方式二：使用类型别名
// 定义一个函数类型别名
type MathOperation = (x: number, y: number) => number;

// 使用类型别名声明变量
let divide: MathOperation;
divide = function(x, y) {                      // 参数类型可以省略，会从类型推断
  return x / y;
};

// 方式三：使用接口定义函数类型
interface SearchFunc {
  // 调用签名（Call Signature）
  // 定义函数的参数类型和返回值类型
  (source: string, subString: string): boolean;
}

let mySearch: SearchFunc;
mySearch = function(source: string, subString: string): boolean {
  return source.search(subString) !== -1;
};

// 参数名不需要和接口中的参数名一致，只要类型匹配即可
let mySearch2: SearchFunc;
mySearch2 = function(src: string, sub: string): boolean {
  return src.includes(sub);
};

// ---------- 返回值类型推断 ----------
// 如果函数有 return 语句，TypeScript 可以自动推断返回值类型
// 但建议显式指定返回值类型，提高代码可读性

// 自动推断返回值类型为 number
function inferReturn(x: number, y: number) {
  return x + y;                                // TypeScript 推断返回 number
}

// 显式指定返回值类型（推荐）
function explicitReturn(x: number, y: number): number {
  return x + y;
}

// 没有返回值的函数，返回类型为 void
function logMessage(message: string): void {
  console.log(message);
  // 不需要 return 语句
  // return;  // 也可以写 return; 返回 undefined
}

// ============================================================
//  2. 可选参数、默认参数、剩余参数
// ============================================================

// ---------- 可选参数 ----------
// 在参数名后加上 ? 表示该参数是可选的
// 可选参数必须放在必需参数的后面

/**
 * 构建完整姓名
 * @param firstName 名（必需）
 * @param lastName 姓（可选）
 * @returns 完整姓名
 */
function buildName(firstName: string, lastName?: string): string {
  if (lastName) {
    return firstName + ' ' + lastName;
  } else {
    return firstName;
  }
}

const name1 = buildName('张三');               // 正确，可以省略可选参数
const name2 = buildName('张三', '李');          // 正确，提供可选参数

// ---------- 默认参数 ----------
// 为参数提供默认值，当调用时不传该参数或传 undefined 时使用默认值
// 默认参数可以放在必需参数的前面或后面
// 如果默认参数在必需参数前面，需要显式传入 undefined 才能使用默认值

/**
 * 计算折扣后的价格
 * @param price 原价
 * @param discount 折扣率，默认 0.9（即 9 折）
 * @returns 折扣后的价格
 */
function calculatePrice(price: number, discount: number = 0.9): number {
  return price * discount;
}

const price1 = calculatePrice(100);            // 使用默认折扣 0.9，结果为 90
const price2 = calculatePrice(100, 0.8);       // 指定折扣 0.8，结果为 80
const price3 = calculatePrice(100, undefined); // 传入 undefined 也会使用默认值

// 默认参数也可以有类型推断
function greet(name: string, greeting: string = '你好') {
  return `${greeting}，${name}！`;
}

// ---------- 剩余参数 ----------
// 使用 ... 语法定义剩余参数（Rest Parameters）
// 剩余参数是一个数组，包含所有传入的额外参数
// 剩余参数必须是最后一个参数

/**
 * 计算多个数字的和
 * @param numbers 要相加的数字（剩余参数）
 * @returns 所有数字的和
 */
function sumAll(...numbers: number[]): number {
  // numbers 是一个 number 类型的数组
  return numbers.reduce((acc, curr) => acc + curr, 0);
}

const total1 = sumAll(1, 2, 3);                // 6
const total2 = sumAll(1, 2, 3, 4, 5);          // 15
const total3 = sumAll();                       // 0（空数组）

// 混合使用普通参数和剩余参数
function greetAll(greeting: string, ...names: string[]): string {
  return greeting + '，' + names.join('、') + '！';
}

const greeting = greetAll('大家好', '张三', '李四', '王五');
// 结果："大家好，张三、李四、王五！"

// ============================================================
//  3. 函数重载（Function Overloads）
// ============================================================

// 函数重载允许同一个函数有多个不同的参数类型和返回值类型
// 通过为同一个函数提供多个函数类型定义来实现
// 注意：函数重载只有类型层面的，运行时还是一个函数

// ---------- 基本重载 ----------
// 假设我们有一个函数，可以接受数字或字符串，返回对应类型的值

// 重载签名（Overload Signatures）
// 定义函数可以接受的不同参数组合
function reverse(x: string): string;
function reverse(x: number): number;

// 实现签名（Implementation Signature）
// 函数的实际实现，必须兼容所有重载签名
// 实现签名不能直接被调用，它只是提供函数的实现
function reverse(x: string | number): string | number {
  if (typeof x === 'string') {
    // 如果是字符串，反转字符串
    return x.split('').reverse().join('');
  } else {
    // 如果是数字，反转数字
    return Number(x.toString().split('').reverse().join(''));
  }
}

const reversedStr = reverse('hello');          // 返回 'olleh'，类型为 string
const reversedNum = reverse(12345);            // 返回 54321，类型为 number

// ---------- 重载的好处 ----------
// 1. 提供更精确的类型推断
// 2. 编辑器可以提供更好的代码提示
// 3. 调用者可以清楚地知道不同参数组合对应的返回类型

// ---------- 更复杂的重载示例 ----------
// 创建不同类型的对象

type Car = {
  type: 'car';
  brand: string;
  wheels: number;
};

type Bike = {
  type: 'bike';
  brand: string;
  hasBasket: boolean;
};

// 重载签名
function createVehicle(type: 'car', brand: string, wheels: number): Car;
function createVehicle(type: 'bike', brand: string, hasBasket: boolean): Bike;

// 实现签名
function createVehicle(
  type: 'car' | 'bike',
  brand: string,
  extra: number | boolean
): Car | Bike {
  if (type === 'car') {
    return {
      type: 'car',
      brand,
      wheels: extra as number
    };
  } else {
    return {
      type: 'bike',
      brand,
      hasBasket: extra as boolean
    };
  }
}

const myCar = createVehicle('car', 'Toyota', 4);    // 类型为 Car
const myBike = createVehicle('bike', 'Giant', true); // 类型为 Bike

// ---------- 重载注意事项 ----------
// 1. 重载签名必须在实现签名之前
// 2. 实现签名必须兼容所有重载签名
// 3. 调用函数时，TypeScript 会按从上到下的顺序匹配重载签名
// 4. 优先把更精确的重载放在前面

// ============================================================
//  4. 箭头函数与类型推断
// ============================================================

// ---------- 箭头函数的类型定义 ----------
// 箭头函数是 ES6 引入的函数简写形式
// TypeScript 支持为箭头函数添加类型注解

// 基本箭头函数
const arrowAdd = (a: number, b: number): number => {
  return a + b;
};

// 简写形式（函数体只有一个 return 语句时）
const arrowMultiply = (a: number, b: number): number => a * b;

// ---------- 类型推断 ----------
// TypeScript 有强大的类型推断能力
// 在很多情况下可以省略类型注解

// 变量类型推断
let inferredStr = 'hello';                       // 推断为 string 类型
let inferredNum = 42;                            // 推断为 number 类型

// 函数返回值推断
const inferredAdd = (a: number, b: number) => {
  return a + b;                                  // 返回值类型推断为 number
};

// 回调函数中的类型推断
const numbers = [1, 2, 3];
numbers.map((num) => {                           // num 的类型被推断为 number
  return num * 2;
});

// ---------- 上下文类型（Contextual Typing）----------
// 当函数出现在有明确类型的位置时，TypeScript 可以根据上下文推断参数类型

// 定义一个函数类型
type StringTransformer = (input: string) => string;

// 赋值时，参数类型可以省略，会从上下文推断
const toUpper: StringTransformer = (input) => {
  // input 被推断为 string 类型
  return input.toUpperCase();
};

// 再比如，在 setTimeout 中
setTimeout(() => {
  // 这个箭头函数的类型由 setTimeout 的参数类型推断
  console.log('延时执行');
}, 1000);

// ---------- 最佳通用类型（Best Common Type）----------
// 当需要从多个表达式推断类型时，TypeScript 会找出所有类型的最佳通用类型

const mixed = [1, 2, null];                      // 推断为 (number | null)[]
const objects = [{ a: 1 }, { b: 2 }];            // 推断为 ({ a: number; b?: undefined } | { b: number; a?: undefined })[]

// ============================================================
//  5. this 参数类型
// ============================================================

// 在 JavaScript 中，this 的值取决于函数的调用方式
// 在 TypeScript 中，可以显式指定 this 的类型
// this 参数是函数的第一个参数，但它只是类型注解，不会出现在编译后的代码中

// ---------- 基本用法 ----------
// 定义一个接口表示 this 的类型
interface PersonThis {
  name: string;
  age: number;
}

// 函数的第一个参数是 this，用于指定 this 的类型
// 这个参数不会被当作真正的参数传递
function introduce(this: PersonThis): void {
  console.log(`我叫${this.name}，今年${this.age}岁`);
}

const personThis: PersonThis = {
  name: '张三',
  age: 25
};

// 使用 call 或 apply 调用，确保 this 的类型正确
introduce.call(personThis);                      // 正确

// 直接调用会报错，因为 this 类型不匹配
// introduce();                                  // 错误，this 类型为 undefined

// ---------- 在对象方法中使用 this ----------
interface Counter {
  count: number;
  increment(): void;
  reset(): void;
}

const counter: Counter = {
  count: 0,
  increment(this: Counter): void {
    this.count++;
  },
  reset(this: Counter): void {
    this.count = 0;
  }
};

counter.increment();                             // this 指向 counter 对象
console.log(counter.count);                      // 输出：1

// ---------- this 参数与回调函数 ----------
// 当函数作为回调函数时，this 的指向可能会发生变化
// 使用 this 参数可以避免 this 类型错误

class Button {
  label: string;

  constructor(label: string) {
    this.label = label;
  }

  // 错误的写法：直接作为回调函数时 this 会丢失
  onClickWrong(): void {
    console.log(this.label);                    // this 可能不是 Button 实例
  }

  // 正确的写法之一：使用箭头函数（箭头函数没有自己的 this）
  onClickArrow = (): void => {
    console.log(this.label);                    // this 始终指向 Button 实例
  };

  // 正确的写法之二：使用 bind 绑定 this
  onClickBound(): void {
    console.log(this.label);
  }
}

// ============================================================
//  6. 高阶函数类型
// ============================================================

// 高阶函数（Higher-Order Functions）是指：
// 1. 接受一个或多个函数作为参数的函数
// 2. 返回一个函数的函数
// 3. 既接受函数参数又返回函数的函数

// ---------- 接受函数作为参数 ----------

/**
 * 对数组中的每个元素执行操作
 * @param arr 输入数组
 * @param fn 对每个元素执行的函数
 * @returns 处理后的新数组
 */
function mapArray<T>(arr: T[], fn: (item: T, index: number) => T): T[] {
  // fn 是一个函数类型的参数
  // 接受 item（当前元素）和 index（当前索引），返回 T 类型
  return arr.map((item, index) => fn(item, index));
}

const doubled = mapArray([1, 2, 3], (x) => x * 2);
// 结果：[2, 4, 6]

// ---------- 返回函数 ----------

/**
 * 创建一个加法器函数
 * @param x 第一个加数
 * @returns 一个接受第二个加数并返回和的函数
 */
function createAdder(x: number): (y: number) => number {
  // 返回一个函数，这个函数"记住"了 x 的值（闭包）
  return function(y: number): number {
    return x + y;
  };
}

const add5 = createAdder(5);                     // 创建一个加 5 的函数
console.log(add5(3));                            // 输出：8
console.log(add5(10));                           // 输出：15

// 使用箭头函数简写
const createMultiplier = (x: number): (y: number) => number => (y: number) => x * y;

const multiplyBy3 = createMultiplier(3);
console.log(multiplyBy3(5));                     // 输出：15

// ---------- 函数柯里化（Currying）----------
// 柯里化是将接受多个参数的函数转换为接受单一参数的函数序列的技术

// 普通函数：接受三个参数
function normalAdd(a: number, b: number, c: number): number {
  return a + b + c;
}

// 柯里化函数：每次接受一个参数，返回接受下一个参数的函数
function curriedAdd(a: number): (b: number) => (c: number) => number {
  return function(b: number): (c: number) => number {
    return function(c: number): number {
      return a + b + c;
    };
  };
}

// 柯里化函数的调用方式
console.log(curriedAdd(1)(2)(3));                // 输出：6

// 也可以分步调用
const add1 = curriedAdd(1);
const add1And2 = add1(2);
console.log(add1And2(3));                        // 输出：6

// ---------- 组合函数 ----------
// 函数组合是将多个函数组合成一个新函数的技术

/**
 * 组合两个函数
 * @param f 第二个执行的函数
 * @param g 第一个执行的函数
 * @returns 组合后的函数
 */
function compose<A, B, C>(
  f: (b: B) => C,
  g: (a: A) => B
): (a: A) => C {
  // 先执行 g，再执行 f
  return function(x: A): C {
    return f(g(x));
  };
}

// 示例：将字符串先转大写，再拼接感叹号
const toUpperCase = (s: string): string => s.toUpperCase();
const addExclamation = (s: string): string => s + '!';

// 组合两个函数
const shout = compose(addExclamation, toUpperCase);
console.log(shout('hello'));                     // 输出：HELLO!

// ---------- 更复杂的高阶函数示例 ----------
// 防抖函数（Debounce）：在事件触发后等待一段时间再执行
// 如果在等待时间内再次触发，则重新计时

/**
 * 创建防抖函数
 * @param fn 需要防抖的函数
 * @param delay 延迟时间（毫秒）
 * @returns 防抖后的函数
 */
function debounce<T extends (...args: any[]) => any>(
  fn: T,
  delay: number
): (...args: Parameters<T>) => void {
  // 定时器 ID
  let timerId: ReturnType<typeof setTimeout> | null = null;

  // 返回防抖后的函数
  return function(this: any, ...args: Parameters<T>): void {
    // 如果已有定时器，清除它
    if (timerId !== null) {
      clearTimeout(timerId);
    }

    // 设置新的定时器
    timerId = setTimeout(() => {
      fn.apply(this, args);
      timerId = null;
    }, delay);
  };
}

// 使用示例
const debouncedLog = debounce((message: string) => {
  console.log('防抖执行：', message);
}, 1000);

// ============================================================
//  演示函数：运行所有函数类型示例
// ============================================================

export function runFunctionsDemo(): void {
  console.log('========== 函数类型演示 ==========');

  // 基本函数
  console.log('1. 基本函数类型:');
  console.log('   add(1, 2) =', add(1, 2));
  console.log('   multiply(3, 4) =', multiply(3, 4));

  // 可选参数和默认参数
  console.log('\n2. 可选参数与默认参数:');
  console.log('   buildName("张三") =', buildName('张三'));
  console.log('   buildName("张三", "李") =', buildName('张三', '李'));
  console.log('   calculatePrice(100) =', calculatePrice(100));
  console.log('   calculatePrice(100, 0.8) =', calculatePrice(100, 0.8));

  // 剩余参数
  console.log('\n3. 剩余参数:');
  console.log('   sumAll(1, 2, 3) =', sumAll(1, 2, 3));
  console.log('   sumAll(1, 2, 3, 4, 5) =', sumAll(1, 2, 3, 4, 5));

  // 函数重载
  console.log('\n4. 函数重载:');
  console.log('   reverse("hello") =', reverse('hello'));
  console.log('   reverse(12345) =', reverse(12345));

  // 箭头函数
  console.log('\n5. 箭头函数:');
  console.log('   arrowAdd(2, 3) =', arrowAdd(2, 3));
  console.log('   arrowMultiply(2, 3) =', arrowMultiply(2, 3));

  // this 参数
  console.log('\n6. this 参数类型:');
  introduce.call(personThis);

  // 高阶函数
  console.log('\n7. 高阶函数:');
  console.log('   mapArray([1,2,3], x => x*2) =', mapArray([1, 2, 3], (x) => x * 2));
  console.log('   createAdder(5)(3) =', createAdder(5)(3));
  console.log('   curriedAdd(1)(2)(3) =', curriedAdd(1)(2)(3));
  console.log('   shout("hello") =', shout('hello'));

  console.log('\n========== 函数类型演示结束 ==========\n');
}
