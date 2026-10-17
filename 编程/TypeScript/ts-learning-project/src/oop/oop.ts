/**
 * ============================================================
 *  TypeScript 面向对象编程（OOP）详解
 * ============================================================
 *
 * 本文件详细介绍 TypeScript 中的面向对象特性，包括：
 * 1. class 类定义、属性、方法
 * 2. 构造函数 constructor、参数属性
 * 3. 继承 extends、super
 * 4. 修饰符 public / private / protected / readonly
 * 5. 静态成员 static
 * 6. 抽象类 abstract
 * 7. 实现接口 implements
 * 8. getter / setter 访问器
 * 9. 多态、方法重写 override
 * 10. 类表达式、类作为类型
 * ============================================================
 */

// ============================================================
//  1. class 类定义、属性、方法
// ============================================================

// TypeScript 的类是基于 ES6 class 语法的扩展
// 添加了类型注解、访问修饰符等特性

/**
 * 人类 - 基础类示例
 * 描述一个人的基本属性和行为
 */
class PersonOOP {
  // ---------- 属性定义 ----------
  // 在类中声明属性，并指定类型
  name: string;                                   // 姓名
  age: number;                                    // 年龄

  // ---------- 构造函数 ----------
  // 构造函数在创建类的实例时自动调用
  // 用于初始化对象的属性
  constructor(name: string, age: number) {
    this.name = name;                             // 初始化 name 属性
    this.age = age;                               // 初始化 age 属性
  }

  // ---------- 方法定义 ----------
  // 类中的函数称为方法
  /**
   * 打招呼的方法
   */
  greet(): void {
    console.log(`你好，我叫${this.name}，今年${this.age}岁`);
  }

  /**
   * 过生日的方法，年龄加一
   */
  haveBirthday(): void {
    this.age++;
    console.log(`生日快乐！${this.name}现在${this.age}岁了`);
  }
}

// 创建类的实例（对象）
const personOOP = new PersonOOP('张三', 25);
personOOP.greet();                               // 调用方法
personOOP.haveBirthday();                        // 调用方法

// ============================================================
//  2. 构造函数 constructor、参数属性
// ============================================================

// ---------- 参数属性（Parameter Properties）----------
// TypeScript 提供了一种简写方式：在构造函数参数前加上访问修饰符
// 可以自动创建并初始化属性
// 这是 TypeScript 特有的语法糖

/**
 * 动物类 - 演示参数属性
 */
class AnimalOOP {
  // 使用参数属性：在构造函数参数前加上 public 等修饰符
  // TypeScript 会自动创建同名属性并赋值
  constructor(
    public name: string,                          // 自动创建 public name 属性
    public species: string,                       // 自动创建 public species 属性
    private _weight: number                       // 自动创建 private _weight 属性
  ) {
    // 不需要手动赋值，TypeScript 会自动处理
    // this.name = name;  // 这些都不需要写了
    // this.species = species;
    // this._weight = _weight;
  }

  /**
   * 介绍自己
   */
  introduce(): void {
    console.log(`我是${this.name}，属于${this.species}，体重${this._weight}kg`);
  }
}

const catOOP = new AnimalOOP('咪咪', '猫', 4);
catOOP.introduce();

// ============================================================
//  3. 继承 extends、super
// ============================================================

// 继承是面向对象的核心特性之一
// 子类可以继承父类的属性和方法，并可以添加自己的属性和方法
// 使用 extends 关键字实现继承

/**
 * 动物基类（父类）
 */
class AnimalBase {
  name: string;
  age: number;

  constructor(name: string, age: number) {
    this.name = name;
    this.age = age;
  }

  /**
   * 吃东西的方法（父类方法）
   */
  eat(): void {
    console.log(`${this.name}在吃东西`);
  }

  /**
   * 睡觉的方法（父类方法）
   */
  sleep(): void {
    console.log(`${this.name}在睡觉`);
  }
}

/**
 * 狗类（子类）- 继承自动物类
 * 使用 extends 关键字继承 AnimalBase
 */
class DogOOP extends AnimalBase {
  breed: string;                                 // 子类特有属性：品种

