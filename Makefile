# 定义编译器
CXX = g++
# 定义编译选项
CXXFLAGS = -std=c++11 -Wall
# 定义目标文件
TARGET = filesystem
# 定义源文件目录
SRCDIR = src
# 定义头文件目录
INCDIR = include
# 定义生成的目标文件存放目录
OBJDIR = obj

# 定义源文件
SRCS = $(wildcard $(SRCDIR)/*.cpp)
# 将SRCS中的.cpp文件名替换为.o文件名
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 生成.o文件
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# 清除生成的文件
clean:
	rm -f $(OBJDIR)/*.o $(TARGET)
