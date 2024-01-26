#ifndef MAN_H
#define MAN_h

#include "memory.h"
#include "fd.h"

class Manager {
public:
    Directory current_dir;
    Memory mem;
    int bitmap[blocknum];

    Manager(const char* disk_filename);
    void init();
    void buffer_to_file(File& file);
    void file_to_buffer(const File& file);
    void buffer_to_directory(Directory& directory);
    void directory_to_buffer(const Directory& directory);
    void init_bitmap();
    void update_bitmap();
    int get_block();
    void release_block(int blknum);
};

#endif