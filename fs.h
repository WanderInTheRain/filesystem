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
    FDnode(const char* _name, int _type, std::size_t _start_block, std::size_t _parent_block) {
        memcpy(name, _name, namelen);
        type = _type;
        start_block = _start_block;
        parent_block = _parent_block;
    }
};

class File {
public:
    FDnode fnode; // type = 0
    char data[blocksize - sizeof(FDnode)]; // read from start_block
    
    File(const FDnode& _fnode, const char* _data) : fnode(_fnode) {
        memcpy(data, _data, blocksize - sizeof(FDnode));
    }

};

class Directory {
public:
    FDnode dnode; // type = 1
    std::size_t nodenum;
    std::vector<FDnode> fdnodes;

    // Constructor
    Directory(const FDnode& _dnode, std::size_t _nodenum,\
         const std::vector<FDnode>& _fdnodes)
        : dnode(_dnode), nodenum(_nodenum), fdnodes(_fdnodes) {}
};

class Memory{
public:
    char buffer[blocksize];
    std::fstream disk;

    Memory(const char* disk_filename){
        disk.open(disk_filename, std::ios::binary | std::ios::in | std::ios::out);
        if (!disk.is_open()) {
            std::cerr << "无法打开磁盘文件" << std::endl;
            exit(EXIT_FAILURE);
        }
        buffer_read_block(0);
    }

    ~Memory(){
        disk.close();
    }

    void buffer_read_block(std::size_t start_block) {
        disk.seekg(start_block * blocksize, std::ios::beg);
        disk.read(buffer, blocksize);
    }

    void buffer_write_disk(std::size_t start_block) {
        disk.seekp(start_block * blocksize, std::ios::beg);
        disk.write(buffer, blocksize);
    }
};

class Manager {
public:
    Directory current_dir;

    Manager() :
     current_dir(FDnode("", 1, 0, 0), 0, std::vector<FDnode>()){}

    void init(Memory& mem){
        buffer_to_directory(current_dir,mem);
    }

    void buffer_to_directory(Directory& directory,Memory& mem) {
        char* buffer_ptr = mem.buffer;
        memcpy(&directory.dnode, buffer_ptr, sizeof(FDnode));
        buffer_ptr += sizeof(FDnode);

        memcpy(&directory.nodenum, buffer_ptr, sizeof(std::size_t));
        buffer_ptr += sizeof(std::size_t);

        for (std::size_t i = 0; i < directory.nodenum; ++i) {
            FDnode node("", 1, 0, 0);
            memcpy(&node, buffer_ptr, sizeof(FDnode));
            directory.fdnodes.push_back(node);
            buffer_ptr += sizeof(FDnode);
        }
    }

    void directory_to_buffer(const Directory& directory,Memory& mem) {
        char* buffer_ptr = mem.buffer;
        memcpy(buffer_ptr, &directory.dnode, sizeof(FDnode));
        buffer_ptr += sizeof(FDnode);

        memcpy(buffer_ptr, &directory.nodenum, sizeof(std::size_t));
        buffer_ptr += sizeof(std::size_t);

        for (auto node : directory.fdnodes) {
            memcpy(buffer_ptr, &node, sizeof(FDnode));
            buffer_ptr += sizeof(FDnode);
        }
    }
};

#endif // FS_H
