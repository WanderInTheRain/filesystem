//run.cpp

#include "shell.h"

void clean_disk(const char* _filename);

int main() {
    Shell shell("disk");

    shell.run();
    /*Manager manager("disk");

    // Create the root directory
    Directory root(FDnode("root", 1, 1, 0), 2, std::vector<FDnode>{});

    // Create an FDnode named "home" inside the root directory
    FDnode homeNode("home", 1, 2 , root.dnode.start_block);

    FDnode tetNode("tet", 1, 3 , root.dnode.start_block);

    // Add the homeNode to the fdnodes vector in the root directory
    root.fdnodes.push_back(homeNode);
    root.fdnodes.push_back(tetNode);

    // Initialize the manager and write the root directory to disk

    manager.directory_to_buffer(root);
    manager.mem.buffer_write_disk(1);

    manager.bitmap[2] = 1;
    manager.bitmap[3] = 1;

    manager.update_bitmap();*/
    return 0;
}
