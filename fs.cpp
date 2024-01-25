// fs.cpp

#include "fs.h"

int main() {
    Manager manager;
    Memory mem("disk");

    // Create the root directory
    Directory root(FDnode("root", 1, 0, 0), 2, std::vector<FDnode>{});

    // Create an FDnode named "home" inside the root directory
    FDnode homeNode("home", 0, 1 , root.dnode.start_block);

    FDnode tetNode("tet", 0, 2 , root.dnode.start_block);

    // Add the homeNode to the fdnodes vector in the root directory
    root.fdnodes.push_back(homeNode);
    root.fdnodes.push_back(tetNode);

    // Initialize the manager and write the root directory to disk
    manager.init(mem);
    manager.directory_to_buffer(root,mem);
    mem.buffer_write_disk(0);

    // Close the disk file

    return 0;
}
