// 练习2解决方案：结构体与数组练习答案
// 对应教程：第2课 - 结构体与数组基础

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * 这是练习2的参考解决方案。
 * 包含图书管理系统和学生管理系统的完整实现。
 */

// 图书结构体定义
typedef struct {
    int id;                    // 图书ID
    char title[100];          // 书名
    char author[50];          // 作者
    int year;                 // 出版年份
    double price;             // 价格
    int available;            // 是否可借（1-可借，0-已借出）
} Book;

// 图书馆结构体定义
typedef struct {
    Book books[1000];         // 图书数组
    int count;                // 当前图书数量
    int capacity;             // 最大容量
} Library;

// 学生结构体定义（额外练习）
typedef struct {
    int id;                   // 学生ID
    char name[50];           // 姓名
    double gpa;              // GPA成绩
    int age;                 // 年龄
} Student;

// === 图书管理系统函数实现 ===

// 初始化图书馆
void init_library(Library *lib) {
    if (!lib) return;
    
    lib->count = 0;
    lib->capacity = 1000;
    
    // 初始化所有图书为空
    for (int i = 0; i < lib->capacity; i++) {
        lib->books[i].id = 0;
        strcpy(lib->books[i].title, "");
        strcpy(lib->books[i].author, "");
        lib->books[i].year = 0;
        lib->books[i].price = 0.0;
        lib->books[i].available = 0;
    }
    
    printf("图书馆初始化完成，容量：%d本\n", lib->capacity);
}

// 清理图书馆（释放资源）
void cleanup_library(Library *lib) {
    if (!lib) return;
    
    printf("清理图书馆，共有%d本图书\n", lib->count);
    lib->count = 0;
}

// 添加图书
int add_book(Library *lib, int id, const char *title, const char *author, int year, double price) {
    if (!lib || !title || !author) {
        printf("错误：无效的参数\n");
        return 0;
    }
    
    if (lib->count >= lib->capacity) {
        printf("错误：图书馆已满，无法添加更多图书\n");
        return 0;
    }
    
    // 检查ID是否已存在
    for (int i = 0; i < lib->count; i++) {
        if (lib->books[i].id == id) {
            printf("错误：图书ID %d 已存在\n", id);
            return 0;
        }
    }
    
    // 添加新图书
    Book *new_book = &lib->books[lib->count];
    new_book->id = id;
    strncpy(new_book->title, title, sizeof(new_book->title) - 1);
    new_book->title[sizeof(new_book->title) - 1] = '\0';
    strncpy(new_book->author, author, sizeof(new_book->author) - 1);
    new_book->author[sizeof(new_book->author) - 1] = '\0';
    new_book->year = year;
    new_book->price = price;
    new_book->available = 1; // 新书默认可借
    
    lib->count++;
    printf("成功添加图书：《%s》 by %s\n", title, author);
    return 1;
}

// 删除图书
int remove_book(Library *lib, int id) {
    if (!lib) {
        printf("错误：无效的图书馆指针\n");
        return 0;
    }
    
    for (int i = 0; i < lib->count; i++) {
        if (lib->books[i].id == id) {
            printf("删除图书：《%s》\n", lib->books[i].title);
            
            // 将后面的图书前移
            for (int j = i; j < lib->count - 1; j++) {
                lib->books[j] = lib->books[j + 1];
            }
            
            lib->count--;
            return 1;
        }
    }
    
    printf("错误：未找到ID为 %d 的图书\n", id);
    return 0;
}

// 查找图书
Book* find_book(Library *lib, int id) {
    if (!lib) return NULL;
    
    for (int i = 0; i < lib->count; i++) {
        if (lib->books[i].id == id) {
            return &lib->books[i];
        }
    }
    
    return NULL;
}

// 按标题查找图书
Book* find_book_by_title(Library *lib, const char *title) {
    if (!lib || !title) return NULL;
    
    for (int i = 0; i < lib->count; i++) {
        if (strcmp(lib->books[i].title, title) == 0) {
            return &lib->books[i];
        }
    }
    
    return NULL;
}

