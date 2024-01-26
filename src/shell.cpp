// shell.cpp

#include "shell.h"
#include <sstream>
#include <cstring>

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
                    if (manager.current_dir.fdnodes[i].name == tokens[1] &&\
                     manager.current_dir.fdnodes[i].type == DIR){
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
        else if (tokens[0] == "touch"){
            char name[namelen];
            strcpy(name, tokens[1].c_str());
            int start_block = manager.get_block();
            int parent_block = manager.current_dir.dnode.start_block;
            FDnode fnode(name, FL, start_block, parent_block);

            manager.current_dir.fdnodes.push_back(fnode);
            manager.current_dir.nodenum += 1;
            manager.directory_to_buffer(manager.current_dir);
            manager.mem.buffer_write_disk(manager.current_dir.dnode.start_block);

            File file(fnode,"test");
            manager.file_to_buffer(file);
            manager.mem.buffer_write_disk(file.fnode.start_block);
        }
        else if (tokens[0] == "vi"){
            for (std::size_t i = 0; i < manager.current_dir.nodenum; ++i){
                if (manager.current_dir.fdnodes[i].name == tokens[1] &&\
                 manager.current_dir.fdnodes[i].type == FL){
                    Vi vi(File(FDnode("",1,1,1),""));
                    manager.mem.buffer_read_block(manager.current_dir.fdnodes[i].start_block);
                    manager.buffer_to_file(vi.file);
                    vi.run();
                    manager.file_to_buffer(vi.file);
                    manager.mem.buffer_write_disk(vi.file.fnode.start_block);
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