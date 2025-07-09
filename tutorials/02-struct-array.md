# 第2课：结构体与数组

> **学习目标**: 学习如何使用结构体和数组来设计学生管理系统的数据结构

## 📋 本课内容

1. [结构体基础](#结构体基础)
2. [学生信息结构体设计](#学生信息结构体设计)
3. [结构体数组](#结构体数组)
4. [系统数据结构设计](#系统数据结构设计)
5. [实践练习](#实践练习)

---

## 结构体基础

### 什么是结构体？

结构体是C语言中用来组织相关数据的复合数据类型。它允许我们将不同类型的数据组合在一起。

```c
#include <stdio.h>

// 定义一个简单的结构体
struct Point {
    int x;
    int y;
};

int main() {
    // 声明结构体变量
    struct Point p1;
    
    // 给结构体成员赋值
    p1.x = 10;
    p1.y = 20;
    
    // 初始化结构体
    struct Point p2 = {30, 40};
    
    printf("点1: (%d, %d)\n", p1.x, p1.y);
    printf("点2: (%d, %d)\n", p2.x, p2.y);
    
    return 0;
}
```

### 使用typedef简化

```c
#include <stdio.h>

// 使用typedef定义结构体类型
typedef struct {
    int x;
    int y;
} Point;

int main() {
    // 现在可以直接使用Point，不需要struct关键字
    Point p1 = {10, 20};
    Point p2;
    
    p2.x = 30;
    p2.y = 40;
    
    printf("点1: (%d, %d)\n", p1.x, p1.y);
    printf("点2: (%d, %d)\n", p2.x, p2.y);
    
    return 0;
}
```

---

## 学生信息结构体设计

### 分析学生信息需求

在设计学生管理系统时，我们需要存储以下信息：
- 学号（字符串）
- 姓名（字符串）
- 年龄（整数）
- 各科成绩（浮点数）
- 平均成绩（浮点数）

### 第一版：基本学生结构体

```c
#include <stdio.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_ID_LEN 20

// 定义学生结构体
typedef struct {
    char id[MAX_ID_LEN];        // 学号
    char name[MAX_NAME_LEN];    // 姓名
    int age;                    // 年龄
    float math_score;           // 数学成绩
    float english_score;        // 英语成绩
    float chinese_score;        // 语文成绩
    float average_score;        // 平均成绩
} Student;

// 计算平均成绩的函数
void calculate_average(Student *student) {
    student->average_score = (student->math_score + 
                             student->english_score + 
                             student->chinese_score) / 3.0;
}

// 打印学生信息的函数
void print_student(Student *student) {
    printf("学号: %s\n", student->id);
    printf("姓名: %s\n", student->name);
    printf("年龄: %d\n", student->age);
    printf("数学: %.2f\n", student->math_score);
    printf("英语: %.2f\n", student->english_score);
    printf("语文: %.2f\n", student->chinese_score);
    printf("平均分: %.2f\n", student->average_score);
}

int main() {
    // 创建一个学生
    Student s1;
    
    // 输入学生信息
    strcpy(s1.id, "2023001");
    strcpy(s1.name, "张三");
    s1.age = 20;
    s1.math_score = 85.5;
    s1.english_score = 92.0;
    s1.chinese_score = 88.5;
    
    // 计算平均分
    calculate_average(&s1);
    
    // 打印学生信息
    printf("=== 学生信息 ===\n");
    print_student(&s1);
    
    return 0;
}
```

### 为什么使用指针？

```c
// 不使用指针 - 值传递（不推荐）
void calculate_average_bad(Student student) {
    student.average_score = (student.math_score + 
                            student.english_score + 
                            student.chinese_score) / 3.0;
    // 这里的修改不会影响原始结构体！
}

// 使用指针 - 地址传递（推荐）
void calculate_average_good(Student *student) {
    student->average_score = (student->math_score + 
                             student->english_score + 
                             student->chinese_score) / 3.0;
    // 这里的修改会影响原始结构体
}
```

**💡 使用指针的优势**:
1. **效率高**: 不需要复制整个结构体
2. **可修改**: 函数可以修改原始数据
3. **内存省**: 只传递地址，不复制数据

---

## 结构体数组

### 管理多个学生

```c
#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LEN 50
#define MAX_ID_LEN 20

typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    int age;
    float math_score;
    float english_score;
    float chinese_score;
    float average_score;
} Student;

// 计算平均成绩
void calculate_average(Student *student) {
    student->average_score = (student->math_score + 
                             student->english_score + 
                             student->chinese_score) / 3.0;
}

// 添加学生信息
void input_student(Student *student) {
    printf("请输入学号: ");
    scanf("%s", student->id);
    
    printf("请输入姓名: ");
    scanf("%s", student->name);
    
    printf("请输入年龄: ");
    scanf("%d", &student->age);
    
    printf("请输入数学成绩: ");
    scanf("%f", &student->math_score);
    
    printf("请输入英语成绩: ");
    scanf("%f", &student->english_score);
    
    printf("请输入语文成绩: ");
    scanf("%f", &student->chinese_score);
    
    // 计算平均分
    calculate_average(student);
}

// 显示所有学生信息
void display_all_students(Student students[], int count) {
    printf("\n=== 所有学生信息 ===\n");
    printf("%-10s %-15s %-5s %-8s %-8s %-8s %-8s\n", 
           "学号", "姓名", "年龄", "数学", "英语", "语文", "平均分");
    printf("----------------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-10s %-15s %-5d %-8.2f %-8.2f %-8.2f %-8.2f\n",
               students[i].id, students[i].name, students[i].age,
               students[i].math_score, students[i].english_score,
               students[i].chinese_score, students[i].average_score);
    }
}

int main() {
    Student students[MAX_STUDENTS];  // 学生数组
    int student_count = 0;           // 当前学生数量
    
    printf("请输入学生数量: ");
    scanf("%d", &student_count);
    
    // 输入所有学生信息
    for (int i = 0; i < student_count; i++) {
        printf("\n--- 输入第%d个学生信息 ---\n", i + 1);
        input_student(&students[i]);
    }
    
    // 显示所有学生信息
    display_all_students(students, student_count);
    
    return 0;
}
```

---

## 系统数据结构设计

### 完整的系统结构体

```c
#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LEN 50
#define MAX_ID_LEN 20

// 学生结构体
typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    int age;
    float math_score;
    float english_score;
    float chinese_score;
    float average_score;
} Student;

// 学生管理系统结构体
typedef struct {
    Student students[MAX_STUDENTS];  // 学生数组
    int count;                       // 当前学生数量
} StudentSystem;

// 初始化系统
void init_system(StudentSystem *system) {
    system->count = 0;
    // 清空学生数组
    memset(system->students, 0, sizeof(system->students));
}

// 添加学生
int add_student(StudentSystem *system, Student *new_student) {
    if (system->count >= MAX_STUDENTS) {
        printf("错误：学生数量已达上限！\n");
        return 0;  // 添加失败
    }
    
    // 复制学生信息
    system->students[system->count] = *new_student;
    system->count++;
    
    printf("学生添加成功！\n");
    return 1;  // 添加成功
}

// 查找学生（按学号）
int find_student_by_id(StudentSystem *system, const char *id) {
    for (int i = 0; i < system->count; i++) {
        if (strcmp(system->students[i].id, id) == 0) {
            return i;  // 返回学生在数组中的索引
        }
    }
    return -1;  // 未找到
}

// 删除学生
int delete_student(StudentSystem *system, const char *id) {
    int index = find_student_by_id(system, id);
    if (index == -1) {
        printf("未找到该学生！\n");
        return 0;
    }
    
    // 将后面的学生前移
    for (int i = index; i < system->count - 1; i++) {
        system->students[i] = system->students[i + 1];
    }
    
    system->count--;
    printf("学生删除成功！\n");
    return 1;
}

// 显示系统统计信息
void show_system_stats(StudentSystem *system) {
    printf("\n=== 系统统计 ===\n");
    printf("当前学生数量: %d\n", system->count);
    printf("系统容量: %d\n", MAX_STUDENTS);
    printf("剩余容量: %d\n", MAX_STUDENTS - system->count);
    
    if (system->count > 0) {
        float total_average = 0;
        for (int i = 0; i < system->count; i++) {
            total_average += system->students[i].average_score;
        }
        printf("全班平均分: %.2f\n", total_average / system->count);
    }
}

int main() {
    StudentSystem system;
    init_system(&system);
    
    // 创建几个测试学生
    Student s1 = {"2023001", "张三", 20, 85.0, 92.0, 88.0, 0};
    Student s2 = {"2023002", "李四", 19, 78.0, 85.0, 90.0, 0};
    Student s3 = {"2023003", "王五", 21, 92.0, 88.0, 85.0, 0};
    
    // 计算平均分
    s1.average_score = (s1.math_score + s1.english_score + s1.chinese_score) / 3.0;
    s2.average_score = (s2.math_score + s2.english_score + s2.chinese_score) / 3.0;
    s3.average_score = (s3.math_score + s3.english_score + s3.chinese_score) / 3.0;
    
    // 添加学生到系统
    add_student(&system, &s1);
    add_student(&system, &s2);
    add_student(&system, &s3);
    
    // 显示统计信息
    show_system_stats(&system);
    
    // 测试查找功能
    int index = find_student_by_id(&system, "2023002");
    if (index != -1) {
        printf("\n找到学生: %s\n", system.students[index].name);
    }
    
    // 测试删除功能
    delete_student(&system, "2023002");
    show_system_stats(&system);
    
    return 0;
}
```

---

## 实践练习

### 练习1：扩展学生结构体

为学生结构体添加更多字段：

```c
// 要求：
// 1. 添加性别字段（char gender）
// 2. 添加班级字段（char class_name[20]）
// 3. 添加入学日期字段（可以用字符串表示）
// 4. 修改相应的输入和显示函数

typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    // 在这里添加新字段
    
    int age;
    float math_score;
    float english_score;
    float chinese_score;
    float average_score;
} Student;

// 修改输入函数
void input_student_extended(Student *student) {
    // 实现扩展的输入功能
}

// 修改显示函数
void print_student_extended(Student *student) {
    // 实现扩展的显示功能
}
```

### 练习2：成绩统计功能

实现以下统计功能：

```c
// 要求：
// 1. 找出平均分最高的学生
// 2. 找出平均分最低的学生
// 3. 计算各科的平均分
// 4. 统计各个分数段的人数（优秀、良好、及格、不及格）

// 找出最高分学生
Student* find_highest_student(StudentSystem *system) {
    // 实现查找逻辑
    return NULL;
}

// 找出最低分学生
Student* find_lowest_student(StudentSystem *system) {
    // 实现查找逻辑
    return NULL;
}

// 计算各科平均分
void calculate_subject_averages(StudentSystem *system) {
    // 实现计算逻辑
}

// 统计分数段
void count_grade_levels(StudentSystem *system) {
    // 实现统计逻辑
    // 优秀: >=90, 良好: 80-89, 及格: 60-79, 不及格: <60
}
```

### 练习3：学生排序

实现按不同条件排序的功能：

```c
// 要求：
// 1. 按平均分排序（升序和降序）
// 2. 按学号排序
// 3. 按姓名排序（字典序）

// 交换两个学生的位置
void swap_students(Student *a, Student *b) {
    // 实现交换逻辑
}

// 按平均分排序（冒泡排序）
void sort_by_average(StudentSystem *system, int ascending) {
    // ascending: 1表示升序，0表示降序
    // 实现排序逻辑
}

// 按学号排序
void sort_by_id(StudentSystem *system) {
    // 实现排序逻辑
}
```

---

## 💡 本课要点总结

### 重要概念
1. **结构体**: 组织相关数据的复合类型
2. **typedef**: 简化类型定义
3. **结构体指针**: 高效传递和修改结构体
4. **结构体数组**: 管理多个相同类型的数据

### 设计原则
1. **数据封装**: 将相关数据组织在一起
2. **模块化**: 为不同功能编写独立函数
3. **可扩展性**: 设计时考虑未来的扩展需求
4. **效率**: 使用指针避免不必要的数据复制

### 常见错误
1. **忘记初始化**: 结构体变量使用前要初始化
2. **数组越界**: 确保数组索引在有效范围内
3. **指针错误**: 确保指针指向有效内存
4. **字符串处理**: 使用strcpy而不是直接赋值

### 内存布局理解

```c
// 结构体在内存中的布局
typedef struct {
    char id[20];      // 20字节
    char name[50];    // 50字节
    int age;          // 4字节
    float score;      // 4字节
} Student;            // 总共约78字节（可能有内存对齐）

// 数组在内存中是连续的
Student students[100]; // 约7800字节的连续内存
```

---

## 🚀 下一步

完成本课学习后，你应该能够：
- ✅ 设计和使用结构体
- ✅ 管理结构体数组
- ✅ 实现基本的增删改查功能
- ✅ 理解指针在结构体操作中的作用

**准备好了吗？** 让我们进入 [第3课：文件操作基础](./03-file-operations.md)，学习如何将数据保存到文件中！

---

*💭 思考题：为什么我们要设计一个包含学生数组和计数器的系统结构体，而不是直接使用全局变量？*