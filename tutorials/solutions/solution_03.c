// 练习3解决方案：文件操作练习答案
// 对应教程：第3课 - 文件操作基础

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/*
 * 这是练习3的参考解决方案。
 * 包含文本文件、二进制文件和CSV文件操作的完整实现。
 */

// 学生结构体定义
typedef struct {
    int id;                    // 学生ID
    char name[50];            // 姓名
    int age;                  // 年龄
    double score;             // 成绩
    char major[30];           // 专业
} Student;

// 学生管理系统结构体
typedef struct {
    Student students[1000];   // 学生数组
    int count;               // 当前学生数量
    int capacity;            // 最大容量
} StudentSystem;

// 文件头结构体（用于二进制文件）
typedef struct {
    char signature[8];        // 文件签名 "STUDENT\0"
    int version;             // 文件版本
    int student_count;       // 学生数量
    long timestamp;          // 创建时间戳
} FileHeader;

// === 基础文件操作函数 ===

// 初始化学生系统
void init_student_system(StudentSystem *sys) {
    if (!sys) return;
    
    sys->count = 0;
    sys->capacity = 1000;
    
    // 初始化学生数组
    for (int i = 0; i < sys->capacity; i++) {
        sys->students[i].id = 0;
        strcpy(sys->students[i].name, "");
        sys->students[i].age = 0;
        sys->students[i].score = 0.0;
        strcpy(sys->students[i].major, "");
    }
    
    printf("学生管理系统初始化完成，容量：%d人\n", sys->capacity);
}

// 添加学生
int add_student(StudentSystem *sys, int id, const char *name, int age, double score, const char *major) {
    if (!sys || !name || !major) {
        printf("错误：无效的参数\n");
        return 0;
    }
    
    if (sys->count >= sys->capacity) {
        printf("错误：系统已满，无法添加更多学生\n");
        return 0;
    }
    
    // 检查ID是否已存在
    for (int i = 0; i < sys->count; i++) {
        if (sys->students[i].id == id) {
            printf("错误：学生ID %d 已存在\n", id);
            return 0;
        }
    }
    
    // 添加新学生
    Student *new_student = &sys->students[sys->count];
    new_student->id = id;
    strncpy(new_student->name, name, sizeof(new_student->name) - 1);
    new_student->name[sizeof(new_student->name) - 1] = '\0';
    new_student->age = age;
    new_student->score = score;
    strncpy(new_student->major, major, sizeof(new_student->major) - 1);
    new_student->major[sizeof(new_student->major) - 1] = '\0';
    
    sys->count++;
    printf("成功添加学生：%s (ID: %d)\n", name, id);
    return 1;
}

// 显示所有学生
void display_students(StudentSystem *sys) {
    if (!sys) {
        printf("错误：无效的系统指针\n");
        return;
    }
    
    if (sys->count == 0) {
        printf("暂无学生信息\n");
        return;
    }
    
    printf("\n=== 学生信息列表 ===\n");
    printf("%-6s %-15s %-4s %-6s %-15s\n", "ID", "姓名", "年龄", "成绩", "专业");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i < sys->count; i++) {
        Student *s = &sys->students[i];
        printf("%-6d %-15s %-4d %-6.1f %-15s\n",
               s->id, s->name, s->age, s->score, s->major);
    }
    
    printf("--------------------------------------------------\n");
    printf("总计：%d名学生\n\n", sys->count);
}

// === 文本文件操作函数 ===

