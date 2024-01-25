// shell.cpp

#include "shell.h"

//Manager
Manager::Manager():\
    current_dir(FDnode("", 1, 0, 0), 0, std::vector<FDnode>()){}


void Manager::init(Memory& mem){
    buffer_to_directory(current_dir,mem);
}

void Manager::buffer_to_directory(Directory& directory,Memory& mem) {
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

void Manager::directory_to_buffer(const Directory& directory,Memory& mem) {
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