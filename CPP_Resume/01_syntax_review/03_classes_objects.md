# 1.3 类与对象

## 📖 本节概述

类是C++面向对象编程（OOP）的核心。本节将回顾类的定义、构造与析构、访问控制、继承和多态等概念，并介绍现代C++在类设计方面的新特性。

---

## 1. 类的基础

### 1.1 类的定义

```cpp
// 类定义
class Person {
private:    // 私有成员：只能在类内部访问
    std::string name_;
    int age_;

public:     // 公有成员：可以在类外部访问
    // 构造函数
    Person(const std::string& name, int age) 
        : name_(name), age_(age) {}  // 成员初始化列表
    
    // 成员函数（方法）
    void introduce() const {
        std::cout << "I'm " << name_ << ", " << age_ << " years old." << std::endl;
    }
    
    // getter
    std::string get_name() const { return name_; }
    int get_age() const { return age_; }
    
    // setter
    void set_age(int age) { age_ = age; }

protected:  // 保护成员：本类和派生类可以访问
    void protected_method() {}
};

// 使用类
int main() {
    Person alice("Alice", 25);
    alice.introduce();  // I'm Alice, 25 years old.
    
    std::cout << alice.get_name() << std::endl;  // Alice
    alice.set_age(26);
    
    return 0;
}
```

### 1.2 struct vs class

```cpp
// struct 默认 public，class 默认 private
struct Point {
    double x;  // 默认 public
    double y;
};

class PointClass {
    double x;  // 默认 private
    double y;
public:
    PointClass(double x, double y) : x(x), y(y) {}
};

// 使用习惯：
// - struct：用于简单数据聚合（POD类型）
// - class：用于有行为的复杂对象
```

### 1.3 成员初始化

```cpp
class Widget {
private:
    int value_;
    std::string name_;
    std::vector<int> data_;

public:
    // ❌ 不推荐：在构造函数体内赋值
    Widget(int v, const std::string& n) {
        value_ = v;     // 先默认构造，再赋值
        name_ = n;      // 效率低
    }
    
    // ✅ 推荐：使用成员初始化列表
    Widget(int v, const std::string& n) 
        : value_(v), name_(n), data_() {}  // 直接构造
    
    // C++11：类内成员初始化（默认值）
};

// C++11 类内成员初始化
class ModernWidget {
private:
    int value_ = 0;                    // 默认值
    std::string name_ = "unnamed";     // 默认值
    std::vector<int> data_{1, 2, 3};   // 默认值
    
public:
    ModernWidget() = default;  // 使用默认值
    
    ModernWidget(int v) : value_(v) {}  // 只覆盖 value_
    
    ModernWidget(int v, const std::string& n) 
        : value_(v), name_(n) {}  // 覆盖 value_ 和 name_
};
```

---

## 2. 构造函数与析构函数

### 2.1 构造函数类型

```cpp
class MyClass {
private:
    int value_;
    int* data_;

public:
    // 1. 默认构造函数
    MyClass() : value_(0), data_(nullptr) {
        std::cout << "Default constructor" << std::endl;
    }
    
    // 2. 参数化构造函数
    MyClass(int v) : value_(v), data_(new int[10]) {
        std::cout << "Parameterized constructor" << std::endl;
    }
    
    // 3. 拷贝构造函数
    MyClass(const MyClass& other) 
        : value_(other.value_), data_(nullptr) {
        if (other.data_) {
            data_ = new int[10];
            std::copy(other.data_, other.data_ + 10, data_);
        }
        std::cout << "Copy constructor" << std::endl;
    }
    
    // 4. 移动构造函数（C++11）
    MyClass(MyClass&& other) noexcept 
        : value_(other.value_), data_(other.data_) {
        other.data_ = nullptr;  // 转移所有权
        std::cout << "Move constructor" << std::endl;
    }
    
    // 析构函数
    ~MyClass() {
        delete[] data_;
        std::cout << "Destructor" << std::endl;
    }
};
```

### 2.2 赋值运算符

```cpp
class MyClass {
    // ... 成员变量 ...

public:
    // 拷贝赋值运算符
    MyClass& operator=(const MyClass& other) {
        if (this != &other) {  // 自赋值检查
            delete[] data_;
            value_ = other.value_;
            if (other.data_) {
                data_ = new int[10];
                std::copy(other.data_, other.data_ + 10, data_);
            } else {
                data_ = nullptr;
            }
        }
        return *this;
    }
    
    // 移动赋值运算符（C++11）
    MyClass& operator=(MyClass&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            value_ = other.value_;
            data_ = other.data_;
            other.data_ = nullptr;
        }
        return *this;
    }
};
```

### 2.3 特殊成员函数规则

C++11 引入了 "Rule of Five"（五法则）：如果你定义了以下任何一个，你可能需要定义全部五个：