  constructor(name: string, age: number, breed: string) {
    // 使用 super 调用父类的构造函数
    // super 必须在使用 this 之前调用
    super(name, age);
    this.breed = breed;
  }

  /**
   * 吠叫的方法（子类特有方法）
   */
  bark(): void {
    console.log(`${this.name}（${this.breed}）在汪汪叫！`);
  }

  /**
   * 重写父类的 eat 方法
   * 子类可以重写（override）父类的方法
   */
  eat(): void {
    // 使用 super 调用父类的方法
    super.eat();
    console.log(`${this.name}在啃骨头`);
  }
}

// 创建子类实例
const dogOOP = new DogOOP('旺财', 3, '金毛');
dogOOP.eat();                                    // 调用重写后的方法
dogOOP.bark();                                   // 调用子类特有方法
dogOOP.sleep();                                  // 调用继承自父类的方法

// ---------- 继承的特点 ----------
// 1. 子类拥有父类的所有 public 和 protected 属性和方法
// 2. 子类可以添加自己的属性和方法
// 3. 子类可以重写父类的方法
// 4. 使用 super 关键字调用父类的构造函数或方法
// 5. TypeScript 只支持单继承（一个类只能继承一个父类），但可以实现多个接口

// ============================================================
//  4. 修饰符 public / private / protected / readonly
// ============================================================

// TypeScript 提供了四种访问修饰符，用于控制类成员的可访问性

/**
 * 修饰符演示类
 */
class ModifierDemo {
  // ---------- public（公共的）----------
  // 默认修饰符，可以在任何地方访问
  public name: string;

  // ---------- private（私有的）----------
  // 只能在类的内部访问，外部和子类都不能访问
  private _password: string;

  // ---------- protected（受保护的）----------
  // 可以在类的内部和子类中访问，但外部不能访问
  protected internalId: string;

  // ---------- readonly（只读的）----------
  // 只读属性只能在声明时或构造函数中赋值，之后不能修改
  readonly createdAt: Date;

  constructor(name: string, password: string, internalId: string) {
    this.name = name;
    this._password = password;
    this.internalId = internalId;
    this.createdAt = new Date();                 // 构造函数中可以赋值
  }

  /**
   * 公共方法：可以在外部调用
   */
  public getInfo(): string {
    // 内部可以访问所有成员
    return `名称：${this.name}，ID：${this.internalId}`;
  }

  /**
   * 私有方法：只能在类内部调用
   */
  private checkPassword(input: string): boolean {
    return input === this._password;
  }

  /**
   * 受保护方法：可以在类内部和子类中调用
   */
  protected getInternalId(): string {
    return this.internalId;
  }
}

const modDemo = new ModifierDemo('测试', '123456', 'id-001');
console.log(modDemo.name);                       // 正确，public 可以外部访问
// console.log(modDemo._password);               // 错误，private 不能外部访问
// console.log(modDemo.internalId);              // 错误，protected 不能外部访问
console.log(modDemo.createdAt);                  // 正确，readonly 可以读取但不能修改
// modDemo.createdAt = new Date();              // 错误，readonly 不能修改

/**
 * 子类演示 protected 的访问
 */
class ModifierChild extends ModifierDemo {
  constructor(name: string, password: string, internalId: string) {
    super(name, password, internalId);
  }

  showInternalId(): void {
    // 子类中可以访问 protected 成员
    console.log('内部ID：', this.internalId);
    console.log('内部ID（通过方法）：', this.getInternalId());
    // console.log(this._password);              // 错误，private 不能在子类中访问
  }
}

const childDemo = new ModifierChild('子测试', 'abc', 'id-002');
childDemo.showInternalId();

// ============================================================
//  5. 静态成员 static
// ============================================================

// 静态成员属于类本身，而不是类的实例
// 使用 static 关键字定义
// 访问静态成员时直接使用 类名.成员名，不需要创建实例

/**
 * 数学工具类 - 演示静态成员
 */
class MathUtils {
  // ---------- 静态属性 ----------
  // 静态属性属于类本身，所有实例共享同一份
  static readonly PI: number = 3.14159265358979;  // 静态只读属性
  static instanceCount: number = 0;               // 静态属性，记录实例数量

