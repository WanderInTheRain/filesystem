#include "shell.h"

void clean_disk(const char* _filename) {
    // 指定文件名
    const char* filename = _filename;
    // 指定文件大小为5MB
    std::size_t file_size = 5 * 1024 * 1024;//512*10*1024

    // 打开文件并写入全0数据
    std::ofstream outfile(filename, std::ios::binary | std::ios::out);

    if (outfile.is_open()) {
        // 分配文件大小的缓冲区，并用0填充
        char* buffer = new char[file_size];
        
        // 写入缓冲区数据到文件
        outfile.write(buffer, file_size);

        // 关闭文件
        outfile.close();

        // 释放缓冲区内存
        delete buffer;

        std::cout << "clean file:" << filename << std::endl;
    } else {
        std::cerr << "cannot open file:" << filename << std::endl;
    }

}