// 保存学生数据到文本文件
int save_students_text(StudentSystem *sys, const char *filename) {
    if (!sys || !filename) {
        printf("错误：无效的参数\n");
        return 0;
    }
    
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("错误：无法创建文件 %s - %s\n", filename, strerror(errno));
        return 0;
    }
    
    // 写入文件头信息
    fprintf(file, "# 学生信息数据文件\n");
    fprintf(file, "# 格式：ID 姓名 年龄 成绩 专业\n");
    fprintf(file, "# 学生数量：%d\n", sys->count);
    fprintf(file, "\n");
    
    // 写入学生数据
    for (int i = 0; i < sys->count; i++) {
        Student *s = &sys->students[i];
        fprintf(file, "%d %s %d %.1f %s\n",
                s->id, s->name, s->age, s->score, s->major);
    }
    
    fclose(file);
    printf("成功保存 %d 名学生信息到文件：%s\n", sys->count, filename);
    return 1;
}

// 从文本文件加载学生数据
int load_students_text(StudentSystem *sys, const char *filename) {
    if (!sys || !filename) {
        printf("错误：无效的参数\n");
        return 0;
    }
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("错误：无法打开文件 %s - %s\n", filename, strerror(errno));
        return 0;
    }
    
    char line[256];
    int loaded_count = 0;
    sys->count = 0; // 重置计数
    
    printf("从文件加载学生数据：%s\n", filename);
    
    while (fgets(line, sizeof(line), file)) {
        // 跳过注释行和空行
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') {
            continue;
        }
        
        // 解析学生数据
        int id, age;
        char name[50], major[30];
        double score;
        
        int parsed = sscanf(line, "%d %49s %d %lf %29s", 
                           &id, name, &age, &score, major);
        
        if (parsed == 5) {
            if (add_student(sys, id, name, age, score, major)) {
                loaded_count++;
            }
        } else {
            printf("警告：跳过无效行：%s", line);
        }
    }
    
    fclose(file);
    printf("成功加载 %d 名学生信息\n", loaded_count);
    return loaded_count;
}

// === 二进制文件操作函数 ===

// 保存学生数据到二进制文件
int save_students_binary(StudentSystem *sys, const char *filename) {
    if (!sys || !filename) {
        printf("错误：无效的参数\n");
        return 0;
    }
    
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("错误：无法创建文件 %s - %s\n", filename, strerror(errno));
        return 0;
    }
    
    // 写入文件头
    FileHeader header;
    strcpy(header.signature, "STUDENT");
    header.version = 1;
    header.student_count = sys->count;
    header.timestamp = (long)time(NULL);
    
    if (fwrite(&header, sizeof(FileHeader), 1, file) != 1) {
        printf("错误：写入文件头失败\n");
        fclose(file);
        return 0;
    }
    
    // 写入学生数据
    if (sys->count > 0) {
        size_t written = fwrite(sys->students, sizeof(Student), sys->count, file);
        if (written != sys->count) {
            printf("错误：写入学生数据失败，期望 %d，实际 %zu\n", sys->count, written);
            fclose(file);
            return 0;
        }
    }
    
    fclose(file);
    printf("成功保存 %d 名学生信息到二进制文件：%s\n", sys->count, filename);
    return 1;
}

// 从二进制文件加载学生数据
int load_students_binary(StudentSystem *sys, const char *filename) {
    if (!sys || !filename) {
        printf("错误：无效的参数\n");
        return 0;
    }
    
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("错误：无法打开文件 %s - %s\n", filename, strerror(errno));
        return 0;
    }
    
    // 读取文件头
    FileHeader header;
    if (fread(&header, sizeof(FileHeader), 1, file) != 1) {
        printf("错误：读取文件头失败\n");
        fclose(file);
        return 0;
    }
    
    // 验证文件签名
    if (strcmp(header.signature, "STUDENT") != 0) {
        printf("错误：无效的文件格式\n");
        fclose(file);
        return 0;
    }
    
    // 验证版本
    if (header.version != 1) {
        printf("错误：不支持的文件版本：%d\n", header.version);
        fclose(file);
        return 0;
    }
    
    // 验证学生数量
    if (header.student_count < 0 || header.student_count > sys->capacity) {
        printf("错误：无效的学生数量：%d\n", header.student_count);
        fclose(file);
        return 0;
    }
    
    printf("从二进制文件加载学生数据：%s\n", filename);
    printf("文件版本：%d，创建时间：%ld，学生数量：%d\n", 
           header.version, header.timestamp, header.student_count);
    
    // 读取学生数据
    sys->count = 0;
    if (header.student_count > 0) {
        size_t read_count = fread(sys->students, sizeof(Student), header.student_count, file);
        if (read_count != header.student_count) {
            printf("错误：读取学生数据失败，期望 %d，实际 %zu\n", header.student_count, read_count);
            fclose(file);
            return 0;
        }
        sys->count = header.student_count;
    }
    
    fclose(file);
    printf("成功加载 %d 名学生信息\n", sys->count);
    return sys->count;
}