  // ---------- 实例属性 ----------
  value: number;

  constructor(value: number) {
    this.value = value;
    MathUtils.instanceCount++;                    // 构造函数中可以访问静态属性
  }

  // ---------- 静态方法 ----------
  /**
   * 计算圆的面积（静态方法）
   * @param radius 半径
   */
  static calculateCircleArea(radius: number): number {
    // 静态方法中可以访问静态属性
    return MathUtils.PI * radius * radius;
  }

  /**
   * 计算两个数的最大值（静态方法）
   */
  static max(a: number, b: number): number {
    return a > b ? a : b;
  }

  /**
   * 实例方法
   */
  getValue(): number {
    return this.value;
    // 实例方法中可以访问静态成员
    // console.log(MathUtils.PI);
  }
}

// 访问静态成员不需要创建实例
console.log('PI =', MathUtils.PI);
console.log('圆面积（r=5）=', MathUtils.calculateCircleArea(5));
console.log('max(10, 20) =', MathUtils.max(10, 20));

// 创建实例
const math1 = new MathUtils(10);
const math2 = new MathUtils(20);
console.log('实例数量：', MathUtils.instanceCount); // 输出：2

// ---------- 静态成员的特点 ----------
// 1. 静态成员属于类，不属于实例
// 2. 静态属性在内存中只有一份，所有实例共享
// 3. 静态方法中不能访问实例成员（不能使用 this 访问实例属性/方法）
// 4. 实例方法中可以访问静态成员（使用 类名.静态成员）
// 5. 常用于工具类、常量、工厂方法等

// ============================================================
//  6. 抽象类 abstract
// ============================================================

// 抽象类是不能被实例化的类，只能被继承
// 抽象类可以包含抽象方法（没有实现的方法）和具体方法
// 抽象方法必须在子类中实现
// 使用 abstract 关键字定义抽象类和抽象方法

/**
 * 形状抽象类
 * 作为所有形状类的基类，定义了形状必须具备的方法
 */
abstract class Shape {
  // 抽象类可以有实例属性
  color: string;

  constructor(color: string) {
    this.color = color;
  }

  // ---------- 抽象方法 ----------
  // 抽象方法没有具体实现，只有方法签名
  // 子类必须实现这些抽象方法
  /**
   * 计算面积（抽象方法，子类必须实现）
   */
  abstract calculateArea(): number;

  /**
   * 计算周长（抽象方法，子类必须实现）
   */
  abstract calculatePerimeter(): number;

  // ---------- 具体方法 ----------
  // 抽象类中也可以有已实现的方法
  // 子类可以直接继承使用，也可以重写
  /**
   * 显示颜色信息（具体方法）
   */
  displayColor(): void {
    console.log(`这个形状是${this.color}的`);
  }
}

// const shape = new Shape('red');                // 错误，抽象类不能被实例化

/**
 * 圆形类 - 继承自抽象类 Shape
 * 必须实现所有抽象方法
 */
class Circle extends Shape {
  radius: number;

  constructor(color: string, radius: number) {
    super(color);
    this.radius = radius;
  }

  // 必须实现抽象方法 calculateArea
  calculateArea(): number {
    return Math.PI * this.radius * this.radius;
  }

  // 必须实现抽象方法 calculatePerimeter
  calculatePerimeter(): number {
    return 2 * Math.PI * this.radius;
  }
}

/**
 * 矩形类 - 继承自抽象类 Shape
 */
class Rectangle extends Shape {
  width: number;
  height: number;

  constructor(color: string, width: number, height: number) {
    super(color);
    this.width = width;
    this.height = height;
  }

  calculateArea(): number {
    return this.width * this.height;
  }

  calculatePerimeter(): number {
    return 2 * (this.width + this.height);
  }
}

// 多态：父类引用指向子类实例
const circle: Shape = new Circle('红色', 5);
const rectangle: Shape = new Rectangle('蓝色', 4, 6);

console.log('圆面积：', circle.calculateArea());
console.log('矩形面积：', rectangle.calculateArea());
circle.displayColor();
rectangle.displayColor();

// ---------- 抽象类的用途 ----------
// 1. 定义子类的共同接口（契约）
// 2. 提供一些通用的实现（具体方法）
// 3. 强制子类实现某些方法
// 4. 实现多态

