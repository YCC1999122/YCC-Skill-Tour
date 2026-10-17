/**
 * ============================================================
 * 文件名：main.js
 * 描述：JavaScript 学习项目 - 主程序入口（交互式命令行菜单）
 * 作者：JavaScript 学习项目
 * 版本：1.0.0
 * ============================================================
 *
 * 本文件是整个学习项目的入口程序，提供交互式命令行菜单
 * 用户可以选择不同的模块进行学习和演示
 *
 * 运行方式：node main.js  或  npm start
 *
 * 功能：
 * 1. 展示所有学习模块
 * 2. 用户输入编号选择模块
 * 3. 运行对应模块的演示代码
 * 4. 演示结束后返回菜单
 *
 * ============================================================
 */

// 启用严格模式
'use strict';

// ============================================================
// 模块导入
// ============================================================

// 引入 Node.js 内置的 readline 模块，用于读取命令行输入
// readline 模块提供了逐行读取流的接口，可以实现交互式命令行
const readline = require('readline');

// 导入各个学习模块的演示函数
// 每个模块导出一个函数，调用该函数即可运行对应模块的演示
const demoTypesAndOperators = require('./basics/01_types_and_operators');
const demoFunctions = require('./basics/02_functions');
const demoOOP = require('./oop/oop');
const demoArrayMethods = require('./advanced/array_methods');
const demoES6Features = require('./advanced/es6_features');
const demoAsyncProgramming = require('./async/async_programming');
const demoDataStructures = require('./data/structures');
const demoDesignPatterns = require('./patterns/design_patterns');

// ============================================================
// 创建 readline 接口
// ============================================================

// createInterface 方法创建一个 readline 接口实例
// input: 指定输入流（这里是标准输入 process.stdin）
// output: 指定输出流（这里是标准输出 process.stdout）
const rl = readline.createInterface({
  input: process.stdin,   // 标准输入流（键盘输入）
  output: process.stdout  // 标准输出流（控制台输出）
});

// ============================================================
// 菜单配置
// ============================================================

// 菜单选项配置数组
// 每个选项包含：编号、标题、描述、对应的演示函数
const menuOptions = [
  {
    id: '1',
    title: '基础语法 - 数据类型与运算符',
    description: '8种数据类型、类型转换、运算符、控制流、var/let/const、解构赋值',
    demo: demoTypesAndOperators
  },
  {
    id: '2',
    title: '基础语法 - 函数',
    description: '函数声明/表达式/箭头函数、参数、闭包、高阶函数、柯里化、this指向',
    demo: demoFunctions
  },
  {
    id: '3',
    title: '面向对象编程',
    description: '原型与原型链、构造函数、ES6 Class、继承、多态、私有字段',
    demo: demoOOP
  },
  {
    id: '4',
    title: '高级特性 - 数组方法',
    description: 'map/filter/reduce/find/some/every/sort、链式调用、手写实现',
    demo: demoArrayMethods
  },
  {
    id: '5',
    title: '高级特性 - ES6+ 特性',
    description: 'Promise、Symbol、迭代器、生成器、Proxy、Reflect、Map、Set',
    demo: demoES6Features
  },
  {
    id: '6',
    title: '异步编程',
    description: '回调、Promise、async/await、并发方法、错误处理、事件循环',
    demo: demoAsyncProgramming
  },
  {
    id: '7',
    title: '数据结构与算法',
    description: '链表、栈、队列、二叉搜索树、冒泡/选择/插入/快速/归并排序',
    demo: demoDataStructures
  },
  {
    id: '8',
    title: '设计模式',
    description: '单例、工厂、观察者、策略、装饰器、代理 6 种常用设计模式',
    demo: demoDesignPatterns
  },
  {
    id: '0',
    title: '退出程序',
    description: '退出学习程序',
    demo: null  // 退出选项没有演示函数
  }
];

// ============================================================
// 辅助函数
// ============================================================

/**
 * 打印分隔线
 * @param {string} char - 分隔线字符，默认 '='
 * @param {number} length - 分隔线长度，默认 60
 */
function printSeparator(char = '=', length = 60) {
  // repeat 方法将字符串重复指定次数
  console.log(char.repeat(length));
}

/**
 * 打印欢迎标题
 */
