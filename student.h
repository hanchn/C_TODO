#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LEN 50
#define MAX_ID_LEN 20
#define FILENAME "students.dat"

// 学生结构体
typedef struct {
    char id[MAX_ID_LEN];        // 学号
    char name[MAX_NAME_LEN];    // 姓名
    int age;                    // 年龄
    float math_score;           // 数学成绩
    float english_score;        // 英语成绩
    float chinese_score;        // 语文成绩
    float average_score;        // 平均成绩
} Student;

// 学生管理系统结构体
typedef struct {
    Student students[MAX_STUDENTS];
    int count;                  // 当前学生数量
} StudentSystem;

// 函数声明
void init_system(StudentSystem *system);
void display_menu();
int add_student(StudentSystem *system);
int delete_student(StudentSystem *system);
int modify_student(StudentSystem *system);
void display_all_students(StudentSystem *system);
void search_student(StudentSystem *system);
void sort_students(StudentSystem *system);
void calculate_statistics(StudentSystem *system);
int save_to_file(StudentSystem *system);
int load_from_file(StudentSystem *system);
void calculate_average(Student *student);
int find_student_by_id(StudentSystem *system, const char *id);
void clear_input_buffer();
int is_valid_id(const char *id);
int is_valid_score(float score);

#endif