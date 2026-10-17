/**
 * ============================================================
 * 文件名：oop.js
 * 描述：JavaScript 面向对象编程 - 原型、原型链、构造函数、ES6 Class、继承、多态
 * 作者：JavaScript 学习项目
 * 版本：1.0.0
 * ============================================================
 *
 * 本文件系统讲解 JavaScript 面向对象编程的核心知识，包括：
 * 1. 原型与原型链（prototype / __proto__）
 * 2. 构造函数模式
 * 3. ES6 Class 语法（constructor / extends / super / static）
 * 4. getter / setter 访问器属性
 * 5. 私有字段（#）
 * 6. 继承与多态
 *
 * 运行方式：node oop/oop.js
 */

'use strict';

module.exports = function demoOOP() {
  console.log('='.repeat(60));
  console.log('一、原型与原型链');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 1.1 原型（prototype）的基本概念
  // ------------------------------------------------------------
  // JavaScript 是基于原型的面向对象语言，每个对象都有一个原型对象
  // 对象从原型中继承属性和方法
  //
  // 关键概念：
  // - prototype：函数的属性，指向该函数作为构造函数时创建的实例的原型
  // - __proto__：对象的属性，指向该对象的原型（即构造函数的 prototype）
  // - constructor：原型对象的属性，指回构造函数
  console.log('\n--- 1.1 原型基本概念 ---');

  // 构造函数（首字母大写是约定俗成的命名规范）
  function Animal(name) {
    // 实例属性：每个实例都有自己的副本
    this.name = name;
  }

  // 原型方法：所有实例共享同一份，节省内存
  Animal.prototype.say = function () {
    console.log(`  ${this.name} 发出叫声`);
  };

  // 创建实例
  const dog = new Animal('小狗');
  const cat = new Animal('小猫');

  console.log('  dog.name:', dog.name);
  console.log('  cat.name:', cat.name);
  dog.say();
  cat.say();

  // 验证原型关系
  console.log('  dog.__proto__ === Animal.prototype:', dog.__proto__ === Animal.prototype); // true
  console.log('  Animal.prototype.constructor === Animal:', Animal.prototype.constructor === Animal); // true
  console.log('  dog.constructor === Animal:', dog.constructor === Animal); // true（继承自原型）

  // 原型方法是共享的
  console.log('  dog.say === cat.say:', dog.say === cat.say); // true（共享同一个方法）

  // ------------------------------------------------------------
  // 1.2 原型链（Prototype Chain）
  // ------------------------------------------------------------
  // 当访问对象的属性时，JavaScript 会先在对象本身查找
  // 如果找不到，就去它的原型对象上找
  // 还找不到，就去原型的原型上找，直到找到 Object.prototype
  // Object.prototype 的原型是 null，这就是原型链的终点
  console.log('\n--- 1.2 原型链 ---');

  console.log('  dog.__proto__:', dog.__proto__); // Animal.prototype
  console.log('  dog.__proto__.__proto__:', dog.__proto__.__proto__); // Object.prototype
  console.log('  dog.__proto__.__proto__.__proto__:', dog.__proto__.__proto__.__proto__); // null（原型链终点）

  // 因为原型链的存在，所有对象都能访问 Object.prototype 上的方法
  console.log('  dog.toString():', dog.toString()); // [object Object]
  console.log('  dog.hasOwnProperty("name"):', dog.hasOwnProperty('name')); // true
  console.log('  dog.hasOwnProperty("say"):', dog.hasOwnProperty('say')); // false（say 在原型上）

  // in 运算符和 hasOwnProperty 的区别
  console.log('  "name" in dog:', 'name' in dog);     // true（自身属性）
  console.log('  "say" in dog:', 'say' in dog);       // true（原型属性）
  console.log('  "toString" in dog:', 'toString' in dog); // true（原型链上的属性）

  // ------------------------------------------------------------
  // 1.3 原型的动态性
  // ------------------------------------------------------------
  // 原型的修改会立即反映到所有实例上，即使是已经创建的实例
  console.log('\n--- 1.3 原型的动态性 ---');

  // 给原型添加新方法
  Animal.prototype.eat = function (food) {
    console.log(`  ${this.name} 在吃 ${food}`);
  };

  // 已经创建的实例也能访问新添加的方法
  dog.eat('骨头');
  cat.eat('鱼');

  // 注意：如果用新对象替换整个 prototype，已创建的实例不会受影响
  // Animal.prototype = { ... } 这种方式要谨慎使用

  // ------------------------------------------------------------
  // 1.4 __proto__ vs Object.getPrototypeOf / setPrototypeOf
  // ------------------------------------------------------------
  // __proto__ 是历史遗留属性，虽然大多数浏览器支持，但不推荐直接使用
  // 推荐使用 Object.getPrototypeOf() 和 Object.setPrototypeOf()
  console.log('\n--- 1.4 原型操作方法 ---');

  console.log('  Object.getPrototypeOf(dog) === Animal.prototype:',
    Object.getPrototypeOf(dog) === Animal.prototype); // true

  // 设置原型（谨慎使用，会影响性能）
  const obj = {};
  Object.setPrototypeOf(obj, { greet() { return 'Hello'; } });
  console.log('  obj.greet():', obj.greet()); // Hello

  // Object.create()：以指定对象为原型创建新对象
  const protoObj = { type: '原型对象' };
  const newObj = Object.create(protoObj);
  console.log('  newObj.type:', newObj.type); // '原型对象'（从原型继承）
  console.log('  Object.getPrototypeOf(newObj) === protoObj:',
    Object.getPrototypeOf(newObj) === protoObj); // true

  // ============================================================
  // 第二部分：构造函数模式
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('二、构造函数模式');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 2.1 new 运算符的工作原理
  // ------------------------------------------------------------
  // 使用 new 调用构造函数时，会经历以下 4 个步骤：
  // 1. 创建一个新的空对象
  // 2. 将这个空对象的原型指向构造函数的 prototype
  // 3. 将构造函数的 this 绑定到这个新对象，执行构造函数代码
  // 4. 如果构造函数返回对象，则返回该对象；否则返回新创建的对象
  console.log('\n--- 2.1 new 运算符的工作原理 ---');

  // 手动模拟 new 运算符
  function myNew(constructor, ...args) {
    // 步骤1 & 2：创建新对象，原型指向构造函数的 prototype
    const obj = Object.create(constructor.prototype);
    // 步骤3：绑定 this 并执行构造函数
    const result = constructor.apply(obj, args);
    // 步骤4：判断返回值
    return (typeof result === 'object' && result !== null) ? result : obj;
  }

  function Person(name, age) {
    this.name = name;
    this.age = age;
  }
  Person.prototype.introduce = function () {
    console.log(`  我是${this.name}，今年${this.age}岁`);
  };

  const person = myNew(Person, '张三', 25);
  console.log('  手动 new 的结果：');
  person.introduce();
  console.log('  person instanceof Person:', person instanceof Person); // true

  // ------------------------------------------------------------
  // 2.2 构造函数的返回值
  // ------------------------------------------------------------
  console.log('\n--- 2.2 构造函数的返回值 ---');

  // 构造函数返回原始类型时，会被忽略，仍然返回新对象
  function RetPrimitive(name) {
    this.name = name;
    return 123; // 返回原始值，会被忽略
  }
  const rp = new RetPrimitive('测试');
  console.log('  返回原始值的构造函数结果：', rp.name); // '测试'

  // 构造函数返回对象时，会返回该对象，而不是新对象
  function RetObject(name) {
    this.name = name;
    return { customName: '自定义对象' }; // 返回对象，会替换默认返回值
  }
  const ro = new RetObject('测试');
  console.log('  返回对象的构造函数结果：', ro.customName); // '自定义对象'

  // ------------------------------------------------------------
  // 2.3 instanceof 运算符
  // ------------------------------------------------------------
  // instanceof 用于检测构造函数的 prototype 是否在实例对象的原型链上
  console.log('\n--- 2.3 instanceof 运算符 ---');

  const animal = new Animal('动物');
  console.log('  animal instanceof Animal:', animal instanceof Animal); // true
  console.log('  animal instanceof Object:', animal instanceof Object); // true
  console.log('  [] instanceof Array:', [] instanceof Array); // true
  console.log('  [] instanceof Object:', [] instanceof Object); // true（数组也是对象）

  // 手动实现 instanceof
  function myInstanceOf(obj, constructor) {
    // 获取对象的原型
    let proto = Object.getPrototypeOf(obj);
    // 沿着原型链向上查找
    while (proto !== null) {
      if (proto === constructor.prototype) {
        return true;
      }
      proto = Object.getPrototypeOf(proto);
    }
    return false;
  }
  console.log('  手动实现 instanceof:', myInstanceOf(animal, Animal)); // true
  console.log('  手动实现 instanceof:', myInstanceOf(animal, Object)); // true

  // ============================================================
  // 第三部分：ES6 Class 语法
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('三、ES6 Class 语法');
  console.log('='.repeat(60));

  // ES6 引入了 class 关键字，提供了更接近传统面向对象语言的语法
  // 但本质上还是基于原型的，是语法糖（syntactic sugar）

  // ------------------------------------------------------------
  // 3.1 类的基本定义
  // ------------------------------------------------------------
  console.log('\n--- 3.1 类的基本定义 ---');

  class Student {
    // 构造方法，使用 new 创建实例时自动调用
    constructor(name, grade) {
      // 实例属性
      this.name = name;
      this.grade = grade;
    }

    // 实例方法（定义在原型上）
    study() {
      console.log(`  ${this.name} 正在学习，年级：${this.grade}`);
    }

    // 另一个实例方法
    introduce() {
      console.log(`  大家好，我是${this.name}，读${this.grade}年级`);
    }
  }

  const stu1 = new Student('小明', 3);
  const stu2 = new Student('小红', 5);
  stu1.introduce();
  stu2.study();

  // 验证：类的方法定义在 prototype 上
  console.log('  stu1.study === stu2.study:', stu1.study === stu2.study); // true
  console.log('  Student.prototype.study === stu1.study:',
    Student.prototype.study === stu1.study); // true

  // 注意：类声明不会提升（和函数声明不同）
  // 类内部默认使用严格模式

  // ------------------------------------------------------------
  // 3.2 静态方法（static）
  // ------------------------------------------------------------
  // 静态方法属于类本身，不属于实例，只能通过类名调用
  // 静态方法中的 this 指向类本身
  console.log('\n--- 3.2 静态方法 ---');

  class MathUtil {
    // 静态属性（ES2022 支持）
    static PI = 3.14159;

    // 静态方法
    static add(a, b) {
      return a + b;
    }

    static max(...nums) {
      return Math.max(...nums);
    }

    static showThis() {
      console.log('  静态方法中的 this:', this === MathUtil); // true
    }
  }

  console.log('  MathUtil.add(1, 2):', MathUtil.add(1, 2)); // 3
  console.log('  MathUtil.max(3, 7, 2, 9):', MathUtil.max(3, 7, 2, 9)); // 9
  console.log('  MathUtil.PI:', MathUtil.PI); // 3.14159
  MathUtil.showThis();

  // 静态方法不能通过实例调用
  // const mu = new MathUtil();
  // mu.add(1, 2); // 报错：mu.add is not a function

  // ------------------------------------------------------------
  // 3.3 getter / setter 访问器属性
  // ------------------------------------------------------------
  // getter 和 setter 可以让我们像访问属性一样调用方法
  // 可以在获取/设置属性时进行额外的逻辑处理
  console.log('\n--- 3.3 getter / setter ---');

  class Circle {
    constructor(radius) {
      this._radius = radius; // _ 开头表示私有属性（约定，不是真正的私有）
    }

    // getter：使用 get 关键字，访问时不需要加括号
    get radius() {
      console.log('  获取 radius 属性');
      return this._radius;
    }

    // setter：使用 set 关键字，赋值时自动调用
    set radius(value) {
      console.log('  设置 radius 属性');
      if (value <= 0) {
        throw new Error('半径必须大于 0');
      }
      this._radius = value;
    }

    // 计算属性：面积（只读，只有 getter 没有 setter）
    get area() {
      return Math.PI * this._radius ** 2;
    }
  }

  const circle = new Circle(5);
  console.log('  circle.radius:', circle.radius); // 5（触发 getter）
  console.log('  circle.area:', circle.area.toFixed(2)); // 78.54

  circle.radius = 10; // 触发 setter
  console.log('  修改后 radius:', circle.radius); // 10

  // ------------------------------------------------------------
  // 3.4 私有字段（#）
  // ------------------------------------------------------------
  // ES2022 引入了真正的私有字段，使用 # 前缀
  // 私有字段只能在类内部访问，外部无法访问
  console.log('\n--- 3.4 私有字段 ---');

  class BankAccount {
    // 私有字段，必须在类中声明（声明字段）
    #balance;

    constructor(initialBalance) {
      this.#balance = initialBalance; // 初始化私有字段
    }

    // 公共方法可以访问私有字段
    deposit(amount) {
      if (amount > 0) {
        this.#balance += amount;
        console.log(`  存款 ${amount} 元成功`);
      }
    }

    withdraw(amount) {
      if (amount > 0 && amount <= this.#balance) {
        this.#balance -= amount;
        console.log(`  取款 ${amount} 元成功`);
        return true;
      }
      console.log('  余额不足');
      return false;
    }

    getBalance() {
      return this.#balance;
    }

    // 私有方法（也使用 # 前缀）
    #calculateInterest() {
      return this.#balance * 0.01;
    }

    addInterest() {
      const interest = this.#calculateInterest();
      this.#balance += interest;
      console.log(`  添加利息 ${interest.toFixed(2)} 元`);
    }
  }

  const account = new BankAccount(1000);
  account.deposit(500);
  account.withdraw(200);
  console.log('  余额：', account.getBalance()); // 1300
  account.addInterest();
  console.log('  利息后余额：', account.getBalance()); // 1313

  // 外部无法访问私有字段
  // console.log(account.#balance); // 语法错误：Private field '#balance' must be declared in an enclosing class
  // account.#calculateInterest(); // 同样报错

  // ============================================================
  // 第四部分：继承与多态
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('四、继承与多态');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 4.1 ES5 原型链继承
  // ------------------------------------------------------------
  console.log('\n--- 4.1 ES5 原型链继承 ---');

  // 父类（构造函数 + 原型方法）
  function AnimalES5(name) {
    this.name = name;
    this.type = '动物';
  }
  AnimalES5.prototype.speak = function () {
    console.log(`  ${this.name} 发出声音`);
  };

  // 子类
  function DogES5(name, breed) {
    // 借用父类构造函数（盗用构造函数），继承实例属性
    AnimalES5.call(this, name);
    this.breed = breed; // 子类自己的属性
  }

  // 继承原型方法：将子类的 prototype 指向父类的实例
  DogES5.prototype = Object.create(AnimalES5.prototype);
  // 修正 constructor 指向
  DogES5.prototype.constructor = DogES5;

  // 子类自己的方法
  DogES5.prototype.bark = function () {
    console.log(`  ${this.name}（${this.breed}）汪汪叫`);
  };

  const dogES5 = new DogES5('旺财', '金毛');
  dogES5.speak(); // 继承自父类
  dogES5.bark();  // 子类自己的方法
  console.log('  dogES5 instanceof DogES5:', dogES5 instanceof DogES5); // true
  console.log('  dogES5 instanceof AnimalES5:', dogES5 instanceof AnimalES5); // true

  // ------------------------------------------------------------
  // 4.2 ES6 extends 继承
  // ------------------------------------------------------------
  // 使用 extends 关键字实现继承，语法更简洁清晰
  console.log('\n--- 4.2 ES6 extends 继承 ---');

  // 父类（基类）
  class AnimalES6 {
    constructor(name) {
      this.name = name;
      this.type = '动物';
    }

    speak() {
      console.log(`  ${this.name} 发出声音`);
    }

    eat(food) {
      console.log(`  ${this.name} 吃 ${food}`);
    }
  }

  // 子类（派生类）
  class DogES6 extends AnimalES6 {
    constructor(name, breed) {
      // super() 调用父类的构造函数，必须在使用 this 之前调用
      super(name);
      this.breed = breed;
    }

    // 重写（覆盖）父类的方法
    speak() {
      console.log(`  ${this.name}（${this.breed}）汪汪叫`);
    }

    // 子类自己的方法
    fetch() {
      console.log(`  ${this.name} 去捡球了`);
    }

    // 调用父类的方法
    eat(food) {
      super.eat(food); // 调用父类的 eat 方法
      console.log(`  ${this.name} 吃得很香`);
    }
  }

  const dogES6 = new DogES6('大黄', '柴犬');
  dogES6.speak();  // 调用子类重写的方法
  dogES6.eat('骨头'); // 调用子类方法，内部调用了父类方法
  dogES6.fetch();  // 调用子类自己的方法

  console.log('  dogES6 instanceof DogES6:', dogES6 instanceof DogES6); // true
  console.log('  dogES6 instanceof AnimalES6:', dogES6 instanceof AnimalES6); // true

  // 子类也可以继承静态方法
  class Parent {
    static staticMethod() {
      return '父类静态方法';
    }
  }
  class Child extends Parent { }
  console.log('  子类继承静态方法:', Child.staticMethod()); // '父类静态方法'

  // ------------------------------------------------------------
  // 4.3 super 关键字
  // ------------------------------------------------------------
  // super 有两种用法：
  // 1. super()：作为函数调用，在子类构造函数中调用父类构造函数
  // 2. super.方法名()：作为对象使用，在子类方法中调用父类的方法
  //    在静态方法中，super 指向父类本身；在实例方法中，super 指向父类的 prototype
  console.log('\n--- 4.3 super 关键字详解 ---');

  class Base {
    static staticProp = '父类静态属性';
    static staticMethod() {
      return '父类静态方法';
    }

    instanceMethod() {
      return '父类实例方法';
    }
  }

  class Derived extends Base {
    static testStatic() {
      console.log('  super.staticProp:', super.staticProp); // 访问父类静态属性
      console.log('  super.staticMethod():', super.staticMethod()); // 调用父类静态方法
    }

    testInstance() {
      console.log('  super.instanceMethod():', super.instanceMethod()); // 调用父类实例方法
    }
  }

  Derived.testStatic();
  const derived = new Derived();
  derived.testInstance();

  // ------------------------------------------------------------
  // 4.4 多态
  // ------------------------------------------------------------
  // 多态是指同一操作作用于不同的对象，可以有不同的结果
  // 在 JavaScript 中，多态通过方法重写（override）实现
  console.log('\n--- 4.4 多态 ---');

  // 不同的类有相同的方法名，但实现不同
  class Cat extends AnimalES6 {
    speak() {
      console.log(`  ${this.name} 喵喵叫`);
    }
  }

  class Bird extends AnimalES6 {
    speak() {
      console.log(`  ${this.name} 叽叽喳喳叫`);
    }
  }

  // 统一的接口，不同的实现
  function makeAnimalSpeak(animal) {
    animal.speak(); // 同一个方法调用，不同对象有不同表现
  }

  const animals = [
    new DogES6('小狗', '哈士奇'),
    new Cat('小猫'),
    new Bird('小鸟')
  ];

  console.log('  多态演示：');
  animals.forEach(animal => makeAnimalSpeak(animal));

  // ------------------------------------------------------------
  // 4.5 抽象类与接口（模拟实现）
  // ------------------------------------------------------------
  // JavaScript 本身没有抽象类和接口的概念
  // 但可以通过约定和运行时检查来模拟
  console.log('\n--- 4.5 模拟抽象类 ---');

  // 模拟抽象类：不能直接实例化，必须由子类继承并实现抽象方法
  class Shape {
    constructor() {
      // 检查是否直接实例化抽象类
      if (new.target === Shape) {
        throw new Error('Shape 是抽象类，不能直接实例化');
      }
    }

    // 抽象方法：子类必须实现，否则调用时报错
    getArea() {
      throw new Error('子类必须实现 getArea 方法');
    }

    getPerimeter() {
      throw new Error('子类必须实现 getPerimeter 方法');
    }
  }

  class Rectangle extends Shape {
    constructor(width, height) {
      super();
      this.width = width;
      this.height = height;
    }

    getArea() {
      return this.width * this.height;
    }

    getPerimeter() {
      return 2 * (this.width + this.height);
    }
  }

  const rect = new Rectangle(4, 5);
  console.log('  矩形面积：', rect.getArea()); // 20
  console.log('  矩形周长：', rect.getPerimeter()); // 18

  // 直接实例化抽象类会报错
  try {
    new Shape();
  } catch (e) {
    console.log('  直接实例化抽象类报错：', e.message);
  }

  // ============================================================
  // 第五部分：类的其他特性
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('五、类的其他特性');
  console.log('='.repeat(60));

  // ------------------------------------------------------------
  // 5.1 类表达式
  // ------------------------------------------------------------
  // 和函数一样，类也可以用表达式的形式定义
  console.log('\n--- 5.1 类表达式 ---');

  // 匿名类表达式
  const MyClass = class {
    constructor(name) {
      this.name = name;
    }
    sayName() {
      console.log('  类表达式：', this.name);
    }
  };
  new MyClass('测试').sayName();

  // 命名类表达式（类名只能在类内部使用）
  const NamedClass = class InnerName {
    constructor() {
      console.log('  类内部名称：', InnerClass.name); // InnerName
    }
  };
  // new InnerName(); // 报错：InnerName is not defined（外部不可见）

  // ------------------------------------------------------------
  // 5.2 类的继承方式总结
  // ------------------------------------------------------------
  console.log('\n--- 5.2 继承方式总结 ---');
  console.log('  JavaScript 常见的继承方式：');
  console.log('    1. 原型链继承：子类原型指向父类实例');
  console.log('       优点：方法复用，原型链完整');
  console.log('       缺点：父类引用属性共享，不能传参给父类构造函数');
  console.log('    2. 构造函数继承（盗用构造函数）：子类中调用父类构造函数');
  console.log('       优点：可以传参，属性不共享');
  console.log('       缺点：方法不能复用，每个实例都有一份方法副本');
  console.log('    3. 组合继承：原型链 + 构造函数（最常用的 ES5 继承方式）');
  console.log('       优点：结合了两者的优点');
  console.log('       缺点：调用了两次父类构造函数');
  console.log('    4. 原型式继承：Object.create() 实现');
  console.log('    5. 寄生式继承：在原型式基础上增强对象');
  console.log('    6. 寄生组合式继承：最优的 ES5 继承方式');
  console.log('    7. ES6 extends 继承：语法清晰，推荐使用');

  // ============================================================
  // 演示结束
  // ============================================================
  console.log('\n' + '='.repeat(60));
  console.log('面向对象编程 演示结束');
  console.log('='.repeat(60));
};

if (require.main === module) {
  module.exports();
}
