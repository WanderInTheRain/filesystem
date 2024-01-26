#include "memory.h"

//Memory
Memory::Memory(const char* disk_filename){
    disk.open(disk_filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!disk.is_open()) {
        std::cerr << "无法打开磁盘文件" << std::endl;
        exit(EXIT_FAILURE);
    }
}

Memory::~Memory(){
    disk.close();
}

void Memory::buffer_read_block(std::size_t start_block) {
    disk.seekg(start_block * blocksize, std::ios::beg);
    disk.read(buffer, blocksize);
}

void Memory::buffer_write_disk(std::size_t start_block) {
    disk.seekp(start_block * blocksize, std::ios::beg);
    disk.write(buffer, blocksize);
}
