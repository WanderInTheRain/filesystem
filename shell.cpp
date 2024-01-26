// shell.cpp

#include "shell.h"
#include <sstream>
#include <cstring>

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

//shell
Shell::Shell(const char* disk_filename):manager(disk_filename){
    prefix.push_back(manager.current_dir.dnode.name);
}

std::vector<std::string> Shell::split(std::string input){
    std::istringstream iss(input);
    std::vector<std::string> tokens;

    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

void Shell::print_prefix(){
    for (auto s : prefix){
        std::cout<<"\\"<<s;
    }
    std::cout<<">";
}

void Shell::run(){
    bool run {true};
    std::vector<std::string> tokens;

    while (run){
        print_prefix();

        std::getline(std::cin, cmd);
        tokens = split(cmd);

        if (tokens.empty()){}
        else if (tokens[0] == "quit"){
            run = false;
        }
        else if (tokens[0] == "ls"){
            for (std::size_t i = 0; i < manager.current_dir.nodenum; ++i){
                if (manager.current_dir.fdnodes[i].type == FL){
                    std::cout<<"file:"<<manager.current_dir.fdnodes[i].name<<"  ";
                }
                else if (manager.current_dir.fdnodes[i].type == DIR){
                    std::cout<<"dir:"<<manager.current_dir.fdnodes[i].name<<"  ";
                }
            }
            std::cout<<std::endl;
        }
        else if (tokens[0] == "mkdir"){
            char name[namelen];
            strcpy(name, tokens[1].c_str());
            int start_block = manager.get_block();
            int parent_block = manager.current_dir.dnode.start_block;
            FDnode dirnode(name, DIR, start_block, parent_block);

            manager.current_dir.fdnodes.push_back(dirnode);
            manager.current_dir.nodenum += 1;
            manager.directory_to_buffer(manager.current_dir);
            manager.mem.buffer_write_disk(manager.current_dir.dnode.start_block);

            Directory dir(dirnode,\
             0, std::vector<FDnode>{});
            manager.directory_to_buffer(dir);
            manager.mem.buffer_write_disk(dir.dnode.start_block);
        }
        else if (tokens[0] == "cd"){
            if (tokens[1] == ".."){
                if (strcmp(manager.current_dir.dnode.name,"root")){
                    prefix.pop_back();
                    manager.mem.buffer_read_block(manager.current_dir.dnode.parent_block);
                    manager.buffer_to_directory(manager.current_dir);
                }
            }
            else{
                for (std::size_t i = 0; i < manager.current_dir.nodenum; ++i){
                    if (manager.current_dir.fdnodes[i].name == tokens[1]){
                        prefix.push_back(tokens[1]);
                        manager.mem.buffer_read_block(manager.current_dir.fdnodes[i].start_block);
                        manager.buffer_to_directory(manager.current_dir);
                    }
                }
            }
        }
        else if (tokens[0] == "rm"){
            for (std::size_t i = 0; i < manager.current_dir.nodenum; ++i){
                if (manager.current_dir.fdnodes[i].name == tokens[1]){
                    if (manager.current_dir.fdnodes[i].type == FL){
                        manager.release_block(manager.current_dir.fdnodes[i].start_block);
                        manager.current_dir.fdnodes.erase(\
                            manager.current_dir.fdnodes.begin() + i);
                        manager.current_dir.nodenum -= 1;
                        manager.directory_to_buffer(manager.current_dir);
                        manager.mem.buffer_write_disk(manager.current_dir.dnode.start_block);
                    }
                    else if (manager.current_dir.fdnodes[i].type == DIR){
                        Directory child_dir(FDnode("",1,1,1), 0, std::vector<FDnode>{});
                        manager.mem.buffer_read_block(manager.current_dir.fdnodes[i].start_block);
                        manager.buffer_to_directory(child_dir);
                        rm_directory(child_dir);
                        manager.current_dir.fdnodes.erase(\
                            manager.current_dir.fdnodes.begin() + i);
                        manager.current_dir.nodenum -= 1;
                        manager.directory_to_buffer(manager.current_dir);
                        manager.mem.buffer_write_disk(manager.current_dir.dnode.start_block);
                    }
                }
            }
        }
    }
}

void Shell::rm_directory(Directory dir){
    while (!dir.fdnodes.empty()){
        if (dir.fdnodes[0].type == FL){
            manager.release_block(dir.fdnodes[0].start_block);
            dir.fdnodes.pop_back();
        }
        else if (dir.fdnodes[0].type == DIR){
            Directory child_dir(FDnode("",1,1,1), 0, std::vector<FDnode>{});
            manager.mem.buffer_read_block(dir.fdnodes[0].start_block);
            manager.buffer_to_directory(child_dir);
            rm_directory(child_dir);
            dir.fdnodes.pop_back();
        }
    }
    dir.nodenum = 0;
    manager.release_block(dir.dnode.start_block);
}