#include "fd.h"

//FDnode
FDnode::FDnode(const char* _name, int _type, std::size_t _start_block, std::size_t _parent_block) {
        memcpy(name, _name, namelen);
        type = _type;
        start_block = _start_block;
        parent_block = _parent_block;
}

//File
File::File(const FDnode& _fnode, const char* _data) : fnode(_fnode) {
        memcpy(data, _data, blocksize - sizeof(FDnode));
}

//Directory
Directory::Directory(const FDnode& _dnode, std::size_t _nodenum,\
         const std::vector<FDnode>& _fdnodes)
        : dnode(_dnode), nodenum(_nodenum), fdnodes(_fdnodes) {}