// 修改图书信息
int update_book(Library *lib, int id, const char *title, const char *author, int year, double price) {
    Book *book = find_book(lib, id);
    if (!book) {
        printf("错误：未找到ID为 %d 的图书\n", id);
        return 0;
    }
    
    printf("更新图书信息：ID %d\n", id);
    printf("原信息：《%s》 by %s (%d年) - ¥%.2f\n", 
           book->title, book->author, book->year, book->price);
    
    if (title) {
        strncpy(book->title, title, sizeof(book->title) - 1);
        book->title[sizeof(book->title) - 1] = '\0';
    }
    if (author) {
        strncpy(book->author, author, sizeof(book->author) - 1);
        book->author[sizeof(book->author) - 1] = '\0';
    }
    if (year > 0) {
        book->year = year;
    }
    if (price >= 0) {
        book->price = price;
    }
    
    printf("新信息：《%s》 by %s (%d年) - ¥%.2f\n", 
           book->title, book->author, book->year, book->price);
    
    return 1;
}

// 显示所有图书
void display_books(Library *lib) {
    if (!lib) {
        printf("错误：无效的图书馆指针\n");
        return;
    }
    
    if (lib->count == 0) {
        printf("图书馆暂无图书\n");
        return;
    }
    
    printf("\n=== 图书馆藏书列表 ===\n");
    printf("%-5s %-30s %-20s %-6s %-8s %-6s\n", 
           "ID", "书名", "作者", "年份", "价格", "状态");
    printf("----------------------------------------------------------------\n");
    
    for (int i = 0; i < lib->count; i++) {
        Book *book = &lib->books[i];
        printf("%-5d %-30s %-20s %-6d ¥%-7.2f %-6s\n",
               book->id, book->title, book->author, book->year, book->price,
               book->available ? "可借" : "已借出");
    }
    
    printf("----------------------------------------------------------------\n");
    printf("总计：%d本图书\n\n", lib->count);
}

// 按价格排序（冒泡排序）
void sort_books_by_price(Library *lib) {
    if (!lib || lib->count <= 1) return;
    
    printf("按价格排序图书...\n");
    
    for (int i = 0; i < lib->count - 1; i++) {
        for (int j = 0; j < lib->count - 1 - i; j++) {
            if (lib->books[j].price > lib->books[j + 1].price) {
                // 交换图书
                Book temp = lib->books[j];
                lib->books[j] = lib->books[j + 1];
                lib->books[j + 1] = temp;
            }
        }
    }
    
    printf("排序完成\n");
}

// 按年份排序（选择排序）
void sort_books_by_year(Library *lib) {
    if (!lib || lib->count <= 1) return;
    
    printf("按出版年份排序图书...\n");
    
    for (int i = 0; i < lib->count - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < lib->count; j++) {
            if (lib->books[j].year < lib->books[min_idx].year) {
                min_idx = j;
            }
        }
        
        if (min_idx != i) {
            Book temp = lib->books[i];
            lib->books[i] = lib->books[min_idx];
            lib->books[min_idx] = temp;
        }
    }
    
    printf("排序完成\n");
}

// 统计图书信息
void print_library_statistics(Library *lib) {
    if (!lib) {
        printf("错误：无效的图书馆指针\n");
        return;
    }
    
    if (lib->count == 0) {
        printf("图书馆暂无图书，无法统计\n");
        return;
    }
    
    double total_price = 0.0;
    double max_price = lib->books[0].price;
    double min_price = lib->books[0].price;
    int available_count = 0;
    int oldest_year = lib->books[0].year;
    int newest_year = lib->books[0].year;
    
    for (int i = 0; i < lib->count; i++) {
        Book *book = &lib->books[i];
        
        total_price += book->price;
        
        if (book->price > max_price) max_price = book->price;
        if (book->price < min_price) min_price = book->price;
        
        if (book->available) available_count++;
        
        if (book->year < oldest_year) oldest_year = book->year;
        if (book->year > newest_year) newest_year = book->year;
    }
    
    printf("\n=== 图书馆统计信息 ===\n");
    printf("总图书数量：%d本\n", lib->count);
    printf("可借图书数量：%d本\n", available_count);
    printf("已借出图书数量：%d本\n", lib->count - available_count);
    printf("图书总价值：¥%.2f\n", total_price);
    printf("平均价格：¥%.2f\n", total_price / lib->count);
    printf("最高价格：¥%.2f\n", max_price);
    printf("最低价格：¥%.2f\n", min_price);
    printf("最早出版年份：%d年\n", oldest_year);
    printf("最新出版年份：%d年\n", newest_year);
    printf("===================\n\n");
}

