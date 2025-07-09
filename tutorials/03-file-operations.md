# 第3课：文件操作基础

> **学习目标**: 学习C语言文件操作，实现学生数据的持久化存储

## 📋 本课内容

1. [文件操作基础](#文件操作基础)
2. [文本文件操作](#文本文件操作)
3. [二进制文件操作](#二进制文件操作)
4. [学生数据的文件存储](#学生数据的文件存储)
5. [错误处理与安全性](#错误处理与安全性)
6. [实践练习](#实践练习)

---

## 文件操作基础

### 文件操作的基本流程

```c
#include <stdio.h>

int main() {
    FILE *file;
    
    // 1. 打开文件
    file = fopen("test.txt", "w");
    
    // 2. 检查文件是否成功打开
    if (file == NULL) {
        printf("无法打开文件！\n");
        return 1;
    }
    
    // 3. 进行文件操作
    fprintf(file, "Hello, World!\n");
    
    // 4. 关闭文件
    fclose(file);
    
    printf("文件写入成功！\n");
    return 0;
}
```

### 文件打开模式

| 模式 | 说明 | 文件不存在时 | 文件存在时 |
|------|------|-------------|----------|
| "r" | 只读 | 失败 | 从头开始读 |
| "w" | 只写 | 创建新文件 | 清空内容 |
| "a" | 追加 | 创建新文件 | 从末尾追加 |
| "r+" | 读写 | 失败 | 从头开始 |
| "w+" | 读写 | 创建新文件 | 清空内容 |
| "a+" | 读写追加 | 创建新文件 | 从末尾追加 |
| "rb" | 二进制只读 | 失败 | 从头开始读 |
| "wb" | 二进制只写 | 创建新文件 | 清空内容 |

```c
#include <stdio.h>

void demonstrate_file_modes() {
    FILE *file;
    
    // 写入模式
    file = fopen("demo.txt", "w");
    if (file != NULL) {
        fprintf(file, "第一行\n第二行\n");
        fclose(file);
        printf("写入完成\n");
    }
    
    // 追加模式
    file = fopen("demo.txt", "a");
    if (file != NULL) {
        fprintf(file, "第三行\n");
        fclose(file);
        printf("追加完成\n");
    }
    
    // 读取模式
    file = fopen("demo.txt", "r");
    if (file != NULL) {
        char line[100];
        printf("文件内容:\n");
        while (fgets(line, sizeof(line), file) != NULL) {
            printf("%s", line);
        }
        fclose(file);
    }
}

int main() {
    demonstrate_file_modes();
    return 0;
}
```

---

## 文本文件操作

### 基本的文本文件读写

```c
#include <stdio.h>
#include <string.h>

#define MAX_NAME_LEN 50

// 简单的学生信息结构
typedef struct {
    char name[MAX_NAME_LEN];
    int age;
    float score;
} SimpleStudent;

// 将学生信息写入文本文件
void write_student_to_text(const char *filename, SimpleStudent *student) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法创建文件 %s\n", filename);
        return;
    }
    
    // 写入学生信息（格式化文本）
    fprintf(file, "姓名: %s\n", student->name);
    fprintf(file, "年龄: %d\n", student->age);
    fprintf(file, "成绩: %.2f\n", student->score);
    
    fclose(file);
    printf("学生信息已保存到 %s\n", filename);
}

// 从文本文件读取学生信息
int read_student_from_text(const char *filename, SimpleStudent *student) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        return 0;
    }
    
    // 读取学生信息
    char temp[100];
    if (fscanf(file, "姓名: %s\n", student->name) != 1 ||
        fscanf(file, "年龄: %d\n", &student->age) != 1 ||
        fscanf(file, "成绩: %f\n", &student->score) != 1) {
        printf("文件格式错误\n");
        fclose(file);
        return 0;
    }
    
    fclose(file);
    return 1;
}

int main() {
    SimpleStudent s1 = {"张三", 20, 85.5};
    SimpleStudent s2;
    
    // 写入文件
    write_student_to_text("student.txt", &s1);
    
    // 从文件读取
    if (read_student_from_text("student.txt", &s2)) {
        printf("读取成功:\n");
        printf("姓名: %s\n", s2.name);
        printf("年龄: %d\n", s2.age);
        printf("成绩: %.2f\n", s2.score);
    }
    
    return 0;
}
```

### CSV格式存储多个学生

```c
#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LEN 50

typedef struct {
    char name[MAX_NAME_LEN];
    int age;
    float math, english, chinese;
    float average;
} Student;

// 保存学生数据到CSV文件
void save_students_csv(const char *filename, Student students[], int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法创建文件 %s\n", filename);
        return;
    }
    
    // 写入CSV头部
    fprintf(file, "姓名,年龄,数学,英语,语文,平均分\n");
    
    // 写入学生数据
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%d,%.2f,%.2f,%.2f,%.2f\n",
                students[i].name, students[i].age,
                students[i].math, students[i].english,
                students[i].chinese, students[i].average);
    }
    
    fclose(file);
    printf("已保存 %d 名学生到 %s\n", count, filename);
}

// 从CSV文件加载学生数据
int load_students_csv(const char *filename, Student students[], int max_count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        return 0;
    }
    
    char line[200];
    int count = 0;
    
    // 跳过头部行
    fgets(line, sizeof(line), file);
    
    // 读取学生数据
    while (fgets(line, sizeof(line), file) != NULL && count < max_count) {
        // 解析CSV行
        if (sscanf(line, "%[^,],%d,%f,%f,%f,%f",
                   students[count].name, &students[count].age,
                   &students[count].math, &students[count].english,
                   &students[count].chinese, &students[count].average) == 6) {
            count++;
        }
    }
    
    fclose(file);
    printf("从 %s 加载了 %d 名学生\n", filename, count);
    return count;
}

int main() {
    Student students[MAX_STUDENTS] = {
        {"张三", 20, 85.0, 92.0, 88.0, 88.33},
        {"李四", 19, 78.0, 85.0, 90.0, 84.33},
        {"王五", 21, 92.0, 88.0, 85.0, 88.33}
    };
    int count = 3;
    
    // 保存到CSV
    save_students_csv("students.csv", students, count);
    
    // 清空数组并重新加载
    memset(students, 0, sizeof(students));
    count = load_students_csv("students.csv", students, MAX_STUDENTS);
    
    // 显示加载的数据
    printf("\n加载的学生信息:\n");
    for (int i = 0; i < count; i++) {
        printf("%s, %d岁, 平均分: %.2f\n", 
               students[i].name, students[i].age, students[i].average);
    }
    
    return 0;
}
```

---

## 二进制文件操作

### 为什么使用二进制文件？

**文本文件 vs 二进制文件**:

| 特性 | 文本文件 | 二进制文件 |
|------|----------|----------|
| 可读性 | 人类可读 | 需要程序解析 |
| 文件大小 | 较大 | 较小 |
| 读写速度 | 较慢 | 较快 |
| 精度 | 可能丢失精度 | 保持原始精度 |
| 跨平台 | 较好 | 需要注意字节序 |

### 二进制文件基本操作

```c
#include <stdio.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_ID_LEN 20

typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    int age;
    float math_score;
    float english_score;
    float chinese_score;
    float average_score;
} Student;

// 保存单个学生到二进制文件
void save_student_binary(const char *filename, Student *student) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("无法创建文件 %s\n", filename);
        return;
    }
    
    // 写入整个结构体
    size_t written = fwrite(student, sizeof(Student), 1, file);
    if (written == 1) {
        printf("学生信息已保存到二进制文件\n");
    } else {
        printf("写入失败\n");
    }
    
    fclose(file);
}

// 从二进制文件读取单个学生
int load_student_binary(const char *filename, Student *student) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        return 0;
    }
    
    // 读取整个结构体
    size_t read = fread(student, sizeof(Student), 1, file);
    fclose(file);
    
    if (read == 1) {
        printf("学生信息加载成功\n");
        return 1;
    } else {
        printf("读取失败\n");
        return 0;
    }
}

// 保存多个学生到二进制文件
void save_students_binary(const char *filename, Student students[], int count) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("无法创建文件 %s\n", filename);
        return;
    }
    
    // 先写入学生数量
    fwrite(&count, sizeof(int), 1, file);
    
    // 再写入所有学生数据
    size_t written = fwrite(students, sizeof(Student), count, file);
    
    fclose(file);
    
    if (written == count) {
        printf("已保存 %d 名学生到二进制文件\n", count);
    } else {
        printf("保存失败，只写入了 %zu 名学生\n", written);
    }
}

// 从二进制文件加载多个学生
int load_students_binary(const char *filename, Student students[], int max_count) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("无法打开文件 %s\n", filename);
        return 0;
    }
    
    int count;
    // 先读取学生数量
    if (fread(&count, sizeof(int), 1, file) != 1) {
        printf("读取学生数量失败\n");
        fclose(file);
        return 0;
    }
    
    if (count > max_count) {
        printf("文件中的学生数量(%d)超过数组容量(%d)\n", count, max_count);
        fclose(file);
        return 0;
    }
    
    // 读取所有学生数据
    size_t read = fread(students, sizeof(Student), count, file);
    fclose(file);
    
    if (read == count) {
        printf("成功加载 %d 名学生\n", count);
        return count;
    } else {
        printf("读取失败，只读取了 %zu 名学生\n", read);
        return read;
    }
}

int main() {
    Student students[] = {
        {"2023001", "张三", 20, 85.0, 92.0, 88.0, 88.33},
        {"2023002", "李四", 19, 78.0, 85.0, 90.0, 84.33},
        {"2023003", "王五", 21, 92.0, 88.0, 85.0, 88.33}
    };
    int count = 3;
    
    // 保存到二进制文件
    save_students_binary("students.dat", students, count);
    
    // 清空数组
    Student loaded_students[100];
    memset(loaded_students, 0, sizeof(loaded_students));
    
    // 从二进制文件加载
    int loaded_count = load_students_binary("students.dat", loaded_students, 100);
    
    // 显示加载的数据
    printf("\n加载的学生信息:\n");
    for (int i = 0; i < loaded_count; i++) {
        printf("%s: %s, %d岁, 平均分: %.2f\n", 
               loaded_students[i].id, loaded_students[i].name, 
               loaded_students[i].age, loaded_students[i].average_score);
    }
    
    return 0;
}
```

---

## 学生数据的文件存储

### 完整的文件操作系统

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LEN 50
#define MAX_ID_LEN 20
#define FILENAME "students.dat"

typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    int age;
    float math_score;
    float english_score;
    float chinese_score;
    float average_score;
} Student;

typedef struct {
    Student students[MAX_STUDENTS];
    int count;
} StudentSystem;

// 保存系统数据到文件
int save_system_to_file(StudentSystem *system, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("错误：无法创建文件 %s\n", filename);
        return 0;
    }
    
    // 写入学生数量
    if (fwrite(&system->count, sizeof(int), 1, file) != 1) {
        printf("错误：写入学生数量失败\n");
        fclose(file);
        return 0;
    }
    
    // 写入学生数据
    if (fwrite(system->students, sizeof(Student), system->count, file) != system->count) {
        printf("错误：写入学生数据失败\n");
        fclose(file);
        return 0;
    }
    
    fclose(file);
    printf("数据保存成功！共保存 %d 名学生\n", system->count);
    return 1;
}

// 从文件加载系统数据
int load_system_from_file(StudentSystem *system, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("提示：数据文件不存在，将创建新的数据文件\n");
        system->count = 0;
        return 0;
    }
    
    // 读取学生数量
    if (fread(&system->count, sizeof(int), 1, file) != 1) {
        printf("错误：读取学生数量失败\n");
        fclose(file);
        system->count = 0;
        return 0;
    }
    
    // 验证学生数量的合理性
    if (system->count < 0 || system->count > MAX_STUDENTS) {
        printf("错误：文件中的学生数量不合理 (%d)\n", system->count);
        fclose(file);
        system->count = 0;
        return 0;
    }
    
    // 读取学生数据
    if (fread(system->students, sizeof(Student), system->count, file) != system->count) {
        printf("错误：读取学生数据失败\n");
        fclose(file);
        system->count = 0;
        return 0;
    }
    
    fclose(file);
    printf("数据加载成功！共加载 %d 名学生\n", system->count);
    return 1;
}

// 备份数据文件
int backup_data_file(const char *source, const char *backup) {
    FILE *src = fopen(source, "rb");
    if (src == NULL) {
        printf("源文件不存在，无需备份\n");
        return 0;
    }
    
    FILE *dst = fopen(backup, "wb");
    if (dst == NULL) {
        printf("无法创建备份文件\n");
        fclose(src);
        return 0;
    }
    
    // 复制文件内容
    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dst);
    }
    
    fclose(src);
    fclose(dst);
    printf("数据备份完成: %s -> %s\n", source, backup);
    return 1;
}

// 导出为CSV格式
int export_to_csv(StudentSystem *system, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法创建CSV文件\n");
        return 0;
    }
    
    // 写入CSV头部
    fprintf(file, "学号,姓名,年龄,数学,英语,语文,平均分\n");
    
    // 写入学生数据
    for (int i = 0; i < system->count; i++) {
        Student *s = &system->students[i];
        fprintf(file, "%s,%s,%d,%.2f,%.2f,%.2f,%.2f\n",
                s->id, s->name, s->age,
                s->math_score, s->english_score, s->chinese_score,
                s->average_score);
    }
    
    fclose(file);
    printf("数据已导出到CSV文件: %s\n", filename);
    return 1;
}