// === CSV文件操作函数 ===

// 导出学生数据到CSV文件
int export_students_csv(StudentSystem *sys, const char *filename) {
    if (!sys || !filename) {
        printf("错误：无效的参数\n");
        return 0;
    }
    
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("错误：无法创建文件 %s - %s\n", filename, strerror(errno));
        return 0;
    }
    
    // 写入CSV头部
    fprintf(file, "ID,姓名,年龄,成绩,专业\n");
    
    // 写入学生数据
    for (int i = 0; i < sys->count; i++) {
        Student *s = &sys->students[i];
        // 处理姓名和专业中可能包含逗号的情况
        fprintf(file, "%d,\"%s\",%d,%.1f,\"%s\"\n",
                s->id, s->name, s->age, s->score, s->major);
    }
    
    fclose(file);
    printf("成功导出 %d 名学生信息到CSV文件：%s\n", sys->count, filename);
    return 1;
}

// 从CSV文件导入学生数据
int import_students_csv(StudentSystem *sys, const char *filename) {
    if (!sys || !filename) {
        printf("错误：无效的参数\n");
        return 0;
    }
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("错误：无法打开文件 %s - %s\n", filename, strerror(errno));
        return 0;
    }
    
    char line[256];
    int loaded_count = 0;
    int line_number = 0;
    
    printf("从CSV文件导入学生数据：%s\n", filename);
    
    while (fgets(line, sizeof(line), file)) {
        line_number++;
        
        // 跳过头部行
        if (line_number == 1) {
            printf("跳过CSV头部：%s", line);
            continue;
        }
        
        // 移除行尾换行符
        line[strcspn(line, "\n\r")] = '\0';
        
        // 简单的CSV解析（假设没有复杂的引号嵌套）
        char *token;
        char *tokens[5];
        int token_count = 0;
        
        // 复制字符串用于strtok
        char line_copy[256];
        strcpy(line_copy, line);
        
        token = strtok(line_copy, ",");
        while (token && token_count < 5) {
            // 移除引号
            if (token[0] == '"' && token[strlen(token)-1] == '"') {
                token[strlen(token)-1] = '\0';
                token++;
            }
            tokens[token_count++] = token;
            token = strtok(NULL, ",");
        }
        
        if (token_count == 5) {
            int id = atoi(tokens[0]);
            char *name = tokens[1];
            int age = atoi(tokens[2]);
            double score = atof(tokens[3]);
            char *major = tokens[4];
            
            if (add_student(sys, id, name, age, score, major)) {
                loaded_count++;
            }
        } else {
            printf("警告：第%d行格式错误，跳过：%s\n", line_number, line);
        }
    }
    
    fclose(file);
    printf("成功从CSV导入 %d 名学生信息\n", loaded_count);
    return loaded_count;
}

// === 文件操作辅助函数 ===

