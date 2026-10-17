/**
 * ============================================================
 * 文件名：array_methods.js
 * 描述：JavaScript 数组高阶方法详解 - map / filter / reduce / find / some / every / sort
 * 作者：JavaScript 学习项目
 * 版本：1.0.0
 * ============================================================
 *
 * 本文件系统讲解 JavaScript 数组的高阶方法，包括：
 * 1. map - 数组映射（转换每个元素）
 * 2. filter - 数组过滤（筛选符合条件的元素）
 * 3. reduce - 数组归约（累计计算）
 * 4. find / findIndex - 查找元素
 * 5. some / every - 条件判断
 * 6. sort - 数组排序
 * 7. 链式调用
 * 8. 展开运算符与数组操作
 *
 * 运行方式：node advanced/array_methods.js
 */

'use strict';

module.exports = function demoArrayMethods() {
  console.log('='.repeat(60));
  console.log('一、数组高阶方法详解');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 1.1 map - 数组映射
  // ------------------------------------------------------------
  // map() 方法创建一个新数组，其结果是对原数组每个元素调用回调函数的返回值
  // 语法：arr.map(callback(element, index, array), thisArg)
  // 特点：不会改变原数组，返回新数组，新数组长度和原数组相同
  console.log('\n--- 1.1 map 数组映射 ---');

  const numbers = [1, 2, 3, 4, 5];

  // 将每个数字乘以 2
  const doubled = numbers.map(function (num) {
    return num * 2;
  });
  console.log('  原数组:', numbers);        // [1, 2, 3, 4, 5]（原数组不变）
  console.log('  每个数乘 2:', doubled);    // [2, 4, 6, 8, 10]

  // 箭头函数简写
  const squared = numbers.map(num => num ** 2);
  console.log('  每个数平方:', squared);    // [1, 4, 9, 16, 25]

  // 利用 index 参数
  const withIndex = numbers.map((num, index) => `第${index}个: ${num}`);
  console.log('  带索引:', withIndex);

  // 对象数组的 map 操作
  const users = [
    { id: 1, name: '张三', age: 20 },
    { id: 2, name: '李四', age: 25 },
    { id: 3, name: '王五', age: 30 }
  ];

  // 提取所有用户名
  const names = users.map(user => user.name);
  console.log('  提取用户名:', names); // ['张三', '李四', '王五']

  // 转换数据结构
  const userList = users.map(user => ({
    userId: user.id,
    userName: user.name.toUpperCase(),
    isAdult: user.age >= 18
  }));
  console.log('  转换数据结构:', JSON.stringify(userList));

  // ------------------------------------------------------------
  // 1.2 filter - 数组过滤
  // ------------------------------------------------------------
  // filter() 方法创建一个新数组，包含所有通过回调函数测试的元素
  // 语法：arr.filter(callback(element, index, array), thisArg)
  // 特点：不会改变原数组，返回新数组，新数组长度 <= 原数组长度
  console.log('\n--- 1.2 filter 数组过滤 ---');

  // 过滤出偶数
  const evens = numbers.filter(num => num % 2 === 0);
  console.log('  偶数:', evens); // [2, 4]

  // 过滤出大于 3 的数
  const greaterThan3 = numbers.filter(num => num > 3);
  console.log('  大于 3:', greaterThan3); // [4, 5]

  // 过滤出成年用户
  const adults = users.filter(user => user.age >= 25);
  console.log('  成年用户（>=25）:', adults.map(u => u.name));

  // 数组去重（经典应用）
  const duplicateArr = [1, 2, 2, 3, 4, 4, 5, 5, 5];
  const uniqueArr = duplicateArr.filter((item, index, arr) => {
    // indexOf 返回元素第一次出现的索引，如果和当前 index 相同，说明是第一次出现
    return arr.indexOf(item) === index;
  });
  console.log('  去重前:', duplicateArr);
  console.log('  去重后:', uniqueArr);
  // 更简单的去重方式：[...new Set(duplicateArr)]

  // 过滤掉 falsy 值
  const mixedArr = [0, 1, '', 'hello', false, true, null, undefined, NaN, {}, []];
  const truthyArr = mixedArr.filter(Boolean);
  console.log('  过滤假值后:', truthyArr); // [1, 'hello', true, {}, []]

  // ------------------------------------------------------------
  // 1.3 reduce - 数组归约
  // ------------------------------------------------------------
  // reduce() 方法对数组中的每个元素执行一个 reducer 函数，将其结果汇总为单个返回值
  // 语法：arr.reduce(callback(accumulator, currentValue, index, array), initialValue)
  // accumulator：累加器，存储上一次回调的返回值
  // currentValue：当前正在处理的元素
  // initialValue：初始值（可选，不传则以第一个元素作为初始值，从第二个元素开始遍历）
  console.log('\n--- 1.3 reduce 数组归约 ---');

  // 数组求和
  const sum = numbers.reduce((acc, cur) => {
    console.log(`  acc=${acc}, cur=${cur}`);
    return acc + cur;
  }, 0);
  console.log('  数组求和:', sum); // 15

  // 数组求最大值
  const max = numbers.reduce((acc, cur) => Math.max(acc, cur), -Infinity);
  console.log('  数组最大值:', max); // 5

  // 统计数组元素出现次数
  const fruits = ['苹果', '香蕉', '苹果', '橙子', '香蕉', '苹果'];
  const countMap = fruits.reduce((acc, fruit) => {
    acc[fruit] = (acc[fruit] || 0) + 1;
    return acc;
  }, {});
  console.log('  水果统计:', countMap); // { 苹果: 3, 香蕉: 2, 橙子: 1 }

  // 二维数组扁平化（展开一维）
  const nestedArr = [[1, 2], [3, 4], [5, 6]];
  const flattened = nestedArr.reduce((acc, cur) => acc.concat(cur), []);
  console.log('  二维数组扁平化:', flattened); // [1, 2, 3, 4, 5, 6]
  // 更简单的方法：nestedArr.flat()

  // 按属性分组
  const people = [
    { name: '张三', city: '北京' },
    { name: '李四', city: '上海' },
    { name: '王五', city: '北京' },
    { name: '赵六', city: '广州' },
    { name: '钱七', city: '上海' }
  ];

  const groupedByCity = people.reduce((acc, person) => {
    const city = person.city;
    // 如果该城市还没有分组，创建一个空数组
    if (!acc[city]) {
      acc[city] = [];
    }
    acc[city].push(person.name);
    return acc;
  }, {});
  console.log('  按城市分组:', JSON.stringify(groupedByCity));

  // reduceRight：从右往左遍历
  const rightSum = numbers.reduceRight((acc, cur) => {
    console.log(`  reduceRight: acc=${acc}, cur=${cur}`);
    return acc + cur;
  }, 0);
  console.log('  reduceRight 求和:', rightSum); // 15（结果一样，但遍历顺序相反）

  // ------------------------------------------------------------
  // 1.4 find / findIndex - 查找元素
  // ------------------------------------------------------------
  // find()：返回第一个满足条件的元素，找不到返回 undefined
  // findIndex()：返回第一个满足条件的元素的索引，找不到返回 -1
  console.log('\n--- 1.4 find / findIndex 查找元素 ---');

  // 找到第一个大于 3 的数
  const firstGreaterThan3 = numbers.find(num => num > 3);
  console.log('  第一个大于 3 的数:', firstGreaterThan3); // 4

  // 找到第一个成年人
  const firstAdult = users.find(user => user.age >= 25);
  console.log('  第一个成年人:', firstAdult ? firstAdult.name : '未找到');

  // 找不存在的元素
  const notFound = numbers.find(num => num > 100);
  console.log('  找不存在的元素:', notFound); // undefined

  // findIndex：找索引
  const indexOfFour = numbers.findIndex(num => num === 4);
  console.log('  数字 4 的索引:', indexOfFour); // 3

  const notFoundIndex = numbers.findIndex(num => num === 100);
  console.log('  不存在元素的索引:', notFoundIndex); // -1

  // findLast / findLastIndex（ES2022 新增，从后往前找）
  // 注意：Node.js 18+ 才支持
  if (typeof numbers.findLast === 'function') {
    const lastEven = numbers.findLast(num => num % 2 === 0);
    console.log('  最后一个偶数:', lastEven); // 4
  } else {
    console.log('  当前环境不支持 findLast（需 Node.js 18+）');
  }

  // ------------------------------------------------------------
  // 1.5 some / every - 条件判断
  // ------------------------------------------------------------
  // some()：测试数组中是否至少有一个元素通过了回调函数的测试，返回布尔值
  // every()：测试数组中是否所有元素都通过了回调函数的测试，返回布尔值
  console.log('\n--- 1.5 some / every 条件判断 ---');

  // some：只要有一个满足就返回 true
  const hasEven = numbers.some(num => num % 2 === 0);
  console.log('  是否有偶数:', hasEven); // true

  const hasNegative = numbers.some(num => num < 0);
  console.log('  是否有负数:', hasNegative); // false

  // every：所有都满足才返回 true
  const allPositive = numbers.every(num => num > 0);
  console.log('  是否全为正数:', allPositive); // true

  const allGreaterThan3 = numbers.every(num => num > 3);
  console.log('  是否都大于 3:', allGreaterThan3); // false

  // 实际应用：检查所有用户是否成年
  const allAdults = users.every(user => user.age >= 18);
  console.log('  是否所有用户都成年:', allAdults); // true

  // some 和 every 都是短路的：
  // some 找到第一个满足条件的就停止遍历
  // every 找到第一个不满足条件的就停止遍历

  // ------------------------------------------------------------
  // 1.6 sort - 数组排序
  // ------------------------------------------------------------
  // sort() 方法对数组元素进行排序，并返回排序后的数组（会改变原数组！）
  // 语法：arr.sort(compareFunction)
  // compareFunction 返回值 < 0，则 a 排在 b 前面
  // compareFunction 返回值 > 0，则 a 排在 b 后面
  // compareFunction 返回值 = 0，相对位置不变
  console.log('\n--- 1.6 sort 数组排序 ---');

  // 默认排序：按字符串 Unicode 编码排序（不是按数值大小！）
  const strArr = ['banana', 'apple', 'cherry', 'date'];
  const sortedStr = [...strArr].sort(); // 用展开运算符复制后再排序，避免修改原数组
  console.log('  字符串排序:', sortedStr); // ['apple', 'banana', 'cherry', 'date']

  // 数字排序的坑：默认按字符串排序
  const numArr = [10, 2, 35, 4, 100];
  const wrongSort = [...numArr].sort();
  console.log('  默认数字排序（错误）:', wrongSort); // [10, 100, 2, 35, 4]（按字符串排序）

  // 正确的数字升序排序
  const ascendingSort = [...numArr].sort((a, b) => a - b);
  console.log('  数字升序排序:', ascendingSort); // [2, 4, 10, 35, 100]

  // 数字降序排序
  const descendingSort = [...numArr].sort((a, b) => b - a);
  console.log('  数字降序排序:', descendingSort); // [100, 35, 10, 4, 2]

  // 对象数组按属性排序
  const sortedByAge = [...users].sort((a, b) => a.age - b.age);
  console.log('  按年龄排序:', sortedByAge.map(u => `${u.name}(${u.age})`));

  // 按字符串属性排序（中文需要使用 localeCompare）
  const sortedByName = [...users].sort((a, b) => a.name.localeCompare(b.name, 'zh-CN'));
  console.log('  按姓名排序:', sortedByName.map(u => u.name));

  // 多级排序：先按年龄，年龄相同按姓名
  const multiSort = [...users, { id: 4, name: '孙七', age: 25 }].sort((a, b) => {
    if (a.age !== b.age) {
      return a.age - b.age; // 先按年龄升序
    }
    return a.name.localeCompare(b.name, 'zh-CN'); // 年龄相同按姓名排序
  });
  console.log('  多级排序:', multiSort.map(u => `${u.name}(${u.age})`));

  // ------------------------------------------------------------
  // 1.7 其他常用数组方法
  // ------------------------------------------------------------
  console.log('\n--- 1.7 其他常用数组方法 ---');

  // forEach：遍历数组，没有返回值
  console.log('  forEach 遍历:');
  numbers.forEach((num, index) => {
    console.log(`    索引 ${index}: ${num}`);
  });

  // includes：判断数组是否包含某个元素（ES7 新增）
  console.log('  includes(3):', numbers.includes(3));   // true
  console.log('  includes(10):', numbers.includes(10)); // false

  // indexOf / lastIndexOf：查找元素索引
  console.log('  indexOf(3):', numbers.indexOf(3));     // 2
  console.log('  lastIndexOf(3):', numbers.lastIndexOf(3)); // 2

  // flat：数组扁平化（ES2019 新增）
  const deepArr = [1, [2, [3, [4, 5]]]];
  console.log('  flat():', deepArr.flat());      // [1, 2, [3, [4, 5]]]（默认展开一层）
  console.log('  flat(2):', deepArr.flat(2));    // [1, 2, 3, [4, 5]]
  console.log('  flat(Infinity):', deepArr.flat(Infinity)); // [1, 2, 3, 4, 5]（全部展开）

  // flatMap：先 map 再 flat（展开一层）
  const flatMapResult = numbers.flatMap(num => [num, num * 2]);
  console.log('  flatMap:', flatMapResult); // [1, 2, 2, 4, 3, 6, 4, 8, 5, 10]

  // ============================================================
  // 第二部分：链式调用
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('二、数组方法链式调用');
  console.log('='.repeat(60));

  // 数组高阶方法的返回值仍然是数组（或可以转为数组的值），因此可以链式调用
  // 这是函数式编程的典型风格，代码简洁清晰
  console.log('\n--- 2.1 链式调用示例 ---');

  // 示例：处理员工数据
  const employees = [
    { name: '张三', department: '技术部', salary: 8000, active: true },
    { name: '李四', department: '市场部', salary: 6000, active: true },
    { name: '王五', department: '技术部', salary: 12000, active: true },
    { name: '赵六', department: '技术部', salary: 5000, active: false },
    { name: '钱七', department: '市场部', salary: 15000, active: true },
    { name: '孙八', department: '技术部', salary: 10000, active: true }
  ];

  // 需求：找出技术部在职员工，计算他们的薪资总和
  const techActiveTotalSalary = employees
    .filter(emp => emp.department === '技术部')  // 筛选技术部
    .filter(emp => emp.active)                    // 筛选在职
    .map(emp => emp.salary)                       // 提取薪资
    .reduce((sum, salary) => sum + salary, 0);    // 求和
  console.log('  技术部在职员工薪资总和:', techActiveTotalSalary); // 30000

  // 需求：获取技术部薪资大于 8000 的员工姓名，按薪资从高到低排序
  const topTechEmployees = employees
    .filter(emp => emp.department === '技术部' && emp.active)
    .filter(emp => emp.salary > 8000)
    .sort((a, b) => b.salary - a.salary)          // 按薪资降序
    .map(emp => `${emp.name}(${emp.salary})`);    // 格式化输出
  console.log('  技术部高薪员工（降序）:', topTechEmployees);

  // 需求：按部门分组，计算每个部门的平均薪资
  const deptAvgSalary = Object.entries(
    employees
      .filter(emp => emp.active)
      .reduce((acc, emp) => {
        if (!acc[emp.department]) {
          acc[emp.department] = { total: 0, count: 0 };
        }
        acc[emp.department].total += emp.salary;
        acc[emp.department].count++;
        return acc;
      }, {})
  ).map(([dept, data]) => ({
    department: dept,
    average: Math.round(data.total / data.count)
  }));
  console.log('  各部门平均薪资:', deptAvgSalary);

  // ============================================================
  // 第三部分：展开运算符与数组操作
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('三、展开运算符与数组操作');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 3.1 展开运算符在数组中的应用
  // ------------------------------------------------------------
  console.log('\n--- 3.1 展开运算符的应用 ---');

  const arr1 = [1, 2, 3];
  const arr2 = [4, 5, 6];

  // 合并数组
  const combined = [...arr1, ...arr2];
  console.log('  合并数组:', combined); // [1, 2, 3, 4, 5, 6]

  // 数组浅拷贝
  const copy = [...arr1];
  console.log('  数组拷贝:', copy);         // [1, 2, 3]
  console.log('  是否同一引用:', copy === arr1); // false

  // 在指定位置插入元素
  const inserted = [...arr1.slice(0, 2), 'a', 'b', ...arr1.slice(2)];
  console.log('  插入元素:', inserted); // [1, 2, 'a', 'b', 3]

  // 将类数组对象转为数组
  function argsToArray(...args) {
    return [...args]; // 剩余参数本身就是数组，这里仅演示
  }
  console.log('  arguments 转数组:', argsToArray(1, 2, 3));

  // 字符串转字符数组
  const str = 'Hello';
  const chars = [...str];
  console.log('  字符串转数组:', chars); // ['H', 'e', 'l', 'l', 'o']

  // 数组去重（Set + 展开运算符）
  const duplicates = [1, 2, 2, 3, 3, 3, 4, 5, 5];
  const unique = [...new Set(duplicates)];
  console.log('  Set 去重:', unique); // [1, 2, 3, 4, 5]

  // ------------------------------------------------------------
  // 3.2 数组解构
  // ------------------------------------------------------------
  console.log('\n--- 3.2 数组解构 ---');

  const colorArr = ['red', 'green', 'blue'];

  // 基本解构
  const [red, green, blue] = colorArr;
  console.log(`  ${red}, ${green}, ${blue}`);

  // 跳过元素
  const [, , blueOnly] = colorArr;
  console.log('  只取第三个:', blueOnly);

  // 默认值
  const [first, second, third, fourth = 'yellow'] = colorArr;
  console.log('  默认值 fourth:', fourth);

  // 剩余元素
  const [head, ...rest] = colorArr;
  console.log('  head:', head);
  console.log('  rest:', rest);

  // 交换变量
  let a = 1, b = 2;
  [a, b] = [b, a];
  console.log('  交换变量: a =', a, ', b =', b);

  // ============================================================
  // 第四部分：数组方法手写实现
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('四、数组方法手写实现（面试常考）');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 4.1 手写 map
  // ------------------------------------------------------------
  console.log('\n--- 4.1 手写 map ---');

  // 在 Array.prototype 上添加自定义 myMap 方法
  Array.prototype.myMap = function (callback, thisArg) {
    // this 指向调用 myMap 的数组
    const result = [];
    // 遍历数组的每个元素
    for (let i = 0; i < this.length; i++) {
      // 确保只处理实际存在的元素（处理稀疏数组）
      if (i in this) {
        // 调用回调函数，绑定 this，传入当前元素、索引、原数组
        result[i] = callback.call(thisArg, this[i], i, this);
      }
    }
    return result;
  };

  const myMapResult = [1, 2, 3].myMap(num => num * 10);
  console.log('  手写 map 结果:', myMapResult); // [10, 20, 30]

  // ------------------------------------------------------------
  // 4.2 手写 filter
  // ------------------------------------------------------------
  console.log('\n--- 4.2 手写 filter ---');

  Array.prototype.myFilter = function (callback, thisArg) {
    const result = [];
    for (let i = 0; i < this.length; i++) {
      if (i in this) {
        // 回调返回 true 时，将元素加入结果数组
        if (callback.call(thisArg, this[i], i, this)) {
          result.push(this[i]);
        }
      }
    }
    return result;
  };

  const myFilterResult = [1, 2, 3, 4, 5].myFilter(num => num > 2);
  console.log('  手写 filter 结果:', myFilterResult); // [3, 4, 5]

  // ------------------------------------------------------------
  // 4.3 手写 reduce
  // ------------------------------------------------------------
  console.log('\n--- 4.3 手写 reduce ---');

  Array.prototype.myReduce = function (callback, initialValue) {
    let accumulator;
    let startIndex;

    // 判断是否传入初始值
    if (initialValue !== undefined) {
      accumulator = initialValue;
      startIndex = 0;
    } else {
      // 没有初始值，用第一个元素作为初始值，从第二个元素开始遍历
      if (this.length === 0) {
        throw new TypeError('Reduce of empty array with no initial value');
      }
      accumulator = this[0];
      startIndex = 1;
    }

    for (let i = startIndex; i < this.length; i++) {
      if (i in this) {
        accumulator = callback(accumulator, this[i], i, this);
      }
    }
    return accumulator;
  };

  const myReduceResult = [1, 2, 3, 4, 5].myReduce((sum, num) => sum + num, 0);
  console.log('  手写 reduce 结果:', myReduceResult); // 15

  // ============================================================
  // 演示结束
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('数组高阶方法 演示结束');
  console.log('='.repeat(60));
};

if (require.main === module) {
  module.exports();
}
