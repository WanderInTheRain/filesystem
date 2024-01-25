//fs.h

#ifndef FS_H
#define FS_H

#include <iostream>
#include <vector>

const std::size_t namelen = 16;
const std::size_t buffersize = 512 * 10; // 10 blocks

class File { // file node
public:
    char name[namelen];
    int type; // 0 for file
    std::size_t size;
    std::size_t start_block;
    // Change data type to char array for storing multiple characters
    char data[buffersize];

    File() : type(0), size(0), start_block(0) {
        // Initialize data array with null characters
        std::fill(std::begin(data), std::end(data), '\0');
    }
};

class Fnode { // file node
public:
    char name[namelen];
    int type; // 0 for file
    std::size_t size;
    std::size_t start_block;
    // Change data type to char array for storing multiple characters

    Fnode() : type(0), size(0), start_block(0) {}
};

class Directory {
public:
    char name[namelen];
    int type; // 1 for directory
    std::size_t size;
    std::size_t start_block;
    std::vector<Fnode> fnodes;

    Directory() : type(1), size(0), start_block(0) {}
};

class Manager {
public:
    Directory root;
    FILE* disk;
    char buffer[buffersize];

    void init() { // read root directory from disk 0 block
        disk = fopen("disk", "rb");
        if (disk == nullptr) {
            std::cerr << "无法打开磁盘文件" << std::endl;
            exit(EXIT_FAILURE);
        }

        buffer_read_block(0);
        buffer_to_directory(root);
    }

    void buffer_read_block(std::size_t block_num) {
        fseek(disk, block_num * buffersize, SEEK_SET);
        fread(buffer, sizeof(char), buffersize, disk);
    }

    void buffer_write_disk(std::size_t block_num) {
        fseek(disk, block_num * buffersize, SEEK_SET);
        fwrite(buffer, sizeof(char), buffersize, disk);
    }

    void buffer_to_directory(Directory& directory) {
        char* buffer_ptr = buffer;  // Create a pointer to traverse the buffer
        memcpy(directory.name, buffer_ptr, namelen);
        buffer_ptr += namelen;

        memcpy(&directory.type, buffer_ptr, sizeof(int));
        buffer_ptr += sizeof(int);

        memcpy(&directory.size, buffer_ptr, sizeof(std::size_t));
        buffer_ptr += sizeof(std::size_t);

        while (buffer_ptr + sizeof(Fnode) <= buffer + buffersize) {
            Fnode node;
            memcpy(&node, buffer_ptr, sizeof(Fnode));
            directory.fnodes.push_back(node);
            buffer_ptr += sizeof(Fnode);
        }
    }
    void directory_to_buffer(const Directory& directory) {
        char* buffer_ptr = buffer;  // Create a pointer to traverse the buffer
        memcpy(buffer_ptr, directory.name, namelen);
        buffer_ptr += namelen;

        memcpy(buffer_ptr, &directory.type, sizeof(int));
        buffer_ptr += sizeof(int);

        memcpy(buffer_ptr, &directory.size, sizeof(std::size_t));
        buffer_ptr += sizeof(std::size_t);

        for (const Fnode& node : directory.fnodes) {
            memcpy(buffer_ptr, node.name, namelen);
            buffer_ptr += namelen;

            memcpy(buffer_ptr, &node.type, sizeof(int));
            buffer_ptr += sizeof(int);

            memcpy(buffer_ptr, &node.size, sizeof(std::size_t));
            buffer_ptr += sizeof(std::size_t);

            memcpy(buffer_ptr, &node.start_block, sizeof(std::size_t));
            buffer_ptr += sizeof(std::size_t);
        }
    }
};

#endif // FS_H
