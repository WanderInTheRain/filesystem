#ifndef SHL_H
#define SHL_H

#include "fd.h"
#include "memory.h"

class Manager {
public:
    Directory current_dir;
    Memory mem;
    int bitmap[blocknum];

    Manager(const char* disk_filename);
    void init();
    void buffer_to_directory(Directory& directory);
    void directory_to_buffer(const Directory& directory);
    void init_bitmap();
    void update_bitmap();
    int get_block();
    void release_block(int blknum);
};

class Shell{
public:
    Manager manager;
    std::vector<std::string> prefix;
    std::string cmd;

    Shell(const char* disk_filename);
    void run();
private:
    void rm_directory(Directory dir);
    void print_prefix();
    std::vector<std::string> split(std::string input);
};

#endif