1. 析构函数
2. 拷贝构造函数
3. 拷贝赋值运算符
4. 移动构造函数
5. 移动赋值运算符

```cpp
class ResourceOwner {
private:
    int* data_;

public:
    // 构造函数
    ResourceOwner() : data_(new int[100]) {}
    
    // 析构函数
    ~ResourceOwner() { delete[] data_; }
    
    // 拷贝构造函数
    ResourceOwner(const ResourceOwner& other) 
        : data_(new int[100]) {
        std::copy(other.data_, other.data_ + 100, data_);
    }
    
    // 拷贝赋值运算符
    ResourceOwner& operator=(const ResourceOwner& other) {
        if (this != &other) {
            std::copy(other.data_, other.data_ + 100, data_);
        }
        return *this;
    }
    
    // 移动构造函数
    ResourceOwner(ResourceOwner&& other) noexcept 
        : data_(other.data_) {
        other.data_ = nullptr;
    }
    
    // 移动赋值运算符
    ResourceOwner& operator=(ResourceOwner&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            other.data_ = nullptr;
        }
        return *this;
    }
};
```

### 2.4 default 和 delete（C++11）

```cpp
class MyClass {
public:
    // 显式使用默认实现
    MyClass() = default;
    ~MyClass() = default;
    MyClass(const MyClass&) = default;
    MyClass& operator=(const MyClass&) = default;
    
    // 禁止某些操作
    MyClass(MyClass&&) = delete;              // 禁止移动构造
    MyClass& operator=(MyClass&&) = delete;   // 禁止移动赋值
};

// 不可拷贝的类
class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

// 不可实例化的类（只有静态成员）
class StaticOnly {
public:
    StaticOnly() = delete;
    
    static void utility_function() {}
};
```

---

## 3. 访问控制

### 3.1 访问说明符

```cpp
class Example {
public:     // 公有：任何地方都可以访问
    void public_method() {}

protected:  // 保护：本类和派生类可以访问
    void protected_method() {}

private:    // 私有：只有本类可以访问
    void private_method() {}
    int private_data_;
};

class Derived : public Example {
public:
    void use_base() {
        public_method();     // ✅ 可以访问
        protected_method();  // ✅ 可以访问
        // private_method(); // ❌ 不可访问
    }
};

void external_function() {
    Example obj;
    obj.public_method();     // ✅ 可以访问
    // obj.protected_method(); // ❌ 不可访问
    // obj.private_method();   // ❌ 不可访问
}
```

### 3.2 友元

```cpp
class SecretKeeper {
private:
    int secret_ = 42;
    
    // 友元函数：可以访问私有成员
    friend void reveal_secret(const SecretKeeper& sk);
    
    // 友元类：整个类可以访问私有成员
    friend class TrustedClass;
};

void reveal_secret(const SecretKeeper& sk) {
    std::cout << sk.secret_ << std::endl;  // ✅ 可以访问
}

class TrustedClass {
public:
    void access_secret(const SecretKeeper& sk) {
        std::cout << sk.secret_ << std::endl;  // ✅ 可以访问
    }
};
```

---

## 4. 静态成员

### 4.1 静态成员变量

```cpp
class Counter {
private:
    static int count_;  // 声明静态成员
    int id_;

public:
    Counter() : id_(++count_) {
        std::cout << "Created Counter #" << id_ << std::endl;
    }
    
    ~Counter() {
        std::cout << "Destroyed Counter #" << id_ << std::endl;
        --count_;
    }
    
    static int get_count() { return count_; }  // 静态成员函数
};

// 定义静态成员（在类外，通常在 .cpp 文件）
int Counter::count_ = 0;

// C++17 inline 静态成员（可以在类内初始化）
class ModernCounter {
    inline static int count_ = 0;  // C++17
};
```

### 4.2 静态成员函数

```cpp
class MathUtils {
public:
    // 静态成员函数：不需要对象实例
    static double square(double x) {
        return x * x;
    }
    
    static double cube(double x) {
        return x * x * x;
    }
    
    // 静态成员函数不能访问非静态成员
    // 因为没有 this 指针
};

// 使用
double result = MathUtils::square(5.0);  // 25.0
```

---

## 5. 继承

### 5.1 基本继承

```cpp
// 基类
class Animal {
protected:
    std::string name_;

public:
    Animal(const std::string& name) : name_(name) {}
    
    void eat() {
        std::cout << name_ << " is eating." << std::endl;
    }
    
    virtual void speak() {  // 虚函数：可被派生类重写
        std::cout << name_ << " makes a sound." << std::endl;
    }
    
    virtual ~Animal() = default;  // 虚析构函数
};

// 派生类
class Dog : public Animal {
public:
    Dog(const std::string& name) : Animal(name) {}
    
    void speak() override {  // C++11 override 关键字
        std::cout << name_ << " barks: Woof!" << std::endl;
    }
    
    void fetch() {
        std::cout << name_ << " fetches the ball." << std::endl;
    }
};

class Cat : public Animal {
public:
    Cat(const std::string& name) : Animal(name) {}
    
    void speak() override {
        std::cout << name_ << " meows: Meow!" << std::endl;
    }
};
```