// ============================================================
//  7. 实现接口 implements
// ============================================================

// 类可以实现（implements）一个或多个接口
// 接口定义了类必须遵守的契约（必须有哪些属性和方法）
// 类必须实现接口中定义的所有属性和方法

// ---------- 定义接口 ----------
interface FlyableOOP {
  // 接口定义了飞行能力的契约
  fly(): void;                                   // 飞行方法
  speed: number;                                 // 飞行速度
}

interface SwimmableOOP {
  // 接口定义了游泳能力的契约
  swim(): void;                                  // 游泳方法
  canDive: boolean;                              // 是否能潜水
}

/**
 * 鸟类 - 实现 Flyable 接口
 * 使用 implements 关键字实现接口
 */
class BirdOOP implements FlyableOOP {
  speed: number;                                 // 必须实现接口中的属性

  constructor(speed: number) {
    this.speed = speed;
  }

  fly(): void {                                  // 必须实现接口中的方法
    console.log(`鸟儿以${this.speed}km/h的速度飞行`);
  }
}

/**
 * 鱼类 - 实现 Swimmable 接口
 */
class FishOOP implements SwimmableOOP {
  canDive: boolean;

  constructor(canDive: boolean) {
    this.canDive = canDive;
  }

  swim(): void {
    console.log(`鱼儿在游泳${this.canDive ? '，还能潜水' : ''}`);
  }
}

/**
 * 鸭类 - 同时实现多个接口
 * 一个类可以实现多个接口，用逗号分隔
 */
class DuckOOP implements FlyableOOP, SwimmableOOP {
  speed: number;
  canDive: boolean;

  constructor(speed: number, canDive: boolean) {
    this.speed = speed;
    this.canDive = canDive;
  }

  fly(): void {
    console.log(`鸭子以${this.speed}km/h的速度飞行`);
  }

  swim(): void {
    console.log(`鸭子在游泳${this.canDive ? '，还能潜水' : ''}`);
  }

  quack(): void {                                // 类可以有接口之外的方法
    console.log('嘎嘎嘎！');
  }
}

// ---------- 接口与抽象类的区别 ----------
// 1. 接口只能定义契约，不能有实现；抽象类可以有实现
// 2. 一个类可以实现多个接口，但只能继承一个抽象类
// 3. 接口不能有构造函数；抽象类可以有构造函数
// 4. 接口中的成员默认是 public 的；抽象类可以有各种访问修饰符
// 5. 接口更轻量，适合定义能力；抽象类更重量级，适合定义基类

// ============================================================
//  8. getter / setter 访问器
// ============================================================

// TypeScript 支持 getters 和 setters，用于控制对象属性的访问和修改
// 使用 get 和 set 关键字定义访问器
// 可以在访问或修改属性时添加额外的逻辑（如验证、计算等）

/**
 * 人员类 - 演示 getter/setter
 */
class PersonAccessor {
  private _name: string;
  private _age: number;

  constructor(name: string, age: number) {
    this._name = name;
    this._age = age;
  }

  // ---------- getter ----------
  // 使用 get 关键字定义 getter
  // 访问时像普通属性一样，但实际上是调用函数
  /**
   * 获取姓名
   */
  get name(): string {
    console.log('正在获取 name 属性');
    return this._name;
  }

  /**
   * 获取年龄
   */
  get age(): number {
    return this._age;
  }

  // ---------- setter ----------
  // 使用 set 关键字定义 setter
  // 赋值时像普通属性一样，但实际上是调用函数
  // 可以添加验证逻辑
  /**
   * 设置姓名（带验证）
   */
  set name(newName: string) {
    if (newName.length > 0 && newName.length <= 20) {
      this._name = newName;
      console.log('name 属性已更新');
    } else {
      console.log('姓名长度必须在 1-20 个字符之间');
    }
  }

  /**
   * 设置年龄（带验证）
   */
  set age(newAge: number) {
    if (newAge >= 0 && newAge <= 150) {
      this._age = newAge;
    } else {
      console.log('年龄必须在 0-150 之间');
    }
  }

