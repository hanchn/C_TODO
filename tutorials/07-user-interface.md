# 第7课：用户界面设计

> **学习目标**: 学习如何设计友好、直观的命令行用户界面

## 📋 本课内容

1. [用户界面设计原则](#用户界面设计原则)
2. [菜单系统设计](#菜单系统设计)
3. [输入验证与处理](#输入验证与处理)
4. [信息显示与格式化](#信息显示与格式化)
5. [错误处理与用户反馈](#错误处理与用户反馈)
6. [交互式操作设计](#交互式操作设计)
7. [界面美化技巧](#界面美化技巧)
8. [实践练习](#实践练习)

---

## 用户界面设计原则

### 良好UI的特征

1. **直观性**: 用户无需学习就能理解
2. **一致性**: 相同操作在不同地方表现一致
3. **反馈性**: 及时给用户操作反馈
4. **容错性**: 能处理用户的错误输入
5. **效率性**: 减少用户的操作步骤

### 命令行界面设计对比

```c
// 糟糕的界面设计
void bad_menu() {
    printf("1 2 3 4 5 6 7 8 9 0\n");  // 没有说明
    printf("?");  // 提示不明确
    int choice;
    scanf("%d", &choice);  // 没有输入验证
    // 没有错误处理
}

// 良好的界面设计
void good_menu() {
    printf("\n" BLUE "=== 学生管理系统 ===" RESET "\n");
    printf("\n");
    printf("  " GREEN "1." RESET " 添加学生信息\n");
    printf("  " GREEN "2." RESET " 删除学生信息\n");
    printf("  " GREEN "3." RESET " 修改学生信息\n");
    printf("  " GREEN "4." RESET " 查找学生信息\n");
    printf("  " GREEN "5." RESET " 显示所有学生\n");
    printf("  " GREEN "6." RESET " 统计分析\n");
    printf("  " GREEN "0." RESET " 退出系统\n");
    printf("\n");
    printf("请选择操作 (0-6): ");
    
    int choice;
    while (!safe_input_int(&choice, 0, 6)) {
        show_error("请输入有效的选项 (0-6)");
        printf("请选择操作 (0-6): ");
    }
}
```

### 用户体验设计原则

```c
// 用户体验设计要点
typedef struct {
    char title[100];           // 界面标题
    char description[200];     // 功能描述
    int min_choice;           // 最小选择值
    int max_choice;           // 最大选择值
    char prompt[50];          // 输入提示
    char error_msg[100];      // 错误信息
} MenuConfig;

// 配置化的菜单显示
void display_configured_menu(MenuConfig *config) {
    // 清屏
    clear_screen();
    
    // 显示标题
    print_title(config->title);
    
    // 显示描述
    if (strlen(config->description) > 0) {
        printf("\n%s\n", config->description);
    }
    
    // 显示选项
    print_menu_options(config->min_choice, config->max_choice);
    
    // 显示提示
    printf("\n%s", config->prompt);
}
```

---

## 菜单系统设计

### 多级菜单结构

```c
// ui.c
#include "ui.h"

// ANSI颜色代码
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

// 菜单项结构
typedef struct {
    int id;
    char title[50];
    char description[100];
    void (*action)(StudentSystem *system);
} MenuItem;

// 主菜单
void display_main_menu() {
    clear_screen();
    
    // 显示系统标题
    printf("\n");
    printf("  " CYAN BOLD "╔══════════════════════════════════════╗" RESET "\n");
    printf("  " CYAN BOLD "║" RESET "        " BLUE BOLD "学生管理系统 v1.0" RESET "        " CYAN BOLD "║" RESET "\n");
    printf("  " CYAN BOLD "╚══════════════════════════════════════╝" RESET "\n");
    printf("\n");
    
    // 显示菜单选项
    printf("  " YELLOW "┌─ 数据管理 ─────────────────────────┐" RESET "\n");
    printf("  " YELLOW "│" RESET "  " GREEN "1." RESET " 添加学生信息                    " YELLOW "│" RESET "\n");
    printf("  " YELLOW "│" RESET "  " GREEN "2." RESET " 删除学生信息                    " YELLOW "│" RESET "\n");
    printf("  " YELLOW "│" RESET "  " GREEN "3." RESET " 修改学生信息                    " YELLOW "│" RESET "\n");
    printf("  " YELLOW "│" RESET "  " GREEN "4." RESET " 查找学生信息                    " YELLOW "│" RESET "\n");
    printf("  " YELLOW "└────────────────────────────────────┘" RESET "\n");
    printf("\n");
    
    printf("  " YELLOW "┌─ 数据展示 ─────────────────────────┐" RESET "\n");
    printf("  " YELLOW "│" RESET "  " GREEN "5." RESET " 显示所有学生                    " YELLOW "│" RESET "\n");
    printf("  " YELLOW "│" RESET "  " GREEN "6." RESET " 排序显示                        " YELLOW "│" RESET "\n");
    printf("  " YELLOW "│" RESET "  " GREEN "7." RESET " 统计分析                        " YELLOW "│" RESET "\n");
    printf("  " YELLOW "└────────────────────────────────────┘" RESET "\n");
    printf("\n");
    
    printf("  " YELLOW "┌─ 文件操作 ─────────────────────────┐" RESET "\n");
    printf("  " YELLOW "│" RESET "  " GREEN "8." RESET " 保存数据                        " YELLOW "│" RESET "\n");
    printf("  " YELLOW "│" RESET "  " GREEN "9." RESET " 加载数据                        " YELLOW "│" RESET "\n");
    printf("  " YELLOW "│" RESET "  " GREEN "10." RESET " 导入/导出                      " YELLOW "│" RESET "\n");
    printf("  " YELLOW "└────────────────────────────────────┘" RESET "\n");
    printf("\n");
    
    printf("  " RED "0." RESET " 退出系统\n");
    printf("\n");
}

// 子菜单：查找菜单
void display_search_menu() {
    clear_screen();
    
    printf("\n");
    printf("  " CYAN BOLD "=== 查找学生信息 ===" RESET "\n");
    printf("\n");
    
    printf("  " GREEN "1." RESET " 按学号查找\n");
    printf("  " GREEN "2." RESET " 按姓名查找\n");
    printf("  " GREEN "3." RESET " 按成绩范围查找\n");
    printf("  " GREEN "4." RESET " 按年龄范围查找\n");
    printf("  " GREEN "5." RESET " 高级组合查找\n");
    printf("  " RED "0." RESET " 返回主菜单\n");
    printf("\n");
}

// 子菜单：排序菜单
void display_sort_menu() {
    clear_screen();
    
    printf("\n");
    printf("  " CYAN BOLD "=== 排序显示 ===" RESET "\n");
    printf("\n");
    
    printf("  " GREEN "1." RESET " 按平均分排序（降序）\n");
    printf("  " GREEN "2." RESET " 按平均分排序（升序）\n");
    printf("  " GREEN "3." RESET " 按学号排序\n");
    printf("  " GREEN "4." RESET " 按姓名排序\n");
    printf("  " GREEN "5." RESET " 按年龄排序\n");
    printf("  " GREEN "6." RESET " 自定义排序\n");
    printf("  " RED "0." RESET " 返回主菜单\n");
    printf("\n");
}

// 子菜单：导入导出菜单
void display_import_export_menu() {
    clear_screen();
    
    printf("\n");
    printf("  " CYAN BOLD "=== 导入/导出数据 ===" RESET "\n");
    printf("\n");
    
    printf("  " YELLOW "导出功能:" RESET "\n");
    printf("  " GREEN "1." RESET " 导出为CSV文件\n");
    printf("  " GREEN "2." RESET " 导出为JSON文件\n");
    printf("  " GREEN "3." RESET " 导出为文本文件\n");
    printf("\n");
    
    printf("  " YELLOW "导入功能:" RESET "\n");
    printf("  " GREEN "4." RESET " 从CSV文件导入\n");
    printf("  " GREEN "5." RESET " 从备份文件导入\n");
    printf("\n");
    
    printf("  " YELLOW "备份功能:" RESET "\n");
    printf("  " GREEN "6." RESET " 创建数据备份\n");
    printf("  " GREEN "7." RESET " 恢复数据备份\n");
    printf("  " GREEN "8." RESET " 查看备份列表\n");
    printf("\n");
    
    printf("  " RED "0." RESET " 返回主菜单\n");
    printf("\n");
}

// 菜单导航系统
int navigate_menu(int menu_type, StudentSystem *system) {
    int choice;
    
    switch (menu_type) {
        case MENU_MAIN:
            display_main_menu();
            printf("请选择操作 (0-10): ");
            if (safe_input_int(&choice, 0, 10)) {
                return choice;
            }
            break;
            
        case MENU_SEARCH:
            display_search_menu();
            printf("请选择查找方式 (0-5): ");
            if (safe_input_int(&choice, 0, 5)) {
                return choice;
            }
            break;
            
        case MENU_SORT:
            display_sort_menu();
            printf("请选择排序方式 (0-6): ");
            if (safe_input_int(&choice, 0, 6)) {
                return choice;
            }
            break;
            
        case MENU_IMPORT_EXPORT:
            display_import_export_menu();
            printf("请选择操作 (0-8): ");
            if (safe_input_int(&choice, 0, 8)) {
                return choice;
            }
            break;
    }
    
    show_error("无效的选择，请重试");
    wait_for_key();
    return -1;
}
```

### 面包屑导航

```c
// 导航路径管理
typedef struct {
    char path[10][50];  // 最多10级菜单
    int depth;          // 当前深度
} NavigationPath;

NavigationPath nav_path = {{0}, 0};

// 添加导航路径
void push_navigation(const char *menu_name) {
    if (nav_path.depth < 9) {
        strcpy(nav_path.path[nav_path.depth], menu_name);
        nav_path.depth++;
    }
}

// 返回上级菜单
void pop_navigation() {
    if (nav_path.depth > 0) {
        nav_path.depth--;
        nav_path.path[nav_path.depth][0] = '\0';
    }
}

// 显示导航路径
void display_breadcrumb() {
    if (nav_path.depth == 0) {
        return;
    }
    
    printf("  " CYAN "导航: " RESET);
    for (int i = 0; i < nav_path.depth; i++) {
        printf("%s", nav_path.path[i]);
        if (i < nav_path.depth - 1) {
            printf(" " CYAN ">" RESET " ");
        }
    }
    printf("\n\n");
}

// 使用示例
void search_menu_handler(StudentSystem *system) {
    push_navigation("查找学生");
    
    while (1) {
        display_breadcrumb();
        int choice = navigate_menu(MENU_SEARCH, system);
        
        if (choice == 0) {
            break;  // 返回主菜单
        }
        
        // 处理查找选择
        handle_search_choice(choice, system);
    }
    
    pop_navigation();
}
```

---

## 输入验证与处理

### 安全输入函数

```c
// 安全的字符串输入
void safe_input_string(char *buffer, int size) {
    if (buffer == NULL || size <= 0) {
        return;
    }
    
    // 清空缓冲区
    memset(buffer, 0, size);
    
    // 读取输入
    if (fgets(buffer, size, stdin) != NULL) {
        // 移除换行符
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        // 如果输入过长，清空输入缓冲区
        if (strlen(buffer) == size - 1 && buffer[size - 2] != '\n') {
            clear_input_buffer();
        }
    }
}

// 安全的整数输入
int safe_input_int(int *value, int min, int max) {
    char input[20];
    char *endptr;
    
    safe_input_string(input, sizeof(input));
    
    // 检查是否为空
    if (strlen(input) == 0) {
        return 0;
    }
    
    // 转换为整数
    long temp = strtol(input, &endptr, 10);
    
    // 检查转换是否成功
    if (*endptr != '\0') {
        return 0;  // 包含非数字字符
    }
    
    // 检查范围
    if (temp < min || temp > max) {
        return 0;
    }
    
    *value = (int)temp;
    return 1;
}

// 安全的浮点数输入
int safe_input_float(float *value, float min, float max) {
    char input[20];
    char *endptr;
    
    safe_input_string(input, sizeof(input));
    
    if (strlen(input) == 0) {
        return 0;
    }
    
    float temp = strtof(input, &endptr);
    
    if (*endptr != '\0') {
        return 0;
    }
    
    if (temp < min || temp > max) {
        return 0;
    }
    
    *value = temp;
    return 1;
}

// 带提示的输入函数
int input_with_prompt(const char *prompt, const char *error_msg, 
                     int (*validator)(const char *), char *result, int size) {
    int attempts = 0;
    const int max_attempts = 3;
    
    while (attempts < max_attempts) {
        printf("%s", prompt);
        safe_input_string(result, size);
        
        if (validator(result)) {
            return 1;  // 输入有效
        }
        
        attempts++;
        if (attempts < max_attempts) {
            printf("  " RED "错误: %s" RESET "\n", error_msg);
            printf("  还有 %d 次机会\n\n", max_attempts - attempts);
        }
    }
    
    printf("  " RED "输入失败次数过多，操作取消" RESET "\n");
    return 0;
}

// 输入验证器示例
int validate_student_id(const char *id) {
    if (strlen(id) < 6 || strlen(id) > 12) {
        return 0;
    }
    
    // 检查格式：前4位数字 + 后面字母数字
    for (int i = 0; i < 4; i++) {
        if (!isdigit(id[i])) {
            return 0;
        }
    }
    
    for (int i = 4; id[i]; i++) {
        if (!isalnum(id[i])) {
            return 0;
        }
    }
    
    return 1;
}

int validate_student_name(const char *name) {
    if (strlen(name) < 2 || strlen(name) > 20) {
        return 0;
    }
    
    // 检查是否包含数字或特殊字符
    for (int i = 0; name[i]; i++) {
        if (isdigit(name[i]) || ispunct(name[i])) {
            return 0;
        }
    }
    
    return 1;
}
```

### 智能输入提示

```c
// 输入提示系统
typedef struct {
    char *suggestions[10];
    int count;
} InputSuggestions;

// 为学号输入提供建议
void suggest_student_id(StudentSystem *system, InputSuggestions *suggestions) {
    suggestions->count = 0;
    
    // 生成建议的学号格式
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    
    char suggestion[20];
    snprintf(suggestion, sizeof(suggestion), "%04d%04d", 
             tm_info->tm_year + 1900, system->count + 1);
    
    suggestions->suggestions[suggestions->count] = strdup(suggestion);
    suggestions->count++;
    
    // 添加更多建议...
}

// 显示输入建议
void display_input_suggestions(InputSuggestions *suggestions) {
    if (suggestions->count == 0) {
        return;
    }
    
    printf("  " CYAN "建议:" RESET "\n");
    for (int i = 0; i < suggestions->count; i++) {
        printf("    %d. %s\n", i + 1, suggestions->suggestions[i]);
    }
    printf("\n");
}

// 智能输入函数
int smart_input_student_id(StudentSystem *system, char *id, int size) {
    InputSuggestions suggestions;
    suggest_student_id(system, &suggestions);
    
    printf("请输入学号 (6-12位，格式：年份+编号):\n");
    display_input_suggestions(&suggestions);
    
    return input_with_prompt(
        "学号: ",
        "学号格式无效，应为6-12位字母数字组合",
        validate_student_id,
        id,
        size
    );
}
```

---

## 信息显示与格式化

### 表格显示系统

```c
// 表格配置
typedef struct {
    char headers[10][20];  // 表头
    int widths[10];        // 列宽
    int column_count;      // 列数
    char title[50];        // 表格标题
} TableConfig;

// 创建学生信息表格配置
TableConfig create_student_table_config() {
    TableConfig config;
    
    strcpy(config.title, "学生信息列表");
    config.column_count = 8;
    
    strcpy(config.headers[0], "序号");
    strcpy(config.headers[1], "学号");
    strcpy(config.headers[2], "姓名");
    strcpy(config.headers[3], "年龄");
    strcpy(config.headers[4], "数学");
    strcpy(config.headers[5], "英语");
    strcpy(config.headers[6], "语文");
    strcpy(config.headers[7], "平均分");
    
    config.widths[0] = 4;   // 序号
    config.widths[1] = 10;  // 学号
    config.widths[2] = 12;  // 姓名
    config.widths[3] = 4;   // 年龄
    config.widths[4] = 6;   // 数学
    config.widths[5] = 6;   // 英语
    config.widths[6] = 6;   // 语文
    config.widths[7] = 8;   // 平均分
    
    return config;
}

// 打印表格边框
void print_table_border(TableConfig *config, char corner, char horizontal, char vertical) {
    printf("  %c", corner);
    
    for (int i = 0; i < config->column_count; i++) {
        for (int j = 0; j < config->widths[i]; j++) {
            printf("%c", horizontal);
        }
        if (i < config->column_count - 1) {
            printf("%c", vertical);
        }
    }
    
    printf("%c\n", corner);
}

// 打印表格头部
void print_table_header(TableConfig *config) {
    // 打印标题
    if (strlen(config->title) > 0) {
        printf("\n  " BOLD CYAN "%s" RESET "\n");
    }
    
    // 打印顶部边框
    print_table_border(config, '+', '-', '+');
    
    // 打印表头
    printf("  " BOLD "|");
    for (int i = 0; i < config->column_count; i++) {
        printf("%-*s", config->widths[i], config->headers[i]);
        printf("|");
    }
    printf(RESET "\n");
    
    // 打印分隔线
    print_table_border(config, '+', '-', '+');
}

// 打印表格行
void print_table_row(TableConfig *config, char *data[]) {
    printf("  |");
    for (int i = 0; i < config->column_count; i++) {
        printf("%-*s", config->widths[i], data[i]);
        printf("|");
    }
    printf("\n");
}

// 打印表格底部
void print_table_footer(TableConfig *config) {
    print_table_border(config, '+', '-', '+');
}

// 显示学生信息表格
void display_students_table(StudentSystem *system) {
    if (system->count == 0) {
        show_info("暂无学生信息");
        return;
    }
    
    TableConfig config = create_student_table_config();
    print_table_header(&config);
    
    for (int i = 0; i < system->count; i++) {
        Student *s = &system->students[i];
        
        char *row_data[8];
        char index_str[10], age_str[10];
        char math_str[10], english_str[10], chinese_str[10], avg_str[10];
        
        snprintf(index_str, sizeof(index_str), "%d", i + 1);
        snprintf(age_str, sizeof(age_str), "%d", s->age);
        snprintf(math_str, sizeof(math_str), "%.1f", s->math_score);
        snprintf(english_str, sizeof(english_str), "%.1f", s->english_score);
        snprintf(chinese_str, sizeof(chinese_str), "%.1f", s->chinese_score);
        snprintf(avg_str, sizeof(avg_str), "%.1f", s->average_score);
        
        row_data[0] = index_str;
        row_data[1] = s->id;
        row_data[2] = s->name;
        row_data[3] = age_str;
        row_data[4] = math_str;
        row_data[5] = english_str;
        row_data[6] = chinese_str;
        row_data[7] = avg_str;
        
        // 根据成绩着色
        if (s->average_score >= 90) {
            printf("  " GREEN);
        } else if (s->average_score >= 80) {
            printf("  " YELLOW);
        } else if (s->average_score < 60) {
            printf("  " RED);
        } else {
            printf("  ");
        }
        
        print_table_row(&config, row_data);
        printf(RESET);
    }
    
    print_table_footer(&config);
    
    // 显示统计信息
    printf("\n  " CYAN "总计: %d 个学生" RESET "\n", system->count);
}
```

### 分页显示

```c
// 分页显示配置
typedef struct {
    int page_size;      // 每页显示数量
    int current_page;   // 当前页码
    int total_items;    // 总项目数
    int total_pages;    // 总页数
} PaginationConfig;

// 初始化分页
PaginationConfig init_pagination(int total_items, int page_size) {
    PaginationConfig config;
    config.page_size = page_size;
    config.current_page = 1;
    config.total_items = total_items;
    config.total_pages = (total_items + page_size - 1) / page_size;
    return config;
}

// 显示分页信息
void display_pagination_info(PaginationConfig *config) {
    printf("\n  " CYAN "第 %d/%d 页，共 %d 条记录" RESET "\n", 
           config->current_page, config->total_pages, config->total_items);
    
    if (config->total_pages > 1) {
        printf("  操作: ");
        if (config->current_page > 1) {
            printf("[P]上一页 ");
        }
        if (config->current_page < config->total_pages) {
            printf("[N]下一页 ");
        }
        printf("[G]跳转 [Q]退出\n");
    }
}

// 分页显示学生信息
void display_students_paginated(StudentSystem *system) {
    const int page_size = 10;
    PaginationConfig pagination = init_pagination(system->count, page_size);
    
    while (1) {
        clear_screen();
        
        // 计算当前页的显示范围
        int start_index = (pagination.current_page - 1) * page_size;
        int end_index = start_index + page_size;
        if (end_index > system->count) {
            end_index = system->count;
        }
        
        // 显示当前页的学生
        TableConfig config = create_student_table_config();
        snprintf(config.title, sizeof(config.title), 
                "学生信息列表 (第 %d/%d 页)", 
                pagination.current_page, pagination.total_pages);
        
        print_table_header(&config);
        
        for (int i = start_index; i < end_index; i++) {
            // 显示学生信息（代码同上）
        }
        
        print_table_footer(&config);
        display_pagination_info(&pagination);
        
        // 处理分页操作
        if (pagination.total_pages <= 1) {
            wait_for_key();
            break;
        }
        
        char choice;
        printf("\n请选择操作: ");
        scanf(" %c", &choice);
        clear_input_buffer();
        
        switch (tolower(choice)) {
            case 'p':
                if (pagination.current_page > 1) {
                    pagination.current_page--;
                }
                break;
            case 'n':
                if (pagination.current_page < pagination.total_pages) {
                    pagination.current_page++;
                }
                break;
            case 'g': {
                int page;
                printf("请输入页码 (1-%d): ", pagination.total_pages);
                if (safe_input_int(&page, 1, pagination.total_pages)) {
                    pagination.current_page = page;
                }
                break;
            }
            case 'q':
                return;
        }
    }
}
```

---

## 错误处理与用户反馈

### 统一的消息显示系统

```c
// 消息类型
typedef enum {
    MSG_INFO,
    MSG_SUCCESS,
    MSG_WARNING,
    MSG_ERROR
} MessageType;

// 显示消息
void show_message(MessageType type, const char *message) {
    const char *prefix;
    const char *color;
    
    switch (type) {
        case MSG_INFO:
            prefix = "信息";
            color = CYAN;
            break;
        case MSG_SUCCESS:
            prefix = "成功";
            color = GREEN;
            break;
        case MSG_WARNING:
            prefix = "警告";
            color = YELLOW;
            break;
        case MSG_ERROR:
            prefix = "错误";
            color = RED;
            break;
    }
    
    printf("  %s[%s]%s %s\n", color, prefix, RESET, message);
}

// 便捷函数
void show_info(const char *message) {
    show_message(MSG_INFO, message);
}

void show_success(const char *message) {
    show_message(MSG_SUCCESS, message);
}

void show_warning(const char *message) {
    show_message(MSG_WARNING, message);
}

void show_error(const char *message) {
    show_message(MSG_ERROR, message);
}

// 带详细信息的错误显示
void show_detailed_error(const char *operation, const char *reason, const char *solution) {
    printf("\n  " RED BOLD "错误详情:" RESET "\n");
    printf("  操作: %s\n", operation);
    printf("  原因: %s\n", reason);
    if (solution) {
        printf("  解决方案: %s\n", solution);
    }
    printf("\n");
}

// 确认对话框
int show_confirmation(const char *message, const char *details) {
    printf("\n  " YELLOW "确认操作" RESET "\n");
    printf("  %s\n", message);
    if (details) {
        printf("  %s\n", details);
    }
    
    char choice;
    while (1) {
        printf("\n  确定要继续吗？[y/N]: ");
        scanf(" %c", &choice);
        clear_input_buffer();
        
        if (choice == 'y' || choice == 'Y') {
            return 1;
        } else if (choice == 'n' || choice == 'N' || choice == '\n') {
            return 0;
        } else {
            show_error("请输入 y 或 n");
        }
    }
}
```

### 进度指示器

```c
// 进度条显示
void show_progress_bar(int current, int total, const char *operation) {
    const int bar_width = 40;
    float progress = (float)current / total;
    int filled = (int)(progress * bar_width);
    
    printf("\r  %s: [", operation);
    
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) {
            printf("█");
        } else {
            printf("░");
        }
    }
    
    printf("] %.1f%% (%d/%d)", progress * 100, current, total);
    fflush(stdout);
    
    if (current == total) {
        printf("\n");
    }
}

// 旋转指示器
void show_spinner(int step, const char *message) {
    const char *spinner = "|/-\\";
    printf("\r  %c %s", spinner[step % 4], message);
    fflush(stdout);
}

// 使用示例
void demo_progress_operations() {
    // 进度条示例
    printf("\n正在处理数据...\n");
    for (int i = 0; i <= 100; i++) {
        show_progress_bar(i, 100, "数据处理");
        usleep(50000);  // 模拟处理时间
    }
    
    // 旋转指示器示例
    printf("\n正在连接服务器...");
    for (int i = 0; i < 20; i++) {
        show_spinner(i, "连接中");
        usleep(200000);
    }
    printf("\r  ✓ 连接成功\n");
}
```

---

## 交互式操作设计

### 向导式操作

```c
// 添加学生向导
int add_student_wizard(StudentSystem *system) {
    if (system->count >= MAX_STUDENTS) {
        show_error("学生数量已达上限");
        return 0;
    }
    
    Student new_student;
    memset(&new_student, 0, sizeof(Student));
    
    clear_screen();
    printf("\n  " CYAN BOLD "=== 添加学生向导 ===" RESET "\n\n");
    
    // 步骤1：输入学号
    printf("  " YELLOW "步骤 1/5: 输入学号" RESET "\n");
    printf("  学号规则: 6-12位字符，建议格式为年份+编号\n\n");
    
    if (!smart_input_student_id(system, new_student.id, MAX_ID_LEN)) {
        show_error("学号输入失败，操作取消");
        return 0;
    }
    
    // 检查学号是否已存在
    if (find_student_by_id(system, new_student.id) != -1) {
        show_error("学号已存在，操作取消");
        return 0;
    }
    
    show_success("学号验证通过");
    wait_for_key();
    
    // 步骤2：输入姓名
    clear_screen();
    printf("\n  " CYAN BOLD "=== 添加学生向导 ===" RESET "\n\n");
    printf("  " YELLOW "步骤 2/5: 输入姓名" RESET "\n");
    printf("  已输入学号: %s\n\n", new_student.id);
    
    if (!input_with_prompt(
        "请输入姓名: ",
        "姓名应为2-20个字符，不能包含数字和特殊符号",
        validate_student_name,
        new_student.name,
        MAX_NAME_LEN)) {
        return 0;
    }
    
    show_success("姓名验证通过");
    wait_for_key();
    
    // 步骤3：输入年龄
    clear_screen();
    printf("\n  " CYAN BOLD "=== 添加学生向导 ===" RESET "\n\n");
    printf("  " YELLOW "步骤 3/5: 输入年龄" RESET "\n");
    printf("  已输入学号: %s\n", new_student.id);
    printf("  已输入姓名: %s\n\n", new_student.name);
    
    while (1) {
        printf("请输入年龄 (15-30): ");
        if (safe_input_int(&new_student.age, 15, 30)) {
            break;
        }
        show_error("年龄必须在15-30之间");
    }
    
    show_success("年龄验证通过");
    wait_for_key();
    
    // 步骤4：输入成绩
    clear_screen();
    printf("\n  " CYAN BOLD "=== 添加学生向导 ===" RESET "\n\n");
    printf("  " YELLOW "步骤 4/5: 输入各科成绩" RESET "\n");
    printf("  学生信息: %s (%s, %d岁)\n\n", 
           new_student.name, new_student.id, new_student.age);
    
    // 数学成绩
    while (1) {
        printf("请输入数学成绩 (0-100): ");
        if (safe_input_float(&new_student.math_score, 0, 100)) {
            break;
        }
        show_error("成绩必须在0-100之间");
    }
    
    // 英语成绩
    while (1) {
        printf("请输入英语成绩 (0-100): ");
        if (safe_input_float(&new_student.english_score, 0, 100)) {
            break;
        }
        show_error("成绩必须在0-100之间");
    }
    
    // 语文成绩
    while (1) {
        printf("请输入语文成绩 (0-100): ");
        if (safe_input_float(&new_student.chinese_score, 0, 100)) {
            break;
        }
        show_error("成绩必须在0-100之间");
    }
    
    // 计算平均分
    calculate_average(&new_student);
    
    show_success("成绩录入完成");
    wait_for_key();
    
    // 步骤5：确认信息
    clear_screen();
    printf("\n  " CYAN BOLD "=== 添加学生向导 ===" RESET "\n\n");
    printf("  " YELLOW "步骤 5/5: 确认学生信息" RESET "\n\n");
    
    // 显示完整信息
    printf("  " BOLD "学生信息预览:" RESET "\n");
    printf("  ┌─────────────────────────────────────┐\n");
    printf("  │ 学号: %-30s │\n", new_student.id);
    printf("  │ 姓名: %-30s │\n", new_student.name);
    printf("  │ 年龄: %-30d │\n", new_student.age);
    printf("  │ 数学: %-30.1f │\n", new_student.math_score);
    printf("  │ 英语: %-30.1f │\n", new_student.english_score);
    printf("  │ 语文: %-30.1f │\n", new_student.chinese_score);
    printf("  │ 平均: %-30.1f │\n", new_student.average_score);
    printf("  │ 等级: %-30s │\n", get_grade_level(new_student.average_score));
    printf("  └─────────────────────────────────────┘\n\n");
    
    if (show_confirmation("确认添加这个学生吗？", "添加后将自动保存到数据文件")) {
        system->students[system->count] = new_student;
        system->count++;
        
        show_success("学生添加成功！");
        
        // 自动保存
        if (save_to_file(system)) {
            show_success("数据已自动保存");
        } else {
            show_warning("数据保存失败，请手动保存");
        }
        
        return 1;
    } else {
        show_info("操作已取消");
        return 0;
    }
}
```

### 快捷键支持

```c
// 快捷键定义
#define KEY_CTRL_C 3
#define KEY_CTRL_S 19
#define KEY_CTRL_Q 17
#define KEY_ESC 27
#define KEY_F1 315

// 快捷键处理
int handle_shortcut_keys(int key, StudentSystem *system) {
    switch (key) {
        case KEY_CTRL_S:
            printf("\n  快捷保存...");
            if (save_to_file(system)) {
                show_success("数据保存成功");
            } else {
                show_error("数据保存失败");
            }
            return 1;
            
        case KEY_CTRL_Q:
            printf("\n  快捷退出...");
            if (show_confirmation("确定要退出吗？", "未保存的数据将丢失")) {
                return -1;  // 退出程序
            }
            return 1;
            
        case KEY_F1:
            display_help();
            return 1;
            
        case KEY_ESC:
            printf("\n  操作已取消\n");
            return 1;
            
        default:
            return 0;  // 未处理的按键
    }
}

// 显示帮助信息
void display_help() {
    clear_screen();
    printf("\n  " CYAN BOLD "=== 帮助信息 ===" RESET "\n\n");
    
    printf("  " YELLOW "快捷键:" RESET "\n");
    printf("    Ctrl+S  - 快速保存数据\n");
    printf("    Ctrl+Q  - 快速退出程序\n");
    printf("    F1      - 显示帮助信息\n");
    printf("    ESC     - 取消当前操作\n\n");
    
    printf("  " YELLOW "操作提示:" RESET "\n");
    printf("    • 输入数据时，按回车确认\n");
    printf("    • 可以输入部分信息进行模糊查找\n");
    printf("    • 重要操作会要求确认\n");
    printf("    • 数据会自动验证格式\n\n");
    
    printf("  " YELLOW "数据格式:" RESET "\n");
    printf("    学号: 6-12位字母数字组合\n");
    printf("    姓名: 2-20个字符，不含数字和符号\n");
    printf("    年龄: 15-30岁\n");
    printf("    成绩: 0-100分\n\n");
    
    wait_for_key();
}
```

---

## 界面美化技巧

### 动画效果

```c
// 打字机效果
void typewriter_effect(const char *text, int delay_ms) {
    for (int i = 0; text[i]; i++) {
        printf("%c", text[i]);
        fflush(stdout);
        usleep(delay_ms * 1000);
    }
}

// 淡入效果
void fade_in_text(const char *text) {
    const char *shades[] = {"░", "▒", "▓", "█"};
    int len = strlen(text);
    
    for (int phase = 0; phase < 4; phase++) {
        printf("\r  ");
        for (int i = 0; i < len; i++) {
            if (text[i] == ' ') {
                printf(" ");
            } else {
                printf("%s", shades[phase]);
            }
        }
        fflush(stdout);
        usleep(200000);
    }
    
    printf("\r  %s\n", text);
}

// 启动动画
void show_startup_animation() {
    clear_screen();
    
    printf("\n\n\n");
    printf("                 " CYAN "╔══════════════════════════════════════╗" RESET "\n");
    printf("                 " CYAN "║" RESET);
    typewriter_effect("        学生管理系统 v1.0        ", 50);
    printf(CYAN "║" RESET "\n");
    printf("                 " CYAN "╚══════════════════════════════════════╝" RESET "\n\n");
    
    printf("                      ");
    typewriter_effect("正在初始化系统...", 80);
    printf("\n\n");
    
    // 模拟加载过程
    const char *loading_steps[] = {
        "加载配置文件",
        "初始化数据结构",
        "检查数据文件",
        "准备用户界面",
        "系统就绪"
    };
    
    for (int i = 0; i < 5; i++) {
        printf("                      [%d/5] %s", i + 1, loading_steps[i]);
        
        for (int j = 0; j < 3; j++) {
            printf(".");
            fflush(stdout);
            usleep(300000);
        }
        
        printf(" " GREEN "✓" RESET "\n");
        usleep(200000);
    }
    
    printf("\n                      ");
    fade_in_text("欢迎使用学生管理系统！");
    
    sleep(2);
}

// 退出动画
void show_exit_animation() {
    clear_screen();
    
    printf("\n\n\n");
    printf("                      ");
    typewriter_effect("正在保存数据...", 80);
    printf("\n\n");
    
    // 保存进度
    for (int i = 0; i <= 100; i += 10) {
        printf("\r                      保存进度: [%3d%%] ", i);
        for (int j = 0; j < i / 5; j++) {
            printf("█");
        }
        fflush(stdout);
        usleep(100000);
    }
    
    printf("\n\n                      ");
    typewriter_effect("感谢使用学生管理系统！", 80);
    printf("\n\n");
    
    sleep(1);
}
```

### 主题系统

```c
// 主题配置
typedef struct {
    char name[20];
    char primary_color[10];
    char secondary_color[10];
    char success_color[10];
    char warning_color[10];
    char error_color[10];
    char border_style[10];
} Theme;

// 预定义主题
Theme themes[] = {
    {"默认", BLUE, CYAN, GREEN, YELLOW, RED, "═"},
    {"简约", WHITE, WHITE, GREEN, YELLOW, RED, "-"},
    {"深色", MAGENTA, CYAN, GREEN, YELLOW, RED, "━"},
    {"彩色", BLUE, MAGENTA, GREEN, YELLOW, RED, "▬"}
};

int current_theme = 0;

// 应用主题
void apply_theme(int theme_index) {
    if (theme_index >= 0 && theme_index < sizeof(themes) / sizeof(Theme)) {
        current_theme = theme_index;
        printf("\n  主题已切换为: %s\n", themes[current_theme].name);
    }
}

// 主题选择菜单
void theme_selection_menu() {
    clear_screen();
    
    printf("\n  " CYAN BOLD "=== 主题选择 ===" RESET "\n\n");
    
    for (int i = 0; i < sizeof(themes) / sizeof(Theme); i++) {
        if (i == current_theme) {
            printf("  " GREEN "● %d. %s (当前)" RESET "\n", i + 1, themes[i].name);
        } else {
            printf("    %d. %s\n", i + 1, themes[i].name);
        }
    }
    
    printf("\n  0. 返回\n\n");
    
    int choice;
    printf("请选择主题 (0-%d): ", (int)(sizeof(themes) / sizeof(Theme)));
    
    if (safe_input_int(&choice, 0, sizeof(themes) / sizeof(Theme))) {
        if (choice > 0) {
            apply_theme(choice - 1);
            wait_for_key();
        }
    }
}
```

---

## 实践练习

### 练习1：实现搜索高亮

```c
// 练习：实现搜索结果高亮显示
// 要求：
// 1. 在搜索结果中高亮匹配的关键词
// 2. 支持多个关键词高亮
// 3. 使用不同颜色区分不同关键词

void highlight_search_results(const char *text, const char *keyword) {
    // TODO: 实现搜索高亮
    // 提示：
    // 1. 使用strstr查找关键词位置
    // 2. 在关键词前后添加颜色代码
    // 3. 处理大小写不敏感的匹配
}
```

### 练习2：实现自定义布局

```c
// 练习：实现可配置的界面布局
// 要求：
// 1. 支持用户自定义表格列的显示/隐藏
// 2. 支持调整列的显示顺序
// 3. 保存用户的布局偏好

typedef struct {
    int column_id;
    int visible;
    int order;
    int width;
} ColumnConfig;

void customize_table_layout(ColumnConfig *config, int column_count) {
    // TODO: 实现布局自定义
}
```

### 练习3：实现多语言支持

```c
// 练习：实现多语言界面
// 要求：
// 1. 支持中文和英文界面
// 2. 动态切换语言
// 3. 所有文本都支持本地化

typedef struct {
    char language_code[5];
    char menu_title[50];
    char menu_items[10][50];
    char messages[20][100];
} LanguagePack;

void switch_language(const char *language_code) {
    // TODO: 实现语言切换
}
```

---

## 💡 本课要点总结

### 用户界面设计原则
1. **直观性**: 界面元素含义明确，操作流程自然
2. **一致性**: 相同功能在不同地方表现一致
3. **反馈性**: 及时给用户操作反馈和状态提示
4. **容错性**: 优雅处理用户错误，提供恢复机制
5. **效率性**: 减少操作步骤，提高使用效率

### 菜单系统设计
1. **层次化结构**: 主菜单-子菜单-功能页面
2. **导航支持**: 面包屑导航，快速返回
3. **视觉美化**: 颜色搭配，边框装饰

### 输入验证与处理
1. **安全输入**: 防止缓冲区溢出，输入验证
2. **智能提示**: 输入建议，格式说明
3. **错误处理**: 友好的错误信息，重试机制

### 信息显示与格式化
1. **表格显示**: 对齐美观，颜色区分
2. **分页支持**: 大数据量的分页显示
3. **统计信息**: 数据汇总，可视化展示

### 交互式操作
1. **向导模式**: 分步骤引导用户完成复杂操作
2. **快捷键**: 提高操作效率
3. **确认对话**: 重要操作的二次确认

### 界面美化
1. **动画效果**: 启动动画，过渡效果
2. **主题系统**: 多种界面风格选择
3. **颜色搭配**: 合理使用ANSI颜色代码

---

## 🎯 下一步学习

在下一课中，我们将学习：
- **第8课：错误处理与调试** - 如何处理程序错误和调试技巧
- 异常处理机制
- 调试工具使用
- 日志系统设计
- 单元测试编写

---

## 📚 扩展阅读

- C语言终端控制和ANSI转义序列
- 用户体验设计原则
- 命令行界面最佳实践
- 跨平台终端兼容性

---

**上一课**: [第6课：数据持久化](06-file-operations.md)  
**下一课**: [第8课：错误处理与调试](08-error-handling.md)  
**返回**: [教程目录](README.md)