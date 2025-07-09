// 结构体和数组示例
// 对应教程：第2课 - 结构体与数组

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 学生结构体定义
typedef struct {
    char id[20];
    char name[50];
    int age;
    float math_score;
    float english_score;
    float chinese_score;
    float average;
} Student;

// 简单的学生管理系统
typedef struct {
    Student students[100];
    int count;
} StudentSystem;

// 函数声明
void init_system(StudentSystem *system);
void add_student(StudentSystem *system, Student *student);
void display_student(Student *student);
void display_all_students(StudentSystem *system);
float calculate_average(Student *student);
void find_student_by_id(StudentSystem *system, const char *id);
void demonstrate_basic_struct();
void demonstrate_student_system();

int main() {
    printf("=== 结构体和数组示例 ===\n\n");
    
    demonstrate_basic_struct();
    demonstrate_student_system();
    
    return 0;
}

// 演示基本结构体操作
void demonstrate_basic_struct() {
    printf("1. 基本结构体操作\n");
    printf("------------------\n");
    
    // 创建学生
    Student student1;
    strcpy(student1.id, "2024001");
    strcpy(student1.name, "张三");
    student1.age = 20;
    student1.math_score = 85.5;
    student1.english_score = 92.0;
    student1.chinese_score = 88.5;
    student1.average = calculate_average(&student1);
    
    printf("学生信息:\n");
    display_student(&student1);
    
    // 使用初始化列表创建学生
    Student student2 = {
        .id = "2024002",
        .name = "李四",
        .age = 21,
        .math_score = 90.0,
        .english_score = 85.0,
        .chinese_score = 92.0
    };
    student2.average = calculate_average(&student2);
    
    printf("\n另一个学生信息:\n");
    display_student(&student2);
    printf("\n");
}

// 演示学生管理系统
void demonstrate_student_system() {
    printf("2. 学生管理系统演示\n");
    printf("--------------------\n");
    
    StudentSystem system;
    init_system(&system);
    
    // 添加多个学生
    Student students[] = {
        {"2024001", "张三", 20, 85.5, 92.0, 88.5, 0},
        {"2024002", "李四", 21, 90.0, 85.0, 92.0, 0},
        {"2024003", "王五", 19, 78.0, 88.0, 85.0, 0},
        {"2024004", "赵六", 22, 92.0, 90.0, 89.0, 0}
    };
    
    int student_count = sizeof(students) / sizeof(students[0]);
    
    for (int i = 0; i < student_count; i++) {
        students[i].average = calculate_average(&students[i]);
        add_student(&system, &students[i]);
    }
    
    printf("添加了 %d 个学生\n\n", system.count);
    
    // 显示所有学生
    display_all_students(&system);
    
    // 查找特定学生
    printf("\n查找学号为 2024002 的学生:\n");
    find_student_by_id(&system, "2024002");
    
    // 统计信息
    printf("\n=== 统计信息 ===\n");
    float total_avg = 0;
    float max_avg = 0;
    float min_avg = 100;
    
    for (int i = 0; i < system.count; i++) {
        total_avg += system.students[i].average;
        if (system.students[i].average > max_avg) {
            max_avg = system.students[i].average;
        }
        if (system.students[i].average < min_avg) {
            min_avg = system.students[i].average;
        }
    }
    
    printf("班级平均分: %.2f\n", total_avg / system.count);
    printf("最高平均分: %.2f\n", max_avg);
    printf("最低平均分: %.2f\n", min_avg);
}

// 初始化系统
void init_system(StudentSystem *system) {
    system->count = 0;
    printf("学生管理系统已初始化\n");
}

// 添加学生
void add_student(StudentSystem *system, Student *student) {
    if (system->count < 100) {
        system->students[system->count] = *student;
        system->count++;
        printf("学生 %s (%s) 添加成功\n", student->name, student->id);
    } else {
        printf("错误: 系统已满，无法添加更多学生\n");
    }
}

// 显示单个学生信息
void display_student(Student *student) {
    printf("学号: %s\n", student->id);
    printf("姓名: %s\n", student->name);
    printf("年龄: %d\n", student->age);
    printf("数学: %.1f\n", student->math_score);
    printf("英语: %.1f\n", student->english_score);
    printf("语文: %.1f\n", student->chinese_score);
    printf("平均分: %.2f\n", student->average);
}

// 显示所有学生
void display_all_students(StudentSystem *system) {
    printf("=== 所有学生信息 ===\n");
    printf("%-10s %-10s %-4s %-6s %-6s %-6s %-8s\n", 
           "学号", "姓名", "年龄", "数学", "英语", "语文", "平均分");
    printf("--------------------------------------------------------\n");
    
    for (int i = 0; i < system->count; i++) {
        Student *s = &system->students[i];
        printf("%-10s %-10s %-4d %-6.1f %-6.1f %-6.1f %-8.2f\n",
               s->id, s->name, s->age, 
               s->math_score, s->english_score, s->chinese_score, s->average);
    }
}

// 计算平均分
float calculate_average(Student *student) {
    return (student->math_score + student->english_score + student->chinese_score) / 3.0;
}

// 按学号查找学生
void find_student_by_id(StudentSystem *system, const char *id) {
    for (int i = 0; i < system->count; i++) {
        if (strcmp(system->students[i].id, id) == 0) {
            printf("找到学生:\n");
            display_student(&system->students[i]);
            return;
        }
    }
    printf("未找到学号为 %s 的学生\n", id);
}