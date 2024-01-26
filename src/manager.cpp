#include "manager.h"

//Manager
Manager::Manager(const char* disk_filename):current_dir(FDnode("", 1, 0, 0), 0, std::vector<FDnode>()),\
 mem(disk_filename){
    init();
}

void Manager::init(){
    init_bitmap();
    mem.buffer_read_block(1);
    buffer_to_directory(current_dir);
}

int Manager::get_block(){
    int blknum=0;
    while (bitmap[blknum] == 1){
        blknum++;
    };
    bitmap[blknum] = 1;
    update_bitmap();
    return blknum;
}

void Manager::release_block(int blknum){
    bitmap[blknum] = 0;
    update_bitmap();
}

void Manager::init_bitmap(){
    mem.buffer_read_block(0);
    char* buffer_ptr = mem.buffer;
    memcpy(bitmap, buffer_ptr, sizeof(int [blocknum]));
    buffer_ptr += sizeof(int [blocknum]);
}

void Manager::update_bitmap(){
    char* buffer_ptr = mem.buffer;
    memcpy(buffer_ptr, bitmap, sizeof(int [blocknum]));
    buffer_ptr += sizeof(int [blocknum]);
    mem.buffer_write_disk(0);
}

void Manager::buffer_to_file(File& file){
    char* buffer_ptr = mem.buffer;
    memcpy(&file.fnode, buffer_ptr, sizeof(FDnode));
    buffer_ptr += sizeof(FDnode);

    memcpy(file.data, buffer_ptr, blocksize - sizeof(FDnode));
    buffer_ptr += blocksize - sizeof(FDnode);
}
void Manager::file_to_buffer(const File& file){
    char* buffer_ptr = mem.buffer;
    memcpy(buffer_ptr, &file.fnode, sizeof(FDnode));
    buffer_ptr += sizeof(FDnode);

    memcpy(buffer_ptr, file.data, blocksize - sizeof(FDnode));
    buffer_ptr += blocksize - sizeof(FDnode);
}

void Manager::buffer_to_directory(Directory& directory) {
    char* buffer_ptr = mem.buffer;
    memcpy(&directory.dnode, buffer_ptr, sizeof(FDnode));
    buffer_ptr += sizeof(FDnode);

    memcpy(&directory.nodenum, buffer_ptr, sizeof(std::size_t));
    buffer_ptr += sizeof(std::size_t);

    directory.fdnodes.clear();

    for (std::size_t i = 0; i < directory.nodenum; ++i) {
        FDnode node("", 1, 0, 0);
        memcpy(&node, buffer_ptr, sizeof(FDnode));
        directory.fdnodes.push_back(node);
        buffer_ptr += sizeof(FDnode);
    }
}

void Manager::directory_to_buffer(const Directory& directory) {
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