// 借书功能
int borrow_book(Library *lib, int id) {
    Book *book = find_book(lib, id);
    if (!book) {
        printf("错误：未找到ID为 %d 的图书\n", id);
        return 0;
    }
    
    if (!book->available) {
        printf("错误：图书《%s》已被借出\n", book->title);
        return 0;
    }
    
    book->available = 0;
    printf("成功借出图书：《%s》\n", book->title);
    return 1;
}

// 还书功能
int return_book(Library *lib, int id) {
    Book *book = find_book(lib, id);
    if (!book) {
        printf("错误：未找到ID为 %d 的图书\n", id);
        return 0;
    }
    
    if (book->available) {
        printf("错误：图书《%s》未被借出\n", book->title);
        return 0;
    }
    
    book->available = 1;
    printf("成功归还图书：《%s》\n", book->title);
    return 1;
}

// === 学生管理系统函数实现（额外练习）===

// 按GPA排序学生（插入排序）
void sort_students_by_gpa(Student students[], int count) {
    if (!students || count <= 1) return;
    
    printf("按GPA排序学生...\n");
    
    for (int i = 1; i < count; i++) {
        Student key = students[i];
        int j = i - 1;
        
        // 降序排列（GPA高的在前）
        while (j >= 0 && students[j].gpa < key.gpa) {
            students[j + 1] = students[j];
            j--;
        }
        
        students[j + 1] = key;
    }
    
    printf("排序完成\n");
}

// 查找GPA最高的学生
Student* find_highest_gpa_student(Student students[], int count) {
    if (!students || count <= 0) return NULL;
    
    Student *highest = &students[0];
    
    for (int i = 1; i < count; i++) {
        if (students[i].gpa > highest->gpa) {
            highest = &students[i];
        }
    }
    
    return highest;
}

// 计算平均GPA
double calculate_average_gpa(Student students[], int count) {
    if (!students || count <= 0) return 0.0;
    
    double total = 0.0;
    for (int i = 0; i < count; i++) {
        total += students[i].gpa;
    }
    
    return total / count;
}

// 显示学生信息
void display_students(Student students[], int count) {
    if (!students || count <= 0) {
        printf("暂无学生信息\n");
        return;
    }
    
    printf("\n=== 学生信息列表 ===\n");
    printf("%-5s %-20s %-6s %-6s\n", "ID", "姓名", "年龄", "GPA");
    printf("------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %-6d %.2f\n",
               students[i].id, students[i].name, 
               students[i].age, students[i].gpa);
    }
    
    printf("------------------------------------\n");
    printf("总计：%d名学生\n\n", count);
}

// 测试函数
void test_library_system() {
    printf("=== 图书管理系统测试 ===\n\n");
    
    Library lib;
    init_library(&lib);
    
    // 添加测试图书
    printf("\n1. 添加图书测试:\n");
    add_book(&lib, 1001, "C程序设计语言", "Brian W. Kernighan", 1988, 59.90);
    add_book(&lib, 1002, "算法导论", "Thomas H. Cormen", 2009, 128.00);
    add_book(&lib, 1003, "深入理解计算机系统", "Randal E. Bryant", 2016, 139.00);
    add_book(&lib, 1004, "编程珠玑", "Jon Bentley", 2000, 49.00);
    add_book(&lib, 1005, "代码大全", "Steve McConnell", 2004, 108.00);
    
    // 尝试添加重复ID
    printf("\n尝试添加重复ID：\n");
    add_book(&lib, 1001, "重复图书", "测试作者", 2023, 99.99);
    
    // 显示所有图书
    display_books(&lib);
    
    // 查找图书测试
    printf("2. 查找图书测试:\n");
    Book *found = find_book(&lib, 1002);
    if (found) {
        printf("找到图书：《%s》 by %s\n", found->title, found->author);
    }
    
    found = find_book_by_title(&lib, "编程珠玑");
    if (found) {
        printf("按标题找到图书：ID %d\n", found->id);
    }
    
    // 借书还书测试
    printf("\n3. 借书还书测试:\n");
    borrow_book(&lib, 1001);
    borrow_book(&lib, 1003);
    borrow_book(&lib, 1001); // 重复借书
    
    display_books(&lib);
    
    return_book(&lib, 1001);
    return_book(&lib, 1002); // 还未借出的书
    
    // 修改图书信息
    printf("\n4. 修改图书信息测试:\n");
    update_book(&lib, 1004, "编程珠玑（第2版）", NULL, 2016, 55.00);
    
    // 排序测试
    printf("\n5. 排序测试:\n");
    sort_books_by_price(&lib);
    printf("按价格排序后：\n");
    display_books(&lib);
    
    sort_books_by_year(&lib);
    printf("按年份排序后：\n");
    display_books(&lib);
    
    // 统计信息
    printf("6. 统计信息测试:\n");
    print_library_statistics(&lib);
    
    // 删除图书
    printf("7. 删除图书测试:\n");
    remove_book(&lib, 1003);
    remove_book(&lib, 9999); // 删除不存在的图书
    
    display_books(&lib);
    
    cleanup_library(&lib);
}

