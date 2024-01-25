#ifndef MEM_H
#define MEM_H

#include <iostream>
#include <fstream>

const std::size_t blocksize = 5120;

class Memory{
public:
    char buffer[blocksize];
    std::fstream disk;

    Memory(const char* disk_filename);
    ~Memory();
    void buffer_read_block(std::size_t start_block);
    void buffer_write_disk(std::size_t start_block);
};

#endif