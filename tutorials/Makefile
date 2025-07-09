# 学生管理系统教程 - Makefile
# 用于编译所有示例代码、练习和解决方案

# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -O2
LDFLAGS = 
LIBS = -lm

# 目录设置
EXAMPLES_DIR = examples
EXERCISES_DIR = exercises
SOLUTIONS_DIR = solutions
BUILD_DIR = build

# 源文件
EXAMPLE_SOURCES = $(wildcard $(EXAMPLES_DIR)/*.c)
EXERCISE_SOURCES = $(wildcard $(EXERCISES_DIR)/*.c)
SOLUTION_SOURCES = $(wildcard $(SOLUTIONS_DIR)/*.c)

# 目标文件
EXAMPLE_TARGETS = $(patsubst $(EXAMPLES_DIR)/%.c,$(BUILD_DIR)/examples/%,$(EXAMPLE_SOURCES))
EXERCISE_TARGETS = $(patsubst $(EXERCISES_DIR)/%.c,$(BUILD_DIR)/exercises/%,$(EXERCISE_SOURCES))
SOLUTION_TARGETS = $(patsubst $(SOLUTIONS_DIR)/%.c,$(BUILD_DIR)/solutions/%,$(SOLUTION_SOURCES))

# 所有目标
ALL_TARGETS = $(EXAMPLE_TARGETS) $(EXERCISE_TARGETS) $(SOLUTION_TARGETS)

# 默认目标
.PHONY: all
all: directories $(ALL_TARGETS)
	@echo "\n=== 编译完成 ==="
	@echo "示例程序: $(words $(EXAMPLE_TARGETS)) 个"
	@echo "练习程序: $(words $(EXERCISE_TARGETS)) 个"
	@echo "解决方案: $(words $(SOLUTION_TARGETS)) 个"
	@echo "总计: $(words $(ALL_TARGETS)) 个程序"
	@echo "\n使用 'make run-examples' 运行所有示例"
	@echo "使用 'make run-solutions' 运行所有解决方案"
	@echo "使用 'make help' 查看更多选项"

# 创建目录
.PHONY: directories
directories:
	@mkdir -p $(BUILD_DIR)/examples
	@mkdir -p $(BUILD_DIR)/exercises
	@mkdir -p $(BUILD_DIR)/solutions

# 编译示例程序
$(BUILD_DIR)/examples/%: $(EXAMPLES_DIR)/%.c
	@echo "编译示例: $<"
	@$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LIBS)

# 编译练习程序
$(BUILD_DIR)/exercises/%: $(EXERCISES_DIR)/%.c
	@echo "编译练习: $<"
	@$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LIBS)

# 编译解决方案
$(BUILD_DIR)/solutions/%: $(SOLUTIONS_DIR)/%.c
	@echo "编译解决方案: $<"
	@$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) $(LIBS)

# 单独编译目标
.PHONY: examples exercises solutions
examples: directories $(EXAMPLE_TARGETS)
	@echo "示例程序编译完成"

exercises: directories $(EXERCISE_TARGETS)
	@echo "练习程序编译完成"

solutions: directories $(SOLUTION_TARGETS)
	@echo "解决方案编译完成"

# 运行程序
.PHONY: run-examples run-solutions run-all
run-examples: examples
	@echo "\n=== 运行所有示例程序 ==="
	@for target in $(EXAMPLE_TARGETS); do \
		if [ -f "$$target" ]; then \
			echo "\n--- 运行 $$target ---"; \
			$$target; \
			echo "--- 完成 $$target ---"; \
		fi; \
	done

run-solutions: solutions
	@echo "\n=== 运行所有解决方案 ==="
	@for target in $(SOLUTION_TARGETS); do \
		if [ -f "$$target" ]; then \
			echo "\n--- 运行 $$target ---"; \
			$$target; \
			echo "--- 完成 $$target ---"; \
		fi; \
	done

run-all: run-examples run-solutions

# 运行特定程序
.PHONY: run-basic run-struct run-file
run-basic: $(BUILD_DIR)/examples/basic_example
	@echo "\n=== 运行C语言基础示例 ==="
	@$(BUILD_DIR)/examples/basic_example

run-struct: $(BUILD_DIR)/examples/struct_array_example
	@echo "\n=== 运行结构体数组示例 ==="
	@$(BUILD_DIR)/examples/struct_array_example

run-file: $(BUILD_DIR)/examples/file_operations_example
	@echo "\n=== 运行文件操作示例 ==="
	@$(BUILD_DIR)/examples/file_operations_example

# 运行特定解决方案
.PHONY: run-solution1 run-solution2 run-solution3
run-solution1: $(BUILD_DIR)/solutions/solution_01
	@echo "\n=== 运行练习1解决方案 ==="
	@$(BUILD_DIR)/solutions/solution_01

run-solution2: $(BUILD_DIR)/solutions/solution_02
	@echo "\n=== 运行练习2解决方案 ==="
	@$(BUILD_DIR)/solutions/solution_02

run-solution3: $(BUILD_DIR)/solutions/solution_03
	@echo "\n=== 运行练习3解决方案 ==="
	@$(BUILD_DIR)/solutions/solution_03

# 测试和验证
.PHONY: test validate
test: solutions
	@echo "\n=== 运行测试 ==="
	@echo "测试解决方案1（C语言基础）..."
	@$(BUILD_DIR)/solutions/solution_01 > /dev/null && echo "✓ 解决方案1测试通过" || echo "✗ 解决方案1测试失败"
	@echo "测试解决方案2（结构体数组）..."
	@$(BUILD_DIR)/solutions/solution_02 > /dev/null && echo "✓ 解决方案2测试通过" || echo "✗ 解决方案2测试失败"
	@echo "测试解决方案3（文件操作）..."
	@$(BUILD_DIR)/solutions/solution_03 > /dev/null && echo "✓ 解决方案3测试通过" || echo "✗ 解决方案3测试失败"

validate: test
	@echo "\n=== 代码验证 ==="
	@echo "检查编译警告..."
	@$(MAKE) clean
	@$(MAKE) all CFLAGS="$(CFLAGS) -Werror" > /dev/null 2>&1 && echo "✓ 无编译警告" || echo "✗ 存在编译警告"

# 调试版本
.PHONY: debug
debug: CFLAGS += -DDEBUG -g3 -O0
debug: clean all
	@echo "调试版本编译完成"

# 发布版本
.PHONY: release
release: CFLAGS += -DNDEBUG -O3
release: clean all
	@echo "发布版本编译完成"

# 内存检查（需要valgrind）
.PHONY: memcheck
memcheck: debug
	@echo "\n=== 内存检查 ==="
	@if command -v valgrind >/dev/null 2>&1; then \
		for target in $(SOLUTION_TARGETS); do \
			if [ -f "$$target" ]; then \
				echo "检查 $$target..."; \
				valgrind --leak-check=full --error-exitcode=1 $$target > /dev/null 2>&1 && echo "✓ 无内存泄漏" || echo "✗ 发现内存问题"; \
			fi; \
		done; \
	else \
		echo "valgrind 未安装，跳过内存检查"; \
	fi

# 代码格式化（需要clang-format）
.PHONY: format
format:
	@echo "格式化代码..."
	@if command -v clang-format >/dev/null 2>&1; then \
		find $(EXAMPLES_DIR) $(EXERCISES_DIR) $(SOLUTIONS_DIR) -name "*.c" -exec clang-format -i {} \;; \
		echo "代码格式化完成"; \
	else \
		echo "clang-format 未安装，跳过格式化"; \
	fi

# 代码分析（需要cppcheck）
.PHONY: analyze
analyze:
	@echo "\n=== 静态代码分析 ==="
	@if command -v cppcheck >/dev/null 2>&1; then \
		cppcheck --enable=all --std=c99 --suppress=missingIncludeSystem $(EXAMPLES_DIR) $(EXERCISES_DIR) $(SOLUTIONS_DIR); \
	else \
		echo "cppcheck 未安装，跳过静态分析"; \
	fi

# 生成文档（需要doxygen）
.PHONY: docs
docs:
	@echo "生成文档..."
	@if command -v doxygen >/dev/null 2>&1; then \
		if [ ! -f Doxyfile ]; then \
			doxygen -g; \
		fi; \
		doxygen; \
		echo "文档生成完成，查看 html/index.html"; \
	else \
		echo "doxygen 未安装，跳过文档生成"; \
	fi

# 清理
.PHONY: clean clean-all
clean:
	@echo "清理编译文件..."
	@rm -rf $(BUILD_DIR)
	@echo "清理完成"

clean-all: clean
	@echo "清理所有生成文件..."
	@rm -f *.txt *.dat *.csv *.log
	@rm -rf html latex
	@rm -f Doxyfile
	@echo "全部清理完成"

# 安装（复制到系统目录）
.PHONY: install
install: release
	@echo "安装程序..."
	@mkdir -p ~/bin/student_tutorials
	@cp -r $(BUILD_DIR)/* ~/bin/student_tutorials/
	@echo "程序已安装到 ~/bin/student_tutorials/"

# 卸载
.PHONY: uninstall
uninstall:
	@echo "卸载程序..."
	@rm -rf ~/bin/student_tutorials
	@echo "程序已卸载"

# 打包
.PHONY: package
package: clean-all
	@echo "创建发布包..."
	@tar -czf student_tutorials.tar.gz \
		--exclude='.git*' \
		--exclude='*.tar.gz' \
		--exclude='build' \
		.
	@echo "发布包已创建: student_tutorials.tar.gz"

# 统计信息
.PHONY: stats
stats:
	@echo "\n=== 项目统计 ==="
	@echo "源文件数量:"
	@echo "  示例: $(words $(EXAMPLE_SOURCES))"
	@echo "  练习: $(words $(EXERCISE_SOURCES))"
	@echo "  解决方案: $(words $(SOLUTION_SOURCES))"
	@echo "  总计: $(words $(EXAMPLE_SOURCES) $(EXERCISE_SOURCES) $(SOLUTION_SOURCES))"
	@echo "\n代码行数:"
	@wc -l $(EXAMPLE_SOURCES) $(EXERCISE_SOURCES) $(SOLUTION_SOURCES) 2>/dev/null | tail -1 || echo "  无法统计"
	@echo "\n文件大小:"
	@du -sh $(EXAMPLES_DIR) $(EXERCISES_DIR) $(SOLUTIONS_DIR) 2>/dev/null || echo "  无法统计"

# 帮助信息
.PHONY: help
help:
	@echo "学生管理系统教程 - Makefile 帮助"
	@echo "================================="
	@echo ""
	@echo "编译目标:"
	@echo "  all          - 编译所有程序（默认）"
	@echo "  examples     - 只编译示例程序"
	@echo "  exercises    - 只编译练习程序"
	@echo "  solutions    - 只编译解决方案"
	@echo "  debug        - 编译调试版本"
	@echo "  release      - 编译发布版本"
	@echo ""
	@echo "运行目标:"
	@echo "  run-examples - 运行所有示例程序"
	@echo "  run-solutions- 运行所有解决方案"
	@echo "  run-all      - 运行所有程序"
	@echo "  run-basic    - 运行C语言基础示例"
	@echo "  run-struct   - 运行结构体数组示例"
	@echo "  run-file     - 运行文件操作示例"
	@echo "  run-solution1- 运行练习1解决方案"
	@echo "  run-solution2- 运行练习2解决方案"
	@echo "  run-solution3- 运行练习3解决方案"
	@echo ""
	@echo "测试和验证:"
	@echo "  test         - 运行所有测试"
	@echo "  validate     - 验证代码质量"
	@echo "  memcheck     - 内存泄漏检查（需要valgrind）"
	@echo "  analyze      - 静态代码分析（需要cppcheck）"
	@echo ""
	@echo "代码质量:"
	@echo "  format       - 格式化代码（需要clang-format）"
	@echo "  docs         - 生成文档（需要doxygen）"
	@echo ""
	@echo "管理目标:"
	@echo "  clean        - 清理编译文件"
	@echo "  clean-all    - 清理所有生成文件"
	@echo "  install      - 安装程序到用户目录"
	@echo "  uninstall    - 卸载程序"
	@echo "  package      - 创建发布包"
	@echo "  stats        - 显示项目统计信息"
	@echo ""
	@echo "示例用法:"
	@echo "  make                    # 编译所有程序"
	@echo "  make run-solutions      # 运行所有解决方案"
	@echo "  make test               # 运行测试"
	@echo "  make clean && make all  # 重新编译"
	@echo ""

# 防止文件名冲突
.SUFFIXES:
.SECONDARY:

# 显示变量（调试用）
.PHONY: show-vars
show-vars:
	@echo "CC = $(CC)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "LIBS = $(LIBS)"
	@echo "EXAMPLE_SOURCES = $(EXAMPLE_SOURCES)"
	@echo "EXERCISE_SOURCES = $(EXERCISE_SOURCES)"
	@echo "SOLUTION_SOURCES = $(SOLUTION_SOURCES)"
	@echo "EXAMPLE_TARGETS = $(EXAMPLE_TARGETS)"
	@echo "EXERCISE_TARGETS = $(EXERCISE_TARGETS)"
	@echo "SOLUTION_TARGETS = $(SOLUTION_TARGETS)"