// 检查文件是否存在
int file_exists(const char *filename) {
    if (!filename) return 0;
    
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

// 获取文件大小
long get_file_size(const char *filename) {
    if (!filename) return -1;
    
    FILE *file = fopen(filename, "rb");
    if (!file) return -1;
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    
    return size;
}

// 备份文件
int backup_file(const char *source, const char *backup) {
    if (!source || !backup) {
        printf("错误：无效的文件名\n");
        return 0;
    }
    
    FILE *src = fopen(source, "rb");
    if (!src) {
        printf("错误：无法打开源文件 %s\n", source);
        return 0;
    }
    
    FILE *dst = fopen(backup, "wb");
    if (!dst) {
        printf("错误：无法创建备份文件 %s\n", backup);
        fclose(src);
        return 0;
    }
    
    // 复制文件内容
    char buffer[4096];
    size_t bytes;
    
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        if (fwrite(buffer, 1, bytes, dst) != bytes) {
            printf("错误：写入备份文件失败\n");
            fclose(src);
            fclose(dst);
            return 0;
        }
    }
    
    fclose(src);
    fclose(dst);
    
    printf("成功备份文件：%s -> %s\n", source, backup);
    return 1;
}

// 删除文件
int delete_file(const char *filename) {
    if (!filename) {
        printf("错误：无效的文件名\n");
        return 0;
    }
    
    if (remove(filename) == 0) {
        printf("成功删除文件：%s\n", filename);
        return 1;
    } else {
        printf("错误：删除文件失败 %s - %s\n", filename, strerror(errno));
        return 0;
    }
}

// === 测试函数 ===

void test_file_operations() {
    printf("=== 文件操作测试 ===\n\n");
    
    StudentSystem sys;
    init_student_system(&sys);
    
    // 添加测试数据
    printf("1. 添加测试学生数据:\n");
    add_student(&sys, 2001, "张三", 20, 85.5, "计算机科学");
    add_student(&sys, 2002, "李四", 19, 92.0, "软件工程");
    add_student(&sys, 2003, "王五", 21, 78.5, "信息安全");
    add_student(&sys, 2004, "赵六", 20, 88.0, "人工智能");
    add_student(&sys, 2005, "钱七", 22, 95.5, "数据科学");
    
    display_students(&sys);
    
    // 测试文本文件操作
    printf("2. 文本文件操作测试:\n");
    const char *text_file = "students.txt";
    
    if (save_students_text(&sys, text_file)) {
        printf("文件大小：%ld 字节\n", get_file_size(text_file));
        
        // 清空系统并重新加载
        StudentSystem sys2;
        init_student_system(&sys2);
        
        if (load_students_text(&sys2, text_file)) {
            printf("验证加载的数据：\n");
            display_students(&sys2);
        }
    }
    
    // 测试二进制文件操作
    printf("\n3. 二进制文件操作测试:\n");
    const char *binary_file = "students.dat";
    
    if (save_students_binary(&sys, binary_file)) {
        printf("文件大小：%ld 字节\n", get_file_size(binary_file));
        
        // 清空系统并重新加载
        StudentSystem sys3;
        init_student_system(&sys3);
        
        if (load_students_binary(&sys3, binary_file)) {
            printf("验证加载的数据：\n");
            display_students(&sys3);
        }
    }
    
    // 测试CSV文件操作
    printf("\n4. CSV文件操作测试:\n");
    const char *csv_file = "students.csv";
    
    if (export_students_csv(&sys, csv_file)) {
        printf("文件大小：%ld 字节\n", get_file_size(csv_file));
        
        // 清空系统并重新导入
        StudentSystem sys4;
        init_student_system(&sys4);
        
        if (import_students_csv(&sys4, csv_file)) {
            printf("验证导入的数据：\n");
            display_students(&sys4);
        }
    }
    
    // 测试文件备份
    printf("\n5. 文件备份测试:\n");
    backup_file(binary_file, "students_backup.dat");
    
    // 测试文件存在性检查
    printf("\n6. 文件存在性检查：\n");
    printf("%s 存在：%s\n", text_file, file_exists(text_file) ? "是" : "否");
    printf("%s 存在：%s\n", binary_file, file_exists(binary_file) ? "是" : "否");
    printf("%s 存在：%s\n", csv_file, file_exists(csv_file) ? "是" : "否");
    printf("不存在的文件.txt 存在：%s\n", file_exists("不存在的文件.txt") ? "是" : "否");
    
    // 文件大小比较
    printf("\n7. 文件大小比较：\n");
    printf("文本文件大小：%ld 字节\n", get_file_size(text_file));
    printf("二进制文件大小：%ld 字节\n", get_file_size(binary_file));
    printf("CSV文件大小：%ld 字节\n", get_file_size(csv_file));
    
    // 清理测试文件
    printf("\n8. 清理测试文件：\n");
    delete_file(text_file);
    delete_file(binary_file);
    delete_file(csv_file);
    delete_file("students_backup.dat");
}

