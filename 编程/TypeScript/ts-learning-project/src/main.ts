/**
 * ============================================================
 *  TypeScript 学习项目 - 主程序入口
 * ============================================================
 *
 * 本文件是项目的入口文件，提供一个交互式菜单
 * 用户可以通过命令行选择运行各个模块的演示
 *
 * 使用 Node.js 的 readline 模块实现命令行交互
 * ============================================================
 */

// 导入 Node.js 的 readline 模块，用于命令行交互
import * as readline from 'readline';

// 导入各个模块的演示函数
import { runTypesDemo } from './basics/types';
import { runFunctionsDemo } from './basics/functions';
import { runOOPDemo } from './oop/oop';
import { runGenericsDemo } from './generics/generics';
import { runAdvancedDemo } from './advanced/advanced';
import { runPatternsDemo } from './patterns/patterns';
import { runUtilityDemo } from './utility/utility';

/**
 * 创建 readline 接口
 * 配置输入输出流
 */
const rl = readline.createInterface({
  input: process.stdin,                          // 标准输入（键盘）
  output: process.stdout                         // 标准输出（控制台）
});

/**
 * 显示主菜单
 * 打印所有可选的模块
 */
function showMenu(): void {
  console.log('');
  console.log('╔══════════════════════════════════════════════╗');
  console.log('║     TypeScript 学习项目 - 交互菜单          ║');
  console.log('╠══════════════════════════════════════════════╣');
  console.log('║  1. 基础类型 (Basic Types)                   ║');
  console.log('║  2. 函数类型 (Functions)                     ║');
  console.log('║  3. 面向对象 (OOP)                           ║');
  console.log('║  4. 泛型 (Generics)                          ║');
  console.log('║  5. 高级特性 (Advanced)                      ║');
  console.log('║  6. 设计模式 (Patterns)                      ║');
  console.log('║  7. 工具类型与类型体操 (Utility)             ║');
  console.log('║  8. 运行全部演示                             ║');
  console.log('║  0. 退出程序                                 ║');
  console.log('╚══════════════════════════════════════════════╝');
  console.log('');
}

/**
 * 询问用户输入
 * @param question 问题文本
 * @returns 用户输入的字符串（Promise）
 */
function askQuestion(question: string): Promise<string> {
  return new Promise((resolve) => {
    rl.question(question, (answer) => {
      resolve(answer.trim());
    });
  });
}

/**
 * 运行指定模块的演示
 * @param choice 用户选择的选项
 * @returns 是否继续运行（true 继续，false 退出）
 */
async function runDemo(choice: string): Promise<boolean> {
  switch (choice) {
    case '1':
      console.log('\n' + '='.repeat(50));
      console.log('  正在运行：基础类型演示');
      console.log('='.repeat(50) + '\n');
      runTypesDemo();
      break;

    case '2':
      console.log('\n' + '='.repeat(50));
      console.log('  正在运行：函数类型演示');
      console.log('='.repeat(50) + '\n');
      runFunctionsDemo();
      break;

    case '3':
      console.log('\n' + '='.repeat(50));
      console.log('  正在运行：面向对象演示');
      console.log('='.repeat(50) + '\n');
      runOOPDemo();
      break;

    case '4':
      console.log('\n' + '='.repeat(50));
      console.log('  正在运行：泛型演示');
      console.log('='.repeat(50) + '\n');
      runGenericsDemo();
      break;

    case '5':
      console.log('\n' + '='.repeat(50));
      console.log('  正在运行：高级特性演示');
      console.log('='.repeat(50) + '\n');
      runAdvancedDemo();
      break;

    case '6':
      console.log('\n' + '='.repeat(50));
      console.log('  正在运行：设计模式演示');
      console.log('='.repeat(50) + '\n');
      runPatternsDemo();
      break;

    case '7':
      console.log('\n' + '='.repeat(50));
      console.log('  正在运行：工具类型与类型体操演示');
      console.log('='.repeat(50) + '\n');
      runUtilityDemo();
      break;

    case '8':
      console.log('\n' + '='.repeat(50));
      console.log('  正在运行：全部演示');
      console.log('='.repeat(50) + '\n');
      runTypesDemo();
      runFunctionsDemo();
      runOOPDemo();
      runGenericsDemo();
      runAdvancedDemo();
      runPatternsDemo();
      runUtilityDemo();
      break;

    case '0':
      console.log('\n感谢使用 TypeScript 学习项目！');
      console.log('再见！\n');
      return false;                                  // 返回 false 表示退出

    default:
      console.log('\n无效的选项，请输入 0-8 之间的数字。\n');
      break;
  }

  return true;                                     // 返回 true 表示继续运行
}

/**
 * 主函数
 * 程序入口，循环显示菜单并处理用户输入
 */
async function main(): Promise<void> {
  // 打印欢迎信息
  console.log('');
  console.log('╔══════════════════════════════════════════════╗');
  console.log('║                                              ║');
  console.log('║   欢迎来到 TypeScript 学习项目！             ║');
  console.log('║                                              ║');
  console.log('║   本项目包含以下模块：                       ║');
  console.log('║   - 基础类型、函数类型                       ║');
  console.log('║   - 面向对象、泛型                           ║');
  console.log('║   - 高级特性、设计模式                       ║');
  console.log('║   - 工具类型与类型体操                       ║');
  console.log('║                                              ║');
  console.log('║   每个文件都有详细的中文注释                 ║');
  console.log('║   建议结合源代码一起学习                     ║');
  console.log('║                                              ║');
  console.log('╚══════════════════════════════════════════════╝');

  // 主循环
  let running = true;
  while (running) {
    // 显示菜单
    showMenu();

    // 询问用户选择
    const choice = await askQuestion('请输入选项编号 (0-8): ');

    // 运行演示
    running = await runDemo(choice);

    // 如果还在运行，等待用户按回车继续
    if (running) {
      await askQuestion('按回车键继续...');
    }
  }

  // 关闭 readline 接口
  rl.close();
}

// 启动程序
main().catch((error) => {
  // 捕获并打印任何未处理的错误
  console.error('程序运行出错：', error);
  rl.close();
  process.exit(1);
});
