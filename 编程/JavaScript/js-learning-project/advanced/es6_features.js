/**
 * ============================================================
 * 文件名：es6_features.js
 * 描述：JavaScript ES6+ 高级特性 - Promise / Symbol / Iterator / Generator / Proxy / Reflect / Map / Set / 模块化
 * 作者：JavaScript 学习项目
 * 版本：1.0.0
 * ============================================================
 *
 * 本文件系统讲解 JavaScript ES6+ 的高级特性，包括：
 * 1. Promise 基础（resolve / reject / then / catch / finally）
 * 2. Symbol 深入用法
 * 3. Iterator（迭代器）与 Generator（生成器）
 * 4. Proxy 与 Reflect
 * 5. Map / Set / WeakMap / WeakSet
 * 6. 模块化（import / export）
 *
 * 运行方式：node advanced/es6_features.js
 */

'use strict';

module.exports = function demoES6Features() {
  console.log('='.repeat(60));
  console.log('一、Promise 基础');
  console.log('='.repeat(60));

  // Promise 是 ES6 引入的异步编程解决方案
  // 三种状态：pending（进行中）、fulfilled（已成功）、rejected（已失败）
  // 状态只能从 pending 变为 fulfilled 或 rejected，且不可逆

  // ------------------------------------------------------------
  // 1.1 Promise 基本用法
  // ------------------------------------------------------------
  console.log('\n--- 1.1 Promise 基本用法 ---');

  // 创建 Promise 实例
  const promise = new Promise((resolve, reject) => {
    // 异步操作代码写在这里
    // 操作成功调用 resolve，失败调用 reject
    console.log('  Promise 立即执行');
    setTimeout(() => {
      const success = true;
      if (success) {
        resolve('操作成功的数据'); // pending -> fulfilled
      } else {
        reject(new Error('操作失败')); // pending -> rejected
      }
    }, 100);
  });

  console.log('  Promise 创建后立即执行，这里是同步代码');

  // then 方法接收两个回调：成功回调和失败回调
  promise.then(
    result => {
      console.log('  then 成功回调:', result);
    },
    error => {
      console.log('  then 失败回调:', error.message);
    }
  );

  // ------------------------------------------------------------
  // 1.2 Promise 链式调用
  // ------------------------------------------------------------
  // then 方法返回一个新的 Promise，所以可以链式调用
  console.log('\n--- 1.2 Promise 链式调用 ---');

  function asyncDouble(num) {
    return new Promise(resolve => {
      setTimeout(() => {
        resolve(num * 2);
      }, 10);
    });
  }

  asyncDouble(1)
    .then(result => {
      console.log('  第一步结果:', result); // 2
      return asyncDouble(result); // 返回 Promise，继续链式调用
    })
    .then(result => {
      console.log('  第二步结果:', result); // 4
      return result + 10; // 返回普通值，会被包装成 resolved 的 Promise
    })
    .then(result => {
      console.log('  第三步结果:', result); // 14
    });

  // ------------------------------------------------------------
  // 1.3 catch 与 finally
  // ------------------------------------------------------------
  // catch 用于捕获 Promise 链中的错误
  // finally 无论成功失败都会执行
  console.log('\n--- 1.3 catch 与 finally ---');

  function riskyOperation() {
    return new Promise((resolve, reject) => {
      setTimeout(() => {
        reject(new Error('出错了！'));
      }, 10);
    });
  }

  riskyOperation()
    .then(result => {
      console.log('  成功:', result);
    })
    .catch(error => {
      console.log('  catch 捕获错误:', error.message); // '出错了！'
    })
    .finally(() => {
      console.log('  finally 总是执行');
    });

  // Promise 链上的错误会"冒泡"到最近的 catch
  Promise.resolve(1)
    .then(result => {
      console.log('  链式调用第一步');
      throw new Error('第二步出错'); // throw 会触发 catch
    })
    .then(result => {
      console.log('  这一步不会执行');
    })
    .catch(error => {
      console.log('  错误冒泡:', error.message);
    });

  // ------------------------------------------------------------
  // 1.4 Promise 静态方法
  // ------------------------------------------------------------
  console.log('\n--- 1.4 Promise 静态方法 ---');

  // Promise.resolve()：返回一个成功的 Promise
  const resolvedPromise = Promise.resolve('直接成功');
  resolvedPromise.then(res => console.log('  Promise.resolve:', res));

  // Promise.reject()：返回一个失败的 Promise
  const rejectedPromise = Promise.reject(new Error('直接失败'));
  rejectedPromise.catch(err => console.log('  Promise.reject:', err.message));

  // Promise.all()：所有 Promise 都成功才成功，有一个失败就失败
  const p1 = Promise.resolve('第一个');
  const p2 = new Promise(resolve => setTimeout(() => resolve('第二个'), 20));
  const p3 = Promise.resolve('第三个');

  Promise.all([p1, p2, p3]).then(results => {
    console.log('  Promise.all 结果:', results); // ['第一个', '第二个', '第三个']
  });

  // Promise.race()：第一个完成的 Promise 决定结果（竞速）
  const fast = new Promise(resolve => setTimeout(() => resolve('快的'), 10));
  const slow = new Promise(resolve => setTimeout(() => resolve('慢的'), 100));
  Promise.race([fast, slow]).then(result => {
    console.log('  Promise.race 结果:', result); // '快的'
  });

  // ============================================================
  // 第二部分：Symbol 深入
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('二、Symbol 深入用法');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 2.1 Symbol 的基本特性回顾
  // ------------------------------------------------------------
  console.log('\n--- 2.1 Symbol 基本特性 ---');

  const sym1 = Symbol('description');
  const sym2 = Symbol('description');
  console.log('  Symbol() 每次创建都是唯一的:', sym1 !== sym2); // true

  // Symbol.for() 在全局注册表中查找，不存在则创建
  const globalSym1 = Symbol.for('globalKey');
  const globalSym2 = Symbol.for('globalKey');
  console.log('  Symbol.for() 复用:', globalSym1 === globalSym2); // true
  console.log('  Symbol.keyFor():', Symbol.keyFor(globalSym1)); // 'globalKey'

  // ------------------------------------------------------------
  // 2.2 内置 Symbol 值（Well-known Symbols）
  // ------------------------------------------------------------
  // JavaScript 内置了一些 Symbol 值，用于控制对象的行为
  console.log('\n--- 2.2 内置 Symbol 值 ---');

  // Symbol.iterator：使对象可迭代（后面迭代器详细讲解）
  const iterableObj = {
    data: [10, 20, 30],
    [Symbol.iterator]() {
      let index = 0;
      const data = this.data;
      return {
        next() {
          return {
            value: data[index],
            done: index++ >= data.length
          };
        }
      };
    }
  };
  console.log('  自定义可迭代对象:');
  for (const val of iterableObj) {
    console.log('    值:', val);
  }

  // Symbol.toStringTag：自定义 Object.prototype.toString 的返回
  const customObj = {
    [Symbol.toStringTag]: 'MyCustomObject'
  };
  console.log('  自定义 toStringTag:', Object.prototype.toString.call(customObj));
  // [object MyCustomObject]

  // Symbol.hasInstance：自定义 instanceof 行为
  class MyClass {
    static [Symbol.hasInstance](instance) {
      return instance.isMyClass === true;
    }
  }
  const obj = { isMyClass: true };
  console.log('  自定义 instanceof:', obj instanceof MyClass); // true

  // Symbol.toPrimitive：自定义对象转为原始值的行为
  const myNumber = {
    value: 42,
    [Symbol.toPrimitive](hint) {
      console.log(`    toPrimitive hint: ${hint}`);
      if (hint === 'number') return this.value;
      if (hint === 'string') return `值为${this.value}`;
      return this.value;
    }
  };
  console.log('  +myNumber:', +myNumber);         // 42 (number)
  console.log('  String(myNumber):', String(myNumber)); // '值为42' (string)

  // ============================================================
  // 第三部分：Iterator（迭代器）与 Generator（生成器）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('三、迭代器与生成器');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 3.1 迭代器（Iterator）
  // ------------------------------------------------------------
  // 迭代器是一个具有 next() 方法的对象，每次调用 next() 返回 { value, done }
  // 可迭代对象是具有 Symbol.iterator 方法的对象，该方法返回一个迭代器
  // 原生可迭代对象：Array, String, Set, Map, arguments, NodeList 等
  console.log('\n--- 3.1 迭代器 ---');

  // 数组的迭代器
  const arr = [1, 2, 3];
  const iterator = arr[Symbol.iterator](); // 获取迭代器
  console.log('  迭代器 next():', iterator.next()); // { value: 1, done: false }
  console.log('  迭代器 next():', iterator.next()); // { value: 2, done: false }
  console.log('  迭代器 next():', iterator.next()); // { value: 3, done: false }
  console.log('  迭代器 next():', iterator.next()); // { value: undefined, done: true }

  // 手写一个可迭代对象：范围迭代器
  const range = {
    start: 1,
    end: 5,
    [Symbol.iterator]() {
      // 返回迭代器对象
      let current = this.start;
      const end = this.end;
      return {
        next() {
          if (current <= end) {
            return { value: current++, done: false };
          } else {
            return { value: undefined, done: true };
          }
        }
      };
    }
  };

  console.log('  自定义范围迭代器:');
  for (const num of range) {
    console.log('    ', num);
  }

  // for...of 循环就是调用迭代器的语法糖
  // 展开运算符也使用迭代器
  console.log('  展开迭代器:', [...range]); // [1, 2, 3, 4, 5]

  // ------------------------------------------------------------
  // 3.2 生成器（Generator）
  // ------------------------------------------------------------
  // 生成器是一种特殊的函数，可以暂停和恢复执行
  // 使用 function* 定义，内部使用 yield 产出值
  // 调用生成器函数返回一个 Generator 对象，它是迭代器，也是可迭代对象
  console.log('\n--- 3.2 生成器 ---');

  // 基本生成器函数
  function* generatorFunc() {
    console.log('  开始执行');
    yield 1;  // 暂停，产出值 1
    console.log('  继续执行');
    yield 2;  // 暂停，产出值 2
    console.log('  再继续执行');
    yield 3;  // 暂停，产出值 3
    console.log('  执行结束');
    return 4; // 结束，返回值 4
  }

  const gen = generatorFunc();
  console.log('  调用生成器函数，返回 Generator 对象');
  console.log('  第一次 next():', gen.next()); // { value: 1, done: false }
  console.log('  第二次 next():', gen.next()); // { value: 2, done: false }
  console.log('  第三次 next():', gen.next()); // { value: 3, done: false }
  console.log('  第四次 next():', gen.next()); // { value: 4, done: true }

  // 生成器也是可迭代的
  function* countGenerator(max) {
    for (let i = 1; i <= max; i++) {
      yield i;
    }
  }
  console.log('  生成器 for...of:');
  for (const num of countGenerator(3)) {
    console.log('    ', num);
  }

  // next() 可以传参，参数作为上一个 yield 表达式的返回值
  function* echoGenerator() {
    console.log('  生成器开始');
    const first = yield '第一个产出值';
    console.log('  收到第一个参数:', first);
    const second = yield '第二个产出值';
    console.log('  收到第二个参数:', second);
    return '结束';
  }

  const echoGen = echoGenerator();
  console.log('  next():', echoGen.next()); // 第一次调用 next 传参无效
  console.log('  next("参数A"):', echoGen.next('参数A'));
  console.log('  next("参数B"):', echoGen.next('参数B'));

  // yield*：在生成器中委托给另一个生成器
  function* innerGenerator() {
    yield '内部1';
    yield '内部2';
  }
  function* outerGenerator() {
    yield '外部1';
    yield* innerGenerator(); // 委托给内部生成器
    yield '外部2';
  }
  console.log('  yield* 委托:');
  for (const val of outerGenerator()) {
    console.log('    ', val);
  }

  // 生成器的 return() 和 throw() 方法
  function* genWithControl() {
    try {
      yield 1;
      yield 2;
      yield 3;
    } catch (e) {
      console.log('  生成器捕获异常:', e.message);
    }
  }
  const controlGen = genWithControl();
  console.log('  next():', controlGen.next());
  console.log('  throw():', controlGen.throw(new Error('测试异常')));
  // return() 可以提前结束生成器
  const controlGen2 = genWithControl();
  controlGen2.next();
  console.log('  return():', controlGen2.return('提前结束')); // { value: '提前结束', done: true }

  // ============================================================
  // 第四部分：Proxy 与 Reflect
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('四、Proxy 与 Reflect');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 4.1 Proxy（代理）
  // ------------------------------------------------------------
  // Proxy 用于创建一个对象的代理，可以拦截并自定义对象的基本操作
  // 语法：new Proxy(target, handler)
  // target：被代理的目标对象
  // handler：一个对象，定义了各种拦截操作（陷阱/trap）
  console.log('\n--- 4.1 Proxy 代理 ---');

  // 基本代理：拦截 get 操作
  const target = {
    name: '张三',
    age: 25
  };

  const proxy = new Proxy(target, {
    // get 陷阱：拦截属性读取
    get(target, property, receiver) {
      console.log(`  读取属性: ${property}`);
      // 属性不存在时返回默认值，而不是 undefined
      if (property in target) {
        return target[property];
      } else {
        return `属性 "${property}" 不存在`;
      }
    },
    // set 陷阱：拦截属性赋值
    set(target, property, value, receiver) {
      console.log(`  设置属性: ${property} = ${value}`);
      // 验证年龄
      if (property === 'age') {
        if (typeof value !== 'number' || value < 0 || value > 150) {
          throw new Error('年龄不合法');
        }
      }
      target[property] = value;
      return true; // set 必须返回 true 表示成功
    },
    // has 陷阱：拦截 in 运算符
    has(target, property) {
      console.log(`  检查属性存在: ${property}`);
      return property in target;
    },
    // deleteProperty 陷阱：拦截 delete 操作
    deleteProperty(target, property) {
      console.log(`  删除属性: ${property}`);
      if (property === 'name') {
        console.log('  name 属性不能删除');
        return false;
      }
      delete target[property];
      return true;
    }
  });

  console.log('  proxy.name:', proxy.name);
  console.log('  proxy.unknownProp:', proxy.unknownProp);
  proxy.age = 30;
  console.log('  proxy.age:', proxy.age);
  console.log('  "name" in proxy:', 'name' in proxy);

  // ------------------------------------------------------------
  // 4.2 Proxy 应用场景
  // ------------------------------------------------------------
  console.log('\n--- 4.2 Proxy 应用场景 ---');

  // 应用1：数据验证
  const validator = {
    set(target, prop, value) {
      if (prop === 'age') {
        if (!Number.isInteger(value)) {
          throw new TypeError('年龄必须是整数');
        }
        if (value > 200) {
          throw new RangeError('年龄不能超过 200');
        }
      }
      target[prop] = value;
      return true;
    }
  };

  const person = new Proxy({}, validator);
  person.age = 25;
  console.log('  验证通过的年龄:', person.age);

  // 应用2：观察者模式（响应式数据）
  function createObservable(obj, onChange) {
    return new Proxy(obj, {
      set(target, prop, value) {
        target[prop] = value;
        onChange(prop, value); // 数据变化时通知
        return true;
      },
      deleteProperty(target, prop) {
        delete target[prop];
        onChange(prop, undefined);
        return true;
      }
    });
  }

  const observable = createObservable({ count: 0 }, (prop, value) => {
    console.log(`  数据变化: ${prop} = ${value}`);
  });
  observable.count = 1;
  observable.count = 2;

  // 应用3：负索引数组（类似 Python）
  function createNegativeIndexArray(arr) {
    return new Proxy(arr, {
      get(target, prop) {
        let index = Number(prop);
        if (index < 0) {
          index = target.length + index; // 负索引转换
        }
        return target[index];
      },
      set(target, prop, value) {
        let index = Number(prop);
        if (index < 0) {
          index = target.length + index;
        }
        target[index] = value;
        return true;
      }
    });
  }

  const negArr = createNegativeIndexArray([1, 2, 3, 4, 5]);
  console.log('  负索引 arr[-1]:', negArr[-1]); // 5
  console.log('  负索引 arr[-2]:', negArr[-2]); // 4

  // ------------------------------------------------------------
  // 4.3 Reflect（反射）
  // ------------------------------------------------------------
  // Reflect 是一个内置对象，提供了拦截 JavaScript 操作的方法
  // 这些方法与 Proxy handler 的方法一一对应
  // Reflect 的方法将 Object 的一些命令式操作转为函数式操作
  console.log('\n--- 4.3 Reflect 反射 ---');

  const obj2 = { a: 1, b: 2 };

  // Reflect.get：读取属性
  console.log('  Reflect.get(obj, "a"):', Reflect.get(obj2, 'a')); // 1

  // Reflect.set：设置属性，返回布尔值表示是否成功
  console.log('  Reflect.set(obj, "c", 3):', Reflect.set(obj2, 'c', 3)); // true
  console.log('  obj.c:', obj2.c); // 3

  // Reflect.has：检查属性是否存在（相当于 in 运算符）
  console.log('  Reflect.has(obj, "a"):', Reflect.has(obj2, 'a')); // true

  // Reflect.deleteProperty：删除属性（相当于 delete 运算符）
  Reflect.deleteProperty(obj2, 'c');
  console.log('  删除后 obj.c:', obj2.c); // undefined

  // Reflect.ownKeys：获取所有自身属性键（包括 Symbol）
  const symKey = Symbol('key');
  const obj3 = { a: 1, [symKey]: 2 };
  console.log('  Reflect.ownKeys:', Reflect.ownKeys(obj3)); // ['a', Symbol(key)]

  // Reflect.apply：调用函数（相当于 Function.prototype.apply）
  function greet(name) {
    return `Hello, ${name}!`;
  }
  console.log('  Reflect.apply:', Reflect.apply(greet, null, ['World'])); // 'Hello, World!'

  // Reflect.construct：用 new 调用构造函数
  function Person(name) {
    this.name = name;
  }
  const p = Reflect.construct(Person, ['张三']);
  console.log('  Reflect.construct:', p.name); // '张三'

  // Proxy 和 Reflect 通常配合使用
  // 在 Proxy 陷阱中使用 Reflect 方法来执行默认行为
  const proxyWithReflect = new Proxy(target, {
    get(target, prop, receiver) {
      console.log(`  拦截读取: ${prop}`);
      return Reflect.get(target, prop, receiver); // 用 Reflect 执行默认行为
    },
    set(target, prop, value, receiver) {
      console.log(`  拦截设置: ${prop} = ${value}`);
      return Reflect.set(target, prop, value, receiver);
    }
  });
  proxyWithReflect.name;
  proxyWithReflect.age = 28;

  // ============================================================
  // 第五部分：Map / Set / WeakMap / WeakSet
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('五、Map / Set / WeakMap / WeakSet');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 5.1 Map
  // ------------------------------------------------------------
  // Map 是键值对集合，键可以是任意类型（对象也可以）
  // 与 Object 的区别：
  // - Map 的键可以是任意类型，Object 的键只能是字符串或 Symbol
  // - Map 保持插入顺序，Object 的键顺序不确定
  // - Map 有 size 属性，Object 需要手动计算
  // - Map 可以直接迭代，Object 需要获取键后迭代
  console.log('\n--- 5.1 Map ---');

  // 创建 Map
  const map = new Map();

  // 设置键值对
  map.set('name', '张三');       // 字符串键
  map.set(123, '数字键');         // 数字键
  map.set(true, '布尔键');        // 布尔键
  const objKey = { id: 1 };
  map.set(objKey, '对象键');      // 对象键

  console.log('  Map 大小:', map.size); // 4

  // 获取值
  console.log('  get("name"):', map.get('name'));
  console.log('  get(objKey):', map.get(objKey));

  // 检查键是否存在
  console.log('  has("name"):', map.has('name')); // true
  console.log('  has("age"):', map.has('age'));   // false

  // 删除键值对
  map.delete(123);
  console.log('  删除后 size:', map.size); // 3

  // 遍历 Map
  console.log('  遍历 Map（for...of）:');
  for (const [key, value] of map) {
    console.log(`    ${String(key)}: ${value}`);
  }

  // forEach 遍历
  console.log('  遍历 Map（forEach）:');
  map.forEach((value, key) => {
    console.log(`    ${String(key)}: ${value}`);
  });

  // keys() / values() / entries()
  console.log('  keys():', [...map.keys()]);
  console.log('  values():', [...map.values()]);

  // Map 与数组的转换
  const mapFromArray = new Map([
    ['a', 1],
    ['b', 2],
    ['c', 3]
  ]);
  console.log('  从数组创建 Map:', [...mapFromArray]);

  // 清空 Map
  map.clear();
  console.log('  clear 后 size:', map.size); // 0

  // ------------------------------------------------------------
  // 5.2 Set
  // ------------------------------------------------------------
  // Set 是值的集合，值是唯一的（没有重复值）
  console.log('\n--- 5.2 Set ---');

  // 创建 Set
  const set = new Set([1, 2, 3, 3, 4, 4, 5]);
  console.log('  Set（自动去重）:', [...set]); // [1, 2, 3, 4, 5]
  console.log('  Set 大小:', set.size); // 5

  // 添加元素
  set.add(6);
  set.add(6); // 重复添加无效
  console.log('  add 后:', [...set]);

  // 检查元素
  console.log('  has(3):', set.has(3)); // true
  console.log('  has(10):', set.has(10)); // false

  // 删除元素
  set.delete(1);
  console.log('  delete 后:', [...set]);

  // 遍历 Set
  console.log('  遍历 Set:');
  for (const value of set) {
    console.log('   ', value);
  }

  // Set 的经典应用：数组去重
  const arrWithDup = [1, 2, 2, 3, 4, 4, 5];
  const uniqueArr = [...new Set(arrWithDup)];
  console.log('  数组去重:', uniqueArr);

  // 并集、交集、差集
  const setA = new Set([1, 2, 3, 4]);
  const setB = new Set([3, 4, 5, 6]);

  const union = new Set([...setA, ...setB]);
  console.log('  并集:', [...union]); // [1, 2, 3, 4, 5, 6]

  const intersection = new Set([...setA].filter(x => setB.has(x)));
  console.log('  交集:', [...intersection]); // [3, 4]

  const difference = new Set([...setA].filter(x => !setB.has(x)));
  console.log('  差集(A-B):', [...difference]); // [1, 2]

  // ------------------------------------------------------------
  // 5.3 WeakMap
  // ------------------------------------------------------------
  // WeakMap 是弱引用的 Map，键必须是对象
  // 弱引用意味着如果键对象没有其他引用，会被垃圾回收
  // WeakMap 不可迭代，没有 size 属性，没有 clear 方法
  // 主要用途：存储私有数据、缓存关联数据
  console.log('\n--- 5.3 WeakMap ---');

  const wm = new WeakMap();

  // 键必须是对象
  let keyObj = { id: 1 };
  wm.set(keyObj, '对象关联的值');
  console.log('  get(keyObj):', wm.get(keyObj));
  console.log('  has(keyObj):', wm.has(keyObj));

  // 当键对象没有其他引用时，会被垃圾回收，WeakMap 中的条目也会消失
  // （这里无法直接演示垃圾回收，只能说明原理）
  keyObj = null; // 释放引用，WeakMap 中的对应条目会被 GC 回收

  // 实际应用：存储对象的私有数据
  const privateData = new WeakMap();

  class User {
    constructor(name, password) {
      // 用 this 作为键，存储私有数据
      privateData.set(this, { password });
      this.name = name;
    }

    checkPassword(pwd) {
      const data = privateData.get(this);
      return data.password === pwd;
    }
  }

  const user = new User('小明', '123456');
  console.log('  密码验证:', user.checkPassword('123456')); // true
  console.log('  密码验证:', user.checkPassword('wrong')); // false
  // 外部无法直接访问 password，因为它存储在 WeakMap 中

  // ------------------------------------------------------------
  // 5.4 WeakSet
  // ------------------------------------------------------------
  // WeakSet 是弱引用的 Set，值必须是对象
  // 同样不可迭代，没有 size，没有 clear
  console.log('\n--- 5.4 WeakSet ---');

  const ws = new WeakSet();

  let obj1 = { name: '对象1' };
  let obj2 = { name: '对象2' };

  ws.add(obj1);
  ws.add(obj2);
  console.log('  has(obj1):', ws.has(obj1)); // true

  ws.delete(obj1);
  console.log('  delete 后 has(obj1):', ws.has(obj1)); // false

  // 应用：检测对象是否属于某个集合，且不影响垃圾回收
  // 例如：标记哪些对象被"选中"了

  // ============================================================
  // 第六部分：模块化
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('六、模块化');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 6.1 CommonJS vs ES6 Module
  // ------------------------------------------------------------
  console.log('\n--- 6.1 模块化概述 ---');

  console.log('  JavaScript 模块系统演化：');
  console.log('    1. CommonJS（Node.js 传统方式）');
  console.log('       - 使用 require() 导入，module.exports 导出');
  console.log('       - 运行时加载，同步执行');
  console.log('       - 导出的是值的拷贝');
  console.log('    2. ES6 Module（ES6 标准）');
  console.log('       - 使用 import 导入，export 导出');
  console.log('       - 编译时加载（静态分析），可以做 tree-shaking');
  console.log('       - 导出的是值的引用（绑定）');
  console.log('       - 浏览器原生支持，Node.js 也支持（.mjs 或 "type": "module"）');

  console.log('\n  本项目使用 CommonJS（为了兼容直接 node 运行）');
  console.log('  但 ES6 Module 是未来趋势，语法示例如下：');
  console.log('    // 导出');
  console.log('    export const a = 1;');
  console.log('    export function foo() {}');
  console.log('    export default class MyClass {}');
  console.log('    // 导入');
  console.log('    import { a, foo } from "./module.js";');
  console.log('    import MyClass from "./module.js";');
  console.log('    import * as mod from "./module.js";');

  // ------------------------------------------------------------
  // 6.2 CommonJS 示例
  // ------------------------------------------------------------
  console.log('\n--- 6.2 CommonJS 使用示例 ---');

  // 本项目中所有模块都使用 CommonJS
  // module.exports 导出函数，require() 导入
  console.log('  本文件就是通过 module.exports 导出的');
  console.log('  在 main.js 中通过 require() 导入使用');

  // ============================================================
  // 演示结束
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('ES6+ 高级特性 演示结束');
  console.log('='.repeat(60));
};

if (require.main === module) {
  module.exports();
}
