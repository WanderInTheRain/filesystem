#ifndef SHL_H
#define SHL_H

#include "fd.h"
#include "memory.h"

class Manager {
public:
    Directory current_dir;

    Manager();
    void init(Memory& mem);
    void buffer_to_directory(Directory& directory,Memory& mem);
    void directory_to_buffer(const Directory& directory,Memory& mem);
};

class Shell{
public:
    Manager manager;
    Memory mem;
    std::string cmd;

    Shell(const char* disk_filename);
    ~Shell();
    void run();
};

#endif