// 练习2：结构体与数组练习
// 对应教程：第2课 - 结构体与数组

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * 练习说明：
 * 本练习将帮助你掌握结构体和数组的使用，包括：
 * - 结构体的定义和使用
 * - 结构体数组的操作
 * - 数据的排序和查找
 * - 内存管理
 */

// 图书结构体
typedef struct {
    char isbn[20];      // ISBN号
    char title[100];    // 书名
    char author[50];    // 作者
    float price;        // 价格
    int year;          // 出版年份
    int stock;         // 库存数量
} Book;

// 图书管理系统
typedef struct {
    Book *books;        // 动态分配的图书数组
    int count;          // 当前图书数量
    int capacity;       // 数组容量
} Library;

// 学生结构体（用于后续练习）
typedef struct {
    int id;
    char name[50];
    int age;
    float gpa;
} Student;

// 函数声明
int init_library(Library *lib, int initial_capacity);
void cleanup_library(Library *lib);
int add_book(Library *lib, const Book *book);
int remove_book(Library *lib, const char *isbn);
Book* find_book_by_isbn(Library *lib, const char *isbn);
Book* find_books_by_author(Library *lib, const char *author, int *count);
void sort_books_by_price(Library *lib);
void sort_books_by_year(Library *lib);
void display_book(const Book *book);
void display_all_books(const Library *lib);
float calculate_total_value(const Library *lib);
int count_books_by_year_range(const Library *lib, int start_year, int end_year);
void update_book_stock(Library *lib, const char *isbn, int new_stock);
Book* get_most_expensive_book(const Library *lib);
Book* get_oldest_book(const Library *lib);

// 学生相关函数（额外练习）
void sort_students_by_gpa(Student students[], int count);
Student* find_top_student(Student students[], int count);
double calculate_average_gpa(Student students[], int count);

// 练习2.1：初始化图书管理系统
// 要求：动态分配内存，设置初始容量
int init_library(Library *lib, int initial_capacity) {
    // TODO: 实现图书管理系统初始化
    // 提示：
    // 1. 使用 malloc 分配内存
    // 2. 设置 count 为 0
    // 3. 设置 capacity 为 initial_capacity
    // 4. 检查内存分配是否成功
    return 0; // 成功返回0，失败返回-1
}

// 练习2.2：清理图书管理系统
// 要求：释放动态分配的内存
void cleanup_library(Library *lib) {
    // TODO: 实现内存清理
    // 提示：
    // 1. 释放 books 数组的内存
    // 2. 将指针设置为 NULL
    // 3. 重置 count 和 capacity
}

// 练习2.3：添加图书
// 要求：如果容量不足，自动扩展数组
int add_book(Library *lib, const Book *book) {
    // TODO: 实现添加图书功能
    // 提示：
    // 1. 检查是否需要扩展容量
    // 2. 如果需要，使用 realloc 扩展数组
    // 3. 复制图书数据到数组中
    // 4. 更新 count
    return 0; // 成功返回0，失败返回-1
}

// 练习2.4：删除图书
// 要求：根据ISBN删除图书，保持数组紧凑
int remove_book(Library *lib, const char *isbn) {
    // TODO: 实现删除图书功能
    // 提示：
    // 1. 找到要删除的图书
    // 2. 将后面的元素前移
    // 3. 更新 count
    return 0; // 成功返回0，未找到返回-1
}

// 练习2.5：按ISBN查找图书
// 要求：返回指向找到图书的指针，未找到返回NULL
Book* find_book_by_isbn(Library *lib, const char *isbn) {
    // TODO: 实现按ISBN查找
    // 提示：遍历数组，使用 strcmp 比较ISBN
    return NULL;
}

// 练习2.6：按作者查找图书
// 要求：返回动态分配的数组，包含所有匹配的图书
Book* find_books_by_author(Library *lib, const char *author, int *count) {
    // TODO: 实现按作者查找
    // 提示：
    // 1. 先遍历一遍统计匹配的数量
    // 2. 分配内存存储结果
    // 3. 再次遍历复制匹配的图书
    // 4. 设置 count 参数
    *count = 0;
    return NULL;
}

