# 学生管理系统 Makefile

# 编译器和编译选项
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# 目标文件
TARGET = student_management

# 源文件
SOURCES = main.c student.c

# 头文件
HEADERS = student.h

# 对象文件
OBJECTS = $(SOURCES:.c=.o)

# 默认目标
all: $(TARGET)

# 链接目标文件
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)
	@echo "编译完成！可执行文件: $(TARGET)"

# 编译源文件为对象文件
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# 清理编译生成的文件
clean:
	rm -f $(OBJECTS) $(TARGET) students.dat
	@echo "清理完成！"

# 只清理对象文件，保留可执行文件和数据文件
clean-obj:
	rm -f $(OBJECTS)
	@echo "对象文件清理完成！"

# 运行程序
run: $(TARGET)
	./$(TARGET)

# 调试模式编译
debug: CFLAGS += -DDEBUG
debug: $(TARGET)

# 安装（复制到系统路径，需要管理员权限）
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/
	@echo "安装完成！"

# 卸载
uninstall:
	rm -f /usr/local/bin/$(TARGET)
	@echo "卸载完成！"

# 显示帮助信息
help:
	@echo "可用的make目标:"
	@echo "  all       - 编译程序（默认）"
	@echo "  clean     - 清理所有生成的文件"
	@echo "  clean-obj - 只清理对象文件"
	@echo "  run       - 编译并运行程序"
	@echo "  debug     - 调试模式编译"
	@echo "  install   - 安装到系统（需要sudo）"
	@echo "  uninstall - 从系统卸载（需要sudo）"
	@echo "  help      - 显示此帮助信息"

# 声明伪目标
.PHONY: all clean clean-obj run debug install uninstall help