void test_student_system() {
    printf("\n=== 学生管理系统测试 ===\n\n");
    
    // 创建测试学生数据
    Student students[] = {
        {2001, "张三", 20, 3.8},
        {2002, "李四", 19, 3.9},
        {2003, "王五", 21, 3.2},
        {2004, "赵六", 20, 3.7},
        {2005, "钱七", 22, 3.95}
    };
    int count = sizeof(students) / sizeof(students[0]);
    
    printf("1. 原始学生信息:\n");
    display_students(students, count);
    
    // GPA排序测试
    printf("2. GPA排序测试:\n");
    sort_students_by_gpa(students, count);
    display_students(students, count);
    
    // 查找最高GPA学生
    printf("3. 查找最高GPA学生:\n");
    Student *highest = find_highest_gpa_student(students, count);
    if (highest) {
        printf("GPA最高的学生：%s (ID: %d, GPA: %.2f)\n", 
               highest->name, highest->id, highest->gpa);
    }
    
    // 计算平均GPA
    printf("\n4. 平均GPA计算:\n");
    double avg_gpa = calculate_average_gpa(students, count);
    printf("平均GPA：%.2f\n", avg_gpa);
    
    // 统计GPA分布
    printf("\n5. GPA分布统计:\n");
    int excellent = 0, good = 0, average = 0, poor = 0;
    
    for (int i = 0; i < count; i++) {
        if (students[i].gpa >= 3.8) excellent++;
        else if (students[i].gpa >= 3.5) good++;
        else if (students[i].gpa >= 3.0) average++;
        else poor++;
    }
    
    printf("优秀 (GPA >= 3.8): %d人\n", excellent);
    printf("良好 (3.5 <= GPA < 3.8): %d人\n", good);
    printf("一般 (3.0 <= GPA < 3.5): %d人\n", average);
    printf("较差 (GPA < 3.0): %d人\n", poor);
}

int main() {
    printf("结构体与数组练习 - 参考解决方案\n");
    printf("==================================\n\n");
    
    // 测试图书管理系统
    test_library_system();
    
    // 测试学生管理系统
    test_student_system();
    
    printf("\n=== 学习要点总结 ===\n");
    printf("1. 结构体设计：合理的数据结构设计是程序的基础\n");
    printf("2. 数组管理：动态管理数组元素的增删改查\n");
    printf("3. 排序算法：冒泡排序、选择排序、插入排序的实现\n");
    printf("4. 查找算法：线性查找的实现和优化\n");
    printf("5. 数据统计：从数组中提取统计信息\n");
    printf("6. 错误处理：参数验证和边界条件检查\n");
    printf("7. 内存安全：字符串操作的安全性考虑\n");
    
    return 0;
}

/*
 * 解决方案说明：
 * 
 * 1. 结构体设计:
 *    - Book结构体包含图书的所有必要信息
 *    - Library结构体管理图书数组和相关元数据
 *    - Student结构体用于额外的学生管理练习
 * 
 * 2. 内存管理:
 *    - 使用固定大小的数组避免动态内存分配
 *    - 通过count字段跟踪实际使用的元素数量
 *    - 字符串操作使用strncpy确保安全性
 * 
 * 3. 功能实现:
 *    - 增删改查的完整实现
 *    - 多种排序算法的演示
 *    - 统计功能的实现
 *    - 借书还书的状态管理
 * 
 * 4. 错误处理:
 *    - 参数有效性检查
 *    - 边界条件处理
 *    - 重复操作的检测
 * 
 * 5. 代码组织:
 *    - 功能模块化
 *    - 清晰的函数命名
 *    - 详细的注释说明
 */