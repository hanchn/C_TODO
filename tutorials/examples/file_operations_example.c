// 文件操作示例
// 对应教程：第3课 - 文件操作基础

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 学生结构体（简化版）
typedef struct {
    char id[20];
    char name[50];
    int age;
    float math_score;
    float english_score;
    float chinese_score;
} Student;

// 函数声明
void demonstrate_text_file_operations();
void demonstrate_binary_file_operations();
void demonstrate_csv_operations();
int save_students_to_text(Student students[], int count, const char *filename);
int load_students_from_text(Student students[], int max_count, const char *filename);
int save_students_to_binary(Student students[], int count, const char *filename);
int load_students_from_binary(Student students[], int max_count, const char *filename);
int export_students_to_csv(Student students[], int count, const char *filename);
int import_students_from_csv(Student students[], int max_count, const char *filename);
void print_student(Student *student);

int main() {
    printf("=== 文件操作示例 ===\n\n");
    
    demonstrate_text_file_operations();
    demonstrate_binary_file_operations();
    demonstrate_csv_operations();
    
    return 0;
}

// 演示文本文件操作
void demonstrate_text_file_operations() {
    printf("1. 文本文件操作演示\n");
    printf("--------------------\n");
    
    // 创建示例数据
    Student students[3] = {
        {"2024001", "张三", 20, 85.5, 92.0, 88.5},
        {"2024002", "李四", 21, 90.0, 85.0, 92.0},
        {"2024003", "王五", 19, 78.0, 88.0, 85.0}
    };
    
    // 保存到文本文件
    const char *text_file = "students.txt";
    if (save_students_to_text(students, 3, text_file) == 0) {
        printf("成功保存 3 个学生到文本文件: %s\n", text_file);
    }
    
    // 从文本文件加载
    Student loaded_students[10];
    int loaded_count = load_students_from_text(loaded_students, 10, text_file);
    
    if (loaded_count > 0) {
        printf("从文本文件加载了 %d 个学生:\n", loaded_count);
        for (int i = 0; i < loaded_count; i++) {
            printf("  %d. ", i + 1);
            print_student(&loaded_students[i]);
        }
    }
    
    printf("\n");
}

// 演示二进制文件操作
void demonstrate_binary_file_operations() {
    printf("2. 二进制文件操作演示\n");
    printf("----------------------\n");
    
    // 创建示例数据
    Student students[2] = {
        {"2024004", "赵六", 22, 92.0, 90.0, 89.0},
        {"2024005", "钱七", 20, 87.0, 93.0, 91.0}
    };
    
    // 保存到二进制文件
    const char *binary_file = "students.dat";
    if (save_students_to_binary(students, 2, binary_file) == 0) {
        printf("成功保存 2 个学生到二进制文件: %s\n", binary_file);
    }
    
    // 从二进制文件加载
    Student loaded_students[10];
    int loaded_count = load_students_from_binary(loaded_students, 10, binary_file);
    
    if (loaded_count > 0) {
        printf("从二进制文件加载了 %d 个学生:\n", loaded_count);
        for (int i = 0; i < loaded_count; i++) {
            printf("  %d. ", i + 1);
            print_student(&loaded_students[i]);
        }
    }
    
    printf("\n");
}

// 演示CSV文件操作
void demonstrate_csv_operations() {
    printf("3. CSV文件操作演示\n");
    printf("------------------\n");
    
    // 创建示例数据
    Student students[4] = {
        {"2024006", "孙八", 21, 88.0, 86.0, 90.0},
        {"2024007", "周九", 19, 91.0, 89.0, 87.0},
        {"2024008", "吴十", 22, 85.0, 92.0, 88.0},
        {"2024009", "郑十一", 20, 89.0, 87.0, 91.0}
    };
    
    // 导出到CSV文件
    const char *csv_file = "students.csv";
    if (export_students_to_csv(students, 4, csv_file) == 0) {
        printf("成功导出 4 个学生到CSV文件: %s\n", csv_file);
    }
    
    // 从CSV文件导入
    Student imported_students[10];
    int imported_count = import_students_from_csv(imported_students, 10, csv_file);
    
    if (imported_count > 0) {
        printf("从CSV文件导入了 %d 个学生:\n", imported_count);
        for (int i = 0; i < imported_count; i++) {
            printf("  %d. ", i + 1);
            print_student(&imported_students[i]);
        }
    }
    
    printf("\n");
}

