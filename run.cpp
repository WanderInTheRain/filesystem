//run.cpp

#include "shell.h"

void clean_disk(const char* _filename);
void init_disk(const char* _filename);

int main() {
    //clean_disk("disk");
    //init_disk("disk");
    
    Shell shell("disk");

    shell.run();
    
    return 0;
}