  // ---------- 计算属性 ----------
  // getter 也可以用于计算属性（不需要对应的 setter）
  /**
   * 计算属性：出生年份（近似）
   */
  get birthYear(): number {
    return new Date().getFullYear() - this._age;
  }
}

const personAcc = new PersonAccessor('张三', 25);

// 使用 getter 访问属性
console.log(personAcc.name);                     // 输出：正在获取 name 属性 \n 张三
console.log(personAcc.age);                      // 输出：25
console.log('出生年份：', personAcc.birthYear);   // 计算属性

// 使用 setter 修改属性
personAcc.name = '李四';                          // 正确，输出：name 属性已更新
personAcc.name = '';                             // 错误，输出：姓名长度必须在 1-20 个字符之间
personAcc.age = 30;                              // 正确
personAcc.age = 200;                             // 错误，输出：年龄必须在 0-150 之间

// ============================================================
//  9. 多态、方法重写 override
// ============================================================

// 多态（Polymorphism）是面向对象的三大特性之一
// 指同一个方法在不同对象上有不同的实现
// 多态通过方法重写（Override）实现

// ---------- 方法重写 ----------
// 子类可以重新实现父类的方法，这叫做方法重写
// 重写的方法必须与父类方法有相同的名称、参数列表和返回类型（或兼容的返回类型）

/**
 * 动物基类
 */
class AnimalPoly {
  name: string;

  constructor(name: string) {
    this.name = name;
  }

  /**
   * 发出声音（基类方法）
   */
  makeSound(): void {
    console.log(`${this.name}发出了声音`);
  }
}

/**
 * 狗类 - 重写 makeSound 方法
 */
class DogPoly extends AnimalPoly {
  constructor(name: string) {
    super(name);
  }

  // 使用 override 关键字显式标记这是重写的方法（TypeScript 4.3+）
  // 可以防止因拼写错误导致的意外创建新方法
  override makeSound(): void {
    console.log(`${this.name}在汪汪叫！`);
  }
}

/**
 * 猫类 - 重写 makeSound 方法
 */
class CatPoly extends AnimalPoly {
  constructor(name: string) {
    super(name);
  }

  override makeSound(): void {
    console.log(`${this.name}在喵喵叫！`);
  }
}

/**
 * 牛类 - 重写 makeSound 方法
 */
class CowPoly extends AnimalPoly {
  constructor(name: string) {
    super(name);
  }

  override makeSound(): void {
    console.log(`${this.name}在哞哞叫！`);
  }
}

// ---------- 多态的体现 ----------
// 父类引用可以指向子类实例
// 调用同一个方法时，会根据实际对象类型执行不同的实现

// 创建动物数组（元素类型是 AnimalPoly，但实际是各种子类）
const animals: AnimalPoly[] = [
  new DogPoly('旺财'),
  new CatPoly('咪咪'),
  new CowPoly('哞哞')
];

// 遍历数组，调用 makeSound 方法
// 虽然都是调用 makeSound，但每种动物的行为不同
console.log('\n--- 多态演示 ---');
animals.forEach(animal => {
  animal.makeSound();                            // 多态调用
});

// ---------- 方法重写的规则 ----------
// 1. 方法名必须相同
// 2. 参数列表必须相同（参数个数、类型、顺序）
// 3. 返回值类型必须相同或是子类型（协变）
// 4. 访问修饰符不能比父类更严格
// 5. 建议使用 override 关键字显式标记重写

// ============================================================
//  10. 类表达式、类作为类型
// ============================================================

// ---------- 类表达式 ----------
// 与函数表达式类似，类也可以用表达式的形式定义
// 类表达式可以是命名的或匿名的

// 匿名类表达式
const PersonExpr = class {
  name: string;
  age: number;

  constructor(name: string, age: number) {
    this.name = name;
    this.age = age;
  }

  greet(): void {
    console.log(`你好，我是${this.name}`);
  }
};

const p1 = new PersonExpr('张三', 25);
p1.greet();

// 命名类表达式（类名只在类内部可见）
const PersonNamed = class PersonInner {
  name: string;

  constructor(name: string) {
    this.name = name;
  }

  getName(): string {
    // PersonInner 只在类内部可见
    return PersonInner.name + ': ' + this.name;
  }
};

