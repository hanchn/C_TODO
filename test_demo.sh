#!/bin/bash

# 学生管理系统演示脚本
# 这个脚本展示了如何使用学生管理系统的各项功能

echo "=== 学生管理系统演示 ==="
echo "本演示将展示系统的主要功能"
echo ""

# 检查可执行文件是否存在
if [ ! -f "./student_management" ]; then
    echo "错误：找不到可执行文件，请先运行 'make' 编译程序"
    exit 1
fi

echo "1. 编译状态：✅ 成功"
echo "2. 可执行文件：✅ student_management"
echo "3. 源代码文件："
ls -la *.c *.h 2>/dev/null | while read line; do
    echo "   $line"
done

echo ""
echo "=== 功能特性 ==="
echo "✅ 学生信息管理（增删改查）"
echo "✅ 成绩录入和计算"
echo "✅ 数据排序和统计"
echo "✅ 文件数据持久化"
echo "✅ 输入验证和错误处理"
echo "✅ 友好的用户界面"

echo ""
echo "=== 使用方法 ==="
echo "直接运行：./student_management"
echo "或使用：make run"

echo ""
echo "=== 示例数据格式 ==="
echo "学号：2023001（支持字母数字）"
echo "姓名：张三（最大50字符）"
echo "年龄：20（1-150之间）"
echo "成绩：85.5（0-100之间的浮点数）"

echo ""
echo "演示完成！现在可以运行 './student_management' 开始使用系统。"
echo "或者运行 'make run' 直接启动程序。"