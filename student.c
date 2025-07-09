#include "student.h"

// 初始化系统
void init_system(StudentSystem *system) {
    system->count = 0;
    memset(system->students, 0, sizeof(system->students));
}

// 显示主菜单
void display_menu() {
    printf("\n========== 学生管理系统 ==========\n");
    printf("1. 添加学生\n");
    printf("2. 删除学生\n");
    printf("3. 修改学生信息\n");
    printf("4. 显示所有学生\n");
    printf("5. 查找学生\n");
    printf("6. 按成绩排序\n");
    printf("7. 统计信息\n");
    printf("8. 保存到文件\n");
    printf("9. 从文件加载\n");
    printf("0. 退出系统\n");
    printf("================================\n");
    printf("请选择操作: ");
}

// 计算平均成绩
void calculate_average(Student *student) {
    student->average_score = (student->math_score + student->english_score + student->chinese_score) / 3.0;
}

// 验证学号格式
int is_valid_id(const char *id) {
    if (strlen(id) == 0 || strlen(id) >= MAX_ID_LEN) {
        return 0;
    }
    for (int i = 0; id[i]; i++) {
        if (!((id[i] >= '0' && id[i] <= '9') || (id[i] >= 'A' && id[i] <= 'Z') || (id[i] >= 'a' && id[i] <= 'z'))) {
            return 0;
        }
    }
    return 1;
}

// 验证成绩范围
int is_valid_score(float score) {
    return score >= 0 && score <= 100;
}

