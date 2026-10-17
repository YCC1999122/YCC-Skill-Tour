/**
 * ============================================================
 * 文件名：01_types_and_operators.js
 * 描述：JavaScript 基础语法 - 数据类型、运算符、控制流、作用域、解构赋值
 * 作者：JavaScript 学习项目
 * 版本：1.0.0
 * ============================================================
 *
 * 本文件系统讲解 JavaScript 的基础语法知识，包括：
 * 1. 8 种数据类型及其特性
 * 2. 类型转换（隐式转换与显式转换）
 * 3. 相等运算符（== vs ===）的区别
 * 4. 各类运算符（算术、比较、逻辑、位运算等）
 * 5. 程序控制流（条件、循环、分支）
 * 6. var / let / const 的区别与作用域
 * 7. 解构赋值（数组解构、对象解构）
 * 8. 模板字符串、扩展运算符、剩余参数
 *
 * 运行方式：node basics/01_types_and_operators.js
 */

// ============================================================
// 第一部分：数据类型（Data Types）
// ============================================================
// JavaScript 共有 8 种数据类型：
// 基本类型（7种）：Number, String, Boolean, Null, Undefined, Symbol, BigInt
// 引用类型（1种）：Object（包含 Array, Function, Date, RegExp 等）
// ============================================================

// 使用严格模式，消除 JavaScript 语法中一些不合理、不严谨的地方
'use strict';

