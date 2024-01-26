# 定义编译器
CXX = g++
# 定义编译选项
CXXFLAGS = -std=c++11 -Wall

# 定义目标文件
TARGET = fs

# 定义源文件
SRCS = fd.cpp manager.cpp memory.cpp run.cpp shell.cpp vi.cpp
# 将SRCS中的.cpp文件名替换为.o文件名
OBJS = $(SRCS:.cpp=.o)

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 生成.o文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清除生成的文件
clean:
	rm -f $(OBJS) $(TARGET)