// 清空输入缓冲区
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 根据学号查找学生
int find_student_by_id(StudentSystem *system, const char *id) {
    for (int i = 0; i < system->count; i++) {
        if (strcmp(system->students[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

// 添加学生
int add_student(StudentSystem *system) {
    if (system->count >= MAX_STUDENTS) {
        printf("错误：学生数量已达上限！\n");
        return 0;
    }

    Student new_student;
    printf("\n=== 添加学生 ===\n");
    
    // 输入学号
    do {
        printf("请输入学号: ");
        scanf("%s", new_student.id);
        
        if (!is_valid_id(new_student.id)) {
            printf("错误：学号格式不正确！\n");
            continue;
        }
        
        if (find_student_by_id(system, new_student.id) != -1) {
            printf("错误：学号已存在！\n");
            continue;
        }
        break;
    } while (1);
    
    // 输入姓名
    printf("请输入姓名: ");
    scanf("%s", new_student.name);
    
    // 输入年龄
    do {
        printf("请输入年龄: ");
        if (scanf("%d", &new_student.age) != 1 || new_student.age < 1 || new_student.age > 150) {
            printf("错误：年龄输入不正确！\n");
            clear_input_buffer();
            continue;
        }
        break;
    } while (1);
    
    // 输入各科成绩
    do {
        printf("请输入数学成绩: ");
        if (scanf("%f", &new_student.math_score) != 1 || !is_valid_score(new_student.math_score)) {
            printf("错误：成绩应在0-100之间！\n");
            clear_input_buffer();
            continue;
        }
        break;
    } while (1);
    
    do {
        printf("请输入英语成绩: ");
        if (scanf("%f", &new_student.english_score) != 1 || !is_valid_score(new_student.english_score)) {
            printf("错误：成绩应在0-100之间！\n");
            clear_input_buffer();
            continue;
        }
        break;
    } while (1);
    
    do {
        printf("请输入语文成绩: ");
        if (scanf("%f", &new_student.chinese_score) != 1 || !is_valid_score(new_student.chinese_score)) {
            printf("错误：成绩应在0-100之间！\n");
            clear_input_buffer();
            continue;
        }
        break;
    } while (1);
    
    calculate_average(&new_student);
    system->students[system->count] = new_student;
    system->count++;
    
    printf("学生添加成功！\n");
    return 1;
}

// 删除学生
int delete_student(StudentSystem *system) {
    if (system->count == 0) {
        printf("系统中没有学生记录！\n");
        return 0;
    }
    
    char id[MAX_ID_LEN];
    printf("\n=== 删除学生 ===\n");
    printf("请输入要删除的学生学号: ");
    scanf("%s", id);
    
    int index = find_student_by_id(system, id);
    if (index == -1) {
        printf("未找到该学生！\n");
        return 0;
    }
    
    printf("确认删除学生 %s (%s)? (y/n): ", system->students[index].name, system->students[index].id);
    char confirm;
    scanf(" %c", &confirm);
    
    if (confirm == 'y' || confirm == 'Y') {
        for (int i = index; i < system->count - 1; i++) {
            system->students[i] = system->students[i + 1];
        }
        system->count--;
        printf("学生删除成功！\n");
        return 1;
    } else {
        printf("取消删除操作。\n");
        return 0;
    }
}

// 修改学生信息
int modify_student(StudentSystem *system) {
    if (system->count == 0) {
        printf("系统中没有学生记录！\n");
        return 0;
    }
    
    char id[MAX_ID_LEN];
    printf("\n=== 修改学生信息 ===\n");
    printf("请输入要修改的学生学号: ");
    scanf("%s", id);
    
    int index = find_student_by_id(system, id);
    if (index == -1) {
        printf("未找到该学生！\n");
        return 0;
    }
    
    Student *student = &system->students[index];
    printf("当前学生信息:\n");
    printf("学号: %s\n", student->id);
    printf("姓名: %s\n", student->name);
    printf("年龄: %d\n", student->age);
    printf("数学: %.2f\n", student->math_score);
    printf("英语: %.2f\n", student->english_score);
    printf("语文: %.2f\n", student->chinese_score);
    
    int choice;
    printf("\n请选择要修改的项目:\n");
    printf("1. 姓名\n");
    printf("2. 年龄\n");
    printf("3. 数学成绩\n");
    printf("4. 英语成绩\n");
    printf("5. 语文成绩\n");
    printf("请选择: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            printf("请输入新姓名: ");
            scanf("%s", student->name);
            break;
        case 2:
            do {
                printf("请输入新年龄: ");
                if (scanf("%d", &student->age) != 1 || student->age < 1 || student->age > 150) {
                    printf("错误：年龄输入不正确！\n");
                    clear_input_buffer();
                    continue;
                }
                break;
            } while (1);
            break;
        case 3:
            do {
                printf("请输入新数学成绩: ");
                if (scanf("%f", &student->math_score) != 1 || !is_valid_score(student->math_score)) {
                    printf("错误：成绩应在0-100之间！\n");
                    clear_input_buffer();
                    continue;
                }
                break;
            } while (1);
            break;
        case 4:
            do {
                printf("请输入新英语成绩: ");
                if (scanf("%f", &student->english_score) != 1 || !is_valid_score(student->english_score)) {
                    printf("错误：成绩应在0-100之间！\n");
                    clear_input_buffer();
                    continue;
                }
                break;
            } while (1);
            break;
        case 5:
            do {
                printf("请输入新语文成绩: ");
                if (scanf("%f", &student->chinese_score) != 1 || !is_valid_score(student->chinese_score)) {
                    printf("错误：成绩应在0-100之间！\n");
                    clear_input_buffer();
                    continue;
                }
                break;
            } while (1);
            break;
        default:
            printf("无效选择！\n");
            return 0;
    }
    
    calculate_average(student);
    printf("学生信息修改成功！\n");
    return 1;
}

// 显示所有学生
void display_all_students(StudentSystem *system) {
    if (system->count == 0) {
        printf("系统中没有学生记录！\n");
        return;
    }
    
    printf("\n=== 所有学生信息 ===\n");
    printf("%-10s %-15s %-5s %-8s %-8s %-8s %-8s\n", 
           "学号", "姓名", "年龄", "数学", "英语", "语文", "平均分");
    printf("----------------------------------------------------------------\n");
    
    for (int i = 0; i < system->count; i++) {
        Student *s = &system->students[i];
        printf("%-10s %-15s %-5d %-8.2f %-8.2f %-8.2f %-8.2f\n",
               s->id, s->name, s->age, s->math_score, s->english_score, s->chinese_score, s->average_score);
    }
}

// 查找学生
void search_student(StudentSystem *system) {
    if (system->count == 0) {
        printf("系统中没有学生记录！\n");
        return;
    }
    
    int choice;
    printf("\n=== 查找学生 ===\n");
    printf("1. 按学号查找\n");
    printf("2. 按姓名查找\n");
    printf("请选择查找方式: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: {
            char id[MAX_ID_LEN];
            printf("请输入学号: ");
            scanf("%s", id);
            
            int index = find_student_by_id(system, id);
            if (index != -1) {
                Student *s = &system->students[index];
                printf("\n找到学生:\n");
                printf("学号: %s\n", s->id);
                printf("姓名: %s\n", s->name);
                printf("年龄: %d\n", s->age);
                printf("数学: %.2f\n", s->math_score);
                printf("英语: %.2f\n", s->english_score);
                printf("语文: %.2f\n", s->chinese_score);
                printf("平均分: %.2f\n", s->average_score);
            } else {
                printf("未找到该学生！\n");
            }
            break;
        }
        case 2: {
            char name[MAX_NAME_LEN];
            printf("请输入姓名: ");
            scanf("%s", name);
            
            int found = 0;
            for (int i = 0; i < system->count; i++) {
                if (strcmp(system->students[i].name, name) == 0) {
                    if (!found) {
                        printf("\n找到学生:\n");
                        printf("%-10s %-15s %-5s %-8s %-8s %-8s %-8s\n", 
                               "学号", "姓名", "年龄", "数学", "英语", "语文", "平均分");
                        printf("----------------------------------------------------------------\n");
                        found = 1;
                    }
                    Student *s = &system->students[i];
                    printf("%-10s %-15s %-5d %-8.2f %-8.2f %-8.2f %-8.2f\n",
                           s->id, s->name, s->age, s->math_score, s->english_score, s->chinese_score, s->average_score);
                }
            }
            
            if (!found) {
                printf("未找到该学生！\n");
            }
            break;
        }
        default:
            printf("无效选择！\n");
    }
}

// 按成绩排序
void sort_students(StudentSystem *system) {
    if (system->count == 0) {
        printf("系统中没有学生记录！\n");
        return;
    }
    
    printf("\n=== 学生排序 ===\n");
    printf("1. 按平均分升序\n");
    printf("2. 按平均分降序\n");
    printf("请选择排序方式: ");
    
    int choice;
    scanf("%d", &choice);
    
    // 冒泡排序
    for (int i = 0; i < system->count - 1; i++) {
        for (int j = 0; j < system->count - 1 - i; j++) {
            int should_swap = 0;
            
            if (choice == 1) { // 升序
                should_swap = system->students[j].average_score > system->students[j + 1].average_score;
            } else if (choice == 2) { // 降序
                should_swap = system->students[j].average_score < system->students[j + 1].average_score;
            }
            
            if (should_swap) {
                Student temp = system->students[j];
                system->students[j] = system->students[j + 1];
                system->students[j + 1] = temp;
            }
        }
    }
    
    if (choice == 1 || choice == 2) {
        printf("排序完成！\n");
        display_all_students(system);
    } else {
        printf("无效选择！\n");
    }
}

// 统计信息
void calculate_statistics(StudentSystem *system) {
    if (system->count == 0) {
        printf("系统中没有学生记录！\n");
        return;
    }
    
    float total_math = 0, total_english = 0, total_chinese = 0, total_average = 0;
    float max_average = system->students[0].average_score;
    float min_average = system->students[0].average_score;
    int max_index = 0, min_index = 0;
    
    for (int i = 0; i < system->count; i++) {
        Student *s = &system->students[i];
        total_math += s->math_score;
        total_english += s->english_score;
        total_chinese += s->chinese_score;
        total_average += s->average_score;
        
        if (s->average_score > max_average) {
            max_average = s->average_score;
            max_index = i;
        }
        
        if (s->average_score < min_average) {
            min_average = s->average_score;
            min_index = i;
        }
    }
    
    printf("\n=== 统计信息 ===\n");
    printf("学生总数: %d\n", system->count);
    printf("平均成绩:\n");
    printf("  数学: %.2f\n", total_math / system->count);
    printf("  英语: %.2f\n", total_english / system->count);
    printf("  语文: %.2f\n", total_chinese / system->count);
    printf("  总平均: %.2f\n", total_average / system->count);
    printf("\n最高分学生: %s (%s) - %.2f\n", 
           system->students[max_index].name, system->students[max_index].id, max_average);
    printf("最低分学生: %s (%s) - %.2f\n", 
           system->students[min_index].name, system->students[min_index].id, min_average);
}

// 保存到文件
int save_to_file(StudentSystem *system) {
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        printf("错误：无法创建文件！\n");
        return 0;
    }
    
    fwrite(&system->count, sizeof(int), 1, file);
    fwrite(system->students, sizeof(Student), system->count, file);
    
    fclose(file);
    printf("数据保存成功！\n");
    return 1;
}

// 从文件加载
int load_from_file(StudentSystem *system) {
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        printf("提示：数据文件不存在，将创建新的数据文件。\n");
        return 0;
    }
    
    if (fread(&system->count, sizeof(int), 1, file) != 1) {
        printf("错误：读取文件失败！\n");
        fclose(file);
        return 0;
    }
    
    if (system->count > MAX_STUDENTS) {
        printf("错误：文件中的学生数量超过系统限制！\n");
        fclose(file);
        return 0;
    }
    
    if (fread(system->students, sizeof(Student), system->count, file) != system->count) {
        printf("错误：读取学生数据失败！\n");
        fclose(file);
        return 0;
    }
    
    fclose(file);
    printf("数据加载成功！共加载 %d 名学生。\n", system->count);
    return 1;
}