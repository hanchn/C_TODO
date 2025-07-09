# 第5课：核心功能实现

> **学习目标**: 学习如何实现学生管理系统的核心业务逻辑

## 📋 本课内容

1. [功能分析与设计](#功能分析与设计)
2. [添加学生功能](#添加学生功能)
3. [删除学生功能](#删除学生功能)
4. [修改学生功能](#修改学生功能)
5. [查找学生功能](#查找学生功能)
6. [显示和排序功能](#显示和排序功能)
7. [统计分析功能](#统计分析功能)
8. [实践练习](#实践练习)

---

## 功能分析与设计

### 核心功能概览

学生管理系统的核心功能可以分为以下几类：

```
核心功能
├── 数据管理
│   ├── 添加学生 (Create)
│   ├── 删除学生 (Delete)
│   ├── 修改学生 (Update)
│   └── 查找学生 (Read)
├── 数据展示
│   ├── 显示所有学生
│   ├── 按条件筛选
│   └── 排序显示
└── 数据分析
    ├── 统计信息
    ├── 成绩分析
    └── 报表生成
```

### 设计原则

1. **输入验证**: 所有用户输入都要验证
2. **错误处理**: 优雅处理各种错误情况
3. **用户友好**: 提供清晰的提示和反馈
4. **数据一致性**: 确保数据的完整性
5. **性能考虑**: 合理的算法复杂度

### 函数设计模式

```c
// 标准的功能函数模式
int function_name(StudentSystem *system) {
    // 1. 参数验证
    if (system == NULL) {
        return 0;  // 失败
    }
    
    // 2. 前置条件检查
    if (/* 某些条件不满足 */) {
        show_error("错误信息");
        return 0;
    }
    
    // 3. 用户输入
    // 获取用户输入并验证
    
    // 4. 核心逻辑
    // 执行主要功能
    
    // 5. 结果反馈
    show_success("操作成功");
    return 1;  // 成功
}
```

---

## 添加学生功能

### 功能分析

添加学生需要考虑：
- 系统容量检查
- 学号唯一性验证
- 输入数据验证
- 自动计算平均成绩

### 实现代码

```c
// core.c
#include "core.h"
#include "ui.h"
#include "utils.h"

int add_student(StudentSystem *system) {
    // 1. 参数验证
    if (system == NULL) {
        show_error("系统未初始化");
        return 0;
    }
    
    // 2. 容量检查
    if (system->count >= MAX_STUDENTS) {
        show_error("学生数量已达上限，无法添加新学生");
        return 0;
    }
    
    Student new_student;
    memset(&new_student, 0, sizeof(Student));
    
    printf("\n=== 添加学生信息 ===\n");
    
    // 3. 输入学号并验证唯一性
    while (1) {
        printf("请输入学号: ");
        safe_input_string(new_student.id, MAX_ID_LEN);
        
        // 验证学号格式
        if (!is_valid_id(new_student.id)) {
            show_error("学号格式无效！学号只能包含字母和数字");
            continue;
        }
        
        // 检查学号是否已存在
        if (find_student_by_id(system, new_student.id) != -1) {
            show_error("学号已存在，请输入其他学号");
            continue;
        }
        
        break;  // 学号有效且唯一
    }
    
    // 4. 输入姓名
    while (1) {
        printf("请输入姓名: ");
        safe_input_string(new_student.name, MAX_NAME_LEN);
        
        // 验证姓名
        trim_string(new_student.name);
        if (strlen(new_student.name) == 0) {
            show_error("姓名不能为空");
            continue;
        }
        
        if (strlen(new_student.name) < 2) {
            show_error("姓名至少需要2个字符");
            continue;
        }
        
        break;
    }
    
    // 5. 输入年龄
    while (1) {
        printf("请输入年龄 (15-30): ");
        if (safe_input_int(&new_student.age, 15, 30)) {
            break;
        }
        show_error("年龄必须在15-30之间");
    }
    
    // 6. 输入各科成绩
    while (1) {
        printf("请输入数学成绩 (0-100): ");
        if (safe_input_float(&new_student.math_score, 0, 100)) {
            break;
        }
        show_error("成绩必须在0-100之间");
    }
    
    while (1) {
        printf("请输入英语成绩 (0-100): ");
        if (safe_input_float(&new_student.english_score, 0, 100)) {
            break;
        }
        show_error("成绩必须在0-100之间");
    }
    
    while (1) {
        printf("请输入语文成绩 (0-100): ");
        if (safe_input_float(&new_student.chinese_score, 0, 100)) {
            break;
        }
        show_error("成绩必须在0-100之间");
    }
    
    // 7. 计算平均成绩
    calculate_average(&new_student);
    
    // 8. 添加到系统
    system->students[system->count] = new_student;
    system->count++;
    
    // 9. 显示添加结果
    printf("\n学生信息添加成功！\n");
    printf("学号: %s\n", new_student.id);
    printf("姓名: %s\n", new_student.name);
    printf("年龄: %d\n", new_student.age);
    printf("数学: %.1f\n", new_student.math_score);
    printf("英语: %.1f\n", new_student.english_score);
    printf("语文: %.1f\n", new_student.chinese_score);
    printf("平均分: %.1f\n", new_student.average_score);
    printf("等级: %s\n", get_grade_level(new_student.average_score));
    
    show_success("学生添加成功");
    return 1;
}

// 批量添加学生功能
int batch_add_students(StudentSystem *system) {
    if (system == NULL) {
        show_error("系统未初始化");
        return 0;
    }
    
    int count;
    printf("请输入要添加的学生数量: ");
    if (!safe_input_int(&count, 1, MAX_STUDENTS - system->count)) {
        show_error("数量无效");
        return 0;
    }
    
    int success_count = 0;
    for (int i = 0; i < count; i++) {
        printf("\n--- 添加第 %d 个学生 ---\n", i + 1);
        if (add_student(system)) {
            success_count++;
        } else {
            printf("第 %d 个学生添加失败\n", i + 1);
        }
    }
    
    printf("\n批量添加完成：成功 %d 个，失败 %d 个\n", 
           success_count, count - success_count);
    return success_count;
}
```

---

## 删除学生功能

### 功能分析

删除学生需要考虑：
- 学生是否存在
- 删除确认
- 数组元素移动
- 计数更新

### 实现代码

```c
int delete_student(StudentSystem *system) {
    if (system == NULL) {
        show_error("系统未初始化");
        return 0;
    }
    
    if (system->count == 0) {
        show_error("系统中没有学生信息");
        return 0;
    }
    
    char id[MAX_ID_LEN];
    printf("\n=== 删除学生信息 ===\n");
    printf("请输入要删除的学生学号: ");
    safe_input_string(id, MAX_ID_LEN);
    
    // 查找学生
    int index = find_student_by_id(system, id);
    if (index == -1) {
        show_error("未找到该学号的学生");
        return 0;
    }
    
    // 显示要删除的学生信息
    Student *student = &system->students[index];
    printf("\n找到学生信息：\n");
    printf("学号: %s\n", student->id);
    printf("姓名: %s\n", student->name);
    printf("年龄: %d\n", student->age);
    printf("平均分: %.1f\n", student->average_score);
    
    // 确认删除
    char confirm;
    printf("\n确定要删除这个学生吗？(y/N): ");
    scanf(" %c", &confirm);
    clear_input_buffer();
    
    if (confirm != 'y' && confirm != 'Y') {
        printf("删除操作已取消\n");
        return 0;
    }
    
    // 删除学生（移动数组元素）
    for (int i = index; i < system->count - 1; i++) {
        system->students[i] = system->students[i + 1];
    }
    
    // 清空最后一个元素
    memset(&system->students[system->count - 1], 0, sizeof(Student));
    system->count--;
    
    show_success("学生删除成功");
    return 1;
}

// 按条件批量删除
int batch_delete_students(StudentSystem *system) {
    if (system == NULL || system->count == 0) {
        show_error("系统中没有学生信息");
        return 0;
    }
    
    printf("\n=== 批量删除学生 ===\n");
    printf("1. 按成绩范围删除\n");
    printf("2. 按年龄范围删除\n");
    printf("请选择删除条件: ");
    
    int choice;
    if (!safe_input_int(&choice, 1, 2)) {
        show_error("选择无效");
        return 0;
    }
    
    int deleted_count = 0;
    
    if (choice == 1) {
        // 按成绩删除
        float min_score, max_score;
        printf("请输入最低平均分: ");
        if (!safe_input_float(&min_score, 0, 100)) {
            show_error("成绩无效");
            return 0;
        }
        
        printf("请输入最高平均分: ");
        if (!safe_input_float(&max_score, min_score, 100)) {
            show_error("成绩无效");
            return 0;
        }
        
        // 从后往前删除，避免索引问题
        for (int i = system->count - 1; i >= 0; i--) {
            if (system->students[i].average_score >= min_score && 
                system->students[i].average_score <= max_score) {
                
                printf("删除学生: %s (%s)\n", 
                       system->students[i].id, 
                       system->students[i].name);
                
                // 移动数组元素
                for (int j = i; j < system->count - 1; j++) {
                    system->students[j] = system->students[j + 1];
                }
                system->count--;
                deleted_count++;
            }
        }
    } else {
        // 按年龄删除
        int min_age, max_age;
        printf("请输入最小年龄: ");
        if (!safe_input_int(&min_age, 15, 30)) {
            show_error("年龄无效");
            return 0;
        }
        
        printf("请输入最大年龄: ");
        if (!safe_input_int(&max_age, min_age, 30)) {
            show_error("年龄无效");
            return 0;
        }
        
        for (int i = system->count - 1; i >= 0; i--) {
            if (system->students[i].age >= min_age && 
                system->students[i].age <= max_age) {
                
                printf("删除学生: %s (%s)\n", 
                       system->students[i].id, 
                       system->students[i].name);
                
                for (int j = i; j < system->count - 1; j++) {
                    system->students[j] = system->students[j + 1];
                }
                system->count--;
                deleted_count++;
            }
        }
    }
    
    printf("\n批量删除完成，共删除 %d 个学生\n", deleted_count);
    return deleted_count;
}
```

---

## 修改学生功能

### 功能分析

修改学生信息需要考虑：
- 学生查找
- 选择修改项目
- 数据验证
- 重新计算平均分

### 实现代码

```c
int modify_student(StudentSystem *system) {
    if (system == NULL) {
        show_error("系统未初始化");
        return 0;
    }
    
    if (system->count == 0) {
        show_error("系统中没有学生信息");
        return 0;
    }
    
    char id[MAX_ID_LEN];
    printf("\n=== 修改学生信息 ===\n");
    printf("请输入要修改的学生学号: ");
    safe_input_string(id, MAX_ID_LEN);
    
    int index = find_student_by_id(system, id);
    if (index == -1) {
        show_error("未找到该学号的学生");
        return 0;
    }
    
    Student *student = &system->students[index];
    
    // 显示当前信息
    printf("\n当前学生信息：\n");
    printf("1. 学号: %s\n", student->id);
    printf("2. 姓名: %s\n", student->name);
    printf("3. 年龄: %d\n", student->age);
    printf("4. 数学成绩: %.1f\n", student->math_score);
    printf("5. 英语成绩: %.1f\n", student->english_score);
    printf("6. 语文成绩: %.1f\n", student->chinese_score);
    printf("7. 平均分: %.1f (自动计算)\n", student->average_score);
    
    while (1) {
        printf("\n请选择要修改的项目 (1-6, 0退出): ");
        int choice;
        if (!safe_input_int(&choice, 0, 6)) {
            show_error("选择无效");
            continue;
        }
        
        if (choice == 0) {
            break;
        }
        
        switch (choice) {
            case 1: {
                // 修改学号
                char new_id[MAX_ID_LEN];
                printf("请输入新学号: ");
                safe_input_string(new_id, MAX_ID_LEN);
                
                if (!is_valid_id(new_id)) {
                    show_error("学号格式无效");
                    break;
                }
                
                // 检查新学号是否已存在（排除当前学生）
                int existing_index = find_student_by_id(system, new_id);
                if (existing_index != -1 && existing_index != index) {
                    show_error("学号已存在");
                    break;
                }
                
                strcpy(student->id, new_id);
                show_success("学号修改成功");
                break;
            }
            
            case 2: {
                // 修改姓名
                char new_name[MAX_NAME_LEN];
                printf("请输入新姓名: ");
                safe_input_string(new_name, MAX_NAME_LEN);
                
                trim_string(new_name);
                if (strlen(new_name) < 2) {
                    show_error("姓名至少需要2个字符");
                    break;
                }
                
                strcpy(student->name, new_name);
                show_success("姓名修改成功");
                break;
            }
            
            case 3: {
                // 修改年龄
                int new_age;
                printf("请输入新年龄 (15-30): ");
                if (safe_input_int(&new_age, 15, 30)) {
                    student->age = new_age;
                    show_success("年龄修改成功");
                } else {
                    show_error("年龄无效");
                }
                break;
            }
            
            case 4: {
                // 修改数学成绩
                float new_score;
                printf("请输入新的数学成绩 (0-100): ");
                if (safe_input_float(&new_score, 0, 100)) {
                    student->math_score = new_score;
                    calculate_average(student);
                    show_success("数学成绩修改成功");
                } else {
                    show_error("成绩无效");
                }
                break;
            }
            
            case 5: {
                // 修改英语成绩
                float new_score;
                printf("请输入新的英语成绩 (0-100): ");
                if (safe_input_float(&new_score, 0, 100)) {
                    student->english_score = new_score;
                    calculate_average(student);
                    show_success("英语成绩修改成功");
                } else {
                    show_error("成绩无效");
                }
                break;
            }
            
            case 6: {
                // 修改语文成绩
                float new_score;
                printf("请输入新的语文成绩 (0-100): ");
                if (safe_input_float(&new_score, 0, 100)) {
                    student->chinese_score = new_score;
                    calculate_average(student);
                    show_success("语文成绩修改成功");
                } else {
                    show_error("成绩无效");
                }
                break;
            }
        }
        
        // 显示修改后的信息
        printf("\n修改后的信息：\n");
        printf("学号: %s, 姓名: %s, 年龄: %d\n", 
               student->id, student->name, student->age);
        printf("数学: %.1f, 英语: %.1f, 语文: %.1f, 平均: %.1f\n",
               student->math_score, student->english_score, 
               student->chinese_score, student->average_score);
    }
    
    return 1;
}
```

---

## 查找学生功能

### 功能分析

查找功能应该支持：
- 按学号精确查找
- 按姓名模糊查找
- 按成绩范围查找
- 按年龄范围查找

### 实现代码

```c
void search_student(StudentSystem *system) {
    if (system == NULL || system->count == 0) {
        show_error("系统中没有学生信息");
        return;
    }
    
    printf("\n=== 查找学生信息 ===\n");
    printf("1. 按学号查找\n");
    printf("2. 按姓名查找\n");
    printf("3. 按成绩范围查找\n");
    printf("4. 按年龄范围查找\n");
    printf("请选择查找方式: ");
    
    int choice;
    if (!safe_input_int(&choice, 1, 4)) {
        show_error("选择无效");
        return;
    }
    
    switch (choice) {
        case 1:
            search_by_id(system);
            break;
        case 2:
            search_by_name(system);
            break;
        case 3:
            search_by_score_range(system);
            break;
        case 4:
            search_by_age_range(system);
            break;
    }
}

// 按学号查找
void search_by_id(StudentSystem *system) {
    char id[MAX_ID_LEN];
    printf("请输入学号: ");
    safe_input_string(id, MAX_ID_LEN);
    
    int index = find_student_by_id(system, id);
    if (index == -1) {
        show_error("未找到该学号的学生");
        return;
    }
    
    printf("\n查找结果：\n");
    display_student_info(&system->students[index], index + 1);
}

// 按姓名查找（支持模糊匹配）
void search_by_name(StudentSystem *system) {
    char name[MAX_NAME_LEN];
    printf("请输入姓名（支持部分匹配）: ");
    safe_input_string(name, MAX_NAME_LEN);
    
    trim_string(name);
    to_lowercase(name);  // 转换为小写进行比较
    
    int found_count = 0;
    printf("\n查找结果：\n");
    
    for (int i = 0; i < system->count; i++) {
        char student_name[MAX_NAME_LEN];
        strcpy(student_name, system->students[i].name);
        to_lowercase(student_name);
        
        if (strstr(student_name, name) != NULL) {
            if (found_count == 0) {
                print_table_header();
            }
            display_student_info(&system->students[i], i + 1);
            found_count++;
        }
    }
    
    if (found_count == 0) {
        show_error("未找到匹配的学生");
    } else {
        printf("\n共找到 %d 个匹配的学生\n", found_count);
    }
}

// 按成绩范围查找
void search_by_score_range(StudentSystem *system) {
    float min_score, max_score;
    
    printf("请输入最低平均分: ");
    if (!safe_input_float(&min_score, 0, 100)) {
        show_error("成绩无效");
        return;
    }
    
    printf("请输入最高平均分: ");
    if (!safe_input_float(&max_score, min_score, 100)) {
        show_error("成绩无效");
        return;
    }
    
    int found_count = 0;
    printf("\n查找结果（平均分 %.1f - %.1f）：\n", min_score, max_score);
    
    for (int i = 0; i < system->count; i++) {
        if (system->students[i].average_score >= min_score && 
            system->students[i].average_score <= max_score) {
            
            if (found_count == 0) {
                print_table_header();
            }
            display_student_info(&system->students[i], i + 1);
            found_count++;
        }
    }
    
    if (found_count == 0) {
        show_error("未找到符合条件的学生");
    } else {
        printf("\n共找到 %d 个符合条件的学生\n", found_count);
    }
}

// 按年龄范围查找
void search_by_age_range(StudentSystem *system) {
    int min_age, max_age;
    
    printf("请输入最小年龄: ");
    if (!safe_input_int(&min_age, 15, 30)) {
        show_error("年龄无效");
        return;
    }
    
    printf("请输入最大年龄: ");
    if (!safe_input_int(&max_age, min_age, 30)) {
        show_error("年龄无效");
        return;
    }
    
    int found_count = 0;
    printf("\n查找结果（年龄 %d - %d）：\n", min_age, max_age);
    
    for (int i = 0; i < system->count; i++) {
        if (system->students[i].age >= min_age && 
            system->students[i].age <= max_age) {
            
            if (found_count == 0) {
                print_table_header();
            }
            display_student_info(&system->students[i], i + 1);
            found_count++;
        }
    }
    
    if (found_count == 0) {
        show_error("未找到符合条件的学生");
    } else {
        printf("\n共找到 %d 个符合条件的学生\n", found_count);
    }
}
```

---

## 显示和排序功能

### 显示功能实现

```c
void display_all_students(StudentSystem *system) {
    if (system == NULL || system->count == 0) {
        show_error("系统中没有学生信息");
        return;
    }
    
    printf("\n=== 所有学生信息 ===\n");
    print_table_header();
    
    for (int i = 0; i < system->count; i++) {
        display_student_info(&system->students[i], i + 1);
    }
    
    printf("\n共有 %d 个学生\n", system->count);
}

// 打印表格头部
void print_table_header() {
    printf("+----+----------+----------+-----+------+------+------+------+--------+\n");
    printf("| 序号 |   学号   |   姓名   | 年龄 | 数学 | 英语 | 语文 | 平均 |  等级  |\n");
    printf("+----+----------+----------+-----+------+------+------+------+--------+\n");
}

// 显示单个学生信息
void display_student_info(Student *student, int index) {
    printf("| %2d | %-8s | %-8s | %3d | %4.1f | %4.1f | %4.1f | %4.1f | %-6s |\n",
           index,
           student->id,
           student->name,
           student->age,
           student->math_score,
           student->english_score,
           student->chinese_score,
           student->average_score,
           get_grade_level(student->average_score));
}
```

### 排序功能实现

```c
void sort_students(StudentSystem *system) {
    if (system == NULL || system->count <= 1) {
        show_error("没有足够的学生信息进行排序");
        return;
    }
    
    printf("\n=== 学生信息排序 ===\n");
    printf("1. 按平均分排序（降序）\n");
    printf("2. 按平均分排序（升序）\n");
    printf("3. 按学号排序\n");
    printf("4. 按姓名排序\n");
    printf("5. 按年龄排序\n");
    printf("请选择排序方式: ");
    
    int choice;
    if (!safe_input_int(&choice, 1, 5)) {
        show_error("选择无效");
        return;
    }
    
    switch (choice) {
        case 1:
            sort_by_average_desc(system);
            printf("按平均分降序排序完成\n");
            break;
        case 2:
            sort_by_average_asc(system);
            printf("按平均分升序排序完成\n");
            break;
        case 3:
            sort_by_id(system);
            printf("按学号排序完成\n");
            break;
        case 4:
            sort_by_name(system);
            printf("按姓名排序完成\n");
            break;
        case 5:
            sort_by_age(system);
            printf("按年龄排序完成\n");
            break;
    }
    
    // 显示排序后的结果
    display_all_students(system);
}

// 按平均分降序排序
void sort_by_average_desc(StudentSystem *system) {
    for (int i = 0; i < system->count - 1; i++) {
        for (int j = 0; j < system->count - 1 - i; j++) {
            if (system->students[j].average_score < 
                system->students[j + 1].average_score) {
                swap_students(&system->students[j], &system->students[j + 1]);
            }
        }
    }
}

// 按平均分升序排序
void sort_by_average_asc(StudentSystem *system) {
    for (int i = 0; i < system->count - 1; i++) {
        for (int j = 0; j < system->count - 1 - i; j++) {
            if (system->students[j].average_score > 
                system->students[j + 1].average_score) {
                swap_students(&system->students[j], &system->students[j + 1]);
            }
        }
    }
}

// 按学号排序
void sort_by_id(StudentSystem *system) {
    for (int i = 0; i < system->count - 1; i++) {
        for (int j = 0; j < system->count - 1 - i; j++) {
            if (strcmp(system->students[j].id, system->students[j + 1].id) > 0) {
                swap_students(&system->students[j], &system->students[j + 1]);
            }
        }
    }
}

// 按姓名排序
void sort_by_name(StudentSystem *system) {
    for (int i = 0; i < system->count - 1; i++) {
        for (int j = 0; j < system->count - 1 - i; j++) {
            if (strcmp(system->students[j].name, system->students[j + 1].name) > 0) {
                swap_students(&system->students[j], &system->students[j + 1]);
            }
        }
    }
}

// 按年龄排序
void sort_by_age(StudentSystem *system) {
    for (int i = 0; i < system->count - 1; i++) {
        for (int j = 0; j < system->count - 1 - i; j++) {
            if (system->students[j].age > system->students[j + 1].age) {
                swap_students(&system->students[j], &system->students[j + 1]);
            }
        }
    }
}

// 交换两个学生的信息
void swap_students(Student *a, Student *b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}
```

---

## 统计分析功能

### 实现代码

```c
void calculate_statistics(StudentSystem *system) {
    if (system == NULL || system->count == 0) {
        show_error("系统中没有学生信息");
        return;
    }
    
    printf("\n=== 统计分析报告 ===\n");
    
    // 基本统计
    printf("\n1. 基本信息统计\n");
    printf("   总学生数: %d\n", system->count);
    
    // 年龄统计
    int age_sum = 0;
    int min_age = system->students[0].age;
    int max_age = system->students[0].age;
    
    for (int i = 0; i < system->count; i++) {
        age_sum += system->students[i].age;
        if (system->students[i].age < min_age) {
            min_age = system->students[i].age;
        }
        if (system->students[i].age > max_age) {
            max_age = system->students[i].age;
        }
    }
    
    printf("   平均年龄: %.1f\n", (float)age_sum / system->count);
    printf("   年龄范围: %d - %d\n", min_age, max_age);
    
    // 成绩统计
    printf("\n2. 成绩统计\n");
    
    float math_sum = 0, english_sum = 0, chinese_sum = 0, average_sum = 0;
    float math_max = 0, english_max = 0, chinese_max = 0, average_max = 0;
    float math_min = 100, english_min = 100, chinese_min = 100, average_min = 100;
    
    for (int i = 0; i < system->count; i++) {
        Student *s = &system->students[i];
        
        // 累加
        math_sum += s->math_score;
        english_sum += s->english_score;
        chinese_sum += s->chinese_score;
        average_sum += s->average_score;
        
        // 最大值
        if (s->math_score > math_max) math_max = s->math_score;
        if (s->english_score > english_max) english_max = s->english_score;
        if (s->chinese_score > chinese_max) chinese_max = s->chinese_score;
        if (s->average_score > average_max) average_max = s->average_score;
        
        // 最小值
        if (s->math_score < math_min) math_min = s->math_score;
        if (s->english_score < english_min) english_min = s->english_score;
        if (s->chinese_score < chinese_min) chinese_min = s->chinese_score;
        if (s->average_score < average_min) average_min = s->average_score;
    }
    
    printf("   数学: 平均%.1f, 最高%.1f, 最低%.1f\n", 
           math_sum / system->count, math_max, math_min);
    printf("   英语: 平均%.1f, 最高%.1f, 最低%.1f\n", 
           english_sum / system->count, english_max, english_min);
    printf("   语文: 平均%.1f, 最高%.1f, 最低%.1f\n", 
           chinese_sum / system->count, chinese_max, chinese_min);
    printf("   总平均: 平均%.1f, 最高%.1f, 最低%.1f\n", 
           average_sum / system->count, average_max, average_min);
    
    // 等级分布统计
    printf("\n3. 成绩等级分布\n");
    
    int excellent = 0, good = 0, pass = 0, fail = 0;
    
    for (int i = 0; i < system->count; i++) {
        float avg = system->students[i].average_score;
        if (avg >= GRADE_EXCELLENT) {
            excellent++;
        } else if (avg >= GRADE_GOOD) {
            good++;
        } else if (avg >= GRADE_PASS) {
            pass++;
        } else {
            fail++;
        }
    }
    
    printf("   优秀(>=90): %d人 (%.1f%%)\n", 
           excellent, (float)excellent * 100 / system->count);
    printf("   良好(80-89): %d人 (%.1f%%)\n", 
           good, (float)good * 100 / system->count);
    printf("   及格(60-79): %d人 (%.1f%%)\n", 
           pass, (float)pass * 100 / system->count);
    printf("   不及格(<60): %d人 (%.1f%%)\n", 
           fail, (float)fail * 100 / system->count);
    
    // 单科最优学生
    printf("\n4. 单科最优学生\n");
    
    int math_best = 0, english_best = 0, chinese_best = 0, average_best = 0;
    
    for (int i = 1; i < system->count; i++) {
        if (system->students[i].math_score > system->students[math_best].math_score) {
            math_best = i;
        }
        if (system->students[i].english_score > system->students[english_best].english_score) {
            english_best = i;
        }
        if (system->students[i].chinese_score > system->students[chinese_best].chinese_score) {
            chinese_best = i;
        }
        if (system->students[i].average_score > system->students[average_best].average_score) {
            average_best = i;
        }
    }
    
    printf("   数学最高: %s (%.1f分)\n", 
           system->students[math_best].name, system->students[math_best].math_score);
    printf("   英语最高: %s (%.1f分)\n", 
           system->students[english_best].name, system->students[english_best].english_score);
    printf("   语文最高: %s (%.1f分)\n", 
           system->students[chinese_best].name, system->students[chinese_best].chinese_score);
    printf("   总分最高: %s (%.1f分)\n", 
           system->students[average_best].name, system->students[average_best].average_score);
}
```

---

## 实践练习

### 练习1：高级查找功能

实现一个组合查找功能：

```c
// 练习：实现组合查找
// 要求：可以同时按多个条件查找学生
// 例如：年龄18-20岁，且平均分80分以上的学生

void advanced_search(StudentSystem *system) {
    // TODO: 实现组合查找功能
    // 提示：
    // 1. 提供多个查找条件选项
    // 2. 允许用户选择多个条件
    // 3. 使用逻辑AND组合条件
    // 4. 显示符合所有条件的学生
}
```

### 练习2：成绩分析功能

实现更详细的成绩分析：

```c
// 练习：实现成绩分析功能
void analyze_scores(StudentSystem *system) {
    // TODO: 实现以下分析功能
    // 1. 计算各科成绩的标准差
    // 2. 找出偏科严重的学生（某科与平均分差距大）
    // 3. 分析成绩分布（直方图）
    // 4. 计算各科及格率
    // 5. 找出进步空间最大的学生
}
```

### 练习3：排名功能

实现学生排名系统：

```c
// 练习：实现排名功能
void calculate_rankings(StudentSystem *system) {
    // TODO: 实现以下排名功能
    // 1. 总分排名
    // 2. 单科排名
    // 3. 显示每个学生的排名信息
    // 4. 处理并列排名的情况
    // 5. 生成排名报告
}
```

---

## 💡 本课要点总结

### 核心功能设计
1. **输入验证**: 所有用户输入都要严格验证
2. **错误处理**: 优雅处理各种异常情况
3. **用户体验**: 提供清晰的提示和反馈
4. **数据一致性**: 确保操作后数据的完整性

### 算法选择
1. **查找算法**: 线性查找适合小规模数据
2. **排序算法**: 冒泡排序简单易懂，适合教学
3. **字符串处理**: 使用标准库函数处理字符串
4. **数组操作**: 注意边界检查和内存管理

### 代码组织
1. **函数职责**: 每个函数只负责一个具体功能
2. **参数传递**: 合理使用指针传递结构体
3. **返回值**: 统一的成功/失败返回值约定
4. **代码复用**: 提取公共功能为独立函数

### 用户界面
1. **菜单设计**: 清晰的选项和提示
2. **信息显示**: 格式化的表格输出
3. **错误提示**: 友好的错误信息
4. **操作确认**: 重要操作需要用户确认

---

## 🚀 下一步

完成本课学习后，你应该能够：
- ✅ 实现完整的CRUD操作
- ✅ 设计用户友好的交互界面
- ✅ 处理各种边界情况和错误
- ✅ 实现数据查找、排序和统计功能

**准备好了吗？** 让我们进入 [第6课：文件操作与数据持久化](./06-file-operations.md)，学习如何保存和加载数据！

---

*💭 思考题：如何优化查找算法的性能？在什么情况下应该使用二分查找而不是线性查找？*