int main() {
    StudentSystem system;
    
    // 初始化系统
    system.count = 0;
    
    // 添加一些测试数据
    strcpy(system.students[0].id, "2023001");
    strcpy(system.students[0].name, "张三");
    system.students[0].age = 20;
    system.students[0].math_score = 85.0;
    system.students[0].english_score = 92.0;
    system.students[0].chinese_score = 88.0;
    system.students[0].average_score = 88.33;
    system.count++;
    
    strcpy(system.students[1].id, "2023002");
    strcpy(system.students[1].name, "李四");
    system.students[1].age = 19;
    system.students[1].math_score = 78.0;
    system.students[1].english_score = 85.0;
    system.students[1].chinese_score = 90.0;
    system.students[1].average_score = 84.33;
    system.count++;
    
    // 保存数据
    save_system_to_file(&system, FILENAME);
    
    // 备份数据
    backup_data_file(FILENAME, "students_backup.dat");
    
    // 导出CSV
    export_to_csv(&system, "students_export.csv");
    
    // 清空系统并重新加载
    system.count = 0;
    load_system_from_file(&system, FILENAME);
    
    // 显示加载的数据
    printf("\n当前系统中的学生:\n");
    for (int i = 0; i < system.count; i++) {
        printf("%s: %s\n", system.students[i].id, system.students[i].name);
    }
    
    return 0;
}
```

---

## 错误处理与安全性

### 文件操作的错误处理

```c
#include <stdio.h>
#include <errno.h>
#include <string.h>

