# Makefile

# 定义变量
CC = gcc
CFLAGS = -W"all" -W"extra" -std=c11

# 默认目标
all: spacefinder


# 编译规则 
spacefinder: spacefinder.c
	$(CC) -g spacefinder.c $(CFLAGS) -o spacefinder.exe

# 清理规则
clean:
	rm -f spacefinder.exe