// ---------- 类作为类型 ----------
// 在 TypeScript 中，类定义同时也创建了一个类型
// 可以直接将类名作为类型使用

class CarOOP {
  brand: string;
  model: string;

  constructor(brand: string, model: string) {
    this.brand = brand;
    this.model = model;
  }

  drive(): void {
    console.log(`${this.brand} ${this.model} 正在行驶`);
  }
}

// 类可以直接作为类型使用
let myCarOOP: CarOOP;                           // CarOOP 既是类名也是类型
myCarOOP = new CarOOP('Toyota', 'Camry');
myCarOOP.drive();

// 结构类型系统：只要结构匹配，就认为类型兼容
// 即使不是通过 CarOOP 创建的对象，只要有相同的结构，也可以赋值给 CarOOP 类型
let carLike: CarOOP = {
  brand: 'Honda',
  model: 'Accord',
  drive(): void {
    console.log(`${this.brand} ${this.model} 也在行驶`);
  }
};
carLike.drive();

// ---------- 类的类型的使用场景 ----------
// 1. 函数参数类型
function driveCar(car: CarOOP): void {
  car.drive();
}

// 2. 函数返回值类型
function createCar(brand: string, model: string): CarOOP {
  return new CarOOP(brand, model);
}

// 3. 数组/对象的元素类型
const cars: CarOOP[] = [
  new CarOOP('Toyota', 'Camry'),
  new CarOOP('Honda', 'Accord')
];

// ============================================================
//  演示函数：运行所有面向对象示例
// ============================================================

export function runOOPDemo(): void {
  console.log('========== 面向对象演示 ==========');

  // 基本类
  console.log('1. 基本类与构造函数:');
  const personOOP = new PersonOOP('张三', 25);
  personOOP.greet();

  // 参数属性
  console.log('\n2. 参数属性:');
  const catOOP = new AnimalOOP('咪咪', '猫', 4);
  catOOP.introduce();

  // 继承
  console.log('\n3. 继承与 super:');
  const dogOOP = new DogOOP('旺财', 3, '金毛');
  dogOOP.eat();
  dogOOP.bark();

  // 修饰符
  console.log('\n4. 访问修饰符:');
  const modDemo = new ModifierDemo('测试', '123456', 'id-001');
  console.log('   公共属性 name:', modDemo.name);
  console.log('   只读属性 createdAt:', modDemo.createdAt);
  console.log('   公共方法 getInfo:', modDemo.getInfo());

  // 静态成员
  console.log('\n5. 静态成员:');
  console.log('   PI =', MathUtils.PI);
  console.log('   圆面积(r=5) =', MathUtils.calculateCircleArea(5));
  const m1 = new MathUtils(10);
  const m2 = new MathUtils(20);
  console.log('   实例数量 =', MathUtils.instanceCount);

  // 抽象类与多态
  console.log('\n6. 抽象类与多态:');
  const shapes: Shape[] = [
    new Circle('红色', 5),
    new Rectangle('蓝色', 4, 6)
  ];
  shapes.forEach(shape => {
    shape.displayColor();
    console.log('   面积:', shape.calculateArea());
    console.log('   周长:', shape.calculatePerimeter());
  });

  // 实现接口
  console.log('\n7. 实现接口:');
  const duck = new DuckOOP(30, true);
  duck.fly();
  duck.swim();
  duck.quack();

  // getter/setter
  console.log('\n8. getter/setter 访问器:');
  const personAcc = new PersonAccessor('张三', 25);
  console.log('   name:', personAcc.name);
  personAcc.age = 30;
  console.log('   age:', personAcc.age);
  console.log('   出生年份:', personAcc.birthYear);

  // 多态
  console.log('\n9. 多态与方法重写:');
  animals.forEach(animal => {
    animal.makeSound();
  });

  // 类表达式
  console.log('\n10. 类表达式与类作为类型:');
  const p1 = new PersonExpr('张三', 25);
  p1.greet();
  const car = createCar('Toyota', 'Camry');
  driveCar(car);

  console.log('\n========== 面向对象演示结束 ==========\n');
}
