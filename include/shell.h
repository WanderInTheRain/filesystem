#ifndef SHL_H
#define SHL_H

#include "manager.h"
#include "vi.h"

class Shell{
public:
    Manager manager;
    std::vector<std::string> prefix;
    std::string cmd;

    Shell(const char* disk_filename);
    void run();
private:
    void rm_directory(Directory dir);
    void print_prefix();
    std::vector<std::string> split(std::string input);
};

#endif