function printWelcome() {
  printSeparator('*');
  // 使用模板字符串进行格式化输出
  console.log(`
    JavaScript 深度学习项目 v1.0.0
    涵盖基础语法、面向对象、高级特性、异步编程、
    设计模式、数据结构等核心知识

    每个文件都包含详细的中文注释，适合学习参考
  `);
  printSeparator('*');
  console.log();
}

/**
 * 打印菜单
 */
function printMenu() {
  printSeparator();
  console.log('  请选择要学习的模块（输入编号后回车）：');
  printSeparator('-');

  // 遍历菜单选项，格式化输出
  menuOptions.forEach(option => {
    // 格式化输出：编号 + 标题 + 描述
    console.log(`  ${option.id}. ${option.title}`);
    console.log(`     ${option.description}`);
  });

  printSeparator();
}

/**
 * 提示用户输入
 * @param {string} prompt - 提示文字
 * @returns {Promise<string>} 用户输入的内容
 */
function askQuestion(prompt) {
  // readline 的 question 方法是回调式的，这里封装成 Promise
  // 这样可以使用 async/await 更优雅地处理
  return new Promise(resolve => {
    rl.question(prompt, answer => {
      resolve(answer.trim()); // trim 去除首尾空白字符
    });
  });
}

// ============================================================
// 核心逻辑
// ============================================================

/**
 * 运行指定模块的演示
 * @param {Object} option - 菜单选项对象
 */
function runDemo(option) {
  console.log();
  printSeparator('>', 60);
  console.log(`  开始演示：${option.title}`);
  printSeparator('>', 60);
  console.log();

  // 调用对应的演示函数
  option.demo();

  // 注意：由于部分模块包含异步代码（如 setTimeout）
  // 演示结束后异步输出可能会继续在后台输出
  // 这是正常现象，体现了 JavaScript 的异步特性
}

/**
 * 主循环：显示菜单 -> 等待输入 -> 执行对应操作 -> 返回菜单
 */
async function mainLoop() {
  // 无限循环，直到用户选择退出
  while (true) {
    // 打印菜单
    printMenu();

    // 等待用户输入
    const choice = await askQuestion('  请输入选项编号：');

    // 根据输入查找对应的选项
    const selectedOption = menuOptions.find(opt => opt.id === choice);

    // 处理用户输入
    if (!selectedOption) {
      // 输入无效
      console.log();
      console.log('  [提示] 无效的选项，请重新输入！');
      console.log();
      continue; // 跳过本次循环，重新显示菜单
    }

    if (selectedOption.id === '0') {
      // 用户选择退出
      console.log();
      printSeparator('*');
      console.log('  感谢使用 JavaScript 学习项目！');
      console.log('  祝你学习愉快，编码顺利！');
      printSeparator('*');
      console.log();

      // 关闭 readline 接口，释放资源
      rl.close();
      // 退出循环
      break;
    }

    // 运行选中模块的演示
    runDemo(selectedOption);

    // 演示结束后，等待用户按回车返回菜单
    console.log();
    await askQuestion('  按回车键返回菜单...');
    console.log();
  }
}

// ============================================================
// 错误处理
// ============================================================

// 监听 readline 接口的 close 事件
// 当用户按下 Ctrl+C 或调用 rl.close() 时触发
rl.on('close', () => {
  // 确保程序正常退出
  // process.exit(0) 表示正常退出（退出码 0）
  process.exit(0);
});

// 监听未捕获的异常，防止程序意外崩溃
process.on('uncaughtException', error => {
  console.error('\n  [错误] 程序发生未捕获的异常：', error.message);
  console.error('  错误详情：', error.stack);
  // 退出码 1 表示异常退出
  process.exit(1);
});

// 监听未处理的 Promise 拒绝
process.on('unhandledRejection', reason => {
  console.error('\n  [警告] 未处理的 Promise 拒绝：', reason);
});

// ============================================================
// 程序启动
// ============================================================

// 打印欢迎信息
printWelcome();

// 启动主循环
// 由于 mainLoop 是 async 函数，返回 Promise
// 这里用 catch 捕获可能的错误
mainLoop().catch(error => {
  console.error('  程序运行出错：', error);
  rl.close();
  process.exit(1);
});
