// 练习1解决方案：C语言基础练习答案
// 对应教程：第1课 - C语言基础回顾

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * 这是练习1的参考解决方案。
 * 每个函数都包含详细的注释说明实现思路。
 */

// 练习1.1：计算最大公约数（欧几里得算法）
int gcd(int a, int b) {
    // 欧几里得算法（辗转相除法）
    // 基本思想：gcd(a, b) = gcd(b, a % b)
    // 当 b = 0 时，gcd(a, 0) = a
    
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}

// 练习1.2：判断字符串是否为回文
int is_palindrome(const char *str) {
    if (!str) return 0;
    
    int len = strlen(str);
    int left = 0, right = len - 1;
    
    while (left < right) {
        // 跳过左边的非字母数字字符
        while (left < right && !isalnum(str[left])) {
            left++;
        }
        
        // 跳过右边的非字母数字字符
        while (left < right && !isalnum(str[right])) {
            right--;
        }
        
        // 比较字符（忽略大小写）
        if (tolower(str[left]) != tolower(str[right])) {
            return 0; // 不是回文
        }
        
        left++;
        right--;
    }
    
    return 1; // 是回文
}

// 练习1.3：计算数组平均值
double array_average(int arr[], int size) {
    if (size <= 0) {
        return 0.0; // 空数组返回0
    }
    
    long long sum = 0; // 使用long long避免溢出
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    
    return (double)sum / size;
}

// 练习1.4：在数组中查找元素
int find_element(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return i; // 返回找到的索引
        }
    }
    return -1; // 未找到
}

// 练习1.5：字符串转大写
void to_uppercase(char *str) {
    if (!str) return;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - 'a' + 'A'; // 手动转换
        }
        // 或者使用库函数：str[i] = toupper(str[i]);
    }
}

// 练习1.6：计算斐波那契数列（递归版本）
long fibonacci(int n) {
    // 基础情况
    if (n <= 1) {
        return n;
    }
    
    // 递归情况
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// 斐波那契数列的迭代版本（更高效）
long fibonacci_iterative(int n) {
    if (n <= 1) return n;
    
    long prev = 0, curr = 1;
    for (int i = 2; i <= n; i++) {
        long next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}

// 练习1.7：反转数组
void reverse_array(int arr[], int size) {
    if (size <= 1) return;
    
    int left = 0, right = size - 1;
    
    while (left < right) {
        // 交换元素
        int temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        
        left++;
        right--;
    }
}

// 练习1.8：统计字符出现次数
void count_characters(const char *str, int count[26]) {
    // 初始化计数数组
    for (int i = 0; i < 26; i++) {
        count[i] = 0;
    }
    
    if (!str) return;
    
    // 遍历字符串
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        
        // 检查是否为字母
        if (c >= 'a' && c <= 'z') {
            count[c - 'a']++;
        } else if (c >= 'A' && c <= 'Z') {
            count[c - 'A']++; // 转换为小写对应的索引
        }
    }
}

// 额外的辅助函数：打印数组
void print_array(int arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
}

// 额外的辅助函数：检查数组是否已排序
int is_sorted(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i-1]) {
            return 0; // 未排序
        }
    }
    return 1; // 已排序
}

