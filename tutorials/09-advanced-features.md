# 第9课：高级功能扩展

> **学习目标**：掌握为学生管理系统添加高级功能的方法，包括数据分析、导入导出、用户权限管理等

---

## 📋 本课内容

1. [数据分析与统计](#数据分析与统计)
2. [导入导出功能](#导入导出功能)
3. [用户权限管理](#用户权限管理)
4. [网络功能](#网络功能)
5. [配置管理](#配置管理)
6. [插件系统](#插件系统)
7. [实践练习](#实践练习)

---

## 数据分析与统计

### 统计分析模块

```c
// statistics.h
#ifndef STATISTICS_H
#define STATISTICS_H

#include "student.h"

// 统计数据结构
typedef struct {
    double mean;        // 平均值
    double median;      // 中位数
    double mode;        // 众数
    double std_dev;     // 标准差
    double variance;    // 方差
    double min;         // 最小值
    double max;         // 最大值
    int count;          // 数据数量
} StatisticsData;

// 成绩分布
typedef struct {
    int excellent;      // 优秀 (90-100)
    int good;          // 良好 (80-89)
    int average;       // 中等 (70-79)
    int poor;          // 及格 (60-69)
    int fail;          // 不及格 (<60)
} GradeDistribution;

// 年龄分布
typedef struct {
    int age_groups[10]; // 年龄组分布
    int min_age;
    int max_age;
    double avg_age;
} AgeDistribution;

// 统计函数
StatisticsData calculate_statistics(double *data, int count);
GradeDistribution analyze_grade_distribution(StudentSystem *system, int subject);
AgeDistribution analyze_age_distribution(StudentSystem *system);
void generate_statistics_report(StudentSystem *system);
void export_statistics_to_csv(StudentSystem *system, const char *filename);

// 数据可视化（简单的ASCII图表）
void draw_histogram(double *data, int count, const char *title);
void draw_grade_chart(GradeDistribution *dist);
void print_correlation_matrix(StudentSystem *system);

#endif // STATISTICS_H
```

### 统计分析实现

```c
// statistics.c
#include "statistics.h"
#include <math.h>
#include <string.h>

// 计算基本统计数据
StatisticsData calculate_statistics(double *data, int count) {
    StatisticsData stats = {0};
    
    if (count == 0) {
        return stats;
    }
    
    stats.count = count;
    
    // 计算平均值
    double sum = 0;
    for (int i = 0; i < count; i++) {
        sum += data[i];
    }
    stats.mean = sum / count;
    
    // 找最大值和最小值
    stats.min = stats.max = data[0];
    for (int i = 1; i < count; i++) {
        if (data[i] < stats.min) stats.min = data[i];
        if (data[i] > stats.max) stats.max = data[i];
    }
    
    // 计算方差和标准差
    double variance_sum = 0;
    for (int i = 0; i < count; i++) {
        double diff = data[i] - stats.mean;
        variance_sum += diff * diff;
    }
    stats.variance = variance_sum / count;
    stats.std_dev = sqrt(stats.variance);
    
    // 计算中位数（需要排序）
    double *sorted_data = malloc(count * sizeof(double));
    memcpy(sorted_data, data, count * sizeof(double));
    
    // 简单冒泡排序
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (sorted_data[j] > sorted_data[j + 1]) {
                double temp = sorted_data[j];
                sorted_data[j] = sorted_data[j + 1];
                sorted_data[j + 1] = temp;
            }
        }
    }
    
    if (count % 2 == 0) {
        stats.median = (sorted_data[count/2 - 1] + sorted_data[count/2]) / 2.0;
    } else {
        stats.median = sorted_data[count/2];
    }
    
    free(sorted_data);
    
    // 计算众数（简化版本）
    stats.mode = stats.mean; // 简化处理
    
    return stats;
}

// 分析成绩分布
GradeDistribution analyze_grade_distribution(StudentSystem *system, int subject) {
    GradeDistribution dist = {0};
    
    for (int i = 0; i < system->count; i++) {
        double score;
        switch (subject) {
            case 0: score = system->students[i].math_score; break;
            case 1: score = system->students[i].english_score; break;
            case 2: score = system->students[i].chinese_score; break;
            default: score = system->students[i].average; break;
        }
        
        if (score >= 90) dist.excellent++;
        else if (score >= 80) dist.good++;
        else if (score >= 70) dist.average++;
        else if (score >= 60) dist.poor++;
        else dist.fail++;
    }
    
    return dist;
}

// 分析年龄分布
AgeDistribution analyze_age_distribution(StudentSystem *system) {
    AgeDistribution dist = {0};
    
    if (system->count == 0) {
        return dist;
    }
    
    dist.min_age = dist.max_age = system->students[0].age;
    int age_sum = 0;
    
    for (int i = 0; i < system->count; i++) {
        int age = system->students[i].age;
        age_sum += age;
        
        if (age < dist.min_age) dist.min_age = age;
        if (age > dist.max_age) dist.max_age = age;
        
        // 年龄组分布（假设年龄在15-25之间）
        if (age >= 15 && age <= 25) {
            dist.age_groups[age - 15]++;
        }
    }
    
    dist.avg_age = (double)age_sum / system->count;
    
    return dist;
}

// 生成统计报告
void generate_statistics_report(StudentSystem *system) {
    printf("\n" BLUE "=== 学生管理系统统计报告 ===" RESET "\n");
    printf("生成时间: %s\n", get_current_time());
    printf("总学生数: %d\n\n", system->count);
    
    if (system->count == 0) {
        printf("暂无学生数据\n");
        return;
    }
    
    // 提取各科成绩数据
    double *math_scores = malloc(system->count * sizeof(double));
    double *english_scores = malloc(system->count * sizeof(double));
    double *chinese_scores = malloc(system->count * sizeof(double));
    double *averages = malloc(system->count * sizeof(double));
    
    for (int i = 0; i < system->count; i++) {
        math_scores[i] = system->students[i].math_score;
        english_scores[i] = system->students[i].english_score;
        chinese_scores[i] = system->students[i].chinese_score;
        averages[i] = system->students[i].average;
    }
    
    // 各科统计
    const char *subjects[] = {"数学", "英语", "语文", "平均分"};
    double *subject_data[] = {math_scores, english_scores, chinese_scores, averages};
    
    for (int s = 0; s < 4; s++) {
        printf(YELLOW "--- %s成绩统计 ---" RESET "\n", subjects[s]);
        StatisticsData stats = calculate_statistics(subject_data[s], system->count);
        
        printf("平均分: %.2f\n", stats.mean);
        printf("中位数: %.2f\n", stats.median);
        printf("标准差: %.2f\n", stats.std_dev);
        printf("最高分: %.2f\n", stats.max);
        printf("最低分: %.2f\n", stats.min);
        
        // 成绩分布
        GradeDistribution dist = analyze_grade_distribution(system, s);
        printf("成绩分布:\n");
        printf("  优秀(90-100): %d人 (%.1f%%)\n", 
               dist.excellent, (double)dist.excellent / system->count * 100);
        printf("  良好(80-89):  %d人 (%.1f%%)\n", 
               dist.good, (double)dist.good / system->count * 100);
        printf("  中等(70-79):  %d人 (%.1f%%)\n", 
               dist.average, (double)dist.average / system->count * 100);
        printf("  及格(60-69):  %d人 (%.1f%%)\n", 
               dist.poor, (double)dist.poor / system->count * 100);
        printf("  不及格(<60):  %d人 (%.1f%%)\n", 
               dist.fail, (double)dist.fail / system->count * 100);
        
        draw_grade_chart(&dist);
        printf("\n");
    }
    
    // 年龄分布
    printf(YELLOW "--- 年龄分布统计 ---" RESET "\n");
    AgeDistribution age_dist = analyze_age_distribution(system);
    printf("平均年龄: %.1f岁\n", age_dist.avg_age);
    printf("年龄范围: %d - %d岁\n", age_dist.min_age, age_dist.max_age);
    
    printf("年龄分布:\n");
    for (int i = 0; i < 10; i++) {
        if (age_dist.age_groups[i] > 0) {
            printf("  %d岁: %d人\n", i + 15, age_dist.age_groups[i]);
        }
    }
    
    // 清理内存
    free(math_scores);
    free(english_scores);
    free(chinese_scores);
    free(averages);
    
    printf("\n" BLUE "=== 报告结束 ===" RESET "\n");
}

// 绘制成绩分布图表
void draw_grade_chart(GradeDistribution *dist) {
    printf("\n成绩分布图:\n");
    
    int total = dist->excellent + dist->good + dist->average + dist->poor + dist->fail;
    if (total == 0) return;
    
    const char *grades[] = {"优秀", "良好", "中等", "及格", "不及格"};
    int counts[] = {dist->excellent, dist->good, dist->average, dist->poor, dist->fail};
    const char *colors[] = {GREEN, BLUE, YELLOW, CYAN, RED};
    
    for (int i = 0; i < 5; i++) {
        printf("%s%s: ", colors[i], grades[i]);
        int bar_length = (counts[i] * 20) / total; // 最大20个字符
        for (int j = 0; j < bar_length; j++) {
            printf("█");
        }
        printf(" %d人" RESET "\n", counts[i]);
    }
}

// 导出统计数据到CSV
void export_statistics_to_csv(StudentSystem *system, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf(RED "错误: 无法创建统计文件 %s" RESET "\n", filename);
        return;
    }
    
    fprintf(file, "统计项目,数学,英语,语文,平均分\n");
    
    // 计算各科统计数据
    double *subject_data[4];
    for (int i = 0; i < 4; i++) {
        subject_data[i] = malloc(system->count * sizeof(double));
    }
    
    for (int i = 0; i < system->count; i++) {
        subject_data[0][i] = system->students[i].math_score;
        subject_data[1][i] = system->students[i].english_score;
        subject_data[2][i] = system->students[i].chinese_score;
        subject_data[3][i] = system->students[i].average;
    }
    
    // 写入统计数据
    const char *stat_names[] = {"平均分", "中位数", "标准差", "最高分", "最低分"};
    
    for (int stat = 0; stat < 5; stat++) {
        fprintf(file, "%s,", stat_names[stat]);
        
        for (int subject = 0; subject < 4; subject++) {
            StatisticsData stats = calculate_statistics(subject_data[subject], system->count);
            
            double value;
            switch (stat) {
                case 0: value = stats.mean; break;
                case 1: value = stats.median; break;
                case 2: value = stats.std_dev; break;
                case 3: value = stats.max; break;
                case 4: value = stats.min; break;
            }
            
            fprintf(file, "%.2f", value);
            if (subject < 3) fprintf(file, ",");
        }
        fprintf(file, "\n");
    }
    
    // 清理内存
    for (int i = 0; i < 4; i++) {
        free(subject_data[i]);
    }
    
    fclose(file);
    printf(GREEN "统计数据已导出到 %s" RESET "\n", filename);
}
```

---

## 导入导出功能

### 多格式支持

```c
// import_export.h
#ifndef IMPORT_EXPORT_H
#define IMPORT_EXPORT_H

#include "student.h"

// 支持的文件格式
typedef enum {
    FORMAT_CSV,
    FORMAT_JSON,
    FORMAT_XML,
    FORMAT_BINARY
} FileFormat;

// 导入导出选项
typedef struct {
    FileFormat format;
    bool include_header;
    char delimiter;
    bool validate_data;
    bool backup_existing;
} ImportExportOptions;

// 导入导出函数
int import_students(StudentSystem *system, const char *filename, ImportExportOptions *options);
int export_students(StudentSystem *system, const char *filename, ImportExportOptions *options);

// 格式特定函数
int import_csv(StudentSystem *system, const char *filename, ImportExportOptions *options);
int export_csv(StudentSystem *system, const char *filename, ImportExportOptions *options);
int import_json(StudentSystem *system, const char *filename);
int export_json(StudentSystem *system, const char *filename);
int import_xml(StudentSystem *system, const char *filename);
int export_xml(StudentSystem *system, const char *filename);

// 数据验证
bool validate_import_data(Student *student);
void fix_import_data(Student *student);

// 批量操作
int batch_import_from_directory(StudentSystem *system, const char *directory);
int batch_export_by_criteria(StudentSystem *system, const char *directory);

#endif // IMPORT_EXPORT_H
```

### CSV导入导出实现

```c
// import_export.c
#include "import_export.h"
#include <ctype.h>

// CSV导出
int export_csv(StudentSystem *system, const char *filename, ImportExportOptions *options) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        log_error("无法创建CSV文件: %s", filename);
        return ERROR_FILE_OPEN;
    }
    
    char delim = options ? options->delimiter : ',';
    
    // 写入表头
    if (!options || options->include_header) {
        fprintf(file, "学号%c姓名%c年龄%c数学%c英语%c语文%c平均分%c是否删除\n",
                delim, delim, delim, delim, delim, delim, delim);
    }
    
    // 写入学生数据
    for (int i = 0; i < system->count; i++) {
        Student *s = &system->students[i];
        fprintf(file, "%s%c%s%c%d%c%.2f%c%.2f%c%.2f%c%.2f%c%d\n",
                s->id, delim, s->name, delim, s->age, delim,
                s->math_score, delim, s->english_score, delim,
                s->chinese_score, delim, s->average, delim, s->is_deleted);
    }
    
    fclose(file);
    log_info("成功导出 %d 条学生记录到 %s", system->count, filename);
    return SUCCESS;
}

// CSV导入
int import_csv(StudentSystem *system, const char *filename, ImportExportOptions *options) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        log_error("无法打开CSV文件: %s", filename);
        return ERROR_FILE_OPEN;
    }
    
    char line[1024];
    int line_number = 0;
    int imported_count = 0;
    int error_count = 0;
    
    char delim = options ? options->delimiter : ',';
    bool validate = options ? options->validate_data : true;
    
    // 跳过表头
    if (!options || options->include_header) {
        if (fgets(line, sizeof(line), file)) {
            line_number++;
        }
    }
    
    // 读取数据行
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        
        // 移除换行符
        line[strcspn(line, "\n")] = 0;
        
        Student student = {0};
        
        // 解析CSV行
        char *token = strtok(line, &delim);
        int field = 0;
        
        while (token && field < 8) {
            // 移除前后空格
            while (isspace(*token)) token++;
            char *end = token + strlen(token) - 1;
            while (end > token && isspace(*end)) *end-- = 0;
            
            switch (field) {
                case 0: strncpy(student.id, token, sizeof(student.id) - 1); break;
                case 1: strncpy(student.name, token, sizeof(student.name) - 1); break;
                case 2: student.age = atoi(token); break;
                case 3: student.math_score = atof(token); break;
                case 4: student.english_score = atof(token); break;
                case 5: student.chinese_score = atof(token); break;
                case 6: student.average = atof(token); break;
                case 7: student.is_deleted = atoi(token); break;
            }
            
            token = strtok(NULL, &delim);
            field++;
        }
        
        // 验证数据
        if (validate && !validate_import_data(&student)) {
            log_warning("第 %d 行数据无效，已跳过: %s", line_number, student.id);
            error_count++;
            continue;
        }
        
        // 修复数据
        fix_import_data(&student);
        
        // 检查是否已存在
        if (find_student_by_id(system, student.id) >= 0) {
            log_warning("第 %d 行学号已存在，已跳过: %s", line_number, student.id);
            error_count++;
            continue;
        }
        
        // 添加学生
        if (add_student(system, &student) == SUCCESS) {
            imported_count++;
        } else {
            log_error("第 %d 行添加失败: %s", line_number, student.id);
            error_count++;
        }
    }
    
    fclose(file);
    
    printf(GREEN "导入完成: 成功 %d 条，错误 %d 条" RESET "\n", 
           imported_count, error_count);
    
    return imported_count > 0 ? SUCCESS : ERROR_IMPORT_FAILED;
}

// JSON导出
int export_json(StudentSystem *system, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        log_error("无法创建JSON文件: %s", filename);
        return ERROR_FILE_OPEN;
    }
    
    fprintf(file, "{\n");
    fprintf(file, "  \"version\": \"1.0\",\n");
    fprintf(file, "  \"export_time\": \"%s\",\n", get_current_time());
    fprintf(file, "  \"total_count\": %d,\n", system->count);
    fprintf(file, "  \"students\": [\n");
    
    for (int i = 0; i < system->count; i++) {
        Student *s = &system->students[i];
        fprintf(file, "    {\n");
        fprintf(file, "      \"id\": \"%s\",\n", s->id);
        fprintf(file, "      \"name\": \"%s\",\n", s->name);
        fprintf(file, "      \"age\": %d,\n", s->age);
        fprintf(file, "      \"scores\": {\n");
        fprintf(file, "        \"math\": %.2f,\n", s->math_score);
        fprintf(file, "        \"english\": %.2f,\n", s->english_score);
        fprintf(file, "        \"chinese\": %.2f,\n", s->chinese_score);
        fprintf(file, "        \"average\": %.2f\n", s->average);
        fprintf(file, "      },\n");
        fprintf(file, "      \"is_deleted\": %s\n", s->is_deleted ? "true" : "false");
        fprintf(file, "    }");
        
        if (i < system->count - 1) {
            fprintf(file, ",");
        }
        fprintf(file, "\n");
    }
    
    fprintf(file, "  ]\n");
    fprintf(file, "}\n");
    
    fclose(file);
    log_info("成功导出 %d 条学生记录到JSON文件: %s", system->count, filename);
    return SUCCESS;
}

// 数据验证
bool validate_import_data(Student *student) {
    // 检查学号
    if (strlen(student->id) == 0 || strlen(student->id) > 19) {
        return false;
    }
    
    // 检查姓名
    if (strlen(student->name) == 0 || strlen(student->name) > 49) {
        return false;
    }
    
    // 检查年龄
    if (student->age < 10 || student->age > 100) {
        return false;
    }
    
    // 检查成绩
    if (student->math_score < 0 || student->math_score > 100 ||
        student->english_score < 0 || student->english_score > 100 ||
        student->chinese_score < 0 || student->chinese_score > 100) {
        return false;
    }
    
    return true;
}

// 修复导入数据
void fix_import_data(Student *student) {
    // 重新计算平均分
    calculate_average(student);
    
    // 确保字符串以null结尾
    student->id[sizeof(student->id) - 1] = '\0';
    student->name[sizeof(student->name) - 1] = '\0';
    
    // 修正年龄范围
    if (student->age < 10) student->age = 18;
    if (student->age > 100) student->age = 22;
    
    // 修正成绩范围
    if (student->math_score < 0) student->math_score = 0;
    if (student->math_score > 100) student->math_score = 100;
    if (student->english_score < 0) student->english_score = 0;
    if (student->english_score > 100) student->english_score = 100;
    if (student->chinese_score < 0) student->chinese_score = 0;
    if (student->chinese_score > 100) student->chinese_score = 100;
}
```

---

## 用户权限管理

### 用户系统设计

```c
// user_management.h
#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <stdbool.h>
#include <time.h>

// 用户角色
typedef enum {
    ROLE_ADMIN,     // 管理员
    ROLE_TEACHER,   // 教师
    ROLE_STUDENT,   // 学生
    ROLE_GUEST      // 访客
} UserRole;

// 权限类型
typedef enum {
    PERM_READ = 1,      // 读取权限
    PERM_WRITE = 2,     // 写入权限
    PERM_DELETE = 4,    // 删除权限
    PERM_ADMIN = 8,     // 管理权限
    PERM_EXPORT = 16,   // 导出权限
    PERM_IMPORT = 32,   // 导入权限
    PERM_STATS = 64     // 统计权限
} Permission;

// 用户信息
typedef struct {
    char username[32];
    char password_hash[65]; // SHA-256哈希
    char salt[17];          // 盐值
    UserRole role;
    int permissions;
    time_t created_time;
    time_t last_login;
    int login_attempts;
    bool is_locked;
    bool is_active;
} User;

// 用户系统
typedef struct {
    User users[100];
    int user_count;
    User *current_user;
    int max_login_attempts;
    int session_timeout;
} UserSystem;

// 用户管理函数
int init_user_system(UserSystem *system);
int create_user(UserSystem *system, const char *username, const char *password, UserRole role);
int authenticate_user(UserSystem *system, const char *username, const char *password);
void logout_user(UserSystem *system);
bool check_permission(UserSystem *system, Permission perm);
int change_password(UserSystem *system, const char *old_password, const char *new_password);
int reset_password(UserSystem *system, const char *username, const char *new_password);
void lock_user(UserSystem *system, const char *username);
void unlock_user(UserSystem *system, const char *username);

// 密码安全
void generate_salt(char *salt, int length);
void hash_password(const char *password, const char *salt, char *hash);
bool verify_password(const char *password, const char *salt, const char *hash);
bool is_strong_password(const char *password);

// 会话管理
bool is_session_valid(UserSystem *system);
void refresh_session(UserSystem *system);
void cleanup_expired_sessions(UserSystem *system);

#endif // USER_MANAGEMENT_H
```

### 用户管理实现

```c
// user_management.c
#include "user_management.h"
#include <string.h>
#include <stdlib.h>
#include <crypt.h> // 需要链接 -lcrypt

// 默认权限配置
static const int default_permissions[] = {
    [ROLE_ADMIN] = PERM_READ | PERM_WRITE | PERM_DELETE | PERM_ADMIN | PERM_EXPORT | PERM_IMPORT | PERM_STATS,
    [ROLE_TEACHER] = PERM_READ | PERM_WRITE | PERM_EXPORT | PERM_STATS,
    [ROLE_STUDENT] = PERM_READ,
    [ROLE_GUEST] = PERM_READ
};

// 初始化用户系统
int init_user_system(UserSystem *system) {
    memset(system, 0, sizeof(UserSystem));
    system->max_login_attempts = 3;
    system->session_timeout = 3600; // 1小时
    
    // 创建默认管理员账户
    if (system->user_count == 0) {
        create_user(system, "admin", "admin123", ROLE_ADMIN);
        log_info("已创建默认管理员账户: admin/admin123");
    }
    
    return SUCCESS;
}

// 创建用户
int create_user(UserSystem *system, const char *username, const char *password, UserRole role) {
    if (system->user_count >= 100) {
        return ERROR_SYSTEM_FULL;
    }
    
    // 检查用户名是否已存在
    for (int i = 0; i < system->user_count; i++) {
        if (strcmp(system->users[i].username, username) == 0) {
            return ERROR_USER_EXISTS;
        }
    }
    
    // 检查密码强度
    if (!is_strong_password(password)) {
        return ERROR_WEAK_PASSWORD;
    }
    
    User *user = &system->users[system->user_count];
    
    // 设置用户信息
    strncpy(user->username, username, sizeof(user->username) - 1);
    user->role = role;
    user->permissions = default_permissions[role];
    user->created_time = time(NULL);
    user->login_attempts = 0;
    user->is_locked = false;
    user->is_active = true;
    
    // 生成盐值和密码哈希
    generate_salt(user->salt, 16);
    hash_password(password, user->salt, user->password_hash);
    
    system->user_count++;
    
    log_info("用户创建成功: %s (角色: %d)", username, role);
    return SUCCESS;
}

// 用户认证
int authenticate_user(UserSystem *system, const char *username, const char *password) {
    User *user = NULL;
    
    // 查找用户
    for (int i = 0; i < system->user_count; i++) {
        if (strcmp(system->users[i].username, username) == 0) {
            user = &system->users[i];
            break;
        }
    }
    
    if (!user) {
        log_warning("登录失败: 用户不存在 - %s", username);
        return ERROR_USER_NOT_FOUND;
    }
    
    if (user->is_locked) {
        log_warning("登录失败: 用户已锁定 - %s", username);
        return ERROR_USER_LOCKED;
    }
    
    if (!user->is_active) {
        log_warning("登录失败: 用户已禁用 - %s", username);
        return ERROR_USER_INACTIVE;
    }
    
    // 验证密码
    if (!verify_password(password, user->salt, user->password_hash)) {
        user->login_attempts++;
        
        if (user->login_attempts >= system->max_login_attempts) {
            user->is_locked = true;
            log_warning("用户已锁定: 登录失败次数过多 - %s", username);
            return ERROR_USER_LOCKED;
        }
        
        log_warning("登录失败: 密码错误 - %s (尝试次数: %d)", 
                   username, user->login_attempts);
        return ERROR_INVALID_PASSWORD;
    }
    
    // 登录成功
    user->login_attempts = 0;
    user->last_login = time(NULL);
    system->current_user = user;
    
    log_info("用户登录成功: %s", username);
    return SUCCESS;
}

// 检查权限
bool check_permission(UserSystem *system, Permission perm) {
    if (!system->current_user) {
        return false;
    }
    
    if (!is_session_valid(system)) {
        logout_user(system);
        return false;
    }
    
    return (system->current_user->permissions & perm) != 0;
}

// 生成盐值
void generate_salt(char *salt, int length) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    
    srand(time(NULL));
    for (int i = 0; i < length; i++) {
        salt[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    salt[length] = '\0';
}

// 密码哈希
void hash_password(const char *password, const char *salt, char *hash) {
    char salted_password[256];
    snprintf(salted_password, sizeof(salted_password), "%s%s", password, salt);
    
    // 简化的哈希实现（实际应用中应使用更安全的方法）
    unsigned long hash_value = 5381;
    for (int i = 0; salted_password[i]; i++) {
        hash_value = ((hash_value << 5) + hash_value) + salted_password[i];
    }
    
    snprintf(hash, 65, "%016lx%016lx%016lx%016lx", 
             hash_value, hash_value ^ 0xAAAAAAAA, 
             hash_value ^ 0x55555555, hash_value ^ 0xFFFFFFFF);
}

// 验证密码
bool verify_password(const char *password, const char *salt, const char *hash) {
    char computed_hash[65];
    hash_password(password, salt, computed_hash);
    return strcmp(computed_hash, hash) == 0;
}

// 检查密码强度
bool is_strong_password(const char *password) {
    int len = strlen(password);
    
    if (len < 8) return false; // 至少8位
    
    bool has_upper = false, has_lower = false, has_digit = false, has_special = false;
    
    for (int i = 0; i < len; i++) {
        char c = password[i];
        if (c >= 'A' && c <= 'Z') has_upper = true;
        else if (c >= 'a' && c <= 'z') has_lower = true;
        else if (c >= '0' && c <= '9') has_digit = true;
        else has_special = true;
    }
    
    // 至少包含3种字符类型
    int types = has_upper + has_lower + has_digit + has_special;
    return types >= 3;
}

// 会话验证
bool is_session_valid(UserSystem *system) {
    if (!system->current_user) {
        return false;
    }
    
    time_t now = time(NULL);
    return (now - system->current_user->last_login) < system->session_timeout;
}
```

---

## 配置管理

### 配置系统

```c
// config.h
#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

// 配置项类型
typedef enum {
    CONFIG_STRING,
    CONFIG_INT,
    CONFIG_FLOAT,
    CONFIG_BOOL
} ConfigType;

// 配置项
typedef struct {
    char key[64];
    ConfigType type;
    union {
        char string_value[256];
        int int_value;
        float float_value;
        bool bool_value;
    } value;
    char description[128];
    bool is_readonly;
} ConfigItem;

// 配置系统
typedef struct {
    ConfigItem items[100];
    int item_count;
    char config_file[256];
    bool auto_save;
} ConfigSystem;

// 配置管理函数
int init_config_system(ConfigSystem *config, const char *config_file);
int load_config(ConfigSystem *config);
int save_config(ConfigSystem *config);

// 配置项操作
int set_config_string(ConfigSystem *config, const char *key, const char *value);
int set_config_int(ConfigSystem *config, const char *key, int value);
int set_config_float(ConfigSystem *config, const char *key, float value);
int set_config_bool(ConfigSystem *config, const char *key, bool value);

const char* get_config_string(ConfigSystem *config, const char *key, const char *default_value);
int get_config_int(ConfigSystem *config, const char *key, int default_value);
float get_config_float(ConfigSystem *config, const char *key, float default_value);
bool get_config_bool(ConfigSystem *config, const char *key, bool default_value);

// 配置管理
void list_config_items(ConfigSystem *config);
int reset_config_to_default(ConfigSystem *config);
int validate_config(ConfigSystem *config);

#endif // CONFIG_H
```

---

## 实践练习

### 练习1：实现数据分析功能

```c
// 练习：实现高级数据分析功能
// 要求：
// 1. 实现相关性分析，计算各科成绩之间的相关系数
// 2. 实现趋势分析，分析成绩变化趋势
// 3. 实现异常检测，找出成绩异常的学生

// TODO: 实现相关性分析
double calculate_correlation(double *x, double *y, int n) {
    // 计算皮尔逊相关系数
}

// TODO: 实现异常检测
void detect_outliers(StudentSystem *system) {
    // 使用Z-score方法检测异常值
}
```

### 练习2：实现插件系统

```c
// 练习：实现简单的插件系统
// 要求：
// 1. 设计插件接口
// 2. 实现插件加载和卸载
// 3. 创建一个示例插件

typedef struct {
    char name[64];
    char version[16];
    int (*init)(void);
    void (*cleanup)(void);
    int (*execute)(const char *command, void *data);
} Plugin;

// TODO: 实现插件管理系统
```

### 练习3：实现网络功能

```c
// 练习：实现基本的网络功能
// 要求：
// 1. 实现HTTP服务器，提供REST API
// 2. 支持JSON格式的数据交换
// 3. 实现基本的身份验证

// TODO: 实现HTTP服务器
int start_http_server(int port) {
    // 启动HTTP服务器
}

// TODO: 实现REST API
void handle_api_request(const char *method, const char *path, const char *body) {
    // 处理API请求
}
```

---

## 💡 本课要点总结

### 数据分析
1. **统计分析**: 平均值、中位数、标准差等基本统计
2. **分布分析**: 成绩分布、年龄分布等
3. **可视化**: ASCII图表、数据报告

### 导入导出
1. **多格式支持**: CSV、JSON、XML等格式
2. **数据验证**: 导入时的数据完整性检查
3. **批量操作**: 批量导入导出功能

### 用户权限
1. **角色管理**: 不同角色的权限控制
2. **身份验证**: 安全的登录验证机制
3. **会话管理**: 会话超时和安全控制

### 系统扩展
1. **配置管理**: 灵活的配置系统
2. **插件架构**: 可扩展的插件系统
3. **网络功能**: HTTP服务和API接口

---

## 🎯 项目总结

通过这9课的学习，我们完成了一个功能完整的学生管理系统：

### 核心功能
- ✅ 学生信息管理（增删改查）
- ✅ 成绩管理和统计
- ✅ 数据持久化
- ✅ 用户界面设计

### 高级功能
- ✅ 错误处理和调试
- ✅ 数据分析和统计
- ✅ 多格式导入导出
- ✅ 用户权限管理
- ✅ 配置管理系统

### 技术特点
- 🔧 模块化设计
- 🛡️ 安全性考虑
- 📊 数据分析能力
- 🔌 可扩展架构
- 📝 完整的错误处理

---

## 📚 扩展阅读

- C语言高级编程技巧
- 数据结构与算法优化
- 软件架构设计模式
- 网络编程基础
- 数据库集成方案

---

**上一课**: [第8课：错误处理与调试](08-error-handling.md)  
**返回**: [教程目录](README.md)

---

🎉 **恭喜完成整个教程系列！** 🎉

现在你已经掌握了C语言项目开发的完整流程，可以开始开发更复杂的应用程序了。