### 5.2 继承方式

```cpp
class Base {
public:
    int pub;
protected:
    int prot;
private:
    int priv;
};

// public 继承：最常用
class PublicDerived : public Base {
    // pub 仍是 public
    // prot 仍是 protected
    // priv 不可访问
};

// protected 继承
class ProtectedDerived : protected Base {
    // pub 变成 protected
    // prot 仍是 protected
    // priv 不可访问
};

// private 继承
class PrivateDerived : private Base {
    // pub 变成 private
    // prot 变成 private
    // priv 不可访问
};
```

### 5.3 override 和 final（C++11）

```cpp
class Base {
public:
    virtual void foo() {}
    virtual void bar() {}
    virtual void baz() final {}  // 禁止派生类重写
};

class Derived : public Base {
public:
    void foo() override {}        // ✅ 正确重写
    // void fooo() override {}    // ❌ 编译错误：基类没有 fooo
    // void baz() override {}     // ❌ 编译错误：baz 是 final
};

// final 类：禁止被继承
class FinalClass final {
    // ...
};

// class CannotDerive : public FinalClass {};  // ❌ 编译错误
```

---

## 6. 多态

### 6.1 虚函数与动态多态

```cpp
#include <vector>
#include <memory>

class Shape {
public:
    virtual double area() const = 0;  // 纯虚函数
    virtual void draw() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
private:
    double radius_;

public:
    Circle(double r) : radius_(r) {}
    
    double area() const override {
        return 3.14159 * radius_ * radius_;
    }
    
    void draw() const override {
        std::cout << "Drawing circle with radius " << radius_ << std::endl;
    }
};

class Rectangle : public Shape {
private:
    double width_, height_;

public:
    Rectangle(double w, double h) : width_(w), height_(h) {}
    
    double area() const override {
        return width_ * height_;
    }
    
    void draw() const override {
        std::cout << "Drawing rectangle " << width_ << "x" << height_ << std::endl;
    }
};

int main() {
    // 多态：使用基类指针/引用操作派生类对象
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rectangle>(3.0, 4.0));
    
    for (const auto& shape : shapes) {
        shape->draw();  // 动态绑定：调用实际类型的方法
        std::cout << "Area: " << shape->area() << std::endl;
    }
    
    return 0;
}
```

### 6.2 抽象类与接口

```cpp
// 抽象类：包含纯虚函数，不能实例化
class AbstractBase {
public:
    virtual void pure_virtual() = 0;  // 纯虚函数
    
    void concrete_method() {  // 可以有具体实现
        std::cout << "Concrete method" << std::endl;
    }
    
    virtual ~AbstractBase() = default;
};

// 接口：只有纯虚函数的抽象类
class IDrawable {
public:
    virtual void draw() const = 0;
    virtual ~IDrawable() = default;
};

class ISerializable {
public:
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
    virtual ~ISerializable() = default;
};

// 实现多个接口
class Widget : public IDrawable, public ISerializable {
public:
    void draw() const override {
        std::cout << "Drawing widget" << std::endl;
    }
    
    std::string serialize() const override {
        return "widget_data";
    }
    
    void deserialize(const std::string& data) override {
        // 反序列化逻辑
    }
};
```

### 6.3 虚析构函数

```cpp
class Base {
public:
    // ❌ 错误：非虚析构函数
    // ~Base() { std::cout << "Base destructor" << std::endl; }
    
    // ✅ 正确：虚析构函数
    virtual ~Base() { std::cout << "Base destructor" << std::endl; }
};

class Derived : public Base {
private:
    int* data_;

public:
    Derived() : data_(new int[100]) {}
    
    ~Derived() override {
        delete[] data_;
        std::cout << "Derived destructor" << std::endl;
    }
};

int main() {
    Base* ptr = new Derived();
    delete ptr;  // 如果析构函数非虚，只调用 Base 的析构函数！
                 // 虚析构函数确保先调用 Derived 析构函数
    return 0;
}
```

---

## 7. 运算符重载

### 7.1 基本运算符重载

