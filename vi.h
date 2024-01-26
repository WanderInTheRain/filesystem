#ifndef VI_H
#define VI_H

#include "memory.h"
#include "fd.h"

class Vi{
public:
    File file;

    Vi(File _file);
    void run();
private:
    std::string read_text();
};

#endif