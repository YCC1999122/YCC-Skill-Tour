/**
 * ============================================================
 * 文件名：async_programming.js
 * 描述：JavaScript 异步编程详解 - 回调、Promise、async/await、并发方法、事件循环
 * 作者：JavaScript 学习项目
 * 版本：1.0.0
 * ============================================================
 *
 * 本文件系统讲解 JavaScript 异步编程的核心知识，包括：
 * 1. 回调函数与回调地狱
 * 2. Promise 链式调用详解
 * 3. async / await 语法
 * 4. Promise.all / race / allSettled / any
 * 5. 错误处理（try/catch vs .catch）
 * 6. 事件循环原理（宏任务 / 微任务）
 *
 * 运行方式：node async/async_programming.js
 */

'use strict';

module.exports = function demoAsyncProgramming() {
  console.log('='.repeat(60));
  console.log('一、回调函数与回调地狱');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 1.1 什么是异步编程
  // ------------------------------------------------------------
  // JavaScript 是单线程语言，一次只能做一件事
  // 但很多操作是耗时的（如网络请求、文件读取、定时器）
  // 如果同步等待，会阻塞整个程序
  // 异步编程就是处理这些耗时操作的方式：发起操作后不等待，继续执行，操作完成后通过回调通知
  console.log('\n--- 1.1 异步编程概述 ---');

  console.log('  JavaScript 是单线程的，但通过事件循环实现异步');
  console.log('  常见的异步操作：');
  console.log('    - 定时器（setTimeout, setInterval）');
  console.log('    - 网络请求（AJAX, fetch）');
  console.log('    - 文件读写（Node.js fs 模块）');
  console.log('    - 数据库操作');
  console.log('    - Promise.then / async-await');

  // ------------------------------------------------------------
  // 1.2 回调函数（Callback）
  // ------------------------------------------------------------
  // 回调函数是最基础的异步处理方式
  // 将一个函数作为参数传给另一个函数，在异步操作完成后调用
  console.log('\n--- 1.2 回调函数 ---');

  // 模拟一个异步操作：n 毫秒后执行回调
  function delay(callback, ms) {
    // setTimeout 是浏览器/Node.js 提供的异步 API
    // 它会在指定时间后将回调函数放入任务队列
    setTimeout(() => {
      callback('延迟 ' + ms + 'ms 后的结果');
    }, ms);
  }

  console.log('  异步操作开始...');
  delay(result => {
    console.log('  回调执行:', result);
  }, 100);
  console.log('  异步操作已发起，继续执行同步代码...');

  // ------------------------------------------------------------
  // 1.3 回调地狱（Callback Hell）
  // ------------------------------------------------------------
  // 当多个异步操作有先后依赖关系时，回调函数会层层嵌套
  // 代码呈金字塔形，难以阅读和维护，称为"回调地狱"或"末日金字塔"
  console.log('\n--- 1.3 回调地狱 ---');

  console.log('  回调地狱示例（伪代码结构）：');
  console.log('    getUser(userId, function(user) {');
  console.log('      getOrders(user.id, function(orders) {');
  console.log('        getOrderDetails(orders[0].id, function(details) {');
  console.log('          getProduct(details.productId, function(product) {');
  console.log('            // 更多嵌套...');
  console.log('            console.log(product.name);');
  console.log('          });');
  console.log('        });');
  console.log('      });');
  console.log('    });');

  console.log('  回调地狱的问题：');
  console.log('    1. 代码结构混乱，难以阅读');
  console.log('    2. 难以调试和维护');
  console.log('    3. 错误处理困难');
  console.log('    4. 代码复用性差');
  console.log('  Promise 的出现就是为了解决这些问题');

  // ============================================================
  // 第二部分：Promise 链式调用
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('二、Promise 链式调用');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 2.1 Promise 解决回调地狱
  // ------------------------------------------------------------
  // Promise 将异步操作的结果和处理逻辑分离开
  // 通过 then 链式调用，将嵌套的代码变成线性的代码
  console.log('\n--- 2.1 Promise 链式调用 ---');

  // 用 Promise 封装延迟操作
  function promiseDelay(ms, value) {
    // 返回一个 Promise 对象
    return new Promise(resolve => {
      setTimeout(() => {
        resolve(value);
      }, ms);
    });
  }

  // 链式调用：每个 then 返回新的 Promise
  console.log('  Promise 链式调用开始...');
  promiseDelay(10, '第一步')
    .then(result => {
      console.log('  完成:', result);
      return promiseDelay(10, '第二步'); // 返回 Promise
    })
    .then(result => {
      console.log('  完成:', result);
      return promiseDelay(10, '第三步');
    })
    .then(result => {
      console.log('  完成:', result);
      console.log('  所有步骤完成！');
    });

  // ------------------------------------------------------------
  // 2.2 then 的返回值
  // ------------------------------------------------------------
  // then 的回调函数可以返回：
  // 1. Promise 对象：下一个 then 等待它 resolve
  // 2. 普通值：会被包装成 resolved 的 Promise，传给下一个 then
  // 3. 抛出异常：会被下一个 catch 捕获
  console.log('\n--- 2.2 then 的返回值 ---');

  Promise.resolve('初始值')
    .then(result => {
      console.log('  第一个 then 收到:', result);
      return '普通返回值'; // 返回普通值
    })
    .then(result => {
      console.log('  第二个 then 收到:', result);
      return Promise.resolve('Promise 返回值'); // 返回 Promise
    })
    .then(result => {
      console.log('  第三个 then 收到:', result);
      // 没有 return，默认返回 undefined
    })
    .then(result => {
      console.log('  第四个 then 收到:', result); // undefined
    });

  // ------------------------------------------------------------
  // 2.3 错误在 Promise 链中的传播
  // ------------------------------------------------------------
  // Promise 链中的错误会向下"冒泡"，直到被 catch 捕获
  // 这使得错误处理非常简洁
  console.log('\n--- 2.3 Promise 错误传播 ---');

  Promise.resolve()
    .then(() => {
      console.log('  第一步正常');
      throw new Error('第二步出错'); // 抛出错误
    })
    .then(() => {
      console.log('  这一步不会执行'); // 跳过
    })
    .then(() => {
      console.log('  这一步也不会执行'); // 跳过
    })
    .catch(error => {
      console.log('  catch 捕获错误:', error.message); // 捕获到错误
    })
    .then(() => {
      console.log('  catch 之后继续执行'); // catch 后还能继续链式调用
    });

  // ============================================================
  // 第三部分：async / await
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('三、async / await');
  console.log('='.repeat(60));

  // async / await 是 ES2017 引入的 Promise 语法糖
  // 让异步代码看起来像同步代码一样直观
  // async 函数返回一个 Promise
  // await 只能在 async 函数中使用，用于等待 Promise 完成

  // ------------------------------------------------------------
  // 3.1 async 函数
  // ------------------------------------------------------------
  console.log('\n--- 3.1 async 函数 ---');

  // async 函数总是返回 Promise
  async function asyncFunc() {
    return 'async 函数的返回值';
    // 等价于 return Promise.resolve('async 函数的返回值');
  }

  asyncFunc().then(result => {
    console.log('  async 函数返回:', result);
  });

  // async 函数抛出异常会返回 rejected 的 Promise
  async function asyncThrow() {
    throw new Error('async 函数抛出错误');
  }

  asyncThrow().catch(error => {
    console.log('  async 函数错误:', error.message);
  });

  // ------------------------------------------------------------
  // 3.2 await 关键字
  // ------------------------------------------------------------
  // await 等待 Promise resolve，并返回其结果
  // 如果 Promise reject，await 会抛出异常
  console.log('\n--- 3.2 await 关键字 ---');

  async function awaitDemo() {
    console.log('  await 演示开始');

    // await 等待 Promise 完成
    const result1 = await promiseDelay(10, '第一个 await 结果');
    console.log('  ', result1);

    const result2 = await promiseDelay(10, '第二个 await 结果');
    console.log('  ', result2);

    console.log('  await 演示结束');
    return '完成';
  }

  awaitDemo();

  // ------------------------------------------------------------
  // 3.3 用 async/await 重写回调地狱
  // ------------------------------------------------------------
  console.log('\n--- 3.3 async/await 解决回调地狱 ---');

  console.log('  对比回调地狱，async/await 写法：');
  console.log('    async function getProductInfo(userId) {');
  console.log('      const user = await getUser(userId);');
  console.log('      const orders = await getOrders(user.id);');
  console.log('      const details = await getOrderDetails(orders[0].id);');
  console.log('      const product = await getProduct(details.productId);');
  console.log('      return product.name;');
  console.log('    }');
  console.log('  代码清晰直观，像同步代码一样线性执行');

  // ============================================================
  // 第四部分：Promise 并发方法
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('四、Promise 并发方法');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 4.1 Promise.all
  // ------------------------------------------------------------
  // 等待所有 Promise 都成功，返回所有结果的数组
  // 如果有一个失败，立即返回失败的 Promise
  console.log('\n--- 4.1 Promise.all ---');

  const p1 = promiseDelay(20, '任务1');
  const p2 = promiseDelay(10, '任务2'); // 虽然先完成，但 all 会等待所有完成
  const p3 = promiseDelay(30, '任务3');

  console.log('  Promise.all 开始...');
  Promise.all([p1, p2, p3]).then(results => {
    console.log('  Promise.all 结果:', results); // ['任务1', '任务2', '任务3']
    console.log('  总耗时约 30ms（取决于最慢的那个）');
  });

  // ------------------------------------------------------------
  // 4.2 Promise.race
  // ------------------------------------------------------------
  // 竞速：第一个完成的 Promise 决定结果
  // 无论成功还是失败，只要有一个完成就返回
  console.log('\n--- 4.2 Promise.race ---');

  const fast = promiseDelay(10, '快的任务');
  const slow = promiseDelay(50, '慢的任务');

  Promise.race([fast, slow]).then(result => {
    console.log('  Promise.race 结果:', result); // '快的任务'
  });

  // race 的应用：超时控制
  function withTimeout(promise, timeout) {
    const timeoutPromise = new Promise((_, reject) => {
      setTimeout(() => {
        reject(new Error('操作超时'));
      }, timeout);
    });
    return Promise.race([promise, timeoutPromise]);
  }

  // 测试超时
  withTimeout(promiseDelay(100, '正常完成'), 50)
    .catch(error => {
      console.log('  超时控制测试:', error.message); // '操作超时'
    });

  // ------------------------------------------------------------
  // 4.3 Promise.allSettled
  // ------------------------------------------------------------
  // 等待所有 Promise 完成（无论成功失败），返回每个 Promise 的状态和结果
  // ES2020 新增
  console.log('\n--- 4.3 Promise.allSettled ---');

  const successP = Promise.resolve('成功');
  const failP = Promise.reject(new Error('失败'));
  const anotherSuccessP = Promise.resolve('又成功了');

  Promise.allSettled([successP, failP, anotherSuccessP]).then(results => {
    console.log('  Promise.allSettled 结果:');
    results.forEach((res, i) => {
      if (res.status === 'fulfilled') {
        console.log(`    第${i + 1}个：成功，值 = ${res.value}`);
      } else {
        console.log(`    第${i + 1}个：失败，原因 = ${res.reason.message}`);
      }
    });
  });

  // ------------------------------------------------------------
  // 4.4 Promise.any
  // ------------------------------------------------------------
  // 等待第一个成功的 Promise，返回其结果
  // 如果所有 Promise 都失败，返回 AggregateError
  // ES2021 新增
  console.log('\n--- 4.4 Promise.any ---');

  const fail1 = Promise.reject(new Error('失败1'));
  const fail2 = Promise.reject(new Error('失败2'));
  const succeed = promiseDelay(10, '成功的那个');

  if (typeof Promise.any === 'function') {
    Promise.any([fail1, fail2, succeed]).then(result => {
      console.log('  Promise.any 结果:', result); // '成功的那个'
    });

    // 全部失败的情况
    Promise.any([fail1, fail2])
      .catch(error => {
        console.log('  全部失败:', error.constructor.name); // AggregateError
        console.log('  失败数量:', error.errors.length);
      });
  } else {
    console.log('  当前环境不支持 Promise.any（需 Node.js 15+）');
  }

  // ------------------------------------------------------------
  // 4.5 并发方法对比
  // ------------------------------------------------------------
  console.log('\n--- 4.5 并发方法对比总结 ---');
  console.log('  | 方法         | 成功条件         | 失败条件         | 返回值         |');
  console.log('  |-------------|------------------|------------------|----------------|');
  console.log('  | all         | 全部成功         | 一个失败就失败   | 结果数组       |');
  console.log('  | race        | 第一个完成       | 第一个失败       | 第一个结果     |');
  console.log('  | allSettled  | 总是成功         | 总是成功         | 状态+结果数组  |');
  console.log('  | any         | 一个成功就成功   | 全部失败才失败   | 第一个成功结果 |');

  // ============================================================
  // 第五部分：错误处理
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('五、错误处理');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 5.1 Promise 的错误处理
  // ------------------------------------------------------------
  console.log('\n--- 5.1 Promise 错误处理 ---');

  // 方式1：then 的第二个参数
  console.log('  方式1：then 的第二个参数');
  Promise.reject(new Error('测试错误1'))
    .then(
      result => console.log('    成功:', result),
      error => console.log('    失败:', error.message) // 捕获错误
    );

  // 方式2：.catch()（推荐）
  console.log('  方式2：.catch()');
  Promise.reject(new Error('测试错误2'))
    .then(result => console.log('    成功:', result))
    .catch(error => console.log('    失败:', error.message)); // 捕获错误

  // catch 可以捕获链式调用中任意位置的错误
  console.log('  catch 可以捕获链上任意位置的错误');
  Promise.resolve()
    .then(() => {
      throw new Error('then 中抛出的错误');
    })
    .then(() => {
      return Promise.reject(new Error('Promise.reject 的错误'));
    })
    .catch(error => {
      console.log('    catch 捕获:', error.message); // 捕获第一个错误
    });

  // .finally()：无论成功失败都会执行
  console.log('  .finally() 总是执行');
  Promise.resolve('成功数据')
    .then(result => console.log('    成功:', result))
    .catch(error => console.log('    失败:', error))
    .finally(() => console.log('    finally: 清理资源'));

  // ------------------------------------------------------------
  // 5.2 async/await 的错误处理
  // ------------------------------------------------------------
  // 使用 try/catch 捕获 await 中的错误
  console.log('\n--- 5.2 async/await 错误处理 ---');

  async function asyncErrorDemo() {
    try {
      console.log('  try 块开始');
      await Promise.reject(new Error('await 出错了'));
      console.log('  这行不会执行'); // 不会执行
    } catch (error) {
      console.log('  catch 捕获:', error.message);
    } finally {
      console.log('  finally 总是执行');
    }
  }
  asyncErrorDemo();

  // 多个 await 共用一个 try/catch
  async function multiAwaitError() {
    try {
      const r1 = await Promise.resolve('第一步成功');
      console.log(' ', r1);
      const r2 = await Promise.reject(new Error('第二步失败'));
      console.log(' ', r2); // 不会执行
      const r3 = await Promise.resolve('第三步成功');
      console.log(' ', r3); // 不会执行
    } catch (error) {
      console.log('  出错了:', error.message);
    }
  }
  multiAwaitError();

  // 单个 await 单独处理错误
  async function individualError() {
    const result1 = await Promise.reject(new Error('第一个失败'))
      .catch(err => {
        console.log('  单独处理第一个错误:', err.message);
        return '默认值1';
      });
    console.log('  result1:', result1); // '默认值1'

    const result2 = await Promise.resolve('第二个成功');
    console.log('  result2:', result2); // '第二个成功'
  }
  individualError();

  // ------------------------------------------------------------
  // 5.3 未捕获的 Promise 错误
  // ------------------------------------------------------------
  // 如果 Promise reject 了但没有 catch 处理，会触发 unhandledRejection 事件
  console.log('\n--- 5.3 未捕获错误提醒 ---');
  console.log('  注意：Promise reject 必须处理，否则会报 UnhandledPromiseRejectionWarning');
  console.log('  最佳实践：');
  console.log('    - 每个 Promise 链都要有 catch');
  console.log('    - async 函数调用也要注意错误处理');
  console.log('    - 全局监听 unhandledRejection 做兜底处理');

  // ============================================================
  // 第六部分：事件循环（Event Loop）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('六、事件循环原理');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 6.1 事件循环基本概念
  // ------------------------------------------------------------
  // JavaScript 是单线程的，但通过事件循环实现非阻塞的异步操作
  // 事件循环机制：
  // 1. 执行栈（Call Stack）：存放当前正在执行的代码
  // 2. 任务队列（Task Queue）：存放待执行的异步回调
  //    - 宏任务（Macrotask）：setTimeout, setInterval, I/O, UI 渲染等
  //    - 微任务（Microtask）：Promise.then, MutationObserver, queueMicrotask 等
  // 3. 执行规则：
  //    - 先执行所有同步代码（执行栈清空）
  //    - 然后执行所有微任务（微任务队列清空）
  //    - 然后执行一个宏任务
  //    - 再执行所有微任务
  //    - 如此循环...
  console.log('\n--- 6.1 事件循环概述 ---');

  console.log('  JavaScript 运行时组成：');
  console.log('    1. 调用栈（Call Stack）：执行同步代码');
  console.log('    2. 堆（Heap）：存储对象');
  console.log('    3. 任务队列（Task Queue）：存放异步回调');
  console.log('       - 宏任务队列：setTimeout, setInterval, I/O, setImmediate(Node)');
  console.log('       - 微任务队列：Promise.then/catch/finally, queueMicrotask, process.nextTick(Node)');

  console.log('\n  事件循环执行顺序：');
  console.log('    1. 执行全局同步代码（执行栈）');
  console.log('    2. 执行所有微任务（清空微任务队列）');
  console.log('    3. 执行一个宏任务（取出一个宏任务执行）');
  console.log('    4. 执行所有微任务（再次清空微任务队列）');
  console.log('    5. 重复 3-4 步骤');

  // ------------------------------------------------------------
  // 6.2 经典面试题
  // ------------------------------------------------------------
  console.log('\n--- 6.2 事件循环执行顺序示例 ---');

  console.log('  经典面试题代码：');
  console.log('    console.log("1");');
  console.log('    setTimeout(() => console.log("2"), 0);');
  console.log('    Promise.resolve().then(() => console.log("3"));');
  console.log('    console.log("4");');
  console.log('  输出顺序是：1, 4, 3, 2');
  console.log('  解释：');
  console.log('    - 同步代码先执行：1, 4');
  console.log('    - 然后执行微任务：3（Promise.then）');
  console.log('    - 然后执行宏任务：2（setTimeout）');

  // 实际运行验证
  console.log('\n  实际运行验证：');
  console.log('    1');
  setTimeout(() => console.log('    2 (setTimeout 宏任务)'), 0);
  Promise.resolve().then(() => console.log('    3 (Promise.then 微任务)'));
  console.log('    4');

  // 更复杂的例子
  console.log('\n  更复杂的例子（请观察输出顺序）：');
  console.log('    同步开始');
  setTimeout(() => {
    console.log('    setTimeout1 宏任务');
    Promise.resolve().then(() => {
      console.log('    setTimeout1 中的微任务');
    });
  }, 0);
  Promise.resolve().then(() => {
    console.log('    微任务1');
  }).then(() => {
    console.log('    微任务2');
  });
  setTimeout(() => {
    console.log('    setTimeout2 宏任务');
  }, 0);
  console.log('    同步结束');

  // 预期输出顺序：
  // 同步开始
  // 同步结束
  // 微任务1
  // 微任务2
  // setTimeout1 宏任务
  // setTimeout1 中的微任务
  // setTimeout2 宏任务

  // ------------------------------------------------------------
  // 6.3 Node.js 与浏览器事件循环的区别
  // ------------------------------------------------------------
  console.log('\n--- 6.3 Node.js vs 浏览器事件循环 ---');

  console.log('  浏览器：');
  console.log('    - 宏任务 + 微任务 两级队列');
  console.log('    - 微任务：Promise.then, MutationObserver, queueMicrotask');
  console.log('    - 宏任务：setTimeout, setInterval, requestAnimationFrame, I/O');

  console.log('\n  Node.js（libuv）：');
  console.log('    - 更复杂的阶段模型：');
  console.log('      timers → pending callbacks → idle, prepare → poll');
  console.log('      → check → close callbacks');
  console.log('    - 每个阶段之间执行微任务');
  console.log('    - process.nextTick 优先级高于 Promise.then');
  console.log('    - setImmediate 在 check 阶段执行');
  console.log('    - setTimeout 在 timers 阶段执行');

  // ============================================================
  // 演示结束
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('异步编程 演示结束');
  console.log('（注意：由于异步特性，部分输出顺序可能与上面的标题不同）');
  console.log('='.repeat(60));
};

if (require.main === module) {
  module.exports();
}