```cpp
class Complex {
private:
    double real_, imag_;

public:
    Complex(double r = 0, double i = 0) : real_(r), imag_(i) {}
    
    // 成员函数形式
    Complex operator+(const Complex& other) const {
        return Complex(real_ + other.real_, imag_ + other.imag_);
    }
    
    Complex operator-(const Complex& other) const {
        return Complex(real_ - other.real_, imag_ - other.imag_);
    }
    
    // 复合赋值运算符
    Complex& operator+=(const Complex& other) {
        real_ += other.real_;
        imag_ += other.imag_;
        return *this;
    }
    
    // 一元运算符
    Complex operator-() const {
        return Complex(-real_, -imag_);
    }
    
    // 比较运算符
    bool operator==(const Complex& other) const {
        return real_ == other.real_ && imag_ == other.imag_;
    }
    
    // 友元函数形式（用于左操作数不是本类的情况）
    friend Complex operator*(double scalar, const Complex& c) {
        return Complex(scalar * c.real_, scalar * c.imag_);
    }
    
    // 输出流运算符
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.real_ << " + " << c.imag_ << "i";
        return os;
    }
};
```

### 7.2 C++20 三路比较运算符

```cpp
#include <compare>

class Version {
private:
    int major_, minor_, patch_;

public:
    Version(int ma, int mi, int pa) : major_(ma), minor_(mi), patch_(pa) {}
    
    // C++20：默认化 <=> 支持关系比较，并隐式声明默认化的 ==
    auto operator<=>(const Version& other) const = default;
    
    // 或者手动实现
    // std::strong_ordering operator<=>(const Version& other) const {
    //     if (auto cmp = major_ <=> other.major_; cmp != 0) return cmp;
    //     if (auto cmp = minor_ <=> other.minor_; cmp != 0) return cmp;
    //     return patch_ <=> other.patch_;
    // }
};
```

---

## 8. 特殊成员函数

### 8.1 this 指针

```cpp
class Builder {
private:
    int value_ = 0;
    std::string name_;

public:
    // 链式调用：返回 *this
    Builder& set_value(int v) {
        value_ = v;
        return *this;
    }
    
    Builder& set_name(const std::string& n) {
        name_ = n;
        return *this;
    }
    
    void build() {
        std::cout << "Building: " << name_ << " = " << value_ << std::endl;
    }
};

// 使用链式调用
Builder builder;
builder.set_name("count").set_value(42).build();
```

### 8.2 explicit 关键字

```cpp
class IntWrapper {
private:
    int value_;

public:
    // 没有 explicit：允许隐式转换
    IntWrapper(int v) : value_(v) {}
};

class SafeInt {
private:
    int value_;

public:
    // 有 explicit：禁止隐式转换
    explicit SafeInt(int v) : value_(v) {}
};

void foo(IntWrapper w) {}
void bar(SafeInt s) {}

int main() {
    foo(42);        // ✅ OK：隐式转换 int -> IntWrapper
    // bar(42);     // ❌ 错误：不能隐式转换
    bar(SafeInt(42));  // ✅ OK：显式构造
    
    IntWrapper w = 42;    // ✅ OK：隐式转换
    // SafeInt s = 42;    // ❌ 错误
    SafeInt s(42);        // ✅ OK
    SafeInt s2{42};       // ✅ OK
    
    return 0;
}
```

### 8.3 mutable 关键字

```cpp
class CacheExample {
private:
    mutable int cache_value_ = 0;   // 可在 const 方法中修改
    mutable bool cache_valid_ = false;
    int data_ = 0;

public:
    int get_computed_value() const {
        if (!cache_valid_) {
            // 即使是 const 方法，也可以修改 mutable 成员
            cache_value_ = expensive_computation();
            cache_valid_ = true;
        }
        return cache_value_;
    }

private:
    int expensive_computation() const {
        return data_ * data_;  // 模拟耗时计算
    }
};
```

---

## 📝 练习题

### 练习1：银行账户类
设计一个 `BankAccount` 类，包含账户余额、存款、取款功能，使用适当的访问控制。

### 练习2：形状继承体系
创建一个形状继承体系，包含 `Shape`（抽象基类）、`Circle`、`Rectangle`、`Triangle`，每个都实现 `area()` 和 `perimeter()` 方法。

### 练习3：复数运算
完善 `Complex` 类，添加乘法、除法、取模等运算符。

### 练习4：日期类
设计一个 `Date` 类，支持日期的加减运算、比较运算、格式化输出。

---

## 💡 要点总结

1. **使用成员初始化列表**：效率更高，某些成员必须在此初始化
2. **遵循五法则**：如果定义了析构/拷贝/移动之一，考虑定义全部
3. **使用 override 和 final**：让编译器帮助检查重写错误
4. **使用虚析构函数**：多态基类必须有虚析构函数
5. **使用 explicit**：防止意外的隐式类型转换
6. **优先使用组合而非继承**：除非真正是 "is-a" 关系
7. **使用智能指针管理资源**：见第二章

---

## ⏭️ 下一节

[1.4 模板基础](./04_templates_basics.md) - C++泛型编程的基石

