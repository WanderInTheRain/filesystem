// fd.h

#ifndef FD_H
#define FD_H

#include <vector>
#include "memory.h"

const int FL = 0;
const int DIR = 1;
const std::size_t namelen = 16;
const int blocknum = 1024;

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

#endif