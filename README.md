# 学生管理系统

一个基于C语言开发的简单而功能完整的学生管理系统，支持学生信息的增删改查、排序、统计和文件存储等功能。

## 功能特性

### 核心功能
- ✅ **添加学生** - 录入学生的基本信息和各科成绩
- ✅ **删除学生** - 根据学号删除学生记录
- ✅ **修改信息** - 修改学生的姓名、年龄或各科成绩
- ✅ **显示所有学生** - 以表格形式展示所有学生信息
- ✅ **查找学生** - 支持按学号或姓名查找
- ✅ **成绩排序** - 按平均分进行升序或降序排列
- ✅ **统计信息** - 显示各科平均分、最高分和最低分学生
- ✅ **数据持久化** - 支持数据的保存和加载

### 数据验证
- 学号格式验证（字母数字组合）
- 成绩范围验证（0-100分）
- 年龄合理性验证（1-150岁）
- 重复学号检测
- 输入错误处理

## 项目结构

```
C_TODO/
├── main.c          # 主程序文件
├── student.c       # 功能实现文件
├── student.h       # 头文件定义
├── Makefile        # 编译配置文件
├── README.md       # 项目说明文档
└── students.dat    # 数据文件（运行后生成）
```

## 编译和运行

### 方法一：使用Makefile（推荐）

```bash
# 编译程序
make

# 编译并运行
make run

# 清理编译文件
make clean

# 查看所有可用命令
make help
```

### 方法二：手动编译

```bash
# 编译
gcc -Wall -Wextra -std=c99 -g main.c student.c -o student_management

# 运行
./student_management
```

## 使用说明

### 主菜单选项

```
========== 学生管理系统 ==========
1. 添加学生
2. 删除学生
3. 修改学生信息
4. 显示所有学生
5. 查找学生
6. 按成绩排序
7. 统计信息
8. 保存到文件
9. 从文件加载
0. 退出系统
================================
```

### 数据格式说明

- **学号**：支持字母和数字组合，长度不超过20字符
- **姓名**：最大长度50字符
- **年龄**：1-150之间的整数
- **成绩**：0-100之间的浮点数（数学、英语、语文）
- **平均分**：自动计算三科成绩的平均值

### 使用示例

1. **添加学生**
   ```
   请输入学号: 2023001
   请输入姓名: 张三
   请输入年龄: 20
   请输入数学成绩: 85.5
   请输入英语成绩: 92.0
   请输入语文成绩: 88.5
   ```

2. **查看所有学生**
   ```
   学号       姓名            年龄  数学     英语     语文     平均分
   ----------------------------------------------------------------
   2023001    张三            20    85.50    92.00    88.50    88.67
   ```

## 技术特点

### 数据结构
- 使用结构体存储学生信息
- 数组管理多个学生记录
- 支持最多100名学生（可配置）

### 文件操作
- 二进制文件存储，提高读写效率
- 程序启动时自动加载数据
- 退出时提示保存数据

### 错误处理
- 完善的输入验证机制
- 友好的错误提示信息
- 异常情况的安全处理

## 系统要求

- **操作系统**：Linux、macOS、Windows（需要GCC编译器）
- **编译器**：GCC 4.9+ 或其他支持C99标准的编译器
- **内存**：最小1MB（实际使用很少）
- **存储**：数据文件大小取决于学生数量

## 扩展建议

如果需要进一步扩展功能，可以考虑：

1. **数据库支持** - 使用SQLite等轻量级数据库
2. **网络功能** - 支持多用户同时访问
3. **图形界面** - 使用GTK+或Qt开发GUI版本
4. **更多统计** - 添加成绩分布、及格率等统计
5. **导入导出** - 支持CSV、Excel等格式
6. **用户权限** - 添加管理员和普通用户权限控制

## 常见问题

**Q: 编译时出现警告怎么办？**
A: 项目使用了严格的编译选项，轻微警告不影响使用。如需消除警告，请检查代码中的类型转换和变量使用。

**Q: 数据文件在哪里？**
A: 数据文件`students.dat`会在程序运行目录下自动创建。

**Q: 如何备份数据？**
A: 直接复制`students.dat`文件即可备份所有学生数据。

**Q: 支持中文姓名吗？**
A: 支持，但建议在支持UTF-8的终端环境下运行以获得最佳显示效果。

## 开发信息

- **开发语言**：C语言（C99标准）
- **开发工具**：GCC编译器 + Make构建工具
- **代码风格**：遵循Linux内核代码风格
- **文档编码**：UTF-8

---

*这是一个教学和学习用途的项目，展示了C语言在系统开发中的应用。代码结构清晰，注释详细，适合C语言学习者参考和改进。*