# 第1课：C语言基础回顾

> **学习目标**: 回顾C语言的基础知识，为构建学生管理系统做准备

## 📋 本课内容

1. [数据类型与变量](#数据类型与变量)
2. [函数的定义与调用](#函数的定义与调用)
3. [指针基础](#指针基础)
4. [字符串处理](#字符串处理)
5. [实践练习](#实践练习)

---

## 数据类型与变量

### 基本数据类型

在学生管理系统中，我们会用到以下数据类型：

```c
#include <stdio.h>

int main() {
    // 整型 - 用于年龄、学生数量等
    int age = 20;
    int student_count = 0;
    
    // 浮点型 - 用于成绩
    float math_score = 85.5;
    float average = 88.67;
    
    // 字符型 - 用于单个字符
    char grade = 'A';
    
    // 字符数组 - 用于姓名、学号等
    char name[50] = "张三";
    char student_id[20] = "2023001";
    
    printf("学生信息:\n");
    printf("姓名: %s\n", name);
    printf("学号: %s\n", student_id);
    printf("年龄: %d\n", age);
    printf("数学成绩: %.2f\n", math_score);
    printf("等级: %c\n", grade);
    
    return 0;
}
```

### 常量定义

使用`#define`定义常量，提高代码可维护性：

```c
#include <stdio.h>

// 定义常量
#define MAX_STUDENTS 100
#define MAX_NAME_LEN 50
#define MAX_ID_LEN 20
#define PASS_SCORE 60.0

int main() {
    printf("系统配置:\n");
    printf("最大学生数: %d\n", MAX_STUDENTS);
    printf("姓名最大长度: %d\n", MAX_NAME_LEN);
    printf("及格分数: %.1f\n", PASS_SCORE);
    
    return 0;
}
```

**💡 为什么使用常量？**
- 提高代码可读性
- 便于统一修改
- 避免魔法数字

---

## 函数的定义与调用

### 基本函数结构

```c
#include <stdio.h>

// 函数声明
float calculate_average(float score1, float score2, float score3);
void print_student_info(char name[], int age, float average);
int is_pass(float score);

// 主函数
int main() {
    char student_name[] = "李四";
    int student_age = 19;
    float math = 85.0, english = 92.0, chinese = 78.0;
    
    // 调用函数计算平均分
    float avg = calculate_average(math, english, chinese);
    
    // 调用函数打印信息
    print_student_info(student_name, student_age, avg);
    
    // 判断是否及格
    if (is_pass(avg)) {
        printf("结果: 及格\n");
    } else {
        printf("结果: 不及格\n");
    }
    
    return 0;
}

// 计算平均分
float calculate_average(float score1, float score2, float score3) {
    return (score1 + score2 + score3) / 3.0;
}

// 打印学生信息
void print_student_info(char name[], int age, float average) {
    printf("\n=== 学生信息 ===\n");
    printf("姓名: %s\n", name);
    printf("年龄: %d\n", age);
    printf("平均分: %.2f\n", average);
}

// 判断是否及格
int is_pass(float score) {
    return score >= 60.0;  // 返回1表示及格，0表示不及格
}
```

### 函数设计原则

1. **单一职责**: 每个函数只做一件事
2. **参数合理**: 参数数量不宜过多
3. **返回值明确**: 清楚函数返回什么
4. **命名规范**: 函数名要能表达其功能

---

## 指针基础

### 指针的基本概念

```c
#include <stdio.h>

void modify_score(float *score) {
    *score = *score + 5.0;  // 通过指针修改原变量的值
}

void swap_scores(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

int main() {
    float math_score = 85.0;
    float english_score = 78.0;
    
    printf("修改前:\n");
    printf("数学: %.1f, 英语: %.1f\n", math_score, english_score);
    
    // 使用指针修改分数
    modify_score(&math_score);
    
    // 交换两个分数
    swap_scores(&math_score, &english_score);
    
    printf("修改后:\n");
    printf("数学: %.1f, 英语: %.1f\n", math_score, english_score);
    
    return 0;
}
```

**🔍 指针的作用**:
- 允许函数修改传入参数的值
- 提高大数据结构传递的效率
- 实现动态内存分配

---

## 字符串处理

### 常用字符串函数

```c
#include <stdio.h>
#include <string.h>

int main() {
    char name1[50] = "张三";
    char name2[50];
    char student_id[20] = "2023";
    char class_id[10] = "001";
    
    // 字符串复制
    strcpy(name2, name1);
    printf("复制后: %s\n", name2);
    
    // 字符串连接
    strcat(student_id, class_id);
    printf("完整学号: %s\n", student_id);
    
    // 字符串比较
    if (strcmp(name1, name2) == 0) {
        printf("两个姓名相同\n");
    }
    
    // 字符串长度
    printf("姓名长度: %lu\n", strlen(name1));
    
    return 0;
}
```

### 安全的字符串输入

```c
#include <stdio.h>
#include <string.h>

void safe_input(char *buffer, int size) {
    fgets(buffer, size, stdin);
    // 移除换行符
    buffer[strcspn(buffer, "\n")] = '\0';
}

int main() {
    char name[50];
    
    printf("请输入姓名: ");
    safe_input(name, sizeof(name));
    
    printf("你输入的姓名是: %s\n", name);
    
    return 0;
}
```

---

## 实践练习

### 练习1：基本信息处理

编写一个程序，实现以下功能：

```c
// 练习要求：
// 1. 定义一个函数输入学生基本信息
// 2. 定义一个函数计算三科成绩的平均分
// 3. 定义一个函数判断学生等级（优秀>=90, 良好>=80, 及格>=60）
// 4. 在main函数中调用这些函数

#include <stdio.h>
#include <string.h>

// 在这里添加你的函数声明

int main() {
    // 在这里实现主逻辑
    return 0;
}

// 在这里实现你的函数
```

### 练习2：字符串验证

编写函数验证学号格式：

```c
// 要求：
// 1. 学号必须是4位数字开头（年份）
// 2. 后面跟3位数字（班级和序号）
// 3. 总长度为7位
// 例如：2023001 是有效的，abc123 是无效的

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_valid_student_id(char *id) {
    // 在这里实现验证逻辑
    return 0; // 返回1表示有效，0表示无效
}

int main() {
    char test_ids[][20] = {
        "2023001",
        "abc123",
        "202300",
        "20230012",
        "2023abc"
    };
    
    int count = sizeof(test_ids) / sizeof(test_ids[0]);
    
    for (int i = 0; i < count; i++) {
        printf("%s: %s\n", test_ids[i], 
               is_valid_student_id(test_ids[i]) ? "有效" : "无效");
    }
    
    return 0;
}
```

### 练习3：成绩统计

编写程序统计多个学生的成绩：

```c
// 要求：
// 1. 输入5个学生的数学成绩
// 2. 计算平均分、最高分、最低分
// 3. 统计及格人数

#include <stdio.h>

#define STUDENT_COUNT 5

int main() {
    float scores[STUDENT_COUNT];
    
    // 在这里实现功能
    
    return 0;
}
```

---

## 💡 本课要点总结

### 重要概念
1. **数据类型选择**: 根据数据特点选择合适的类型
2. **函数设计**: 遵循单一职责原则
3. **指针使用**: 理解指针的基本概念和用途
4. **字符串处理**: 掌握常用字符串函数

### 编程技巧
1. 使用有意义的变量名和函数名
2. 添加适当的注释
3. 进行输入验证
4. 处理边界情况

### 常见错误
1. **数组越界**: 确保数组索引在有效范围内
2. **字符串未终止**: 确保字符串以'\0'结尾
3. **指针未初始化**: 使用指针前要确保其指向有效内存
4. **格式化输出错误**: 确保printf格式符与变量类型匹配

---

## 🚀 下一步

完成本课练习后，你应该能够：
- ✅ 熟练使用C语言基本数据类型
- ✅ 编写和调用简单函数
- ✅ 理解指针的基本概念
- ✅ 处理字符串操作

**准备好了吗？** 让我们进入 [第2课：结构体与数组](./02-struct-array.md)，学习如何设计学生信息的数据结构！

---

*💭 思考题：为什么在学生管理系统中，我们选择使用字符数组而不是单个字符来存储姓名？*