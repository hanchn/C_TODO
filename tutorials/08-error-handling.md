# 第8课：错误处理与调试

> **学习目标**: 学习如何设计健壮的错误处理机制和掌握调试技巧

## 📋 本课内容

1. [错误处理基础](#错误处理基础)
2. [错误代码设计](#错误代码设计)
3. [异常处理机制](#异常处理机制)
4. [日志系统设计](#日志系统设计)
5. [调试技巧与工具](#调试技巧与工具)
6. [单元测试](#单元测试)
7. [内存管理与检测](#内存管理与检测)
8. [实践练习](#实践练习)

---

## 错误处理基础

### 错误处理的重要性

```c
// 糟糕的错误处理
void bad_error_handling() {
    FILE *file = fopen("data.txt", "r");  // 没有检查返回值
    char buffer[100];
    fgets(buffer, sizeof(buffer), file);  // 可能导致程序崩溃
    fclose(file);
}

// 良好的错误处理
int good_error_handling() {
    FILE *file = fopen("data.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "错误: 无法打开文件 data.txt\n");
        return -1;
    }
    
    char buffer[100];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fprintf(stderr, "错误: 读取文件失败\n");
        fclose(file);
        return -1;
    }
    
    fclose(file);
    return 0;
}
```

### 错误处理原则

1. **及早检测**: 在错误发生的地方立即检测
2. **明确报告**: 提供清晰的错误信息
3. **优雅降级**: 程序应该能够从错误中恢复
4. **资源清理**: 确保在错误情况下也能正确释放资源
5. **用户友好**: 向用户提供有意义的错误信息

### 错误处理策略

```c
// error_handling.h
#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

// 错误级别
typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3,
    LOG_FATAL = 4
} LogLevel;

// 错误处理宏
#define CHECK_NULL(ptr, msg) \
    do { \
        if ((ptr) == NULL) { \
            log_error("NULL pointer: %s", (msg)); \
            return -1; \
        } \
    } while(0)

#define CHECK_RESULT(result, msg) \
    do { \
        if ((result) != 0) { \
            log_error("Operation failed: %s", (msg)); \
            return (result); \
        } \
    } while(0)

#define SAFE_FREE(ptr) \
    do { \
        if ((ptr) != NULL) { \
            free(ptr); \
            (ptr) = NULL; \
        } \
    } while(0)

// 错误处理函数声明
void log_message(LogLevel level, const char *format, ...);
void log_error(const char *format, ...);
void log_warning(const char *format, ...);
void log_info(const char *format, ...);
void log_debug(const char *format, ...);

int handle_file_error(const char *filename, const char *operation);
int handle_memory_error(const char *operation);
int handle_input_error(const char *input, const char *expected);

#endif // ERROR_HANDLING_H
```

---

## 错误代码设计

### 统一的错误代码系统

```c
// error_codes.h
#ifndef ERROR_CODES_H
#define ERROR_CODES_H

// 成功代码
#define SUCCESS 0

// 通用错误代码 (1-99)
#define ERROR_GENERAL           1
#define ERROR_INVALID_PARAMETER 2
#define ERROR_NULL_POINTER      3
#define ERROR_MEMORY_ALLOCATION 4
#define ERROR_BUFFER_OVERFLOW   5
#define ERROR_INVALID_STATE     6

// 文件操作错误 (100-199)
#define ERROR_FILE_NOT_FOUND    100
#define ERROR_FILE_OPEN_FAILED  101
#define ERROR_FILE_READ_FAILED  102
#define ERROR_FILE_WRITE_FAILED 103
#define ERROR_FILE_CLOSE_FAILED 104
#define ERROR_FILE_CORRUPTED    105
#define ERROR_FILE_PERMISSION   106

// 数据操作错误 (200-299)
#define ERROR_STUDENT_NOT_FOUND 200
#define ERROR_STUDENT_EXISTS    201
#define ERROR_INVALID_ID        202
#define ERROR_INVALID_NAME      203
#define ERROR_INVALID_AGE       204
#define ERROR_INVALID_SCORE     205
#define ERROR_SYSTEM_FULL       206

// 输入验证错误 (300-399)
#define ERROR_INPUT_TOO_LONG    300
#define ERROR_INPUT_TOO_SHORT   301
#define ERROR_INPUT_INVALID_CHAR 302
#define ERROR_INPUT_OUT_OF_RANGE 303
#define ERROR_INPUT_FORMAT      304

// 系统错误 (400-499)
#define ERROR_SYSTEM_INIT       400
#define ERROR_SYSTEM_SHUTDOWN   401
#define ERROR_RESOURCE_LIMIT    402
#define ERROR_PERMISSION_DENIED 403

// 错误信息结构
typedef struct {
    int code;
    char message[256];
    char details[512];
    time_t timestamp;
    char function[64];
    char file[128];
    int line;
} ErrorInfo;

// 错误信息获取函数
const char* get_error_message(int error_code);
ErrorInfo* create_error_info(int code, const char *message, const char *details,
                           const char *function, const char *file, int line);
void print_error_info(const ErrorInfo *error);
void free_error_info(ErrorInfo *error);

// 错误报告宏
#define REPORT_ERROR(code, msg, details) \
    create_error_info((code), (msg), (details), __FUNCTION__, __FILE__, __LINE__)

#endif // ERROR_CODES_H
```

### 错误信息实现

```c
// error_codes.c
#include "error_codes.h"

// 错误消息映射表
static const struct {
    int code;
    const char *message;
} error_messages[] = {
    {SUCCESS, "操作成功"},
    {ERROR_GENERAL, "一般错误"},
    {ERROR_INVALID_PARAMETER, "无效参数"},
    {ERROR_NULL_POINTER, "空指针错误"},
    {ERROR_MEMORY_ALLOCATION, "内存分配失败"},
    {ERROR_BUFFER_OVERFLOW, "缓冲区溢出"},
    {ERROR_INVALID_STATE, "无效状态"},
    
    {ERROR_FILE_NOT_FOUND, "文件未找到"},
    {ERROR_FILE_OPEN_FAILED, "文件打开失败"},
    {ERROR_FILE_READ_FAILED, "文件读取失败"},
    {ERROR_FILE_WRITE_FAILED, "文件写入失败"},
    {ERROR_FILE_CLOSE_FAILED, "文件关闭失败"},
    {ERROR_FILE_CORRUPTED, "文件已损坏"},
    {ERROR_FILE_PERMISSION, "文件权限不足"},
    
    {ERROR_STUDENT_NOT_FOUND, "学生未找到"},
    {ERROR_STUDENT_EXISTS, "学生已存在"},
    {ERROR_INVALID_ID, "无效学号"},
    {ERROR_INVALID_NAME, "无效姓名"},
    {ERROR_INVALID_AGE, "无效年龄"},
    {ERROR_INVALID_SCORE, "无效成绩"},
    {ERROR_SYSTEM_FULL, "系统已满"},
    
    {ERROR_INPUT_TOO_LONG, "输入过长"},
    {ERROR_INPUT_TOO_SHORT, "输入过短"},
    {ERROR_INPUT_INVALID_CHAR, "包含无效字符"},
    {ERROR_INPUT_OUT_OF_RANGE, "输入超出范围"},
    {ERROR_INPUT_FORMAT, "输入格式错误"},
    
    {ERROR_SYSTEM_INIT, "系统初始化失败"},
    {ERROR_SYSTEM_SHUTDOWN, "系统关闭失败"},
    {ERROR_RESOURCE_LIMIT, "资源限制"},
    {ERROR_PERMISSION_DENIED, "权限被拒绝"},
    
    {-1, NULL}  // 结束标记
};

// 获取错误消息
const char* get_error_message(int error_code) {
    for (int i = 0; error_messages[i].message != NULL; i++) {
        if (error_messages[i].code == error_code) {
            return error_messages[i].message;
        }
    }
    return "未知错误";
}

// 创建错误信息
ErrorInfo* create_error_info(int code, const char *message, const char *details,
                           const char *function, const char *file, int line) {
    ErrorInfo *error = malloc(sizeof(ErrorInfo));
    if (error == NULL) {
        return NULL;
    }
    
    error->code = code;
    error->timestamp = time(NULL);
    error->line = line;
    
    strncpy(error->message, message ? message : get_error_message(code), 
            sizeof(error->message) - 1);
    error->message[sizeof(error->message) - 1] = '\0';
    
    strncpy(error->details, details ? details : "", 
            sizeof(error->details) - 1);
    error->details[sizeof(error->details) - 1] = '\0';
    
    strncpy(error->function, function ? function : "", 
            sizeof(error->function) - 1);
    error->function[sizeof(error->function) - 1] = '\0';
    
    strncpy(error->file, file ? file : "", 
            sizeof(error->file) - 1);
    error->file[sizeof(error->file) - 1] = '\0';
    
    return error;
}

// 打印错误信息
void print_error_info(const ErrorInfo *error) {
    if (error == NULL) {
        return;
    }
    
    char time_str[64];
    struct tm *tm_info = localtime(&error->timestamp);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);
    
    fprintf(stderr, "\n=== 错误报告 ===\n");
    fprintf(stderr, "时间: %s\n", time_str);
    fprintf(stderr, "代码: %d\n", error->code);
    fprintf(stderr, "消息: %s\n", error->message);
    if (strlen(error->details) > 0) {
        fprintf(stderr, "详情: %s\n", error->details);
    }
    fprintf(stderr, "位置: %s() in %s:%d\n", 
            error->function, error->file, error->line);
    fprintf(stderr, "================\n\n");
}

// 释放错误信息
void free_error_info(ErrorInfo *error) {
    if (error != NULL) {
        free(error);
    }
}
```

### 错误处理的实际应用

```c
// 在学生管理系统中应用错误处理
int add_student_with_error_handling(StudentSystem *system, const Student *student) {
    // 参数验证
    if (system == NULL) {
        ErrorInfo *error = REPORT_ERROR(ERROR_NULL_POINTER, 
                                      "系统指针为空", 
                                      "StudentSystem指针不能为NULL");
        print_error_info(error);
        free_error_info(error);
        return ERROR_NULL_POINTER;
    }
    
    if (student == NULL) {
        ErrorInfo *error = REPORT_ERROR(ERROR_NULL_POINTER, 
                                      "学生指针为空", 
                                      "Student指针不能为NULL");
        print_error_info(error);
        free_error_info(error);
        return ERROR_NULL_POINTER;
    }
    
    // 检查系统容量
    if (system->count >= MAX_STUDENTS) {
        ErrorInfo *error = REPORT_ERROR(ERROR_SYSTEM_FULL, 
                                      "学生系统已满", 
                                      "无法添加更多学生，请删除一些记录");
        print_error_info(error);
        free_error_info(error);
        return ERROR_SYSTEM_FULL;
    }
    
    // 验证学生数据
    int validation_result = validate_student_data(student);
    if (validation_result != SUCCESS) {
        ErrorInfo *error = REPORT_ERROR(validation_result, 
                                      "学生数据验证失败", 
                                      "请检查学号、姓名、年龄和成绩格式");
        print_error_info(error);
        free_error_info(error);
        return validation_result;
    }
    
    // 检查学号是否已存在
    if (find_student_by_id(system, student->id) != -1) {
        ErrorInfo *error = REPORT_ERROR(ERROR_STUDENT_EXISTS, 
                                      "学号已存在", 
                                      student->id);
        print_error_info(error);
        free_error_info(error);
        return ERROR_STUDENT_EXISTS;
    }
    
    // 添加学生
    system->students[system->count] = *student;
    system->count++;
    
    log_info("成功添加学生: %s (%s)", student->name, student->id);
    return SUCCESS;
}

// 数据验证函数
int validate_student_data(const Student *student) {
    // 验证学号
    if (strlen(student->id) < 6 || strlen(student->id) > 12) {
        return ERROR_INVALID_ID;
    }
    
    // 验证姓名
    if (strlen(student->name) < 2 || strlen(student->name) > 20) {
        return ERROR_INVALID_NAME;
    }
    
    // 验证年龄
    if (student->age < 15 || student->age > 30) {
        return ERROR_INVALID_AGE;
    }
    
    // 验证成绩
    if (student->math_score < 0 || student->math_score > 100 ||
        student->english_score < 0 || student->english_score > 100 ||
        student->chinese_score < 0 || student->chinese_score > 100) {
        return ERROR_INVALID_SCORE;
    }
    
    return SUCCESS;
}
```

---

## 异常处理机制

### 使用setjmp/longjmp实现异常处理

```c
// exception.h
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

// 异常类型
typedef enum {
    EXCEPTION_NONE = 0,
    EXCEPTION_NULL_POINTER,
    EXCEPTION_OUT_OF_MEMORY,
    EXCEPTION_FILE_ERROR,
    EXCEPTION_INVALID_DATA,
    EXCEPTION_SYSTEM_ERROR
} ExceptionType;

// 异常信息
typedef struct {
    ExceptionType type;
    char message[256];
    char file[128];
    int line;
    char function[64];
} Exception;

// 异常处理上下文
typedef struct {
    jmp_buf buffer;
    Exception exception;
    int active;
} ExceptionContext;

// 全局异常上下文栈
#define MAX_EXCEPTION_DEPTH 10
extern ExceptionContext exception_stack[MAX_EXCEPTION_DEPTH];
extern int exception_depth;

// 异常处理宏
#define TRY \
    do { \
        if (exception_depth >= MAX_EXCEPTION_DEPTH) { \
            fprintf(stderr, "异常栈溢出\n"); \
            exit(1); \
        } \
        ExceptionContext *ctx = &exception_stack[exception_depth++]; \
        ctx->active = 1; \
        if (setjmp(ctx->buffer) == 0) {

#define CATCH(exception_type) \
        ctx->active = 0; \
        exception_depth--; \
        } else { \
            ctx->active = 0; \
            exception_depth--; \
            if (ctx->exception.type == (exception_type)) {

#define CATCH_ALL \
        ctx->active = 0; \
        exception_depth--; \
        } else { \
            ctx->active = 0; \
            exception_depth--; \
            {

#define END_TRY \
            } \
        } \
    } while(0)

#define THROW(exception_type, msg) \
    throw_exception((exception_type), (msg), __FILE__, __LINE__, __FUNCTION__)

#define RETHROW() \
    do { \
        if (exception_depth > 0) { \
            ExceptionContext *ctx = &exception_stack[exception_depth - 1]; \
            if (ctx->active) { \
                longjmp(ctx->buffer, 1); \
            } \
        } \
    } while(0)

// 异常处理函数
void throw_exception(ExceptionType type, const char *message, 
                    const char *file, int line, const char *function);
void print_exception(const Exception *ex);
const char* get_exception_name(ExceptionType type);

#endif // EXCEPTION_H
```

### 异常处理实现

```c
// exception.c
#include "exception.h"

// 全局异常上下文栈
ExceptionContext exception_stack[MAX_EXCEPTION_DEPTH];
int exception_depth = 0;

// 抛出异常
void throw_exception(ExceptionType type, const char *message, 
                    const char *file, int line, const char *function) {
    if (exception_depth <= 0) {
        fprintf(stderr, "未捕获的异常: %s\n", message);
        fprintf(stderr, "位置: %s() in %s:%d\n", function, file, line);
        exit(1);
    }
    
    ExceptionContext *ctx = &exception_stack[exception_depth - 1];
    if (!ctx->active) {
        fprintf(stderr, "异常上下文不活跃\n");
        exit(1);
    }
    
    // 设置异常信息
    ctx->exception.type = type;
    strncpy(ctx->exception.message, message ? message : "", 
            sizeof(ctx->exception.message) - 1);
    ctx->exception.message[sizeof(ctx->exception.message) - 1] = '\0';
    
    strncpy(ctx->exception.file, file ? file : "", 
            sizeof(ctx->exception.file) - 1);
    ctx->exception.file[sizeof(ctx->exception.file) - 1] = '\0';
    
    ctx->exception.line = line;
    
    strncpy(ctx->exception.function, function ? function : "", 
            sizeof(ctx->exception.function) - 1);
    ctx->exception.function[sizeof(ctx->exception.function) - 1] = '\0';
    
    // 跳转到异常处理代码
    longjmp(ctx->buffer, 1);
}

// 打印异常信息
void print_exception(const Exception *ex) {
    if (ex == NULL) {
        return;
    }
    
    fprintf(stderr, "\n=== 异常信息 ===\n");
    fprintf(stderr, "类型: %s\n", get_exception_name(ex->type));
    fprintf(stderr, "消息: %s\n", ex->message);
    fprintf(stderr, "位置: %s() in %s:%d\n", 
            ex->function, ex->file, ex->line);
    fprintf(stderr, "================\n\n");
}

// 获取异常类型名称
const char* get_exception_name(ExceptionType type) {
    switch (type) {
        case EXCEPTION_NONE: return "无异常";
        case EXCEPTION_NULL_POINTER: return "空指针异常";
        case EXCEPTION_OUT_OF_MEMORY: return "内存不足异常";
        case EXCEPTION_FILE_ERROR: return "文件错误异常";
        case EXCEPTION_INVALID_DATA: return "无效数据异常";
        case EXCEPTION_SYSTEM_ERROR: return "系统错误异常";
        default: return "未知异常";
    }
}
```

### 异常处理的使用示例

```c
// 使用异常处理的文件操作
int safe_file_operation(const char *filename) {
    FILE *file = NULL;
    char *buffer = NULL;
    int result = 0;
    
    TRY {
        // 尝试打开文件
        file = fopen(filename, "r");
        if (file == NULL) {
            THROW(EXCEPTION_FILE_ERROR, "无法打开文件");
        }
        
        // 分配内存
        buffer = malloc(1024);
        if (buffer == NULL) {
            THROW(EXCEPTION_OUT_OF_MEMORY, "内存分配失败");
        }
        
        // 读取文件
        if (fread(buffer, 1, 1024, file) == 0 && ferror(file)) {
            THROW(EXCEPTION_FILE_ERROR, "文件读取失败");
        }
        
        // 处理数据
        if (strlen(buffer) == 0) {
            THROW(EXCEPTION_INVALID_DATA, "文件内容为空");
        }
        
        printf("文件操作成功\n");
        result = 1;
        
    } CATCH(EXCEPTION_FILE_ERROR) {
        fprintf(stderr, "文件操作异常: %s\n", 
                exception_stack[exception_depth].exception.message);
        result = 0;
        
    } CATCH(EXCEPTION_OUT_OF_MEMORY) {
        fprintf(stderr, "内存异常: %s\n", 
                exception_stack[exception_depth].exception.message);
        result = 0;
        
    } CATCH_ALL {
        print_exception(&exception_stack[exception_depth].exception);
        result = 0;
        
    } END_TRY;
    
    // 清理资源
    if (file != NULL) {
        fclose(file);
    }
    if (buffer != NULL) {
        free(buffer);
    }
    
    return result;
}

// 嵌套异常处理示例
void nested_exception_example() {
    TRY {
        printf("外层异常处理开始\n");
        
        TRY {
            printf("内层异常处理开始\n");
            
            // 模拟一个错误
            char *ptr = NULL;
            if (ptr == NULL) {
                THROW(EXCEPTION_NULL_POINTER, "指针为空");
            }
            
        } CATCH(EXCEPTION_NULL_POINTER) {
            printf("捕获到空指针异常，重新抛出\n");
            RETHROW();
            
        } END_TRY;
        
    } CATCH(EXCEPTION_NULL_POINTER) {
        printf("在外层捕获到重新抛出的异常\n");
        print_exception(&exception_stack[exception_depth].exception);
        
    } END_TRY;
}
```

---

## 日志系统设计

### 完整的日志系统

```c
// logger.h
#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <pthread.h>

// 日志级别
typedef enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARNING = 2,
    LOG_LEVEL_ERROR = 3,
    LOG_LEVEL_FATAL = 4
} LogLevel;

// 日志输出目标
typedef enum {
    LOG_TARGET_CONSOLE = 1,
    LOG_TARGET_FILE = 2,
    LOG_TARGET_BOTH = 3
} LogTarget;

// 日志配置
typedef struct {
    LogLevel min_level;        // 最小日志级别
    LogTarget target;          // 输出目标
    char log_file[256];        // 日志文件路径
    int max_file_size;         // 最大文件大小(MB)
    int max_backup_files;      // 最大备份文件数
    int enable_timestamp;      // 是否启用时间戳
    int enable_thread_id;      // 是否启用线程ID
    int enable_colors;         // 是否启用颜色
} LogConfig;

// 日志记录结构
typedef struct {
    LogLevel level;
    time_t timestamp;
    pthread_t thread_id;
    char message[1024];
    char function[64];
    char file[128];
    int line;
} LogRecord;

// 全局日志配置
extern LogConfig g_log_config;
extern pthread_mutex_t g_log_mutex;

// 日志系统函数
int logger_init(const LogConfig *config);
void logger_cleanup();
void logger_set_level(LogLevel level);
void logger_rotate_files();

void log_write(LogLevel level, const char *function, const char *file, 
               int line, const char *format, ...);

// 日志宏
#define LOG_DEBUG(fmt, ...) \
    log_write(LOG_LEVEL_DEBUG, __FUNCTION__, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) \
    log_write(LOG_LEVEL_INFO, __FUNCTION__, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_WARNING(fmt, ...) \
    log_write(LOG_LEVEL_WARNING, __FUNCTION__, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
    log_write(LOG_LEVEL_ERROR, __FUNCTION__, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_FATAL(fmt, ...) \
    log_write(LOG_LEVEL_FATAL, __FUNCTION__, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

// 便捷宏
#define LOG_FUNCTION_ENTRY() LOG_DEBUG("进入函数")
#define LOG_FUNCTION_EXIT() LOG_DEBUG("退出函数")
#define LOG_FUNCTION_EXIT_WITH_RESULT(result) \
    LOG_DEBUG("退出函数，返回值: %d", (result))

#endif // LOGGER_H
```

### 日志系统实现

```c
// logger.c
#include "logger.h"

// 全局变量
LogConfig g_log_config = {
    .min_level = LOG_LEVEL_INFO,
    .target = LOG_TARGET_CONSOLE,
    .log_file = "student_system.log",
    .max_file_size = 10,
    .max_backup_files = 5,
    .enable_timestamp = 1,
    .enable_thread_id = 0,
    .enable_colors = 1
};

pthread_mutex_t g_log_mutex = PTHREAD_MUTEX_INITIALIZER;
static FILE *log_file_handle = NULL;

// 日志级别名称
static const char *level_names[] = {
    "DEBUG", "INFO", "WARNING", "ERROR", "FATAL"
};

// 日志级别颜色
static const char *level_colors[] = {
    "\033[36m",  // DEBUG - 青色
    "\033[32m",  // INFO - 绿色
    "\033[33m",  // WARNING - 黄色
    "\033[31m",  // ERROR - 红色
    "\033[35m"   // FATAL - 紫色
};

#define COLOR_RESET "\033[0m"

// 初始化日志系统
int logger_init(const LogConfig *config) {
    if (config != NULL) {
        g_log_config = *config;
    }
    
    if (g_log_config.target & LOG_TARGET_FILE) {
        log_file_handle = fopen(g_log_config.log_file, "a");
        if (log_file_handle == NULL) {
            fprintf(stderr, "无法打开日志文件: %s\n", g_log_config.log_file);
            return -1;
        }
    }
    
    LOG_INFO("日志系统初始化完成");
    return 0;
}

// 清理日志系统
void logger_cleanup() {
    LOG_INFO("日志系统关闭");
    
    if (log_file_handle != NULL) {
        fclose(log_file_handle);
        log_file_handle = NULL;
    }
}

// 设置日志级别
void logger_set_level(LogLevel level) {
    g_log_config.min_level = level;
    LOG_INFO("日志级别设置为: %s", level_names[level]);
}

// 获取文件大小
long get_file_size(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    
    return size;
}

// 日志文件轮转
void logger_rotate_files() {
    if (log_file_handle != NULL) {
        fclose(log_file_handle);
        log_file_handle = NULL;
    }
    
    // 轮转备份文件
    for (int i = g_log_config.max_backup_files - 1; i > 0; i--) {
        char old_name[300], new_name[300];
        snprintf(old_name, sizeof(old_name), "%s.%d", 
                g_log_config.log_file, i - 1);
        snprintf(new_name, sizeof(new_name), "%s.%d", 
                g_log_config.log_file, i);
        rename(old_name, new_name);
    }
    
    // 重命名当前日志文件
    char backup_name[300];
    snprintf(backup_name, sizeof(backup_name), "%s.0", g_log_config.log_file);
    rename(g_log_config.log_file, backup_name);
    
    // 重新打开日志文件
    log_file_handle = fopen(g_log_config.log_file, "w");
}

// 格式化时间戳
void format_timestamp(char *buffer, size_t size, time_t timestamp) {
    struct tm *tm_info = localtime(&timestamp);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

// 写入日志
void log_write(LogLevel level, const char *function, const char *file, 
               int line, const char *format, ...) {
    // 检查日志级别
    if (level < g_log_config.min_level) {
        return;
    }
    
    pthread_mutex_lock(&g_log_mutex);
    
    // 检查文件大小并轮转
    if (g_log_config.target & LOG_TARGET_FILE && log_file_handle != NULL) {
        long file_size = get_file_size(g_log_config.log_file);
        if (file_size > g_log_config.max_file_size * 1024 * 1024) {
            logger_rotate_files();
        }
    }
    
    // 创建日志记录
    LogRecord record;
    record.level = level;
    record.timestamp = time(NULL);
    record.thread_id = pthread_self();
    record.line = line;
    
    strncpy(record.function, function ? function : "", 
            sizeof(record.function) - 1);
    record.function[sizeof(record.function) - 1] = '\0';
    
    strncpy(record.file, file ? file : "", 
            sizeof(record.file) - 1);
    record.file[sizeof(record.file) - 1] = '\0';
    
    // 格式化消息
    va_list args;
    va_start(args, format);
    vsnprintf(record.message, sizeof(record.message), format, args);
    va_end(args);
    
    // 构建日志行
    char log_line[2048];
    char timestamp_str[64] = "";
    
    if (g_log_config.enable_timestamp) {
        format_timestamp(timestamp_str, sizeof(timestamp_str), record.timestamp);
    }
    
    // 格式化日志行
    if (g_log_config.enable_thread_id) {
        snprintf(log_line, sizeof(log_line), 
                "[%s] [%s] [%lu] %s (%s:%d) - %s\n",
                timestamp_str, level_names[level], 
                (unsigned long)record.thread_id,
                record.function, record.file, record.line, record.message);
    } else {
        snprintf(log_line, sizeof(log_line), 
                "[%s] [%s] %s (%s:%d) - %s\n",
                timestamp_str, level_names[level],
                record.function, record.file, record.line, record.message);
    }
    
    // 输出到控制台
    if (g_log_config.target & LOG_TARGET_CONSOLE) {
        if (g_log_config.enable_colors) {
            printf("%s%s%s", level_colors[level], log_line, COLOR_RESET);
        } else {
            printf("%s", log_line);
        }
        fflush(stdout);
    }
    
    // 输出到文件
    if (g_log_config.target & LOG_TARGET_FILE && log_file_handle != NULL) {
        fprintf(log_file_handle, "%s", log_line);
        fflush(log_file_handle);
    }
    
    pthread_mutex_unlock(&g_log_mutex);
}
```

### 日志系统的使用示例

```c
// 在学生管理系统中使用日志
void demo_logging_usage() {
    // 初始化日志系统
    LogConfig config = {
        .min_level = LOG_LEVEL_DEBUG,
        .target = LOG_TARGET_BOTH,
        .log_file = "student_system.log",
        .max_file_size = 5,
        .max_backup_files = 3,
        .enable_timestamp = 1,
        .enable_thread_id = 0,
        .enable_colors = 1
    };
    
    logger_init(&config);
    
    // 使用日志记录程序运行
    LOG_INFO("学生管理系统启动");
    
    StudentSystem system;
    if (init_system(&system) != 0) {
        LOG_ERROR("系统初始化失败");
        return;
    }
    
    LOG_DEBUG("系统初始化成功，容量: %d", MAX_STUDENTS);
    
    // 记录用户操作
    Student student = {
        .id = "2024001",
        .name = "张三",
        .age = 20,
        .math_score = 85.5,
        .english_score = 92.0,
        .chinese_score = 88.5
    };
    
    LOG_INFO("尝试添加学生: %s (%s)", student.name, student.id);
    
    int result = add_student_with_error_handling(&system, &student);
    if (result == SUCCESS) {
        LOG_INFO("学生添加成功: %s", student.name);
    } else {
        LOG_ERROR("学生添加失败，错误代码: %d", result);
    }
    
    // 记录性能信息
    clock_t start = clock();
    display_all_students(&system);
    clock_t end = clock();
    
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    LOG_DEBUG("显示所有学生耗时: %.3f 秒", cpu_time);
    
    LOG_INFO("学生管理系统关闭");
    logger_cleanup();
}
```

---

## 调试技巧与工具

### 调试宏和工具

```c
// debug.h
#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// 调试开关
#ifdef DEBUG
    #define DEBUG_ENABLED 1
#else
    #define DEBUG_ENABLED 0
#endif

// 调试打印宏
#define DEBUG_PRINT(fmt, ...) \
    do { \
        if (DEBUG_ENABLED) { \
            fprintf(stderr, "[DEBUG] %s:%d %s(): " fmt "\n", \
                    __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
        } \
    } while(0)

// 变量打印宏
#define DEBUG_VAR_INT(var) \
    DEBUG_PRINT(#var " = %d", (var))

#define DEBUG_VAR_FLOAT(var) \
    DEBUG_PRINT(#var " = %.2f", (var))

#define DEBUG_VAR_STRING(var) \
    DEBUG_PRINT(#var " = \"%s\"", (var) ? (var) : "NULL")

#define DEBUG_VAR_POINTER(var) \
    DEBUG_PRINT(#var " = %p", (void*)(var))

// 函数跟踪宏
#define TRACE_FUNCTION() \
    DEBUG_PRINT("函数调用")

#define TRACE_FUNCTION_WITH_ARGS(fmt, ...) \
    DEBUG_PRINT("函数调用: " fmt, ##__VA_ARGS__)

// 断言宏
#define ASSERT_NOT_NULL(ptr) \
    do { \
        if ((ptr) == NULL) { \
            fprintf(stderr, "断言失败: %s 为 NULL (%s:%d)\n", \
                    #ptr, __FILE__, __LINE__); \
            abort(); \
        } \
    } while(0)

#define ASSERT_VALID_RANGE(value, min, max) \
    do { \
        if ((value) < (min) || (value) > (max)) { \
            fprintf(stderr, "断言失败: %s (%d) 不在范围 [%d, %d] 内 (%s:%d)\n", \
                    #value, (value), (min), (max), __FILE__, __LINE__); \
            abort(); \
        } \
    } while(0)

// 内存调试
#ifdef DEBUG
    void* debug_malloc(size_t size, const char *file, int line);
    void debug_free(void *ptr, const char *file, int line);
    void debug_memory_report();
    
    #define MALLOC(size) debug_malloc((size), __FILE__, __LINE__)
    #define FREE(ptr) debug_free((ptr), __FILE__, __LINE__)
    #define MEMORY_REPORT() debug_memory_report()
#else
    #define MALLOC(size) malloc(size)
    #define FREE(ptr) free(ptr)
    #define MEMORY_REPORT()
#endif

// 性能测量
typedef struct {
    clock_t start_time;
    char name[64];
} PerformanceTimer;

#define PERF_TIMER_START(timer, timer_name) \
    do { \
        (timer).start_time = clock(); \
        strncpy((timer).name, (timer_name), sizeof((timer).name) - 1); \
        (timer).name[sizeof((timer).name) - 1] = '\0'; \
        DEBUG_PRINT("性能计时开始: %s", (timer).name); \
    } while(0)

#define PERF_TIMER_END(timer) \
    do { \
        clock_t end_time = clock(); \
        double elapsed = ((double)(end_time - (timer).start_time)) / CLOCKS_PER_SEC; \
        DEBUG_PRINT("性能计时结束: %s, 耗时: %.3f 秒", (timer).name, elapsed); \
    } while(0)

#endif // DEBUG_H
```

### 内存调试实现

```c
// debug.c
#include "debug.h"

#ifdef DEBUG

// 内存分配记录
typedef struct MemoryRecord {
    void *ptr;
    size_t size;
    char file[128];
    int line;
    struct MemoryRecord *next;
} MemoryRecord;

static MemoryRecord *memory_records = NULL;
static size_t total_allocated = 0;
static size_t allocation_count = 0;

// 调试版本的malloc
void* debug_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "内存分配失败: %zu 字节 (%s:%d)\n", size, file, line);
        return NULL;
    }
    
    // 记录分配信息
    MemoryRecord *record = malloc(sizeof(MemoryRecord));
    if (record != NULL) {
        record->ptr = ptr;
        record->size = size;
        strncpy(record->file, file, sizeof(record->file) - 1);
        record->file[sizeof(record->file) - 1] = '\0';
        record->line = line;
        record->next = memory_records;
        memory_records = record;
        
        total_allocated += size;
        allocation_count++;
        
        DEBUG_PRINT("内存分配: %p, 大小: %zu, 总计: %zu 字节, 次数: %zu", 
                   ptr, size, total_allocated, allocation_count);
    }
    
    return ptr;
}

// 调试版本的free
void debug_free(void *ptr, const char *file, int line) {
    if (ptr == NULL) {
        DEBUG_PRINT("尝试释放空指针 (%s:%d)", file, line);
        return;
    }
    
    // 查找并移除记录
    MemoryRecord **current = &memory_records;
    while (*current != NULL) {
        if ((*current)->ptr == ptr) {
            MemoryRecord *to_remove = *current;
            *current = (*current)->next;
            
            total_allocated -= to_remove->size;
            allocation_count--;
            
            DEBUG_PRINT("内存释放: %p, 大小: %zu, 剩余: %zu 字节, 次数: %zu", 
                       ptr, to_remove->size, total_allocated, allocation_count);
            
            free(to_remove);
            free(ptr);
            return;
        }
        current = &(*current)->next;
    }
    
    fprintf(stderr, "警告: 尝试释放未记录的内存: %p (%s:%d)\n", ptr, file, line);
    free(ptr);
}

// 内存使用报告
void debug_memory_report() {
    printf("\n=== 内存使用报告 ===\n");
    printf("当前分配: %zu 字节\n", total_allocated);
    printf("分配次数: %zu\n", allocation_count);
    
    if (memory_records != NULL) {
        printf("\n未释放的内存:\n");
        MemoryRecord *current = memory_records;
        while (current != NULL) {
            printf("  %p: %zu 字节 (%s:%d)\n", 
                   current->ptr, current->size, current->file, current->line);
            current = current->next;
        }
    } else {
        printf("\n所有内存已正确释放\n");
    }
    printf("==================\n\n");
}

#endif // DEBUG
```

### GDB调试技巧

```bash
# 编译时添加调试信息
gcc -g -DDEBUG -o student_management *.c

# 使用GDB调试
gdb ./student_management

# GDB常用命令
(gdb) break main              # 在main函数设置断点
(gdb) break student.c:100     # 在指定文件行设置断点
(gdb) run                     # 运行程序
(gdb) next                    # 单步执行（不进入函数）
(gdb) step                    # 单步执行（进入函数）
(gdb) continue                # 继续执行
(gdb) print variable_name     # 打印变量值
(gdb) info locals             # 显示局部变量
(gdb) backtrace               # 显示调用栈
(gdb) watch variable_name     # 监视变量变化
(gdb) quit                    # 退出GDB
```

### Valgrind内存检测

```bash
# 安装Valgrind（在Linux/macOS上）
# Ubuntu/Debian: sudo apt-get install valgrind
# macOS: brew install valgrind

# 使用Valgrind检测内存泄漏
valgrind --leak-check=full --show-leak-kinds=all ./student_management

# 检测内存错误
valgrind --tool=memcheck ./student_management

# 生成详细报告
valgrind --leak-check=full --log-file=valgrind_report.txt ./student_management
```

---

## 单元测试

### 简单的单元测试框架

```c
// test_framework.h
#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// 测试结果
typedef struct {
    int total_tests;
    int passed_tests;
    int failed_tests;
} TestResults;

// 全局测试结果
extern TestResults g_test_results;

// 测试宏
#define TEST_ASSERT(condition, message) \
    do { \
        g_test_results.total_tests++; \
        if (condition) { \
            g_test_results.passed_tests++; \
            printf("  ✓ %s\n", message); \
        } else { \
            g_test_results.failed_tests++; \
            printf("  ✗ %s (失败: %s:%d)\n", message, __FILE__, __LINE__); \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL_INT(expected, actual, message) \
    TEST_ASSERT((expected) == (actual), message)

#define TEST_ASSERT_EQUAL_FLOAT(expected, actual, tolerance, message) \
    TEST_ASSERT(fabs((expected) - (actual)) < (tolerance), message)

#define TEST_ASSERT_EQUAL_STRING(expected, actual, message) \
    TEST_ASSERT(strcmp((expected), (actual)) == 0, message)

#define TEST_ASSERT_NOT_NULL(ptr, message) \
    TEST_ASSERT((ptr) != NULL, message)

#define TEST_ASSERT_NULL(ptr, message) \
    TEST_ASSERT((ptr) == NULL, message)

// 测试套件宏
#define TEST_SUITE_BEGIN(suite_name) \
    printf("\n=== 测试套件: %s ===\n", suite_name); \
    TestResults suite_start = g_test_results;

#define TEST_SUITE_END(suite_name) \
    do { \
        int suite_total = g_test_results.total_tests - suite_start.total_tests; \
        int suite_passed = g_test_results.passed_tests - suite_start.passed_tests; \
        int suite_failed = g_test_results.failed_tests - suite_start.failed_tests; \
        printf("\n%s 结果: %d/%d 通过\n", suite_name, suite_passed, suite_total); \
        if (suite_failed > 0) { \
            printf("  %d 个测试失败\n", suite_failed); \
        } \
    } while(0)

// 测试函数类型
typedef void (*TestFunction)(void);

// 测试运行器
void run_test(const char *test_name, TestFunction test_func);
void run_all_tests();
void print_test_summary();

#endif // TEST_FRAMEWORK_H
```

### 单元测试实现

```c
// test_framework.c
#include "test_framework.h"

// 全局测试结果
TestResults g_test_results = {0, 0, 0};

// 运行单个测试
void run_test(const char *test_name, TestFunction test_func) {
    printf("\n--- 运行测试: %s ---\n", test_name);
    TestResults test_start = g_test_results;
    
    test_func();
    
    int test_total = g_test_results.total_tests - test_start.total_tests;
    int test_passed = g_test_results.passed_tests - test_start.passed_tests;
    int test_failed = g_test_results.failed_tests - test_start.failed_tests;
    
    if (test_failed == 0) {
        printf("测试 %s: 通过 (%d/%d)\n", test_name, test_passed, test_total);
    } else {
        printf("测试 %s: 失败 (%d/%d 通过, %d 失败)\n", 
               test_name, test_passed, test_total, test_failed);
    }
}

// 打印测试总结
void print_test_summary() {
    printf("\n" "=" * 50 "\n");
    printf("测试总结:\n");
    printf("  总计: %d\n", g_test_results.total_tests);
    printf("  通过: %d\n", g_test_results.passed_tests);
    printf("  失败: %d\n", g_test_results.failed_tests);
    
    if (g_test_results.failed_tests == 0) {
        printf("\n🎉 所有测试通过！\n");
    } else {
        printf("\n❌ 有 %d 个测试失败\n", g_test_results.failed_tests);
    }
    printf("" "=" * 50 "\n");
}
```

### 学生管理系统的单元测试

```c
// test_student_system.c
#include "test_framework.h"
#include "student.h"
#include "error_codes.h"

// 测试系统初始化
void test_system_initialization() {
    StudentSystem system;
    int result = init_system(&system);
    
    TEST_ASSERT_EQUAL_INT(SUCCESS, result, "系统初始化应该成功");
    TEST_ASSERT_EQUAL_INT(0, system.count, "初始学生数量应该为0");
    TEST_ASSERT_NOT_NULL(system.students, "学生数组不应该为NULL");
}

// 测试添加学生
void test_add_student() {
    StudentSystem system;
    init_system(&system);
    
    Student student = {
        .id = "2024001",
        .name = "张三",
        .age = 20,
        .math_score = 85.5,
        .english_score = 92.0,
        .chinese_score = 88.5
    };
    calculate_average(&student);
    
    int result = add_student_with_error_handling(&system, &student);
    
    TEST_ASSERT_EQUAL_INT(SUCCESS, result, "添加有效学生应该成功");
    TEST_ASSERT_EQUAL_INT(1, system.count, "添加后学生数量应该为1");
    TEST_ASSERT_EQUAL_STRING("张三", system.students[0].name, "学生姓名应该正确");
    TEST_ASSERT_EQUAL_STRING("2024001", system.students[0].id, "学生学号应该正确");
}

// 测试添加重复学生
void test_add_duplicate_student() {
    StudentSystem system;
    init_system(&system);
    
    Student student1 = {
        .id = "2024001",
        .name = "张三",
        .age = 20,
        .math_score = 85.5,
        .english_score = 92.0,
        .chinese_score = 88.5
    };
    
    Student student2 = {
        .id = "2024001",  // 相同学号
        .name = "李四",
        .age = 21,
        .math_score = 90.0,
        .english_score = 85.0,
        .chinese_score = 92.0
    };
    
    add_student_with_error_handling(&system, &student1);
    int result = add_student_with_error_handling(&system, &student2);
    
    TEST_ASSERT_EQUAL_INT(ERROR_STUDENT_EXISTS, result, "添加重复学号的学生应该失败");
    TEST_ASSERT_EQUAL_INT(1, system.count, "学生数量应该仍为1");
    TEST_ASSERT_EQUAL_STRING("张三", system.students[0].name, "第一个学生应该保留");
}

// 测试查找学生
void test_find_student() {
    StudentSystem system;
    init_system(&system);
    
    Student student1 = {"2024001", "张三", 20, 85.5, 92.0, 88.5, 0};
    Student student2 = {"2024002", "李四", 21, 90.0, 85.0, 92.0, 0};
    
    calculate_average(&student1);
    calculate_average(&student2);
    
    add_student_with_error_handling(&system, &student1);
    add_student_with_error_handling(&system, &student2);
    
    int index = find_student_by_id(&system, "2024002");
    TEST_ASSERT_EQUAL_INT(1, index, "应该找到第二个学生");
    
    index = find_student_by_id(&system, "2024003");
    TEST_ASSERT_EQUAL_INT(-1, index, "不存在的学号应该返回-1");
}

// 测试删除学生
void test_delete_student() {
    StudentSystem system;
    init_system(&system);
    
    Student student1 = {"2024001", "张三", 20, 85.5, 92.0, 88.5, 0};
    Student student2 = {"2024002", "李四", 21, 90.0, 85.0, 92.0, 0};
    
    calculate_average(&student1);
    calculate_average(&student2);
    
    add_student_with_error_handling(&system, &student1);
    add_student_with_error_handling(&system, &student2);
    
    int result = delete_student_by_id(&system, "2024001");
    TEST_ASSERT_EQUAL_INT(SUCCESS, result, "删除存在的学生应该成功");
    TEST_ASSERT_EQUAL_INT(1, system.count, "删除后学生数量应该为1");
    TEST_ASSERT_EQUAL_STRING("2024002", system.students[0].id, "剩余学生应该是李四");
    
    result = delete_student_by_id(&system, "2024003");
    TEST_ASSERT_EQUAL_INT(ERROR_STUDENT_NOT_FOUND, result, "删除不存在的学生应该失败");
}

// 运行所有测试
void run_student_system_tests() {
    TEST_SUITE_BEGIN("学生管理系统测试");
    
    run_test("系统初始化", test_system_initialization);
    run_test("添加学生", test_add_student);
    run_test("添加重复学生", test_add_duplicate_student);
    run_test("查找学生", test_find_student);
    run_test("删除学生", test_delete_student);
    
    TEST_SUITE_END("学生管理系统测试");
}

// 主测试函数
int main() {
    printf("开始运行单元测试...\n");
    
    run_student_system_tests();
    
    print_test_summary();
    return g_test_results.failed_tests > 0 ? 1 : 0;
}
```

---

## 内存管理与检测

### 内存泄漏检测

```c
// memory_check.h
#ifndef MEMORY_CHECK_H
#define MEMORY_CHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 内存分配跟踪结构
typedef struct MemoryAllocation {
    void *ptr;
    size_t size;
    char file[128];
    int line;
    struct MemoryAllocation *next;
} MemoryAllocation;

// 内存统计
typedef struct {
    size_t total_allocated;
    size_t total_freed;
    size_t current_usage;
    size_t peak_usage;
    size_t allocation_count;
    size_t free_count;
} MemoryStats;

// 内存检测函数
void memory_check_init();
void memory_check_cleanup();
void* memory_check_malloc(size_t size, const char *file, int line);
void memory_check_free(void *ptr, const char *file, int line);
void memory_check_report();
MemoryStats memory_check_get_stats();

// 替换标准内存函数的宏
#ifdef MEMORY_CHECK
    #define malloc(size) memory_check_malloc((size), __FILE__, __LINE__)
    #define free(ptr) memory_check_free((ptr), __FILE__, __LINE__)
    #define MEMORY_REPORT() memory_check_report()
#else
    #define MEMORY_REPORT()
#endif

#endif // MEMORY_CHECK_H
```

### 内存检测实现

```c
// memory_check.c
#include "memory_check.h"

// 全局变量
static MemoryAllocation *allocations = NULL;
static MemoryStats stats = {0};

// 初始化内存检测
void memory_check_init() {
    allocations = NULL;
    memset(&stats, 0, sizeof(MemoryStats));
    printf("内存检测系统已初始化\n");
}

// 清理内存检测
void memory_check_cleanup() {
    memory_check_report();
    
    // 释放内存分配记录
    MemoryAllocation *current = allocations;
    while (current != NULL) {
        MemoryAllocation *next = current->next;
        free(current);
        current = next;
    }
    
    allocations = NULL;
    printf("内存检测系统已清理\n");
}

// 分配内存并跟踪
void* memory_check_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "内存分配失败: %zu 字节 (%s:%d)\n", size, file, line);
        return NULL;
    }
    
    // 创建分配记录
    MemoryAllocation *record = malloc(sizeof(MemoryAllocation));
    if (record == NULL) {
        fprintf(stderr, "无法为内存跟踪分配内存 (%s:%d)\n", file, line);
        return ptr;
    }
    
    // 初始化记录
    record->ptr = ptr;
    record->size = size;
    strncpy(record->file, file, sizeof(record->file) - 1);
    record->file[sizeof(record->file) - 1] = '\0';
    record->line = line;
    
    // 添加到链表
    record->next = allocations;
    allocations = record;
    
    // 更新统计信息
    stats.total_allocated += size;
    stats.current_usage += size;
    stats.allocation_count++;
    
    if (stats.current_usage > stats.peak_usage) {
        stats.peak_usage = stats.current_usage;
    }
    
    return ptr;
}

// 释放内存并跟踪
void memory_check_free(void *ptr, const char *file, int line) {
    if (ptr == NULL) {
        fprintf(stderr, "尝试释放空指针 (%s:%d)\n", file, line);
        return;
    }
    
    // 查找分配记录
    MemoryAllocation **current = &allocations;
    while (*current != NULL) {
        if ((*current)->ptr == ptr) {
            // 找到记录
            MemoryAllocation *record = *current;
            *current = record->next;
            
            // 更新统计信息
            stats.total_freed += record->size;
            stats.current_usage -= record->size;
            stats.free_count++;
            
            // 释放记录和指针
            free(record);
            free(ptr);
            return;
        }
        current = &(*current)->next;
    }
    
    fprintf(stderr, "尝试释放未分配的内存: %p (%s:%d)\n", ptr, file, line);
    free(ptr);
}

// 生成内存报告
void memory_check_report() {
    printf("\n=== 内存使用报告 ===\n");
    printf("总分配: %zu 字节 (%zu 次)\n", stats.total_allocated, stats.allocation_count);
    printf("总释放: %zu 字节 (%zu 次)\n", stats.total_freed, stats.free_count);
    printf("当前使用: %zu 字节\n", stats.current_usage);
    printf("峰值使用: %zu 字节\n", stats.peak_usage);
    
    if (stats.current_usage > 0) {
        printf("\n未释放的内存:\n");
        int count = 0;
        MemoryAllocation *current = allocations;
        
        while (current != NULL) {
            printf("  %p: %zu 字节 (%s:%d)\n", 
                   current->ptr, current->size, current->file, current->line);
            current = current->next;
            count++;
        }
        
        printf("\n总计 %d 处内存泄漏，共 %zu 字节\n", count, stats.current_usage);
    } else {
        printf("\n没有内存泄漏\n");
    }
    
    printf("==================\n");
}

// 获取内存统计信息
MemoryStats memory_check_get_stats() {
    return stats;
}
```

---

## 实践练习

### 练习1：实现自定义断言宏

```c
// 练习：实现自定义断言宏
// 要求：
// 1. 创建一个ASSERT_STUDENT_VALID宏，用于验证学生数据的有效性
// 2. 宏应该检查学号、姓名、年龄和成绩的有效性
// 3. 如果验证失败，应该打印详细的错误信息

#define ASSERT_STUDENT_VALID(student) \
    /* TODO: 实现学生数据验证断言 */
```

### 练习2：实现错误恢复机制

```c
// 练习：实现错误恢复机制
// 要求：
// 1. 修改文件操作函数，添加自动恢复功能
// 2. 当文件打开失败时，尝试从备份文件恢复
// 3. 实现一个事务系统，确保数据操作的原子性

int save_with_recovery(StudentSystem *system) {
    // TODO: 实现带恢复机制的保存函数
}
```

### 练习3：实现完整的单元测试

```c
// 练习：为学生管理系统实现完整的单元测试
// 要求：
// 1. 为每个核心功能编写测试用例
// 2. 测试边界条件和错误情况
// 3. 实现测试夹具(fixture)来简化测试设置

// TODO: 实现测试夹具和完整的测试用例
```

---

## 💡 本课要点总结

### 错误处理
1. **错误代码系统**: 统一的错误代码和消息
2. **错误检测**: 及早检测错误并提供明确信息
3. **资源管理**: 确保在错误情况下释放资源

### 异常处理
1. **setjmp/longjmp**: C语言中模拟异常处理
2. **异常上下文**: 管理异常信息和处理流程
3. **嵌套异常**: 处理多层函数调用中的异常

### 日志系统
1. **日志级别**: 不同严重程度的日志分类
2. **日志格式**: 时间戳、位置、消息等信息
3. **日志轮转**: 管理日志文件大小和备份

### 调试技巧
1. **调试宏**: 条件编译和调试信息输出
2. **内存检测**: 跟踪内存分配和释放
3. **性能测量**: 记录函数执行时间

### 单元测试
1. **测试框架**: 简单的测试断言和报告
2. **测试用例**: 针对各个功能的测试
3. **测试套件**: 组织和管理测试用例

---

## 🎯 下一步学习

在下一课中，我们将学习：
- **第9课：高级功能扩展** - 如何为学生管理系统添加高级功能
- 数据分析与统计
- 导入导出功能
- 用户权限管理
- 网络功能

---

## 📚 扩展阅读

- C语言错误处理最佳实践
- 内存泄漏检测工具使用
- 单元测试框架比较
- 日志系统设计模式

---

**上一课**: [第7课：用户界面设计](07-user-interface.md)  
**下一课**: [第9课：高级功能扩展](09-advanced-features.md)  
**返回**: [教程目录](README.md)