void test_error_handling() {
    printf("\n=== 错误处理测试 ===\n\n");
    
    StudentSystem sys;
    init_student_system(&sys);
    
    // 测试无效文件操作
    printf("1. 无效文件操作测试:\n");
    
    // 尝试读取不存在的文件
    load_students_text(&sys, "不存在的文件.txt");
    load_students_binary(&sys, "不存在的文件.dat");
    import_students_csv(&sys, "不存在的文件.csv");
    
    // 尝试写入无效路径
    save_students_text(&sys, "/invalid/path/file.txt");
    
    // 测试空指针参数
    printf("\n2. 空指针参数测试:\n");
    save_students_text(NULL, "test.txt");
    save_students_text(&sys, NULL);
    load_students_text(NULL, "test.txt");
    load_students_text(&sys, NULL);
    
    // 测试文件权限（如果可能）
    printf("\n3. 文件权限测试（创建只读文件）:\n");
    const char *readonly_file = "readonly_test.txt";
    
    // 先创建文件
    if (save_students_text(&sys, readonly_file)) {
        // 在实际应用中，这里可以设置文件为只读
        // chmod(readonly_file, 0444); // Unix/Linux系统
        printf("创建了测试文件：%s\n", readonly_file);
        
        // 清理
        delete_file(readonly_file);
    }
}

int main() {
    printf("文件操作练习 - 参考解决方案\n");
    printf("==============================\n\n");
    
    // 测试基本文件操作
    test_file_operations();
    
    // 测试错误处理
    test_error_handling();
    
    printf("\n=== 学习要点总结 ===\n");
    printf("1. 文件操作基础：fopen、fclose、fread、fwrite的正确使用\n");
    printf("2. 文本文件处理：fprintf、fscanf、fgets的应用\n");
    printf("3. 二进制文件处理：结构体的直接读写和文件头设计\n");
    printf("4. CSV文件处理：逗号分隔值的解析和生成\n");
    printf("5. 错误处理：文件操作的异常情况处理\n");
    printf("6. 文件管理：备份、删除、大小检查等实用功能\n");
    printf("7. 数据完整性：文件格式验证和版本控制\n");
    
    return 0;
}

/*
 * 解决方案说明：
 * 
 * 1. 文件操作策略:
 *    - 文本文件：人类可读，便于调试和手动编辑
 *    - 二进制文件：紧凑高效，包含文件头验证
 *    - CSV文件：标准格式，便于与其他程序交换数据
 * 
 * 2. 错误处理:
 *    - 参数有效性检查
 *    - 文件操作返回值检查
 *    - 使用errno和strerror提供详细错误信息
 * 
 * 3. 数据完整性:
 *    - 二进制文件包含签名和版本信息
 *    - 读取时验证文件格式和数据有效性
 *    - 提供备份和恢复功能
 * 
 * 4. 性能考虑:
 *    - 使用缓冲区进行文件复制
 *    - 批量读写减少系统调用
 *    - 合理的文件格式设计
 * 
 * 5. 可维护性:
 *    - 模块化的函数设计
 *    - 清晰的错误消息
 *    - 详细的注释说明
 */