// 安全的文件打开函数
FILE* safe_fopen(const char *filename, const char *mode) {
    FILE *file = fopen(filename, mode);
    if (file == NULL) {
        printf("错误：无法打开文件 '%s'\n", filename);
        printf("原因：%s\n", strerror(errno));
    }
    return file;
}

// 检查文件是否存在
int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

// 获取文件大小
long get_file_size(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return -1;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    
    return size;
}

// 验证文件完整性
int validate_data_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return 0;
    }
    
    int count;
    if (fread(&count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    
    // 检查学生数量是否合理
    if (count < 0 || count > MAX_STUDENTS) {
        fclose(file);
        return 0;
    }
    
    // 检查文件大小是否匹配
    long expected_size = sizeof(int) + count * sizeof(Student);
    long actual_size = get_file_size(filename);
    
    fclose(file);
    
    return (expected_size == actual_size);
}

int main() {
    const char *filename = "students.dat";
    
    // 检查文件是否存在
    if (file_exists(filename)) {
        printf("文件存在\n");
        
        // 获取文件大小
        long size = get_file_size(filename);
        printf("文件大小: %ld 字节\n", size);
        
        // 验证文件完整性
        if (validate_data_file(filename)) {
            printf("文件完整性验证通过\n");
        } else {
            printf("文件完整性验证失败\n");
        }
    } else {
        printf("文件不存在\n");
    }
    
    return 0;
}
```

---

## 实践练习

### 练习1：文本格式配置文件

创建一个配置文件系统：

```c
// 要求：
// 1. 创建一个配置文件存储系统设置
// 2. 支持读取和写入配置
// 3. 配置项包括：最大学生数、默认及格分数、数据文件名等

typedef struct {
    int max_students;
    float pass_score;
    char data_filename[100];
    int auto_save;
} Config;

// 保存配置到文件
int save_config(Config *config, const char *filename) {
    // 实现保存逻辑
    return 0;
}

// 从文件加载配置
int load_config(Config *config, const char *filename) {
    // 实现加载逻辑
    return 0;
}

// 设置默认配置
void set_default_config(Config *config) {
    // 实现默认设置
}
```

### 练习2：数据文件版本控制

实现简单的版本控制：

```c
// 要求：
// 1. 在文件头部添加版本信息
// 2. 支持不同版本的数据格式
// 3. 实现版本升级功能

#define FILE_VERSION 1

typedef struct {
    int version;
    int student_count;
    char reserved[24];  // 预留空间
} FileHeader;

// 写入带版本信息的文件
int save_with_version(StudentSystem *system, const char *filename) {
    // 实现带版本的保存
    return 0;
}

// 读取并检查版本
int load_with_version(StudentSystem *system, const char *filename) {
    // 实现版本检查和加载
    return 0;
}
```

### 练习3：数据压缩存储

实现简单的数据压缩：

```c
// 要求：
// 1. 只存储非空的学生记录
// 2. 使用更紧凑的数据格式
// 3. 比较压缩前后的文件大小

// 紧凑的学生结构
typedef struct {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    unsigned char age;  // 使用unsigned char节省空间
    unsigned short scores[3];  // 将浮点分数转为整数(乘以100)
} CompactStudent;

// 转换为紧凑格式
CompactStudent to_compact(Student *student) {
    // 实现转换逻辑
    CompactStudent compact = {0};
    return compact;
}

// 从紧凑格式转换回来
Student from_compact(CompactStudent *compact) {
    // 实现转换逻辑
    Student student = {0};
    return student;
}
```

---

## 💡 本课要点总结

### 重要概念
1. **文件指针**: FILE* 类型，用于文件操作
2. **文件模式**: 不同的打开模式适用于不同场景
3. **二进制 vs 文本**: 各有优缺点，根据需求选择
4. **错误处理**: 文件操作必须检查返回值

### 最佳实践
1. **总是检查文件操作的返回值**
2. **及时关闭文件**，避免资源泄露
3. **使用二进制模式**存储结构体数据
4. **添加数据验证**，确保文件完整性
5. **实现备份机制**，防止数据丢失

### 常见错误
1. **忘记检查fopen返回值**
2. **文件打开后忘记关闭**
3. **读写模式不匹配**
4. **缓冲区溢出**
5. **字节序问题**（跨平台时）

### 性能考虑
1. **批量读写**比逐个读写效率高
2. **二进制文件**比文本文件读写更快
3. **适当的缓冲区大小**可以提高性能
4. **避免频繁的文件开关**操作

---

## 🚀 下一步

完成本课学习后，你应该能够：
- ✅ 熟练进行文件的读写操作
- ✅ 选择合适的文件格式存储数据
- ✅ 实现数据的持久化存储
- ✅ 处理文件操作中的错误情况

**准备好了吗？** 让我们进入 [第4课：项目架构设计](./04-project-structure.md)，学习如何组织一个完整的C语言项目！

---

*💭 思考题：在什么情况下你会选择文本文件而不是二进制文件来存储数据？*