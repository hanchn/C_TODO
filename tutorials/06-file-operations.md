# 第6课：文件操作与数据持久化

> **学习目标**: 掌握C语言文件操作，实现数据的持久化存储

## 📋 本课内容

1. [数据持久化概念](#数据持久化概念)
2. [二进制文件操作](#二进制文件操作)
3. [文本文件操作](#文本文件操作)
4. [数据备份与恢复](#数据备份与恢复)
5. [文件完整性验证](#文件完整性验证)
6. [导入导出功能](#导入导出功能)
7. [错误处理与恢复](#错误处理与恢复)
8. [实践练习](#实践练习)

---

## 数据持久化概念

### 什么是数据持久化？

数据持久化是指将内存中的数据保存到永久存储设备（如硬盘）中，使得程序重启后数据仍然存在。

### 持久化的重要性

```c
// 没有持久化的问题
int main() {
    StudentSystem system;
    init_system(&system);
    
    // 用户辛苦输入了很多学生信息
    add_student(&system);  // 添加学生1
    add_student(&system);  // 添加学生2
    add_student(&system);  // 添加学生3
    
    // 程序结束，所有数据丢失！
    return 0;
}

// 有持久化的解决方案
int main() {
    StudentSystem system;
    init_system(&system);
    
    // 程序启动时加载数据
    load_from_file(&system);
    
    // 用户操作...
    add_student(&system);
    modify_student(&system);
    
    // 程序结束前保存数据
    save_to_file(&system);
    
    return 0;  // 数据已保存，下次启动可以恢复
}
```

### 文件格式选择

| 格式 | 优点 | 缺点 | 适用场景 |
|------|------|------|----------|
| 二进制 | 存储效率高，读写速度快 | 不可读，平台相关 | 程序内部数据存储 |
| 文本 | 可读性好，跨平台 | 存储效率低，解析复杂 | 配置文件，数据交换 |
| CSV | 简单，Excel兼容 | 功能有限 | 数据导入导出 |
| JSON | 结构化，跨语言 | 解析复杂 | 配置文件，API数据 |

---

## 二进制文件操作

### 二进制文件的优势

1. **存储效率高**: 直接存储内存中的数据结构
2. **读写速度快**: 无需格式转换
3. **类型安全**: 保持原始数据类型

### 基本的保存和加载

```c
// file.c
#include "file.h"
#include "ui.h"

// 文件头结构，用于验证文件完整性
typedef struct {
    char magic[8];        // 文件标识 "STUDENT\0"
    int version;          // 文件版本
    int student_count;    // 学生数量
    time_t save_time;     // 保存时间
    unsigned int checksum; // 校验和
} FileHeader;

int save_to_file(StudentSystem *system) {
    if (system == NULL) {
        show_error("系统未初始化");
        return 0;
    }
    
    FILE *file = fopen(FILENAME, "wb");
    if (file == NULL) {
        show_error("无法创建数据文件");
        return 0;
    }
    
    // 创建文件头
    FileHeader header;
    strcpy(header.magic, "STUDENT");
    header.version = 1;
    header.student_count = system->count;
    header.save_time = time(NULL);
    header.checksum = calculate_checksum(system);
    
    // 写入文件头
    if (fwrite(&header, sizeof(FileHeader), 1, file) != 1) {
        show_error("写入文件头失败");
        fclose(file);
        return 0;
    }
    
    // 写入学生数据
    if (system->count > 0) {
        size_t written = fwrite(system->students, sizeof(Student), 
                               system->count, file);
        if (written != system->count) {
            show_error("写入学生数据失败");
            fclose(file);
            return 0;
        }
    }
    
    fclose(file);
    
    char time_str[64];
    get_current_time(time_str, sizeof(time_str));
    printf("数据保存成功！\n");
    printf("保存时间: %s\n", time_str);
    printf("学生数量: %d\n", system->count);
    
    return 1;
}

int load_from_file(StudentSystem *system) {
    if (system == NULL) {
        show_error("系统未初始化");
        return 0;
    }
    
    FILE *file = fopen(FILENAME, "rb");
    if (file == NULL) {
        // 文件不存在是正常情况（首次运行）
        printf("数据文件不存在，将创建新的数据文件\n");
        return 1;
    }
    
    // 读取文件头
    FileHeader header;
    if (fread(&header, sizeof(FileHeader), 1, file) != 1) {
        show_error("读取文件头失败");
        fclose(file);
        return 0;
    }
    
    // 验证文件格式
    if (strcmp(header.magic, "STUDENT") != 0) {
        show_error("文件格式错误");
        fclose(file);
        return 0;
    }
    
    // 检查版本兼容性
    if (header.version > 1) {
        show_error("文件版本过高，请升级程序");
        fclose(file);
        return 0;
    }
    
    // 验证学生数量
    if (header.student_count < 0 || header.student_count > MAX_STUDENTS) {
        show_error("文件中的学生数量无效");
        fclose(file);
        return 0;
    }
    
    // 读取学生数据
    system->count = header.student_count;
    if (system->count > 0) {
        size_t read_count = fread(system->students, sizeof(Student), 
                                 system->count, file);
        if (read_count != system->count) {
            show_error("读取学生数据失败");
            system->count = 0;
            fclose(file);
            return 0;
        }
    }
    
    fclose(file);
    
    // 验证数据完整性
    unsigned int current_checksum = calculate_checksum(system);
    if (current_checksum != header.checksum) {
        show_error("数据文件可能已损坏");
        printf("是否继续加载？(y/N): ");
        char choice;
        scanf(" %c", &choice);
        clear_input_buffer();
        
        if (choice != 'y' && choice != 'Y') {
            system->count = 0;
            return 0;
        }
    }
    
    char time_str[64];
    struct tm *tm_info = localtime(&header.save_time);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
    
    printf("数据加载成功！\n");
    printf("保存时间: %s\n", time_str);
    printf("学生数量: %d\n", system->count);
    
    return 1;
}

// 计算校验和
unsigned int calculate_checksum(StudentSystem *system) {
    unsigned int checksum = 0;
    
    for (int i = 0; i < system->count; i++) {
        Student *s = &system->students[i];
        
        // 对学号进行校验
        for (int j = 0; s->id[j]; j++) {
            checksum += (unsigned char)s->id[j];
        }
        
        // 对姓名进行校验
        for (int j = 0; s->name[j]; j++) {
            checksum += (unsigned char)s->name[j];
        }
        
        // 对数值字段进行校验
        checksum += s->age;
        checksum += (unsigned int)(s->math_score * 100);
        checksum += (unsigned int)(s->english_score * 100);
        checksum += (unsigned int)(s->chinese_score * 100);
    }
    
    return checksum;
}
```

### 增量保存功能

```c
// 增量保存：只保存修改过的数据
typedef struct {
    Student student;
    int is_modified;  // 是否被修改
    time_t modify_time; // 修改时间
} StudentRecord;

typedef struct {
    StudentRecord records[MAX_STUDENTS];
    int count;
    time_t last_save_time;
} AdvancedStudentSystem;

int incremental_save(AdvancedStudentSystem *system) {
    FILE *file = fopen("students_incremental.dat", "ab");
    if (file == NULL) {
        return 0;
    }
    
    int saved_count = 0;
    time_t current_time = time(NULL);
    
    for (int i = 0; i < system->count; i++) {
        StudentRecord *record = &system->records[i];
        
        // 只保存修改过的记录
        if (record->is_modified && 
            record->modify_time > system->last_save_time) {
            
            // 写入操作类型和时间戳
            char operation = 'U';  // Update
            fwrite(&operation, sizeof(char), 1, file);
            fwrite(&current_time, sizeof(time_t), 1, file);
            fwrite(&record->student, sizeof(Student), 1, file);
            
            record->is_modified = 0;
            saved_count++;
        }
    }
    
    system->last_save_time = current_time;
    fclose(file);
    
    printf("增量保存完成，保存了 %d 条记录\n", saved_count);
    return 1;
}
```

---

## 文本文件操作

### CSV格式导出

```c
int export_to_csv(StudentSystem *system, const char *filename) {
    if (system == NULL || filename == NULL) {
        show_error("参数无效");
        return 0;
    }
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        show_error("无法创建CSV文件");
        return 0;
    }
    
    // 写入CSV头部
    fprintf(file, "学号,姓名,年龄,数学成绩,英语成绩,语文成绩,平均分,等级\n");
    
    // 写入学生数据
    for (int i = 0; i < system->count; i++) {
        Student *s = &system->students[i];
        
        // 处理姓名中可能包含的逗号
        char escaped_name[MAX_NAME_LEN * 2];
        escape_csv_field(s->name, escaped_name, sizeof(escaped_name));
        
        fprintf(file, "%s,%s,%d,%.1f,%.1f,%.1f,%.1f,%s\n",
                s->id,
                escaped_name,
                s->age,
                s->math_score,
                s->english_score,
                s->chinese_score,
                s->average_score,
                get_grade_level(s->average_score));
    }
    
    fclose(file);
    
    printf("CSV文件导出成功: %s\n", filename);
    printf("导出学生数量: %d\n", system->count);
    
    return 1;
}

// CSV字段转义处理
void escape_csv_field(const char *input, char *output, int output_size) {
    int input_len = strlen(input);
    int output_pos = 0;
    
    // 检查是否需要引号包围
    int needs_quotes = 0;
    for (int i = 0; i < input_len; i++) {
        if (input[i] == ',' || input[i] == '"' || input[i] == '\n') {
            needs_quotes = 1;
            break;
        }
    }
    
    if (needs_quotes) {
        output[output_pos++] = '"';
    }
    
    for (int i = 0; i < input_len && output_pos < output_size - 3; i++) {
        if (input[i] == '"') {
            // 双引号需要转义
            output[output_pos++] = '"';
            output[output_pos++] = '"';
        } else {
            output[output_pos++] = input[i];
        }
    }
    
    if (needs_quotes) {
        output[output_pos++] = '"';
    }
    
    output[output_pos] = '\0';
}
```

### CSV格式导入

```c
int import_from_csv(StudentSystem *system, const char *filename) {
    if (system == NULL || filename == NULL) {
        show_error("参数无效");
        return 0;
    }
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        show_error("无法打开CSV文件");
        return 0;
    }
    
    char line[1024];
    int line_number = 0;
    int imported_count = 0;
    int error_count = 0;
    
    // 跳过标题行
    if (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
    }
    
    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        
        // 移除行尾换行符
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            continue;  // 跳过空行
        }
        
        Student student;
        memset(&student, 0, sizeof(Student));
        
        // 解析CSV行
        if (parse_csv_line(line, &student)) {
            // 验证数据
            if (validate_student_data(&student)) {
                // 检查学号是否已存在
                if (find_student_by_id(system, student.id) == -1) {
                    if (system->count < MAX_STUDENTS) {
                        calculate_average(&student);
                        system->students[system->count] = student;
                        system->count++;
                        imported_count++;
                    } else {
                        printf("警告：学生数量已达上限，跳过第%d行\n", line_number);
                        error_count++;
                    }
                } else {
                    printf("警告：第%d行学号重复，跳过\n", line_number);
                    error_count++;
                }
            } else {
                printf("警告：第%d行数据无效，跳过\n", line_number);
                error_count++;
            }
        } else {
            printf("警告：第%d行格式错误，跳过\n", line_number);
            error_count++;
        }
    }
    
    fclose(file);
    
    printf("CSV导入完成\n");
    printf("成功导入: %d 个学生\n", imported_count);
    printf("错误记录: %d 个\n", error_count);
    
    return imported_count;
}

// 解析CSV行
int parse_csv_line(const char *line, Student *student) {
    char *line_copy = strdup(line);
    if (line_copy == NULL) {
        return 0;
    }
    
    char *fields[8];  // 8个字段
    int field_count = 0;
    
    // 分割CSV字段
    char *token = strtok(line_copy, ",");
    while (token != NULL && field_count < 8) {
        fields[field_count] = token;
        field_count++;
        token = strtok(NULL, ",");
    }
    
    if (field_count < 6) {  // 至少需要6个字段
        free(line_copy);
        return 0;
    }
    
    // 解析各字段
    strncpy(student->id, fields[0], MAX_ID_LEN - 1);
    strncpy(student->name, fields[1], MAX_NAME_LEN - 1);
    student->age = atoi(fields[2]);
    student->math_score = atof(fields[3]);
    student->english_score = atof(fields[4]);
    student->chinese_score = atof(fields[5]);
    
    free(line_copy);
    return 1;
}

// 验证学生数据
int validate_student_data(Student *student) {
    if (!is_valid_id(student->id)) {
        return 0;
    }
    
    if (strlen(student->name) < 2) {
        return 0;
    }
    
    if (student->age < 15 || student->age > 30) {
        return 0;
    }
    
    if (!is_valid_score(student->math_score) ||
        !is_valid_score(student->english_score) ||
        !is_valid_score(student->chinese_score)) {
        return 0;
    }
    
    return 1;
}
```

---

## 数据备份与恢复

### 自动备份功能

```c
int backup_data(const char *backup_name) {
    if (backup_name == NULL) {
        // 生成默认备份文件名
        char default_name[256];
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        
        strftime(default_name, sizeof(default_name), 
                "backup_%Y%m%d_%H%M%S.dat", tm_info);
        backup_name = default_name;
    }
    
    // 检查原文件是否存在
    FILE *source = fopen(FILENAME, "rb");
    if (source == NULL) {
        show_error("原数据文件不存在");
        return 0;
    }
    
    FILE *backup = fopen(backup_name, "wb");
    if (backup == NULL) {
        show_error("无法创建备份文件");
        fclose(source);
        return 0;
    }
    
    // 复制文件内容
    char buffer[4096];
    size_t bytes_read;
    size_t total_bytes = 0;
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        size_t bytes_written = fwrite(buffer, 1, bytes_read, backup);
        if (bytes_written != bytes_read) {
            show_error("备份写入失败");
            fclose(source);
            fclose(backup);
            remove(backup_name);  // 删除不完整的备份文件
            return 0;
        }
        total_bytes += bytes_written;
    }
    
    fclose(source);
    fclose(backup);
    
    printf("数据备份成功\n");
    printf("备份文件: %s\n", backup_name);
    printf("备份大小: %zu 字节\n", total_bytes);
    
    return 1;
}

// 恢复数据
int restore_data(const char *backup_name) {
    if (backup_name == NULL) {
        show_error("备份文件名无效");
        return 0;
    }
    
    // 检查备份文件是否存在
    FILE *backup = fopen(backup_name, "rb");
    if (backup == NULL) {
        show_error("备份文件不存在");
        return 0;
    }
    
    // 验证备份文件格式
    FileHeader header;
    if (fread(&header, sizeof(FileHeader), 1, backup) != 1 ||
        strcmp(header.magic, "STUDENT") != 0) {
        show_error("备份文件格式错误");
        fclose(backup);
        return 0;
    }
    
    fclose(backup);
    
    // 备份当前数据文件
    if (access(FILENAME, F_OK) == 0) {
        char current_backup[256];
        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        
        strftime(current_backup, sizeof(current_backup), 
                "current_backup_%Y%m%d_%H%M%S.dat", tm_info);
        
        printf("正在备份当前数据到: %s\n", current_backup);
        if (!backup_data(current_backup)) {
            printf("警告：当前数据备份失败\n");
        }
    }
    
    // 恢复数据
    backup = fopen(backup_name, "rb");
    FILE *target = fopen(FILENAME, "wb");
    
    if (target == NULL) {
        show_error("无法创建目标文件");
        fclose(backup);
        return 0;
    }
    
    char buffer[4096];
    size_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), backup)) > 0) {
        if (fwrite(buffer, 1, bytes_read, target) != bytes_read) {
            show_error("恢复写入失败");
            fclose(backup);
            fclose(target);
            return 0;
        }
    }
    
    fclose(backup);
    fclose(target);
    
    printf("数据恢复成功\n");
    printf("从备份文件: %s\n", backup_name);
    
    return 1;
}

// 列出所有备份文件
void list_backups() {
    printf("\n=== 备份文件列表 ===\n");
    
    // 使用系统命令列出备份文件
    #ifdef _WIN32
        system("dir backup_*.dat /B 2>nul");
    #else
        system("ls -la backup_*.dat 2>/dev/null || echo '没有找到备份文件'");
    #endif
}
```

---

## 文件完整性验证

### 高级校验功能

```c
// 使用CRC32校验
#include <stdint.h>

// CRC32查找表
static uint32_t crc32_table[256];
static int crc32_table_initialized = 0;

// 初始化CRC32查找表
void init_crc32_table() {
    if (crc32_table_initialized) {
        return;
    }
    
    uint32_t polynomial = 0xEDB88320;
    
    for (int i = 0; i < 256; i++) {
        uint32_t crc = i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ polynomial;
            } else {
                crc >>= 1;
            }
        }
        crc32_table[i] = crc;
    }
    
    crc32_table_initialized = 1;
}

// 计算CRC32校验值
uint32_t calculate_crc32(const void *data, size_t length) {
    init_crc32_table();
    
    uint32_t crc = 0xFFFFFFFF;
    const uint8_t *bytes = (const uint8_t *)data;
    
    for (size_t i = 0; i < length; i++) {
        uint8_t table_index = (crc ^ bytes[i]) & 0xFF;
        crc = (crc >> 8) ^ crc32_table[table_index];
    }
    
    return crc ^ 0xFFFFFFFF;
}

// 验证文件完整性
int validate_data_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return 0;
    }
    
    // 读取文件头
    FileHeader header;
    if (fread(&header, sizeof(FileHeader), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    
    // 验证魔数
    if (strcmp(header.magic, "STUDENT") != 0) {
        fclose(file);
        return 0;
    }
    
    // 读取学生数据
    Student *students = malloc(header.student_count * sizeof(Student));
    if (students == NULL) {
        fclose(file);
        return 0;
    }
    
    size_t read_count = fread(students, sizeof(Student), 
                             header.student_count, file);
    fclose(file);
    
    if (read_count != header.student_count) {
        free(students);
        return 0;
    }
    
    // 计算当前校验值
    uint32_t current_crc = calculate_crc32(students, 
                                          header.student_count * sizeof(Student));
    
    free(students);
    
    // 比较校验值（假设校验值存储在文件头中）
    return current_crc == header.checksum;
}

// 修复损坏的数据文件
int repair_data_file(const char *filename) {
    printf("正在尝试修复数据文件: %s\n", filename);
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return 0;
    }
    
    // 读取文件头
    FileHeader header;
    if (fread(&header, sizeof(FileHeader), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    
    printf("文件头信息:\n");
    printf("  魔数: %s\n", header.magic);
    printf("  版本: %d\n", header.version);
    printf("  学生数量: %d\n", header.student_count);
    
    // 尝试读取学生数据
    Student *students = malloc(MAX_STUDENTS * sizeof(Student));
    if (students == NULL) {
        fclose(file);
        return 0;
    }
    
    int valid_count = 0;
    
    for (int i = 0; i < header.student_count && i < MAX_STUDENTS; i++) {
        if (fread(&students[valid_count], sizeof(Student), 1, file) == 1) {
            // 验证学生数据
            if (validate_student_data(&students[valid_count])) {
                valid_count++;
            } else {
                printf("跳过无效的学生记录 #%d\n", i + 1);
            }
        } else {
            printf("读取学生记录 #%d 失败\n", i + 1);
            break;
        }
    }
    
    fclose(file);
    
    if (valid_count == 0) {
        printf("没有找到有效的学生记录\n");
        free(students);
        return 0;
    }
    
    // 创建修复后的文件
    char repair_filename[256];
    snprintf(repair_filename, sizeof(repair_filename), "%s.repaired", filename);
    
    FILE *repair_file = fopen(repair_filename, "wb");
    if (repair_file == NULL) {
        free(students);
        return 0;
    }
    
    // 写入新的文件头
    FileHeader new_header;
    strcpy(new_header.magic, "STUDENT");
    new_header.version = 1;
    new_header.student_count = valid_count;
    new_header.save_time = time(NULL);
    new_header.checksum = calculate_crc32(students, valid_count * sizeof(Student));
    
    fwrite(&new_header, sizeof(FileHeader), 1, repair_file);
    fwrite(students, sizeof(Student), valid_count, repair_file);
    
    fclose(repair_file);
    free(students);
    
    printf("文件修复完成\n");
    printf("修复后文件: %s\n", repair_filename);
    printf("恢复的学生记录: %d\n", valid_count);
    
    return 1;
}
```

---

## 导入导出功能

### JSON格式支持

```c
// 导出为JSON格式
int export_to_json(StudentSystem *system, const char *filename) {
    if (system == NULL || filename == NULL) {
        return 0;
    }
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return 0;
    }
    
    fprintf(file, "{\n");
    fprintf(file, "  \"version\": 1,\n");
    fprintf(file, "  \"export_time\": \"%ld\",\n", time(NULL));
    fprintf(file, "  \"student_count\": %d,\n", system->count);
    fprintf(file, "  \"students\": [\n");
    
    for (int i = 0; i < system->count; i++) {
        Student *s = &system->students[i];
        
        fprintf(file, "    {\n");
        fprintf(file, "      \"id\": \"%s\",\n", s->id);
        fprintf(file, "      \"name\": \"%s\",\n", s->name);
        fprintf(file, "      \"age\": %d,\n", s->age);
        fprintf(file, "      \"scores\": {\n");
        fprintf(file, "        \"math\": %.1f,\n", s->math_score);
        fprintf(file, "        \"english\": %.1f,\n", s->english_score);
        fprintf(file, "        \"chinese\": %.1f,\n", s->chinese_score);
        fprintf(file, "        \"average\": %.1f\n", s->average_score);
        fprintf(file, "      },\n");
        fprintf(file, "      \"grade\": \"%s\"\n", get_grade_level(s->average_score));
        fprintf(file, "    }");
        
        if (i < system->count - 1) {
            fprintf(file, ",");
        }
        fprintf(file, "\n");
    }
    
    fprintf(file, "  ]\n");
    fprintf(file, "}\n");
    
    fclose(file);
    
    printf("JSON文件导出成功: %s\n", filename);
    return 1;
}

// 批量导入功能
int batch_import(StudentSystem *system) {
    printf("\n=== 批量导入数据 ===\n");
    printf("1. 从CSV文件导入\n");
    printf("2. 从备份文件导入\n");
    printf("3. 从文本文件导入\n");
    printf("请选择导入方式: ");
    
    int choice;
    if (!safe_input_int(&choice, 1, 3)) {
        return 0;
    }
    
    char filename[256];
    printf("请输入文件名: ");
    safe_input_string(filename, sizeof(filename));
    
    switch (choice) {
        case 1:
            return import_from_csv(system, filename);
        case 2:
            return import_from_backup(system, filename);
        case 3:
            return import_from_text(system, filename);
        default:
            return 0;
    }
}
```

---

## 错误处理与恢复

### 异常处理机制

```c
// 错误代码定义
typedef enum {
    FILE_ERROR_NONE = 0,
    FILE_ERROR_NOT_FOUND,
    FILE_ERROR_PERMISSION_DENIED,
    FILE_ERROR_DISK_FULL,
    FILE_ERROR_CORRUPTED,
    FILE_ERROR_VERSION_MISMATCH,
    FILE_ERROR_INVALID_FORMAT,
    FILE_ERROR_MEMORY_ERROR
} FileError;

// 错误处理函数
FileError handle_file_error(const char *operation, const char *filename) {
    int error_code = errno;
    
    printf("文件操作失败: %s\n", operation);
    printf("文件名: %s\n", filename);
    
    switch (error_code) {
        case ENOENT:
            printf("错误: 文件不存在\n");
            return FILE_ERROR_NOT_FOUND;
            
        case EACCES:
            printf("错误: 权限不足\n");
            printf("解决方案: 检查文件权限或以管理员身份运行\n");
            return FILE_ERROR_PERMISSION_DENIED;
            
        case ENOSPC:
            printf("错误: 磁盘空间不足\n");
            printf("解决方案: 清理磁盘空间后重试\n");
            return FILE_ERROR_DISK_FULL;
            
        case ENOMEM:
            printf("错误: 内存不足\n");
            return FILE_ERROR_MEMORY_ERROR;
            
        default:
            printf("错误: %s (错误代码: %d)\n", strerror(error_code), error_code);
            return FILE_ERROR_CORRUPTED;
    }
}

// 自动恢复功能
int auto_recovery(StudentSystem *system) {
    printf("检测到数据文件异常，正在尝试自动恢复...\n");
    
    // 1. 尝试从最近的备份恢复
    char backup_pattern[] = "backup_*.dat";
    // 这里应该实现查找最新备份文件的逻辑
    
    // 2. 尝试修复当前文件
    if (repair_data_file(FILENAME)) {
        printf("文件修复成功，正在重新加载...\n");
        char repaired_file[256];
        snprintf(repaired_file, sizeof(repaired_file), "%s.repaired", FILENAME);
        
        if (load_from_file_with_name(system, repaired_file)) {
            // 备份原文件
            char corrupted_backup[256];
            time_t now = time(NULL);
            struct tm *tm_info = localtime(&now);
            strftime(corrupted_backup, sizeof(corrupted_backup), 
                    "corrupted_%Y%m%d_%H%M%S.dat", tm_info);
            
            rename(FILENAME, corrupted_backup);
            rename(repaired_file, FILENAME);
            
            printf("自动恢复成功\n");
            printf("损坏的文件已备份为: %s\n", corrupted_backup);
            return 1;
        }
    }
    
    // 3. 创建新的空数据文件
    printf("无法恢复数据，将创建新的数据文件\n");
    init_system(system);
    save_to_file(system);
    
    return 0;
}
```

---

## 实践练习

### 练习1：实现数据压缩

```c
// 练习：实现简单的数据压缩功能
// 要求：
// 1. 压缩学生数据以节省存储空间
// 2. 实现压缩和解压缩函数
// 3. 保持数据完整性

int compress_data(StudentSystem *system, const char *compressed_file) {
    // TODO: 实现数据压缩
    // 提示：
    // 1. 可以使用简单的RLE（行程编码）
    // 2. 或者使用zlib库进行压缩
    // 3. 记录压缩前后的大小
}

int decompress_data(StudentSystem *system, const char *compressed_file) {
    // TODO: 实现数据解压缩
}
```

### 练习2：实现数据同步

```c
// 练习：实现多文件数据同步
// 要求：
// 1. 支持多个数据文件之间的同步
// 2. 检测数据冲突
// 3. 提供冲突解决策略

int sync_data_files(const char *file1, const char *file2) {
    // TODO: 实现数据同步
    // 提示：
    // 1. 比较两个文件的时间戳
    // 2. 合并不冲突的数据
    // 3. 标记冲突的记录供用户选择
}
```

### 练习3：实现数据加密

```c
// 练习：实现数据加密存储
// 要求：
// 1. 使用简单的加密算法保护数据
// 2. 支持密码验证
// 3. 确保加密后数据的完整性

int encrypt_save(StudentSystem *system, const char *password) {
    // TODO: 实现加密保存
    // 提示：
    // 1. 使用XOR或简单的替换加密
    // 2. 基于密码生成密钥
    // 3. 添加密码验证机制
}

int decrypt_load(StudentSystem *system, const char *password) {
    // TODO: 实现解密加载
}
```

---

## 💡 本课要点总结

### 文件操作最佳实践
1. **错误检查**: 每个文件操作都要检查返回值
2. **资源管理**: 及时关闭文件句柄
3. **数据验证**: 读取数据后要验证完整性
4. **备份策略**: 重要操作前先备份数据

### 数据格式选择
1. **二进制格式**: 适合程序内部存储，效率高
2. **文本格式**: 适合数据交换，可读性好
3. **CSV格式**: 适合与Excel等工具交互
4. **JSON格式**: 适合配置文件和API数据

### 完整性保证
1. **校验和**: 使用CRC32等算法验证数据
2. **版本控制**: 文件格式版本管理
3. **原子操作**: 确保写入操作的原子性
4. **恢复机制**: 提供数据恢复功能

### 性能优化
1. **缓冲区**: 使用适当大小的缓冲区
2. **批量操作**: 减少文件I/O次数
3. **增量保存**: 只保存修改的数据
4. **压缩存储**: 节省存储空间

---

## 🚀 下一步

完成本课学习后，你应该能够：
- ✅ 实现可靠的数据持久化
- ✅ 处理各种文件格式
- ✅ 设计数据备份和恢复机制
- ✅ 保证数据的完整性和安全性

**准备好了吗？** 让我们进入 [第7课：用户界面设计](./07-user-interface.md)，学习如何创建友好的用户交互界面！

---

*💭 思考题：在什么情况下应该选择二进制格式而不是文本格式？如何在保证数据安全的同时提高文件操作的性能？*