// 练习1：C语言基础练习
// 对应教程：第1课 - C语言基础回顾

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * 练习说明：
 * 请完成以下函数的实现，每个函数都有详细的要求说明。
 * 完成后可以运行程序进行测试。
 */

// 练习1.1：实现一个函数，计算两个整数的最大公约数
// 要求：使用欧几里得算法（辗转相除法）
int gcd(int a, int b) {
    // TODO: 在这里实现最大公约数计算
    // 提示：如果 b == 0，返回 a；否则递归调用 gcd(b, a % b)
    return 0; // 请替换为正确的实现
}

// 练习1.2：实现一个函数，判断一个字符串是否为回文
// 要求：忽略大小写，只考虑字母和数字
int is_palindrome(const char *str) {
    // TODO: 在这里实现回文判断
    // 提示：
    // 1. 使用两个指针，一个从头开始，一个从尾开始
    // 2. 跳过非字母数字字符
    // 3. 比较字符时忽略大小写
    return 0; // 请替换为正确的实现（1表示是回文，0表示不是）
}

// 练习1.3：实现一个函数，计算数组中所有元素的平均值
// 要求：如果数组为空，返回0
double array_average(int arr[], int size) {
    // TODO: 在这里实现数组平均值计算
    // 提示：注意处理空数组的情况
    return 0.0; // 请替换为正确的实现
}

// 练习1.4：实现一个函数，在数组中查找指定元素的位置
// 要求：如果找到返回索引，如果没找到返回-1
int find_element(int arr[], int size, int target) {
    // TODO: 在这里实现元素查找
    // 提示：遍历数组，比较每个元素与目标值
    return -1; // 请替换为正确的实现
}

// 练习1.5：实现一个函数，将字符串中的所有字母转换为大写
// 要求：只转换字母，其他字符保持不变
void to_uppercase(char *str) {
    // TODO: 在这里实现字符串大写转换
    // 提示：遍历字符串，使用 toupper() 函数或手动转换
}

// 练习1.6：实现一个函数，计算斐波那契数列的第n项
// 要求：使用递归实现
long fibonacci(int n) {
    // TODO: 在这里实现斐波那契数列计算
    // 提示：
    // - 如果 n <= 1，返回 n
    // - 否则返回 fibonacci(n-1) + fibonacci(n-2)
    return 0; // 请替换为正确的实现
}

// 练习1.7：实现一个函数，反转整数数组
// 要求：原地反转，不使用额外数组
void reverse_array(int arr[], int size) {
    // TODO: 在这里实现数组反转
    // 提示：使用两个指针，一个指向开头，一个指向结尾，交换元素
}

// 练习1.8：实现一个函数，统计字符串中每个字符的出现次数
// 要求：只统计字母（忽略大小写），结果存储在count数组中
// count[0]对应'a'或'A'，count[1]对应'b'或'B'，以此类推
void count_characters(const char *str, int count[26]) {
    // TODO: 在这里实现字符统计
    // 提示：
    // 1. 初始化count数组为0
    // 2. 遍历字符串，对于每个字母，增加对应的计数
    // 3. 使用 tolower() 或手动转换来忽略大小写
}

// 测试函数
void test_functions() {
    printf("=== 练习测试 ===\n\n");
    
    // 测试最大公约数
    printf("1. 最大公约数测试:\n");
    printf("gcd(48, 18) = %d (期望: 6)\n", gcd(48, 18));
    printf("gcd(100, 25) = %d (期望: 25)\n", gcd(100, 25));
    printf("\n");
    
    // 测试回文判断
    printf("2. 回文判断测试:\n");
    printf("is_palindrome(\"racecar\") = %d (期望: 1)\n", is_palindrome("racecar"));
    printf("is_palindrome(\"hello\") = %d (期望: 0)\n", is_palindrome("hello"));
    printf("is_palindrome(\"A man a plan a canal Panama\") = %d (期望: 1)\n", 
           is_palindrome("A man a plan a canal Panama"));
    printf("\n");
    
    // 测试数组平均值
    printf("3. 数组平均值测试:\n");
    int arr1[] = {1, 2, 3, 4, 5};
    printf("array_average([1,2,3,4,5]) = %.2f (期望: 3.00)\n", 
           array_average(arr1, 5));
    int arr2[] = {10, 20, 30};
    printf("array_average([10,20,30]) = %.2f (期望: 20.00)\n", 
           array_average(arr2, 3));
    printf("\n");
    
    // 测试元素查找
    printf("4. 元素查找测试:\n");
    int arr3[] = {10, 20, 30, 40, 50};
    printf("find_element([10,20,30,40,50], 30) = %d (期望: 2)\n", 
           find_element(arr3, 5, 30));
    printf("find_element([10,20,30,40,50], 60) = %d (期望: -1)\n", 
           find_element(arr3, 5, 60));
    printf("\n");
    
    // 测试字符串大写转换
    printf("5. 字符串大写转换测试:\n");
    char str1[] = "Hello World!";
    printf("原字符串: %s\n", str1);
    to_uppercase(str1);
    printf("转换后: %s (期望: HELLO WORLD!)\n", str1);
    printf("\n");
    
    // 测试斐波那契数列
    printf("6. 斐波那契数列测试:\n");
    printf("fibonacci(0) = %ld (期望: 0)\n", fibonacci(0));
    printf("fibonacci(1) = %ld (期望: 1)\n", fibonacci(1));
    printf("fibonacci(10) = %ld (期望: 55)\n", fibonacci(10));
    printf("\n");
    
    // 测试数组反转
    printf("7. 数组反转测试:\n");
    int arr4[] = {1, 2, 3, 4, 5};
    printf("原数组: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr4[i]);
    }
    printf("\n");
    
    reverse_array(arr4, 5);
    printf("反转后: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr4[i]);
    }
    printf("(期望: 5 4 3 2 1)\n\n");
    
    // 测试字符统计
    printf("8. 字符统计测试:\n");
    int count[26] = {0};
    count_characters("Hello World", count);
    printf("字符串 \"Hello World\" 中的字母统计:\n");
    for (int i = 0; i < 26; i++) {
        if (count[i] > 0) {
            printf("%c: %d次\n", 'a' + i, count[i]);
        }
    }
    printf("\n");
}

int main() {
    printf("C语言基础练习\n");
    printf("================\n\n");
    
    printf("请完成上述函数的实现，然后运行测试。\n");
    printf("如果你已经完成了实现，请取消下面这行的注释：\n");
    printf("// test_functions();\n\n");
    
    // 取消注释来运行测试
    // test_functions();
    
    return 0;
}

/*
 * 练习提示：
 * 
 * 1. 最大公约数：使用递归的欧几里得算法
 * 2. 回文判断：注意处理大小写和非字母数字字符
 * 3. 数组平均值：注意除零错误
 * 4. 元素查找：简单的线性搜索
 * 5. 大写转换：可以使用 <ctype.h> 中的 toupper() 函数
 * 6. 斐波那契：递归实现，注意基础情况
 * 7. 数组反转：双指针技术
 * 8. 字符统计：使用数组作为计数器
 * 
 * 完成练习后，可以编译运行程序查看结果：
 * gcc -o exercise_01 exercise_01.c
 * ./exercise_01
 */