// 测试函数
void test_functions() {
    printf("=== 练习解决方案测试 ===\n\n");
    
    // 测试最大公约数
    printf("1. 最大公约数测试:\n");
    printf("gcd(48, 18) = %d (期望: 6) %s\n", 
           gcd(48, 18), gcd(48, 18) == 6 ? "✓" : "✗");
    printf("gcd(100, 25) = %d (期望: 25) %s\n", 
           gcd(100, 25), gcd(100, 25) == 25 ? "✓" : "✗");
    printf("gcd(17, 13) = %d (期望: 1) %s\n", 
           gcd(17, 13), gcd(17, 13) == 1 ? "✓" : "✗");
    printf("\n");
    
    // 测试回文判断
    printf("2. 回文判断测试:\n");
    printf("is_palindrome(\"racecar\") = %d (期望: 1) %s\n", 
           is_palindrome("racecar"), is_palindrome("racecar") == 1 ? "✓" : "✗");
    printf("is_palindrome(\"hello\") = %d (期望: 0) %s\n", 
           is_palindrome("hello"), is_palindrome("hello") == 0 ? "✓" : "✗");
    printf("is_palindrome(\"A man a plan a canal Panama\") = %d (期望: 1) %s\n", 
           is_palindrome("A man a plan a canal Panama"), 
           is_palindrome("A man a plan a canal Panama") == 1 ? "✓" : "✗");
    printf("is_palindrome(\"race a car\") = %d (期望: 0) %s\n", 
           is_palindrome("race a car"), is_palindrome("race a car") == 0 ? "✓" : "✗");
    printf("\n");
    
    // 测试数组平均值
    printf("3. 数组平均值测试:\n");
    int arr1[] = {1, 2, 3, 4, 5};
    double avg1 = array_average(arr1, 5);
    printf("array_average([1,2,3,4,5]) = %.2f (期望: 3.00) %s\n", 
           avg1, (avg1 >= 2.99 && avg1 <= 3.01) ? "✓" : "✗");
    
    int arr2[] = {10, 20, 30};
    double avg2 = array_average(arr2, 3);
    printf("array_average([10,20,30]) = %.2f (期望: 20.00) %s\n", 
           avg2, (avg2 >= 19.99 && avg2 <= 20.01) ? "✓" : "✗");
    
    double avg3 = array_average(NULL, 0);
    printf("array_average([], 0) = %.2f (期望: 0.00) %s\n", 
           avg3, avg3 == 0.0 ? "✓" : "✗");
    printf("\n");
    
    // 测试元素查找
    printf("4. 元素查找测试:\n");
    int arr3[] = {10, 20, 30, 40, 50};
    printf("find_element([10,20,30,40,50], 30) = %d (期望: 2) %s\n", 
           find_element(arr3, 5, 30), find_element(arr3, 5, 30) == 2 ? "✓" : "✗");
    printf("find_element([10,20,30,40,50], 60) = %d (期望: -1) %s\n", 
           find_element(arr3, 5, 60), find_element(arr3, 5, 60) == -1 ? "✓" : "✗");
    printf("find_element([10,20,30,40,50], 10) = %d (期望: 0) %s\n", 
           find_element(arr3, 5, 10), find_element(arr3, 5, 10) == 0 ? "✓" : "✗");
    printf("\n");
    
    // 测试字符串大写转换
    printf("5. 字符串大写转换测试:\n");
    char str1[] = "Hello World!";
    printf("原字符串: %s\n", str1);
    to_uppercase(str1);
    printf("转换后: %s (期望: HELLO WORLD!) %s\n", 
           str1, strcmp(str1, "HELLO WORLD!") == 0 ? "✓" : "✗");
    
    char str2[] = "abc123XYZ";
    printf("原字符串: %s\n", str2);
    to_uppercase(str2);
    printf("转换后: %s (期望: ABC123XYZ) %s\n", 
           str2, strcmp(str2, "ABC123XYZ") == 0 ? "✓" : "✗");
    printf("\n");
    
    // 测试斐波那契数列
    printf("6. 斐波那契数列测试:\n");
    printf("fibonacci(0) = %ld (期望: 0) %s\n", 
           fibonacci(0), fibonacci(0) == 0 ? "✓" : "✗");
    printf("fibonacci(1) = %ld (期望: 1) %s\n", 
           fibonacci(1), fibonacci(1) == 1 ? "✓" : "✗");
    printf("fibonacci(10) = %ld (期望: 55) %s\n", 
           fibonacci(10), fibonacci(10) == 55 ? "✓" : "✗");
    printf("fibonacci(15) = %ld (期望: 610) %s\n", 
           fibonacci(15), fibonacci(15) == 610 ? "✓" : "✗");
    
    // 比较递归和迭代版本的性能
    printf("\n性能比较（计算fibonacci(30)）:\n");
    printf("递归版本: %ld\n", fibonacci(30));
    printf("迭代版本: %ld\n", fibonacci_iterative(30));
    printf("\n");
    
    // 测试数组反转
    printf("7. 数组反转测试:\n");
    int arr4[] = {1, 2, 3, 4, 5};
    printf("原数组: ");
    print_array(arr4, 5);
    
    reverse_array(arr4, 5);
    printf("反转后: ");
    print_array(arr4, 5);
    
    int expected[] = {5, 4, 3, 2, 1};
    int is_correct = 1;
    for (int i = 0; i < 5; i++) {
        if (arr4[i] != expected[i]) {
            is_correct = 0;
            break;
        }
    }
    printf("结果正确: %s\n", is_correct ? "✓" : "✗");
    
    // 测试偶数长度数组
    int arr5[] = {1, 2, 3, 4};
    printf("\n偶数长度数组测试:\n");
    printf("原数组: ");
    print_array(arr5, 4);
    reverse_array(arr5, 4);
    printf("反转后: ");
    print_array(arr5, 4);
    printf("\n");
    
    // 测试字符统计
    printf("8. 字符统计测试:\n");
    int count[26] = {0};
    const char *test_str = "Hello World";
    count_characters(test_str, count);
    
    printf("字符串 \"%s\" 中的字母统计:\n", test_str);
    for (int i = 0; i < 26; i++) {
        if (count[i] > 0) {
            printf("%c: %d次\n", 'a' + i, count[i]);
        }
    }
    
    // 验证特定字符的计数
    printf("\n验证结果:\n");
    printf("字母 'l' 出现次数: %d (期望: 3) %s\n", 
           count['l' - 'a'], count['l' - 'a'] == 3 ? "✓" : "✗");
    printf("字母 'o' 出现次数: %d (期望: 2) %s\n", 
           count['o' - 'a'], count['o' - 'a'] == 2 ? "✓" : "✗");
    printf("字母 'h' 出现次数: %d (期望: 1) %s\n", 
           count['h' - 'a'], count['h' - 'a'] == 1 ? "✓" : "✗");
    
    // 测试大小写混合
    int count2[26] = {0};
    count_characters("AaAa", count2);
    printf("字符串 \"AaAa\" 中 'a' 的出现次数: %d (期望: 4) %s\n", 
           count2[0], count2[0] == 4 ? "✓" : "✗");
    
    printf("\n=== 所有测试完成 ===\n");
}