// 练习2.7：按价格排序图书
// 要求：使用冒泡排序或选择排序，从低到高排序
void sort_books_by_price(Library *lib) {
    // TODO: 实现按价格排序
    // 提示：使用简单的排序算法，比较 price 字段
}

// 练习2.8：按出版年份排序图书
// 要求：从新到旧排序
void sort_books_by_year(Library *lib) {
    // TODO: 实现按年份排序
    // 提示：比较 year 字段，新书在前
}

// 练习2.9：计算图书总价值
// 要求：计算所有图书的总价值（价格 × 库存）
float calculate_total_value(const Library *lib) {
    // TODO: 实现总价值计算
    // 提示：遍历所有图书，累加 price * stock
    return 0.0;
}

// 练习2.10：统计指定年份范围内的图书数量
int count_books_by_year_range(const Library *lib, int start_year, int end_year) {
    // TODO: 实现年份范围统计
    // 提示：遍历图书，检查年份是否在范围内
    return 0;
}

// 练习2.11：更新图书库存
void update_book_stock(Library *lib, const char *isbn, int new_stock) {
    // TODO: 实现库存更新
    // 提示：先找到图书，然后更新 stock 字段
}

// 练习2.12：找到最贵的图书
Book* get_most_expensive_book(const Library *lib) {
    // TODO: 实现查找最贵图书
    // 提示：遍历数组，记录最大价格的图书
    return NULL;
}

// 练习2.13：找到最老的图书
Book* get_oldest_book(const Library *lib) {
    // TODO: 实现查找最老图书
    // 提示：遍历数组，记录最小年份的图书
    return NULL;
}

// 额外练习：学生GPA排序
void sort_students_by_gpa(Student students[], int count) {
    // TODO: 按GPA从高到低排序学生数组
    // 提示：使用冒泡排序或选择排序
}

// 额外练习：找到GPA最高的学生
Student* find_top_student(Student students[], int count) {
    // TODO: 找到GPA最高的学生
    return NULL;
}

// 额外练习：计算平均GPA
double calculate_average_gpa(Student students[], int count) {
    // TODO: 计算所有学生的平均GPA
    return 0.0;
}

// 辅助函数：显示图书信息
void display_book(const Book *book) {
    if (book) {
        printf("ISBN: %s\n", book->isbn);
        printf("书名: %s\n", book->title);
        printf("作者: %s\n", book->author);
        printf("价格: %.2f元\n", book->price);
        printf("出版年份: %d\n", book->year);
        printf("库存: %d本\n", book->stock);
        printf("------------------------\n");
    }
}

// 辅助函数：显示所有图书
void display_all_books(const Library *lib) {
    printf("=== 图书列表 ===\n");
    for (int i = 0; i < lib->count; i++) {
        printf("图书 %d:\n", i + 1);
        display_book(&lib->books[i]);
    }
}

