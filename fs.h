// fs.h

#ifndef FS_H
#define FS_H

#include <iostream>
#include <fstream>
#include <vector>

const std::size_t namelen = 16;
const std::size_t blocksize = 5120;

class FDnode { // file or directory node
public:
    char name[namelen];
    int type; // 0 for file, 1 for directory
    std::size_t start_block;
    std::size_t parent_block;

    // Constructor
    FDnode(const char* _name, int _type, std::size_t _start_block, std::size_t _parent_block);
};

class File {
public:
    FDnode fnode; // type = 0
    char data[blocksize - sizeof(FDnode)]; // read from start_block
    
    File(const FDnode& _fnode, const char* _data);

};

class Directory {
public:
    FDnode dnode; // type = 1
    std::size_t nodenum;
    std::vector<FDnode> fdnodes;

    // Constructor
    Directory(const FDnode& _dnode, std::size_t _nodenum,\
     const std::vector<FDnode>& _fdnodes);
};

class Memory{
public:
    char buffer[blocksize];
    std::fstream disk;

    Memory(const char* disk_filename);
    ~Memory();
    void buffer_read_block(std::size_t start_block);
    void buffer_write_disk(std::size_t start_block);
};

class Manager {
public:
    Directory current_dir;

    Manager();
    void init(Memory& mem);
    void buffer_to_directory(Directory& directory,Memory& mem);
    void directory_to_buffer(const Directory& directory,Memory& mem);
};

#endif // FS_H