int main() {
    printf("C语言基础练习 - 参考解决方案\n");
    printf("================================\n\n");
    
    test_functions();
    
    printf("\n=== 学习要点总结 ===\n");
    printf("1. 递归算法：欧几里得算法和斐波那契数列\n");
    printf("2. 字符串处理：回文判断、大小写转换、字符统计\n");
    printf("3. 数组操作：查找、反转、平均值计算\n");
    printf("4. 边界条件处理：空数组、空字符串等特殊情况\n");
    printf("5. 性能考虑：递归vs迭代、溢出防护\n");
    
    return 0;
}

/*
 * 解决方案说明：
 * 
 * 1. 最大公约数 (gcd):
 *    - 使用经典的欧几里得算法
 *    - 递归实现简洁明了
 *    - 时间复杂度: O(log(min(a,b)))
 * 
 * 2. 回文判断 (is_palindrome):
 *    - 双指针技术，从两端向中间移动
 *    - 跳过非字母数字字符
 *    - 使用 tolower() 忽略大小写
 * 
 * 3. 数组平均值 (array_average):
 *    - 注意处理空数组情况
 *    - 使用 long long 防止整数溢出
 *    - 返回 double 类型保证精度
 * 
 * 4. 元素查找 (find_element):
 *    - 简单的线性搜索
 *    - 时间复杂度: O(n)
 *    - 返回索引或-1表示未找到
 * 
 * 5. 大写转换 (to_uppercase):
 *    - 可以手动转换或使用 toupper()
 *    - 只转换小写字母，其他字符不变
 * 
 * 6. 斐波那契数列 (fibonacci):
 *    - 递归版本简洁但效率低
 *    - 迭代版本效率高，适合大数值
 *    - 注意递归的指数时间复杂度
 * 
 * 7. 数组反转 (reverse_array):
 *    - 双指针技术，原地反转
 *    - 时间复杂度: O(n/2) = O(n)
 *    - 空间复杂度: O(1)
 * 
 * 8. 字符统计 (count_characters):
 *    - 使用数组作为计数器
 *    - 将字符映射到数组索引
 *    - 忽略大小写统一计数
 */