// 保存学生到文本文件
int save_students_to_text(Student students[], int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("错误: 无法创建文件 %s\n", filename);
        return -1;
    }
    
    fprintf(file, "学生数量: %d\n", count);
    fprintf(file, "\n");
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "学生 %d:\n", i + 1);
        fprintf(file, "  学号: %s\n", students[i].id);
        fprintf(file, "  姓名: %s\n", students[i].name);
        fprintf(file, "  年龄: %d\n", students[i].age);
        fprintf(file, "  数学: %.1f\n", students[i].math_score);
        fprintf(file, "  英语: %.1f\n", students[i].english_score);
        fprintf(file, "  语文: %.1f\n", students[i].chinese_score);
        fprintf(file, "\n");
    }
    
    fclose(file);
    return 0;
}

// 从文本文件加载学生（简化版本）
int load_students_from_text(Student students[], int max_count, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("错误: 无法打开文件 %s\n", filename);
        return 0;
    }
    
    int count = 0;
    fscanf(file, "学生数量: %d\n", &count);
    
    if (count > max_count) {
        count = max_count;
    }
    
    for (int i = 0; i < count; i++) {
        fscanf(file, "\n学生 %*d:\n");
        fscanf(file, "  学号: %s\n", students[i].id);
        fscanf(file, "  姓名: %s\n", students[i].name);
        fscanf(file, "  年龄: %d\n", &students[i].age);
        fscanf(file, "  数学: %f\n", &students[i].math_score);
        fscanf(file, "  英语: %f\n", &students[i].english_score);
        fscanf(file, "  语文: %f\n", &students[i].chinese_score);
    }
    
    fclose(file);
    return count;
}

// 保存学生到二进制文件
int save_students_to_binary(Student students[], int count, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("错误: 无法创建二进制文件 %s\n", filename);
        return -1;
    }
    
    // 写入文件头
    const char header[] = "STUDENT_DATA";
    fwrite(header, sizeof(char), strlen(header), file);
    
    // 写入学生数量
    fwrite(&count, sizeof(int), 1, file);
    
    // 写入学生数据
    fwrite(students, sizeof(Student), count, file);
    
    fclose(file);
    return 0;
}

// 从二进制文件加载学生
int load_students_from_binary(Student students[], int max_count, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("错误: 无法打开二进制文件 %s\n", filename);
        return 0;
    }
    
    // 读取并验证文件头
    char header[20];
    fread(header, sizeof(char), 12, file);
    header[12] = '\0';
    
    if (strcmp(header, "STUDENT_DATA") != 0) {
        printf("错误: 无效的文件格式\n");
        fclose(file);
        return 0;
    }
    
    // 读取学生数量
    int count;
    fread(&count, sizeof(int), 1, file);
    
    if (count > max_count) {
        count = max_count;
    }
    
    // 读取学生数据
    int actual_count = fread(students, sizeof(Student), count, file);
    
    fclose(file);
    return actual_count;
}

// 导出学生到CSV文件
int export_students_to_csv(Student students[], int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("错误: 无法创建CSV文件 %s\n", filename);
        return -1;
    }
    
    // 写入CSV表头
    fprintf(file, "学号,姓名,年龄,数学,英语,语文\n");
    
    // 写入学生数据
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%d,%.1f,%.1f,%.1f\n",
                students[i].id, students[i].name, students[i].age,
                students[i].math_score, students[i].english_score, 
                students[i].chinese_score);
    }
    
    fclose(file);
    return 0;
}

// 从CSV文件导入学生
int import_students_from_csv(Student students[], int max_count, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("错误: 无法打开CSV文件 %s\n", filename);
        return 0;
    }
    
    char line[256];
    int count = 0;
    
    // 跳过表头
    fgets(line, sizeof(line), file);
    
    // 读取数据行
    while (fgets(line, sizeof(line), file) && count < max_count) {
        // 解析CSV行
        char *token = strtok(line, ",");
        if (token) strcpy(students[count].id, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(students[count].name, token);
        
        token = strtok(NULL, ",");
        if (token) students[count].age = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) students[count].math_score = atof(token);
        
        token = strtok(NULL, ",");
        if (token) students[count].english_score = atof(token);
        
        token = strtok(NULL, ",");
        if (token) students[count].chinese_score = atof(token);
        
        count++;
    }
    
    fclose(file);
    return count;
}

// 打印学生信息
void print_student(Student *student) {
    printf("%s (%s, %d岁) - 数学:%.1f 英语:%.1f 语文:%.1f\n",
           student->name, student->id, student->age,
           student->math_score, student->english_score, student->chinese_score);
}