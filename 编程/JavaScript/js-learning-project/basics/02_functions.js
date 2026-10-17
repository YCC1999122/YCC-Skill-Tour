/**
 * ============================================================
 * 文件名：02_functions.js
 * 描述：JavaScript 函数详解 - 声明方式、参数、闭包、高阶函数、柯里化、this指向
 * 作者：JavaScript 学习项目
 * 版本：1.0.0
 * ============================================================
 *
 * 本文件系统讲解 JavaScript 函数的核心知识，包括：
 * 1. 函数的三种定义方式（声明、表达式、箭头函数）
 * 2. 函数参数（默认参数、rest 参数、解构参数）
 * 3. 闭包原理与实际应用
 * 4. 高阶函数（函数作为参数、函数作为返回值）
 * 5. 柯里化与偏函数
 * 6. this 指向问题（普通函数、箭头函数、call/apply/bind）
 *
 * 运行方式：node basics/02_functions.js
 */

// 启用严格模式
'use strict';

// 导出演示函数
module.exports = function demoFunctions() {
  console.log('='.repeat(60));
  console.log('一、函数的定义方式');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 1.1 函数声明（Function Declaration）
  // ------------------------------------------------------------
  // 使用 function 关键字声明函数
  // 特点：存在函数提升（hoisting），可以在声明之前调用
  console.log('\n--- 1.1 函数声明 ---');

  // 函数声明会被提升到作用域顶部，所以可以在声明前调用
  console.log('函数提升测试：', add(3, 5)); // 8（函数声明提升，调用成功）

  /**
   * 计算两个数的和
   * @param {number} a - 第一个加数
   * @param {number} b - 第二个加数
   * @returns {number} 两数之和
   */
  function add(a, b) {
    return a + b; // return 语句返回结果并结束函数执行
  }

  console.log('add(2, 3) =', add(2, 3));

  // 没有 return 语句的函数默认返回 undefined
  function noReturn() {
    console.log('  这个函数没有 return 语句');
  }
  console.log('无 return 函数的返回值：', noReturn());

  // ------------------------------------------------------------
  // 1.2 函数表达式（Function Expression）
  // ------------------------------------------------------------
  // 将一个匿名函数赋值给一个变量
  // 特点：不会被提升，必须在赋值后才能调用
  console.log('\n--- 1.2 函数表达式 ---');

  // console.log(subtract(10, 5)); // 报错：Cannot access 'subtract' before initialization
  const subtract = function (a, b) {
    return a - b;
  };
  console.log('subtract(10, 5) =', subtract(10, 5));

  // 命名函数表达式（函数有自己的名字，只能在函数内部使用，用于递归等场景）
  const factorial = function fact(n) {
    if (n <= 1) return 1;
    return n * fact(n - 1); // 内部可以通过函数名 fact 递归调用
  };
  console.log('factorial(5) =', factorial(5)); // 120
  // console.log(fact(5)); // 报错：fact is not defined（外部无法访问函数名）

  // 函数声明 vs 函数表达式的主要区别：
  // 1. 函数声明会提升，函数表达式不会
  // 2. 函数声明在严格模式下是块级作用域，函数表达式遵循变量的作用域规则

  // ------------------------------------------------------------
  // 1.3 箭头函数（Arrow Function）
  // ------------------------------------------------------------
  // ES6 引入的新的函数定义方式，使用 => 语法
  // 特点：
  // - 没有自己的 this，继承外层作用域的 this
  // - 没有 arguments 对象
  // - 不能作为构造函数（不能用 new 调用）
  // - 没有 prototype 属性
  // - 不能用作 Generator 函数
  console.log('\n--- 1.3 箭头函数 ---');

  // 基本语法：(参数) => { 函数体 }
  const multiply = (a, b) => {
    return a * b;
  };
  console.log('multiply(3, 4) =', multiply(3, 4));

  // 简写形式1：只有一个参数时，可以省略括号
  const square = x => {
    return x * x;
  };
  console.log('square(5) =', square(5));

  // 简写形式2：函数体只有一条 return 语句时，可以省略花括号和 return
  const divide = (a, b) => a / b;
  console.log('divide(10, 2) =', divide(10, 2));

  // 简写形式3：返回对象时需要用圆括号包裹，否则会被当作代码块
  const createUser = (name, age) => ({ name, age });
  console.log('createUser:', createUser('小明', 20));

  // 箭头函数的 this 指向外层作用域，后面会详细讲解

  // ============================================================
  // 第二部分：函数参数
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('二、函数参数');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 2.1 参数数量可以不匹配
  // ------------------------------------------------------------
  console.log('\n--- 2.1 参数数量不匹配 ---');

  function showParams(a, b, c) {
    console.log(`  a = ${a}, b = ${b}, c = ${c}`);
  }

  // 传入参数多于形参：多余的参数会被忽略
  console.log('传入 5 个参数：');
  showParams(1, 2, 3, 4, 5); // a=1, b=2, c=3

  // 传入参数少于形参：未传入的参数为 undefined
  console.log('传入 1 个参数：');
  showParams(10); // a=10, b=undefined, c=undefined

  // 不传参数
  console.log('不传参数：');
  showParams(); // a=undefined, b=undefined, c=undefined

  // ------------------------------------------------------------
  // 2.2 默认参数（Default Parameters）
  // ------------------------------------------------------------
  // ES6 引入的语法，在形参中直接设置默认值
  // 当参数为 undefined 时，使用默认值
  console.log('\n--- 2.2 默认参数 ---');

  // 基本用法
  function greet(name, greeting = '你好') {
    console.log(`  ${greeting}，${name}！`);
  }
  greet('小红');               // '你好，小红！'
  greet('小李', '早上好');     // '早上好，小李！'
  greet('小王', undefined);    // '你好，小王！'（undefined 会触发默认值）
  greet('小张', null);         // 'null，小张！'（null 不会触发默认值）

  // 默认参数可以是表达式，每次调用时重新计算
  function getDefaultAge() {
    console.log('  调用了 getDefaultAge 函数');
    return 18;
  }
  function createPerson(name, age = getDefaultAge()) {
    console.log(`  姓名：${name}，年龄：${age}`);
  }
  console.log('传入年龄时：');
  createPerson('张三', 25);  // 不会调用 getDefaultAge
  console.log('不传入年龄时：');
  createPerson('李四');      // 会调用 getDefaultAge

  // 默认参数可以引用前面的参数
  function createArray(length, value = length) {
    return new Array(length).fill(value);
  }
  console.log('createArray(3):', createArray(3));   // [3, 3, 3]
  console.log('createArray(3, 0):', createArray(3, 0)); // [0, 0, 0]

  // ------------------------------------------------------------
  // 2.3 剩余参数（Rest Parameters）
  // ------------------------------------------------------------
  // 使用 ... 将剩余的参数收集为一个数组
  // 必须放在参数列表的最后
  console.log('\n--- 2.3 剩余参数 ---');

  // 求和函数：接受任意数量的参数
  function sum(...nums) {
    // nums 是一个真正的数组，可以使用所有数组方法
    return nums.reduce((total, num) => total + num, 0);
  }
  console.log('sum(1, 2, 3) =', sum(1, 2, 3));
  console.log('sum(1, 2, 3, 4, 5, 6) =', sum(1, 2, 3, 4, 5, 6));

  // 前几个参数单独命名，剩余的收集为数组
  function formatDate(separator, ...dateParts) {
    return dateParts.join(separator);
  }
  console.log('formatDate:', formatDate('-', 2024, 1, 15)); // '2024-1-15'

  // ------------------------------------------------------------
  // 2.4 arguments 对象
  // ------------------------------------------------------------
  // arguments 是函数内部的类数组对象，包含所有传入的参数
  // 箭头函数没有 arguments 对象
  console.log('\n--- 2.4 arguments 对象 ---');

  function printArgs() {
    console.log('  arguments 长度：', arguments.length);
    console.log('  第一个参数：', arguments[0]);
    // arguments 是类数组，不能直接使用数组方法，需要转换
    const argsArray = Array.from(arguments); // 或者 [...arguments]
    console.log('  转为数组后：', argsArray);
  }
  printArgs('a', 'b', 'c');

  // 推荐使用剩余参数代替 arguments，因为剩余参数是真正的数组

  // ------------------------------------------------------------
  // 2.5 参数解构
  // ------------------------------------------------------------
  // 可以直接在参数中使用解构赋值
  console.log('\n--- 2.5 参数解构 ---');

  // 对象参数解构
  function printUserInfo({ name, age, city = '未知' }) {
    console.log(`  姓名：${name}，年龄：${age}，城市：${city}`);
  }
  const user = { name: '王五', age: 28 };
  printUserInfo(user);

  // 数组参数解构
  function sumFirstTwo([a, b]) {
    return a + b;
  }
  console.log('sumFirstTwo([10, 20, 30]):', sumFirstTwo([10, 20, 30])); // 30

  // ============================================================
  // 第三部分：闭包（Closure）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('三、闭包（Closure）');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 3.1 闭包的概念
  // ------------------------------------------------------------
  // 闭包是指有权访问另一个函数作用域中变量的函数
  // 简单说：当内部函数引用了外部函数的变量时，就形成了闭包
  // 闭包使得函数可以"记住"它被创建时的环境
  console.log('\n--- 3.1 闭包的基本概念 ---');

  /**
   * 创建一个计数器函数
   * 这是闭包的经典应用：通过闭包实现私有变量
   * @returns {Function} 计数器函数
   */
  function createCounter() {
    // count 是局部变量，外部无法直接访问
    let count = 0;

    // 返回的内部函数引用了外部的 count 变量，形成闭包
    return function () {
      count++; // 访问并修改外部函数的变量
      return count;
    };
  }

  const counter1 = createCounter();
  console.log('counter1 第一次调用：', counter1()); // 1
  console.log('counter1 第二次调用：', counter1()); // 2
  console.log('counter1 第三次调用：', counter1()); // 3

  // 每个闭包都有独立的变量环境
  const counter2 = createCounter();
  console.log('counter2 第一次调用：', counter2()); // 1（独立的 count）
  console.log('counter1 继续调用：', counter1());   // 4

  // ------------------------------------------------------------
  // 3.2 闭包的应用场景
  // ------------------------------------------------------------
  console.log('\n--- 3.2 闭包的应用场景 ---');

  // 应用1：实现私有变量和方法（封装）
  function createBankAccount(initialBalance) {
    // balance 是私有变量，外部无法直接访问，只能通过提供的方法操作
    let balance = initialBalance;

    return {
      // 存款
      deposit(amount) {
        if (amount > 0) {
          balance += amount;
          console.log(`  存款 ${amount} 元，当前余额：${balance} 元`);
        }
      },
      // 取款
      withdraw(amount) {
        if (amount > 0 && amount <= balance) {
          balance -= amount;
          console.log(`  取款 ${amount} 元，当前余额：${balance} 元`);
        } else {
          console.log('  余额不足或金额无效');
        }
      },
      // 查询余额（只读访问）
      getBalance() {
        return balance;
      }
    };
  }

  const account = createBankAccount(1000);
  account.deposit(500);
  account.withdraw(200);
  console.log('  当前余额：', account.getBalance());
  // console.log(account.balance); // undefined（无法直接访问私有变量）

  // 应用2：函数柯里化（后面详细讲解）
  // 应用3：循环中创建闭包（经典面试题）
  console.log('\n  循环闭包经典问题：');

  // 使用 var 时的问题（var 没有块级作用域，所有函数共享同一个 i）
  function createFunctionsWithVar() {
    var funcs = [];
    for (var i = 0; i < 3; i++) {
      funcs.push(function () {
        console.log(`    var 循环：i = ${i}`);
      });
    }
    return funcs;
  }
  const varFuncs = createFunctionsWithVar();
  varFuncs.forEach(fn => fn()); // 全部输出 3

  // 使用闭包解决（立即执行函数 IIFE）
  function createFunctionsWithClosure() {
    var funcs = [];
    for (var i = 0; i < 3; i++) {
      // 立即执行函数创建独立的作用域，保存当前的 i 值
      (function (j) {
        funcs.push(function () {
          console.log(`    闭包解决：j = ${j}`);
        });
      })(i);
    }
    return funcs;
  }
  const closureFuncs = createFunctionsWithClosure();
  closureFuncs.forEach(fn => fn()); // 输出 0, 1, 2

  // 使用 let 解决（let 有块级作用域，每次循环创建新的绑定）
  function createFunctionsWithLet() {
    var funcs = [];
    for (let i = 0; i < 3; i++) {
      funcs.push(function () {
        console.log(`    let 循环：i = ${i}`);
      });
    }
    return funcs;
  }
  const letFuncs = createFunctionsWithLet();
  letFuncs.forEach(fn => fn()); // 输出 0, 1, 2

  // ------------------------------------------------------------
  // 3.3 闭包的注意事项
  // ------------------------------------------------------------
  // 闭包会导致变量不会被垃圾回收，使用不当可能造成内存泄漏
  // 合理使用闭包，在不需要时及时释放引用
  console.log('\n--- 3.3 闭包注意事项 ---');
  console.log('  闭包的优点：');
  console.log('    1. 可以读取函数内部的变量');
  console.log('    2. 让变量的值始终保持在内存中');
  console.log('    3. 可以封装私有变量和方法');
  console.log('  闭包的缺点：');
  console.log('    1. 变量常驻内存，增加内存开销');
  console.log('    2. 使用不当可能导致内存泄漏');
  console.log('    3. 闭包可以在父函数外部改变内部变量值');

  // ============================================================
  // 第四部分：高阶函数（Higher-Order Function）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('四、高阶函数');
  console.log('='.repeat(60));

  // 高阶函数是指满足以下任一条件的函数：
  // 1. 接受一个或多个函数作为参数
  // 2. 返回一个函数作为结果
  // JavaScript 中函数是一等公民（first-class citizen），可以像值一样传递和返回

  // ------------------------------------------------------------
  // 4.1 函数作为参数
  // ------------------------------------------------------------
  console.log('\n--- 4.1 函数作为参数 ---');

  // 自定义一个数组遍历函数，接受回调函数处理每个元素
  function forEach(arr, callback) {
    for (let i = 0; i < arr.length; i++) {
      // 调用回调函数，传入当前元素、索引和原数组
      callback(arr[i], i, arr);
    }
  }

  const numbers = [1, 2, 3, 4, 5];
  console.log('  遍历数组：');
  forEach(numbers, function (item, index) {
    console.log(`    索引 ${index}：${item}`);
  });

  // 自定义数组过滤函数
  function filter(arr, callback) {
    const result = [];
    for (let i = 0; i < arr.length; i++) {
      // 回调函数返回 true 时，将元素加入结果数组
      if (callback(arr[i], i, arr)) {
        result.push(arr[i]);
      }
    }
    return result;
  }

  const evens = filter(numbers, n => n % 2 === 0);
  console.log('  过滤偶数：', evens);

  // ------------------------------------------------------------
  // 4.2 函数作为返回值
  // ------------------------------------------------------------
  console.log('\n--- 4.2 函数作为返回值 ---');

  // 函数工厂：根据参数创建不同的函数
  function createMultiplier(multiplier) {
    // 返回一个新函数，这个函数记住了 multiplier 的值（闭包）
    return function (num) {
      return num * multiplier;
    };
  }

  const double = createMultiplier(2);  // 创建翻倍函数
  const triple = createMultiplier(3);  // 创建三倍函数

  console.log('  double(5) =', double(5));   // 10
  console.log('  triple(5) =', triple(5));   // 15

  // ------------------------------------------------------------
  // 4.3 常见高阶函数
  // ------------------------------------------------------------
  console.log('\n--- 4.3 常见高阶函数 ---');

  // 数组的 map/filter/reduce 都是高阶函数（在 advanced/array_methods.js 中详细讲解）
  const result = numbers
    .map(n => n * 2)           // 每个元素乘 2
    .filter(n => n > 5)        // 过滤出大于 5 的
    .reduce((sum, n) => sum + n, 0); // 求和
  console.log('  链式调用结果：', result); // (6 + 8 + 10) = 24

  // ============================================================
  // 第五部分：柯里化与偏函数
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('五、柯里化与偏函数');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 5.1 柯里化（Currying）
  // ------------------------------------------------------------
  // 柯里化是将一个多参数函数转换为一系列单参数函数的技术
  // 即 f(a, b, c) 转换为 f(a)(b)(c)
  console.log('\n--- 5.1 柯里化 ---');

  // 普通的三参数函数
  function addThree(a, b, c) {
    return a + b + c;
  }

  // 柯里化版本
  function curriedAdd(a) {
    return function (b) {
      return function (c) {
        return a + b + c;
      };
    };
  }

  console.log('  curriedAdd(1)(2)(3) =', curriedAdd(1)(2)(3)); // 6

  // 可以分步调用，保存中间结果
  const add1 = curriedAdd(1);     // 保存第一个参数
  const add1And2 = add1(2);        // 保存第二个参数
  console.log('  分步调用结果：', add1And2(3)); // 6

  // 通用柯里化函数：将任意多参数函数转为柯里化函数
  function curry(fn) {
    // 返回一个函数，负责收集参数
    return function curried(...args) {
      // 如果收集到的参数数量 >= 原函数所需参数数量
      if (args.length >= fn.length) {
        // 直接调用原函数
        return fn.apply(this, args);
      } else {
        // 否则返回一个新函数，继续收集参数
        return function (...moreArgs) {
          return curried.apply(this, args.concat(moreArgs));
        };
      }
    };
  }

  const curriedSum = curry(addThree);
  console.log('  通用柯里化：');
  console.log('    curriedSum(1, 2, 3) =', curriedSum(1, 2, 3)); // 6
  console.log('    curriedSum(1)(2)(3) =', curriedSum(1)(2)(3)); // 6
  console.log('    curriedSum(1, 2)(3) =', curriedSum(1, 2)(3)); // 6

  // ------------------------------------------------------------
  // 5.2 偏函数（Partial Application）
  // ------------------------------------------------------------
  // 偏函数是指固定一个函数的一部分参数，然后返回一个新函数
  // 与柯里化的区别：
  // - 柯里化是将多参数函数转为一系列单参数函数
  // - 偏函数是固定部分参数，产生一个参数更少的函数
  console.log('\n--- 5.2 偏函数 ---');

  // 使用 bind 实现偏函数（bind 的第一个参数是 this，后面的参数会被固定）
  function addAll(a, b, c, d) {
    return a + b + c + d;
  }

  // 固定前两个参数为 10 和 20
  const partialAdd = addAll.bind(null, 10, 20);
  console.log('  partialAdd(1, 2) =', partialAdd(1, 2)); // 10 + 20 + 1 + 2 = 33

  // 自定义偏函数
  function partial(fn, ...presetArgs) {
    return function (...laterArgs) {
      return fn(...presetArgs, ...laterArgs);
    };
  }

  const partialAdd2 = partial(addAll, 1, 2);
  console.log('  partialAdd2(3, 4) =', partialAdd2(3, 4)); // 1 + 2 + 3 + 4 = 10

  // ============================================================
  // 第六部分：this 指向问题
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('六、this 指向详解');
  console.log('='.repeat(60));

  // this 是 JavaScript 中最容易混淆的概念之一
  // this 的指向不是在函数定义时确定的，而是在函数调用时确定的
  // 记住：this 永远指向调用函数的那个对象（箭头函数除外）

  // ------------------------------------------------------------
  // 6.1 普通函数的 this
  // ------------------------------------------------------------
  console.log('\n--- 6.1 普通函数的 this 指向 ---');

  // 规则1：全局函数调用，this 指向全局对象
  // 浏览器中是 window，Node.js 中是 global（严格模式下是 undefined）
  function globalFunc() {
    console.log('  全局函数调用 this:', this);
  }
  console.log('  严格模式下全局函数调用 this:');
  globalFunc(); // undefined（严格模式下全局函数调用 this 为 undefined）

  // 规则2：对象方法调用，this 指向调用该方法的对象
  const person = {
    name: '张三',
    sayHello() {
      console.log(`  对象方法 this.name = ${this.name}`);
    }
  };
  person.sayHello(); // '张三'（this 指向 person 对象）

  // 规则3：构造函数调用，this 指向新创建的实例对象
  function Person(name) {
    this.name = name;
    console.log(`  构造函数 this.name = ${this.name}`);
  }
  const p1 = new Person('李四'); // this 指向新创建的 p1 对象

  // 规则4：call / apply / bind 调用，this 指向指定的对象
  function introduce(greeting) {
    console.log(`  ${greeting}，我是${this.name}`);
  }
  const personA = { name: '王五' };
  const personB = { name: '赵六' };

  // call：第一个参数是 this，后面依次传入参数
  introduce.call(personA, '你好');   // '你好，我是王五'
  // apply：第一个参数是 this，第二个参数是参数数组
  introduce.apply(personB, ['大家好']); // '大家好，我是赵六'
  // bind：返回一个新函数，this 永久绑定到指定对象
  const boundIntro = introduce.bind(personA);
  boundIntro('嗨');                  // '嗨，我是王五'
  // bind 绑定后不能再通过 call/apply 改变 this
  boundIntro.call(personB, '测试');  // '测试，我是王五'（仍然是 personA）

  // ------------------------------------------------------------
  // 6.2 箭头函数的 this
  // ------------------------------------------------------------
  // 箭头函数没有自己的 this，它的 this 继承自外层作用域的 this
  // 箭头函数的 this 在定义时就确定了，不会随调用方式改变
  console.log('\n--- 6.2 箭头函数的 this 指向 ---');

  const obj = {
    name: '对象',
    // 普通函数方法，this 指向调用它的对象
    normalMethod() {
      console.log('  普通方法 this.name:', this.name);

      // 普通函数作为回调，this 会丢失（严格模式下为 undefined）
      setTimeout(function () {
        console.log('  普通函数回调 this:', this); // undefined（严格模式）
      }, 0);

      // 箭头函数作为回调，this 继承自外层（即 normalMethod 的 this）
      setTimeout(() => {
        console.log('  箭头函数回调 this.name:', this.name); // '对象'
      }, 0);
    },
    // 箭头函数方法，this 继承自外层作用域（全局作用域）
    arrowMethod: () => {
      console.log('  箭头方法 this:', this); // 全局对象或 undefined（取决于环境）
    }
  };

  obj.normalMethod();
  obj.arrowMethod();

  // 注意：箭头函数不能作为构造函数，不能用 call/apply/bind 改变 this
  // 箭头函数也没有 arguments 对象

  // ------------------------------------------------------------
  // 6.3 this 优先级
  // ------------------------------------------------------------
  // new 绑定 > call/apply/bind 绑定 > 隐式绑定（对象方法） > 默认绑定（全局）
  console.log('\n--- 6.3 this 绑定优先级 ---');
  console.log('  优先级从高到低：');
  console.log('    1. new 绑定（构造函数调用）');
  console.log('    2. 显式绑定（call / apply / bind）');
  console.log('    3. 隐式绑定（作为对象方法调用）');
  console.log('    4. 默认绑定（全局调用，严格模式下为 undefined）');

  // ============================================================
  // 演示结束
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('基础语法 - 函数 演示结束');
  console.log('='.repeat(60));
};

// 直接运行此文件时执行演示
if (require.main === module) {
  module.exports();
}