// 测试函数
void test_library_functions() {
    printf("=== 图书管理系统测试 ===\n\n");
    
    Library lib;
    
    // 测试初始化
    if (init_library(&lib, 5) == 0) {
        printf("✓ 图书管理系统初始化成功\n");
    } else {
        printf("✗ 图书管理系统初始化失败\n");
        return;
    }
    
    // 创建测试图书
    Book books[] = {
        {"978-0-123456-78-9", "C程序设计语言", "Dennis Ritchie", 89.00, 1978, 10},
        {"978-0-234567-89-0", "算法导论", "Thomas Cormen", 128.00, 2009, 5},
        {"978-0-345678-90-1", "深入理解计算机系统", "Randal Bryant", 139.00, 2015, 8},
        {"978-0-456789-01-2", "设计模式", "Gang of Four", 79.00, 1994, 12},
        {"978-0-567890-12-3", "重构", "Martin Fowler", 99.00, 1999, 6}
    };
    
    // 测试添加图书
    printf("\n添加图书测试:\n");
    for (int i = 0; i < 5; i++) {
        if (add_book(&lib, &books[i]) == 0) {
            printf("✓ 成功添加图书: %s\n", books[i].title);
        } else {
            printf("✗ 添加图书失败: %s\n", books[i].title);
        }
    }
    
    // 显示所有图书
    printf("\n");
    display_all_books(&lib);
    
    // 测试查找功能
    printf("\n查找测试:\n");
    Book *found = find_book_by_isbn(&lib, "978-0-123456-78-9");
    if (found) {
        printf("✓ 找到图书: %s\n", found->title);
    } else {
        printf("✗ 未找到指定ISBN的图书\n");
    }
    
    // 测试统计功能
    printf("\n统计测试:\n");
    float total_value = calculate_total_value(&lib);
    printf("图书总价值: %.2f元\n", total_value);
    
    Book *most_expensive = get_most_expensive_book(&lib);
    if (most_expensive) {
        printf("最贵的图书: %s (%.2f元)\n", most_expensive->title, most_expensive->price);
    }
    
    Book *oldest = get_oldest_book(&lib);
    if (oldest) {
        printf("最老的图书: %s (%d年)\n", oldest->title, oldest->year);
    }
    
    // 测试排序
    printf("\n排序测试:\n");
    sort_books_by_price(&lib);
    printf("按价格排序后的图书列表:\n");
    for (int i = 0; i < lib.count; i++) {
        printf("%d. %s - %.2f元\n", i+1, lib.books[i].title, lib.books[i].price);
    }
    
    // 清理资源
    cleanup_library(&lib);
    printf("\n✓ 资源清理完成\n");
}

void test_student_functions() {
    printf("\n=== 学生管理测试 ===\n\n");
    
    Student students[] = {
        {1001, "张三", 20, 3.8},
        {1002, "李四", 21, 3.6},
        {1003, "王五", 19, 3.9},
        {1004, "赵六", 22, 3.5},
        {1005, "钱七", 20, 3.7}
    };
    
    int count = sizeof(students) / sizeof(students[0]);
    
    printf("原始学生列表:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s (ID: %d, 年龄: %d, GPA: %.1f)\n", 
               i+1, students[i].name, students[i].id, students[i].age, students[i].gpa);
    }
    
    // 测试排序
    sort_students_by_gpa(students, count);
    printf("\n按GPA排序后:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s (GPA: %.1f)\n", i+1, students[i].name, students[i].gpa);
    }
    
    // 测试查找最高GPA
    Student *top = find_top_student(students, count);
    if (top) {
        printf("\nGPA最高的学生: %s (%.1f)\n", top->name, top->gpa);
    }
    
    // 测试平均GPA
    double avg_gpa = calculate_average_gpa(students, count);
    printf("平均GPA: %.2f\n", avg_gpa);
}

int main() {
    printf("结构体与数组练习\n");
    printf("==================\n\n");
    
    printf("请完成上述函数的实现，然后运行测试。\n");
    printf("如果你已经完成了实现，请取消下面这些行的注释：\n");
    printf("// test_library_functions();\n");
    printf("// test_student_functions();\n\n");
    
    // 取消注释来运行测试
    // test_library_functions();
    // test_student_functions();
    
    return 0;
}

/*
 * 练习提示：
 * 
 * 1. 动态内存管理：
 *    - 使用 malloc() 分配内存
 *    - 使用 realloc() 扩展内存
 *    - 使用 free() 释放内存
 *    - 始终检查内存分配是否成功
 * 
 * 2. 结构体操作：
 *    - 使用 . 操作符访问结构体成员
 *    - 使用 -> 操作符访问指针指向的结构体成员
 *    - 使用 memcpy() 或直接赋值复制结构体
 * 
 * 3. 数组操作：
 *    - 注意数组边界检查
 *    - 删除元素时要移动后续元素
 *    - 排序时可以使用简单的冒泡排序或选择排序
 * 
 * 4. 字符串比较：
 *    - 使用 strcmp() 比较字符串
 *    - 使用 strcpy() 复制字符串
 * 
 * 编译运行：
 * gcc -o exercise_02 exercise_02.c
 * ./exercise_02
 */