// 导出演示函数，供主程序调用
// module.exports 是 CommonJS 规范的导出方式，相当于 ES6 的 export default
module.exports = function demoTypesAndOperators() {
  // 打印分隔线，用于在控制台中区分不同的演示部分
  console.log('='.repeat(60));
  console.log('一、JavaScript 数据类型详解');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 1.1 Number 类型
  // ------------------------------------------------------------
  // Number 类型表示整数和浮点数
  // JavaScript 的 Number 采用 IEEE 754 标准的 64 位双精度浮点数
  // 所以 0.1 + 0.2 !== 0.3 是经典的浮点数精度问题
  console.log('\n--- 1.1 Number 数字类型 ---');

  // 整数声明
  const integerNum = 42;                // 普通整数
  const hexNum = 0xff;                  // 十六进制表示（0x 前缀），值为 255
  const binaryNum = 0b1010;             // 二进制表示（0b 前缀），值为 10
  const octalNum = 0o77;                // 八进制表示（0o 前缀），值为 63

  // 浮点数声明
  const floatNum = 3.14;                // 普通浮点数
  const sciNum = 2.5e3;                 // 科学计数法，表示 2.5 * 10^3 = 2500

  // 特殊的 Number 值
  const infinityNum = Infinity;         // 正无穷大（除以 0 得到）
  const negInfinityNum = -Infinity;     // 负无穷大
  const notANumber = NaN;               // Not a Number，表示非数值
  // NaN 是唯一一个不等于自身的值，即 NaN !== NaN

  // typeof 运算符用于检测数据类型，返回类型的字符串表示
  console.log('整数 42 的类型:', typeof integerNum);        // 'number'
  console.log('浮点数 3.14 的类型:', typeof floatNum);       // 'number'
  console.log('Infinity 的类型:', typeof infinityNum);       // 'number'
  console.log('NaN 的类型:', typeof notANumber);             // 'number'
  console.log('0.1 + 0.2 =', 0.1 + 0.2);                    // 0.30000000000000004（浮点数精度问题）
  console.log('NaN === NaN?', NaN === NaN);                  // false（NaN 不等于自身）
  console.log('isNaN(NaN)?', isNaN(NaN));                    // true（使用 isNaN 判断）
  console.log('Number.isNaN(NaN)?', Number.isNaN(NaN));      // true（更严格的判断）

  // ------------------------------------------------------------
  // 1.2 String 类型
  // ------------------------------------------------------------
  // String 类型用于表示文本数据，由零个或多个 16 位 Unicode 字符组成
  console.log('\n--- 1.2 String 字符串类型 ---');

  // 字符串的三种表示方式
  const singleQuoteStr = '单引号字符串';         // 单引号
  const doubleQuoteStr = "双引号字符串";         // 双引号
  const templateStr = `模板字符串`;              // 反引号（模板字面量）

  // 字符串中可以包含转义字符
  const escapeStr = '第一行\n第二行\t缩进\\反斜杠\'单引号"双引号';
  // \n = 换行符，\t = 制表符，\\ = 反斜杠本身，\' = 单引号，\" = 双引号

  // 模板字符串的强大功能：支持多行和变量插值
  const name = '张三';
  const age = 25;
  // ${expression} 是模板字符串的插值语法，可以嵌入任意 JavaScript 表达式
  const greeting = `你好，我叫${name}，今年${age}岁。
明年我就${age + 1}岁了。`;
  // 模板字符串会保留所有空格和换行，输出格式与书写格式一致

  // 字符串常用属性和方法
  const sampleStr = 'Hello, JavaScript!';
  console.log('字符串长度:', sampleStr.length);              // 18（length 是属性，不是方法）
  console.log('转大写:', sampleStr.toUpperCase());           // HELLO, JAVASCRIPT!
  console.log('转小写:', sampleStr.toLowerCase());           // hello, javascript!
  console.log('截取子串(0,5):', sampleStr.substring(0, 5));  // Hello
  console.log('是否包含 Java:', sampleStr.includes('Java')); // true
  console.log('以 Hello 开头:', sampleStr.startsWith('Hello')); // true
  console.log('以 ! 结尾:', sampleStr.endsWith('!'));        // true
  console.log('模板字符串示例:', greeting);

  // ------------------------------------------------------------
  // 1.3 Boolean 类型
  // ------------------------------------------------------------
  // Boolean 类型只有两个值：true（真）和 false（假）
  console.log('\n--- 1.3 Boolean 布尔类型 ---');

  const isTrue = true;   // 布尔值真
  const isFalse = false; // 布尔值假

  // 以下值在转换为布尔值时会变成 false，称为"假值"（falsy values）：
  // false, 0, '', null, undefined, NaN
  // 其他所有值转换为布尔值时都是 true，称为"真值"（truthy values）
  console.log('Boolean(0):', Boolean(0));           // false
  console.log('Boolean(""):', Boolean(''));         // false
  console.log('Boolean(null):', Boolean(null));     // false
  console.log('Boolean(undefined):', Boolean(undefined)); // false
  console.log('Boolean(NaN):', Boolean(NaN));       // false
  console.log('Boolean("false"):', Boolean('false')); // true（非空字符串都是真值）
  console.log('Boolean({}):', Boolean({}));         // true（空对象也是真值）
  console.log('Boolean([]):', Boolean([]));         // true（空数组也是真值）

  // ------------------------------------------------------------
  // 1.4 Null 类型
  // ------------------------------------------------------------
  // Null 类型只有一个值：null，表示"空值"或"不存在的对象"
  console.log('\n--- 1.4 Null 空值类型 ---');

  const nullValue = null;
  // typeof null 返回 'object'，这是 JavaScript 的一个历史遗留 bug
  // 因为在 JavaScript 最初的实现中，对象的类型标签是 0，而 null 表示空指针（0x00）
  // 所以 null 被错误地判断为对象类型
  console.log('null 的值:', nullValue);             // null
  console.log('typeof null:', typeof nullValue);    // 'object'（历史遗留 bug）
  console.log('null == undefined:', null == undefined); // true（宽松相等时两者相等）
  console.log('null === undefined:', null === undefined); // false（严格相等时两者不等）

  // ------------------------------------------------------------
  // 1.5 Undefined 类型
  // ------------------------------------------------------------
  // Undefined 类型只有一个值：undefined，表示"未定义"
  // 当声明了变量但未赋值时，变量的值就是 undefined
  console.log('\n--- 1.5 Undefined 未定义类型 ---');

  let undefinedVar; // 声明了变量但未赋值，值为 undefined
  // const 声明的变量必须初始化，否则会报错
  // const constUndefined; // SyntaxError: Missing initializer in const declaration

  function noReturnFunc() {
    // 函数没有 return 语句时，默认返回 undefined
  }

  console.log('未赋值变量:', undefinedVar);                 // undefined
  console.log('typeof undefinedVar:', typeof undefinedVar); // 'undefined'
  console.log('无返回值函数:', noReturnFunc());             // undefined

  // null 和 undefined 的区别：
  // - null 表示"空对象指针"，通常用来表示主动释放的对象或预期的空值
  // - undefined 表示"未定义"，通常是变量声明后未赋值的默认状态
  // - 一般来说，如果你想明确表示"没有值"，用 null；
  //   如果你只是声明变量还没赋值，它会自动是 undefined

  // ------------------------------------------------------------
  // 1.6 Symbol 类型
  // ------------------------------------------------------------
  // Symbol 是 ES6 引入的新类型，表示独一无二的值
  // 主要用途是作为对象的属性名，防止属性名冲突
  console.log('\n--- 1.6 Symbol 符号类型 ---');

  // 创建 Symbol，参数是对 Symbol 的描述（仅用于调试，不参与比较）
  const sym1 = Symbol('foo');
  const sym2 = Symbol('foo');
  // 即使描述相同，两个 Symbol 也是不同的
  console.log('Symbol("foo") === Symbol("foo")?', sym1 === sym2); // false

  // Symbol 作为对象属性名，可以避免属性名冲突
  const objWithSymbol = {
    [sym1]: 'symbol 属性值',   // 使用计算属性名语法
    name: '普通属性'
  };
  console.log('通过 Symbol 访问属性:', objWithSymbol[sym1]);     // 'symbol 属性值'
  console.log('Object.keys 不包含 Symbol:', Object.keys(objWithSymbol)); // ['name']
  // Symbol 属性不会被 for...in、Object.keys()、JSON.stringify() 等遍历到
  // 需要使用 Object.getOwnPropertySymbols() 或 Reflect.ownKeys() 来获取

  // Symbol.for() 可以创建全局可复用的 Symbol
  const globalSym1 = Symbol.for('globalKey');
  const globalSym2 = Symbol.for('globalKey');
  console.log('Symbol.for 相等:', globalSym1 === globalSym2);   // true（全局注册表中复用）

  // ------------------------------------------------------------
  // 1.7 BigInt 类型
  // ------------------------------------------------------------
  // BigInt 是 ES2020 引入的新类型，用于表示任意精度的整数
  // 可以表示超出 Number 安全整数范围（2^53 - 1）的大整数
  console.log('\n--- 1.7 BigInt 大整数类型 ---');

  // 在整数末尾加 n 即可创建 BigInt
  const bigIntNum = 9007199254740991n; // Number 的最大安全整数
  const biggerNum = 9007199254740992n; // 超过安全整数范围，但 BigInt 可以精确表示

  // 也可以用 BigInt() 函数创建
  const bigIntFromNum = BigInt(100);

  console.log('BigInt 类型:', typeof bigIntNum);   // 'bigint'
  console.log('大整数运算:', biggerNum + 100n);     // 9007199254741092n（精确计算）
  // BigInt 不能与 Number 直接运算，需要显式转换
  // console.log(bigIntNum + 1); // TypeError: Cannot mix BigInt and other types

  // ------------------------------------------------------------
  // 1.8 Object 类型
  // ------------------------------------------------------------
  // Object 是引用类型，其他所有引用类型都继承自 Object
  // 包括：Array, Function, Date, RegExp, Error 等
  console.log('\n--- 1.8 Object 对象类型 ---');

  // 对象字面量语法创建对象（最常用）
  const person = {
    name: '李四',          // 属性名: 属性值
    age: 30,
    'full-name': '李四先生', // 属性名含特殊字符时需要加引号
    greet() {              // ES6 简写的方法定义
      console.log('你好，我是' + this.name);
    }
  };

  // 数组也是对象的一种
  const numbers = [1, 2, 3, 4, 5];

  // 函数也是对象
  function sayHello() {
    console.log('Hello!');
  }

  console.log('对象类型:', typeof person);      // 'object'
  console.log('数组类型:', typeof numbers);     // 'object'（数组也是对象）
  console.log('函数类型:', typeof sayHello);    // 'function'（函数有特殊的 typeof 结果）
  console.log('是否为数组:', Array.isArray(numbers)); // true（正确判断数组的方法）
  console.log('对象属性访问:', person.name);    // '李四'（点语法）
  console.log('属性名含特殊字符:', person['full-name']); // '李四先生'（方括号语法）

  // ============================================================
  // 第二部分：类型转换（Type Conversion）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('二、类型转换');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 2.1 显式类型转换（手动调用转换函数）
  // ------------------------------------------------------------
  console.log('\n--- 2.1 显式类型转换 ---');

  // 转换为 Number
  console.log('Number("123"):', Number('123'));       // 123
  console.log('Number("abc"):', Number('abc'));       // NaN（转换失败）
  console.log('Number(true):', Number(true));         // 1
  console.log('Number(false):', Number(false));       // 0
  console.log('Number(null):', Number(null));         // 0
  console.log('Number(undefined):', Number(undefined)); // NaN
  console.log('parseInt("123px"):', parseInt('123px')); // 123（从左到右解析，遇到非数字停止）
  console.log('parseFloat("3.14em"):', parseFloat('3.14em')); // 3.14

  // 转换为 String
  console.log('String(123):', String(123));           // '123'
  console.log('String(true):', String(true));         // 'true'
  console.log('String(null):', String(null));         // 'null'
  console.log('(123).toString():', (123).toString()); // '123'（注意：数字要加括号再调用方法）

  // 转换为 Boolean
  console.log('Boolean("hello"):', Boolean('hello')); // true
  console.log('Boolean(0):', Boolean(0));             // false

  // ------------------------------------------------------------
  // 2.2 隐式类型转换（JavaScript 自动进行的转换）
  // ------------------------------------------------------------
  console.log('\n--- 2.2 隐式类型转换 ---');

  // 算术运算符中的隐式转换
  console.log('"5" + 3 =', '5' + 3);        // '53'（+ 号遇到字符串会拼接，数字转字符串）
  console.log('"5" - 3 =', '5' - 3);        // 2（- 号会将字符串转为数字）
  console.log('"5" * 2 =', '5' * 2);        // 10
  console.log('true + 1 =', true + 1);      // 2（true 转为 1）
  console.log('false + 1 =', false + 1);    // 1（false 转为 0）

  // 逻辑非运算符中的隐式转换
  console.log('!!"hello" =', !!'hello');    // true（两次取反等价于 Boolean()）
  console.log('!!0 =', !!0);                // false

  // ------------------------------------------------------------
  // 2.3 == vs ===
  // ------------------------------------------------------------
  // == 宽松相等：会进行类型转换后再比较
  // === 严格相等：不进行类型转换，类型不同直接返回 false
  // 最佳实践：优先使用 ===，避免隐式类型转换带来的意外
  console.log('\n--- 2.3 == 与 === 的区别 ---');

  console.log('"5" == 5:', '5' == 5);         // true（字符串转数字后比较）
  console.log('"5" === 5:', '5' === 5);       // false（类型不同）
  console.log('true == 1:', true == 1);       // true（布尔转数字后比较）
  console.log('true === 1:', true === 1);     // false（类型不同）
  console.log('null == undefined:', null == undefined); // true（特殊规则：两者相等）
  console.log('null === undefined:', null === undefined); // false（类型不同）
  console.log('0 == false:', 0 == false);     // true
  console.log('0 === false:', 0 === false);   // false
  console.log('"" == false:', '' == false);   // true
  console.log('"" === false:', '' === false); // false
  console.log('NaN == NaN:', NaN == NaN);     // false（NaN 不等于任何值，包括它自己）
  console.log('NaN === NaN:', NaN === NaN);   // false

  // ============================================================
  // 第三部分：运算符（Operators）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('三、运算符');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 3.1 算术运算符
  // ------------------------------------------------------------
  console.log('\n--- 3.1 算术运算符 ---');

  let a = 10;
  let b = 3;
  console.log('a + b =', a + b);   // 13（加法）
  console.log('a - b =', a - b);   // 7（减法）
  console.log('a * b =', a * b);   // 30（乘法）
  console.log('a / b =', a / b);   // 3.333...（除法）
  console.log('a % b =', a % b);   // 1（取模/求余）
  console.log('a ** b =', a ** b); // 1000（幂运算，ES7 新增，相当于 Math.pow(a, b)）

  // 自增自减运算符
  let counter = 0;
  console.log('counter++ =', counter++); // 0（先返回原值，再加 1）
  console.log('counter =', counter);     // 1
  console.log('++counter =', ++counter); // 2（先加 1，再返回新值）
  console.log('counter-- =', counter--); // 2（先返回原值，再减 1）
  console.log('--counter =', --counter); // 0（先减 1，再返回新值）

  // ------------------------------------------------------------
  // 3.2 赋值运算符
  // ------------------------------------------------------------
  console.log('\n--- 3.2 赋值运算符 ---');

  let x = 10;       // 简单赋值
  console.log('x = 10:', x);
  x += 5;           // 等价于 x = x + 5
  console.log('x += 5:', x); // 15
  x -= 3;           // 等价于 x = x - 3
  console.log('x -= 3:', x); // 12
  x *= 2;           // 等价于 x = x * 2
  console.log('x *= 2:', x); // 24
  x /= 4;           // 等价于 x = x / 4
  console.log('x /= 4:', x); // 6
  x %= 4;           // 等价于 x = x % 4
  console.log('x %= 4:', x); // 2
  x **= 3;          // 等价于 x = x ** 3
  console.log('x **= 3:', x); // 8

  // ------------------------------------------------------------
  // 3.3 比较运算符
  // ------------------------------------------------------------
  console.log('\n--- 3.3 比较运算符 ---');

  console.log('5 > 3:', 5 > 3);       // true（大于）
  console.log('5 < 3:', 5 < 3);       // false（小于）
  console.log('5 >= 5:', 5 >= 5);     // true（大于等于）
  console.log('5 <= 5:', 5 <= 5);     // true（小于等于）
  console.log('5 == "5":', 5 == '5'); // true（宽松相等）
  console.log('5 === "5":', 5 === '5'); // false（严格相等）
  console.log('5 != "5":', 5 != '5'); // false（宽松不等）
  console.log('5 !== "5":', 5 !== '5'); // true（严格不等）

  // ------------------------------------------------------------
  // 3.4 逻辑运算符
  // ------------------------------------------------------------
  console.log('\n--- 3.4 逻辑运算符 ---');

  // 逻辑与 &&：两边都为 true 才返回 true
  // 短路特性：如果左边为 false，直接返回左边的值，不执行右边
  console.log('true && true:', true && true);       // true
  console.log('true && false:', true && false);     // false
  console.log('0 && "hello":', 0 && 'hello');       // 0（短路：左边为假，返回左边）

  // 逻辑或 ||：有一边为 true 就返回 true
  // 短路特性：如果左边为 true，直接返回左边的值，不执行右边
  console.log('true || false:', true || false);     // true
  console.log('false || false:', false || false);   // false
  console.log('"hello" || 0:', 'hello' || 0);       // 'hello'（短路：左边为真，返回左边）

  // 逻辑非 !：取反
  console.log('!true:', !true);                     // false
  console.log('!0:', !0);                           // true

  // 空值合并运算符 ??（ES2020 新增）
  // 只有当左边为 null 或 undefined 时，才返回右边的值
  // 与 || 的区别：|| 会把 0、''、false 等假值也当作左边无效
  const value1 = 0 ?? '默认值';     // 0（0 不是 null/undefined，返回左边）
  const value2 = 0 || '默认值';     // '默认值'（0 是假值，返回右边）
  const value3 = null ?? '默认值';  // '默认值'（null 触发空值合并）
  console.log('0 ?? "默认值":', value1);
  console.log('0 || "默认值":', value2);
  console.log('null ?? "默认值":', value3);

  // 可选链运算符 ?.（ES2020 新增）
  // 安全地访问深层嵌套属性，属性不存在时返回 undefined 而不是报错
  const user = { profile: { name: '王五' } };
  console.log('user.profile.name:', user?.profile?.name);    // '王五'
  console.log('user.address?.city:', user?.address?.city);   // undefined（不会报错）
  // 可选链也可以用于函数调用：func?.()
  // 可选链也可以用于数组：arr?.[index]

  // ------------------------------------------------------------
  // 3.5 三元运算符
  // ------------------------------------------------------------
  console.log('\n--- 3.5 三元运算符 ---');

  const score = 85;
  // 语法：条件 ? 表达式1 : 表达式2
  // 条件为真时执行表达式1，为假时执行表达式2
  const result = score >= 60 ? '及格' : '不及格';
  console.log('分数 85:', result); // '及格'

  // 三元运算符可以嵌套（但可读性会下降，需谨慎使用）
  const grade = score >= 90 ? '优秀' : score >= 80 ? '良好' : score >= 60 ? '及格' : '不及格';
  console.log('等级:', grade); // '良好'

  // ------------------------------------------------------------
  // 3.6 位运算符
  // ------------------------------------------------------------
  console.log('\n--- 3.6 位运算符（简单了解） ---');

  // 位运算符将操作数视为 32 位二进制数进行运算
  console.log('5 & 3 =', 5 & 3);     // 1（按位与：0101 & 0011 = 0001）
  console.log('5 | 3 =', 5 | 3);     // 7（按位或：0101 | 0011 = 0111）
  console.log('5 ^ 3 =', 5 ^ 3);     // 6（按位异或：0101 ^ 0011 = 0110）
  console.log('~5 =', ~5);           // -6（按位取反：~x = -(x+1)）
  console.log('5 << 1 =', 5 << 1);   // 10（左移：相当于乘以 2）
  console.log('5 >> 1 =', 5 >> 1);   // 2（右移：相当于除以 2 取整）

  // ============================================================
  // 第四部分：控制流（Control Flow）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('四、控制流');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 4.1 if...else 条件语句
  // ------------------------------------------------------------
  console.log('\n--- 4.1 if...else 条件语句 ---');

  const temperature = 28;
  if (temperature >= 30) {
    console.log('天气很热，注意防暑降温');
  } else if (temperature >= 20) {
    console.log('天气宜人，适合外出活动');  // 这条会执行
  } else if (temperature >= 10) {
    console.log('天气较凉，注意添衣');
  } else {
    console.log('天气寒冷，注意保暖');
  }

  // ------------------------------------------------------------
  // 4.2 switch 分支语句
  // ------------------------------------------------------------
  console.log('\n--- 4.2 switch 分支语句 ---');

  const day = 3; // 0=周日, 1=周一, ..., 6=周六
  switch (day) {
    case 0:
      console.log('今天是星期日');
      break; // break 用于跳出 switch，否则会继续执行后面的 case（穿透）
    case 1:
      console.log('今天是星期一');
      break;
    case 2:
      console.log('今天是星期二');
      break;
    case 3:
      console.log('今天是星期三'); // 匹配到这里
      break;
    case 4:
      console.log('今天是星期四');
      break;
    case 5:
      console.log('今天是星期五');
      break;
    case 6:
      console.log('今天是星期六');
      break;
    default: // 所有 case 都不匹配时执行
      console.log('无效的日期');
  }

  // switch 的穿透特性（多个 case 共用同一段代码）
  const fruit = '苹果';
  switch (fruit) {
    case '苹果':
    case '梨':
    case '香蕉':
      console.log('这是常见的水果'); // 苹果会匹配到这里
      break;
    case '榴莲':
      console.log('这是热带水果');
      break;
    default:
      console.log('未知水果');
  }

  // ------------------------------------------------------------
  // 4.3 for 循环
  // ------------------------------------------------------------
  console.log('\n--- 4.3 for 循环 ---');

  // 基本 for 循环：初始化; 条件; 更新
  console.log('基本 for 循环（1到5）：');
  for (let i = 1; i <= 5; i++) {
    console.log('  i =', i);
  }

  // for...in 循环：遍历对象的可枚举属性（键）
  const personInfo = { name: '赵六', age: 28, city: '北京' };
  console.log('for...in 遍历对象：');
  for (const key in personInfo) {
    console.log(`  ${key}: ${personInfo[key]}`);
  }

  // for...of 循环（ES6 新增）：遍历可迭代对象的值（数组、字符串、Set、Map 等）
  const fruits = ['苹果', '香蕉', '橙子'];
  console.log('for...of 遍历数组：');
  for (const fruit of fruits) {
    console.log('  ', fruit);
  }

  // ------------------------------------------------------------
  // 4.4 while 和 do...while 循环
  // ------------------------------------------------------------
  console.log('\n--- 4.4 while 和 do...while 循环 ---');

  // while 循环：先判断条件，再执行循环体
  let whileCount = 0;
  console.log('while 循环：');
  while (whileCount < 3) {
    console.log('  whileCount =', whileCount);
    whileCount++;
  }

  // do...while 循环：先执行循环体，再判断条件（至少执行一次）
  let doWhileCount = 0;
  console.log('do...while 循环：');
  do {
    console.log('  doWhileCount =', doWhileCount);
    doWhileCount++;
  } while (doWhileCount < 3);

  // ------------------------------------------------------------
  // 4.5 break 和 continue
  // ------------------------------------------------------------
  console.log('\n--- 4.5 break 和 continue ---');

  // break：立即跳出整个循环
  console.log('break 示例（遇到3跳出）：');
  for (let i = 1; i <= 5; i++) {
    if (i === 3) {
      break; // 当 i 等于 3 时，跳出循环
    }
    console.log('  i =', i);
  }

  // continue：跳过当前循环的剩余部分，继续下一次循环
  console.log('continue 示例（跳过3）：');
  for (let i = 1; i <= 5; i++) {
    if (i === 3) {
      continue; // 当 i 等于 3 时，跳过本次循环
    }
    console.log('  i =', i);
  }

  // ============================================================
  // 第五部分：var / let / const 的区别与作用域
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('五、var / let / const 的区别与作用域');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 5.1 作用域的概念
  // ------------------------------------------------------------
  // - 全局作用域：在代码任何地方都能访问
  // - 函数作用域：在函数内部才能访问
  // - 块级作用域：在代码块（{}）内部才能访问（ES6 引入）
  console.log('\n--- 5.1 作用域演示 ---');

  // 全局变量：在函数外部声明，整个脚本都能访问
  const globalVar = '我是全局变量';

  function scopeDemo() {
    // 函数作用域变量：在函数内部声明，只能在函数内部访问
    const functionVar = '我是函数作用域变量';
    console.log('函数内部访问 globalVar:', globalVar);      // 可以访问
    console.log('函数内部访问 functionVar:', functionVar);  // 可以访问

    // 块级作用域（let 和 const 支持，var 不支持）
    if (true) {
      let blockLet = '我是块级作用域变量（let）';
      var blockVar = '我是 var 声明的变量（函数作用域）';
      console.log('块内访问 blockLet:', blockLet);         // 可以访问
    }
    // console.log(blockLet); // 报错：blockLet is not defined（let 是块级作用域）
    console.log('块外访问 blockVar:', blockVar);           // 可以访问（var 是函数作用域）
  }
  scopeDemo();

  // ------------------------------------------------------------
  // 5.2 var 的特性（不推荐使用）
  // ------------------------------------------------------------
  // - 函数作用域（不是块级作用域）
  // - 变量提升（hoisting）：声明会被提升到函数/全局顶部，但赋值不会提升
  // - 可以重复声明
  // - 全局声明的 var 变量会挂载到 window 对象上（浏览器环境）
  console.log('\n--- 5.2 var 的特性 ---');

  // 变量提升演示
  console.log('var 变量提升：hoistedVar =', hoistedVar); // undefined（声明被提升，赋值未提升）
  var hoistedVar = '我是被提升的变量';
  console.log('赋值后：hoistedVar =', hoistedVar);        // '我是被提升的变量'

  // var 可以重复声明（这是不好的特性，容易导致 bug）
  var repeatVar = '第一次声明';
  var repeatVar = '第二次声明'; // 不会报错，直接覆盖
  console.log('var 重复声明后：', repeatVar); // '第二次声明'

  // ------------------------------------------------------------
  // 5.3 let 的特性（推荐用于可变变量）
  // ------------------------------------------------------------
  // - 块级作用域
  // - 暂时性死区（TDZ）：在声明之前访问会报错
  // - 不可重复声明
  // - 不会挂载到 window 对象上
  console.log('\n--- 5.3 let 的特性 ---');

  // console.log(letVar); // 报错：Cannot access 'letVar' before initialization
  let letVar = 'let 声明的变量';

  // let 不能重复声明
  // let letVar = '重复声明'; // 报错：Identifier 'letVar' has already been declared

  // 经典面试题：var 和 let 在循环中的区别
  console.log('var 循环的闭包问题（全部输出 3）：');
  var varArr = [];
  for (var i = 0; i < 3; i++) {
    varArr.push(function () {
      console.log('  i =', i); // 由于 var 没有块级作用域，所有函数共享同一个 i
    });
  }
  varArr.forEach(fn => fn()); // 输出 3, 3, 3（循环结束后 i 已经是 3）

  console.log('let 循环解决闭包问题（输出 0, 1, 2）：');
  const letArr = [];
  for (let j = 0; j < 3; j++) {
    letArr.push(function () {
      console.log('  j =', j); // let 有块级作用域，每次循环创建一个新的 j
    });
  }
  letArr.forEach(fn => fn()); // 输出 0, 1, 2

  // ------------------------------------------------------------
  // 5.4 const 的特性（推荐优先使用）
  // ------------------------------------------------------------
  // - 块级作用域
  // - 声明时必须赋值
  // - 声明后不能重新赋值（注意：对象/数组的内容可以修改）
  // - 不可重复声明
  console.log('\n--- 5.4 const 的特性 ---');

  const constVar = 'const 声明的常量';
  // constVar = '重新赋值'; // 报错：Assignment to constant variable

  // const 声明的对象，其属性可以修改（因为 const 保证的是引用不变，不是内容不变）
  const constObj = { name: '孙七' };
  constObj.name = '孙七改'; // 可以修改属性
  constObj.age = 30;       // 可以添加属性
  console.log('const 对象修改属性后：', constObj);

  // const 声明的数组，其元素可以修改
  const constArr = [1, 2, 3];
  constArr.push(4); // 可以添加元素
  constArr[0] = 10; // 可以修改元素
  console.log('const 数组修改后：', constArr);

  // 总结：优先使用 const，需要改变变量值时用 let，尽量不要用 var

  // ============================================================
  // 第六部分：解构赋值（Destructuring）
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('六、解构赋值');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 6.1 数组解构
  // ------------------------------------------------------------
  // 数组解构是按照位置顺序来赋值的
  console.log('\n--- 6.1 数组解构 ---');

  // 基本解构
  const [first, second, third] = ['苹果', '香蕉', '橙子'];
  console.log('first:', first);     // '苹果'
  console.log('second:', second);   // '香蕉'
  console.log('third:', third);     // '橙子'

  // 跳过某些元素
  const [, , thirdFruit] = ['苹果', '香蕉', '橙子'];
  console.log('thirdFruit:', thirdFruit); // '橙子'

  // 默认值：当解构值为 undefined 时使用默认值
  const [a1 = '默认A', b1 = '默认B'] = ['已定义'];
  console.log('a1:', a1); // '已定义'
  console.log('b1:', b1); // '默认B'

  // 剩余参数：使用 ... 收集剩余元素到一个新数组
  const [head, ...tail] = [1, 2, 3, 4, 5];
  console.log('head:', head);   // 1
  console.log('tail:', tail);   // [2, 3, 4, 5]

  // 交换变量（经典应用，无需临时变量）
  let x1 = 10, y1 = 20;
  [x1, y1] = [y1, x1];
  console.log('交换后 x1:', x1, 'y1:', y1); // x1=20, y1=10

  // ------------------------------------------------------------
  // 6.2 对象解构
  // ------------------------------------------------------------
  // 对象解构是按照属性名来匹配的，与顺序无关
  console.log('\n--- 6.2 对象解构 ---');

  // 基本解构
  const student = { sname: '周八', sage: 22, smajor: '计算机' };
  const { sname, sage } = student; // 从对象中提取同名属性
  console.log('sname:', sname); // '周八'
  console.log('sage:', sage);   // 22

  // 重命名：属性名: 新变量名
  const { sname: studentName, sage: studentAge } = student;
  console.log('studentName:', studentName); // '周八'
  console.log('studentAge:', studentAge);   // 22

  // 默认值
  const { sname: n = '匿名', sgender: g = '未知' } = student;
  console.log('n:', n); // '周八'
  console.log('g:', g); // '未知'（student 没有 sgender 属性，使用默认值）

  // 嵌套解构
  const company = {
    cname: '科技公司',
    location: {
      city: '上海',
      district: '浦东新区'
    }
  };
  const { location: { city, district } } = company;
  console.log('city:', city);         // '上海'
  console.log('district:', district); // '浦东新区'

  // 剩余参数：收集剩余属性到一个新对象
  const personObj = { pname: '吴九', page: 30, pcity: '广州', pjob: '工程师' };
  const { pname, page, ...restInfo } = personObj;
  console.log('pname:', pname);         // '吴九'
  console.log('page:', page);           // 30
  console.log('restInfo:', restInfo);   // { pcity: '广州', pjob: '工程师' }

  // 函数参数解构（非常常用）
  function printUser({ username, age: userAge = 18 }) {
    // 直接在参数中解构，并重命名 age 为 userAge，设置默认值 18
    console.log(`用户：${username}，年龄：${userAge}`);
  }
  printUser({ username: '郑十', age: 25 }); // '用户：郑十，年龄：25'
  printUser({ username: '冯十一' });          // '用户：冯十一，年龄：18'

  // ============================================================
  // 第七部分：模板字符串、扩展运算符、剩余参数
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('七、模板字符串、扩展运算符、剩余参数');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 7.1 模板字符串（Template Literals）
  // ------------------------------------------------------------
  // 使用反引号 `` 包裹字符串
  console.log('\n--- 7.1 模板字符串 ---');

  const userName = '陈十二';
  const userAge = 28;

  // 1. 支持变量插值（${表达式}）
  const intro = `我是${userName}，今年${userAge}岁。`;
  console.log('变量插值：', intro);

  // 2. 支持表达式计算
  const mathStr = `1 + 1 = ${1 + 1}，2 的 3 次方 = ${2 ** 3}`;
  console.log('表达式计算：', mathStr);

  // 3. 支持多行字符串（保留换行和空格）
  const multiLineStr = `
    第一行
      第二行（缩进）
    第三行
  `;
  console.log('多行字符串：', multiLineStr);

  // 4. 标签模板（Tagged Templates）：高级用法，自定义模板字符串的处理方式
  function tag(strings, ...values) {
    // strings 是模板字符串中被 ${} 分割的字符串数组
    // values 是各个 ${} 表达式的值
    console.log('strings:', strings); // ['姓名：', '，年龄：', '']
    console.log('values:', values);   // ['陈十二', 28]
    return strings.reduce((result, str, i) => {
      return result + str + (values[i] !== undefined ? `[${values[i]}]` : '');
    }, '');
  }
  const taggedStr = tag`姓名：${userName}，年龄：${userAge}`;
  console.log('标签模板结果：', taggedStr); // '姓名：[陈十二]，年龄：[28]'

  // ------------------------------------------------------------
  // 7.2 扩展运算符（Spread Operator）...
  // ------------------------------------------------------------
  // 扩展运算符可以将可迭代对象（数组、字符串、对象等）展开为单独的元素
  console.log('\n--- 7.2 扩展运算符 ---');

  // 数组展开
  const arr1 = [1, 2, 3];
  const arr2 = [4, 5, 6];
  const combinedArr = [...arr1, ...arr2]; // 合并数组
  console.log('数组合并：', combinedArr); // [1, 2, 3, 4, 5, 6]

  // 复制数组（浅拷贝）
  const copyArr = [...arr1];
  console.log('数组拷贝：', copyArr);      // [1, 2, 3]
  console.log('是否同一引用：', copyArr === arr1); // false（新数组）

  // 字符串展开为字符数组
  const str = 'hello';
  const charArr = [...str];
  console.log('字符串展开：', charArr);     // ['h', 'e', 'l', 'l', 'o']

  // 对象展开（ES2018 新增）
  const obj1 = { a: 1, b: 2 };
  const obj2 = { c: 3, d: 4 };
  const combinedObj = { ...obj1, ...obj2 }; // 合并对象
  console.log('对象合并：', combinedObj);   // { a: 1, b: 2, c: 3, d: 4 }

  // 对象展开时，后面的属性会覆盖前面的同名属性
  const overrideObj = { ...obj1, b: 99, c: 3 };
  console.log('属性覆盖：', overrideObj);   // { a: 1, b: 99, c: 3 }

  // 函数调用时展开数组作为参数
  function sum(a, b, c) {
    return a + b + c;
  }
  const nums = [10, 20, 30];
  console.log('函数参数展开：', sum(...nums)); // 60（相当于 sum(10, 20, 30)）

  // ------------------------------------------------------------
  // 7.3 剩余参数（Rest Parameters）...
  // ------------------------------------------------------------
  // 剩余参数用于函数参数中，将剩余的参数收集为一个数组
  // 注意：剩余参数必须放在参数列表的最后
  console.log('\n--- 7.3 剩余参数 ---');

  // 计算所有传入参数的和
  function sumAll(...args) {
    // args 是一个数组，包含所有传入的参数
    return args.reduce((total, num) => total + num, 0);
  }
  console.log('sumAll(1, 2, 3):', sumAll(1, 2, 3));       // 6
  console.log('sumAll(1, 2, 3, 4, 5):', sumAll(1, 2, 3, 4, 5)); // 15

  // 前几个参数单独命名，剩余的收集为数组
  function greet(greeting, ...names) {
    return `${greeting}，${names.join('、')}！`;
  }
  console.log('greet("你好", "小明", "小红", "小刚"):', greet('你好', '小明', '小红', '小刚'));

  // 剩余参数与 arguments 对象的区别：
  // - arguments 是类数组对象，不是真正的数组，不能直接使用数组方法
  // - 剩余参数是真正的数组，可以直接使用 map、filter、reduce 等方法
  // - 箭头函数没有 arguments 对象，但可以使用剩余参数

  // ============================================================
  // 演示结束
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('基础语法 - 数据类型与运算符 演示结束');
  console.log('='.repeat(60));
};

// 如果直接运行此文件（不是被 require），则执行演示函数
// require.main === module 是判断模块是否为入口文件的标准方法
if (require.main === module) {
  module.exports();
}
