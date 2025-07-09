// 基础示例：C语言基本语法演示
// 对应教程：第1课 - C语言基础回顾

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 函数声明
void demonstrate_variables();
void demonstrate_functions();
void demonstrate_pointers();
void demonstrate_strings();

// 主函数
int main() {
    printf("=== C语言基础示例 ===\n\n");
    
    demonstrate_variables();
    demonstrate_functions();
    demonstrate_pointers();
    demonstrate_strings();
    
    return 0;
}

// 演示变量和数据类型
void demonstrate_variables() {
    printf("1. 变量和数据类型演示\n");
    printf("------------------------\n");
    
    // 基本数据类型
    int age = 20;
    float score = 85.5;
    double average = 87.25;
    char grade = 'A';
    
    printf("整数: %d\n", age);
    printf("浮点数: %.1f\n", score);
    printf("双精度: %.2f\n", average);
    printf("字符: %c\n", grade);
    
    // 数组
    int numbers[5] = {1, 2, 3, 4, 5};
    printf("数组: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n\n");
}

// 演示函数
void demonstrate_functions() {
    printf("2. 函数演示\n");
    printf("------------\n");
    
    // 调用自定义函数
    int a = 10, b = 20;
    int sum = add_numbers(a, b);
    printf("%d + %d = %d\n", a, b, sum);
    
    // 计算平均分
    float scores[] = {85.5, 92.0, 78.5};
    float avg = calculate_average(scores, 3);
    printf("平均分: %.2f\n\n", avg);
}

// 演示指针
void demonstrate_pointers() {
    printf("3. 指针演示\n");
    printf("------------\n");
    
    int value = 42;
    int *ptr = &value;
    
    printf("变量值: %d\n", value);
    printf("变量地址: %p\n", (void*)&value);
    printf("指针值: %p\n", (void*)ptr);
    printf("指针指向的值: %d\n", *ptr);
    
    // 通过指针修改值
    *ptr = 100;
    printf("修改后的值: %d\n\n", value);
}

// 演示字符串
void demonstrate_strings() {
    printf("4. 字符串演示\n");
    printf("--------------\n");
    
    char name[50] = "张三";
    char greeting[100];
    
    printf("姓名: %s\n", name);
    printf("姓名长度: %zu\n", strlen(name));
    
    // 字符串拼接
    strcpy(greeting, "你好, ");
    strcat(greeting, name);
    printf("问候语: %s\n", greeting);
    
    // 字符串比较
    if (strcmp(name, "张三") == 0) {
        printf("姓名匹配成功\n");
    }
    
    printf("\n");
}

// 辅助函数：加法
int add_numbers(int a, int b) {
    return a + b;
}

// 辅助函数：计算平均值
float calculate_average(float scores[], int count) {
    float sum = 0;
    for (int i = 0; i < count; i++) {
        sum += scores[i];
    }
    return sum / count;
}