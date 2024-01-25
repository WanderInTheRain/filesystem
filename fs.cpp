// fs.cpp

#include "fs.h"

int main() {
    Manager manager;

    // Create the root directory
    Directory root;
    memcpy(root.name, "root", namelen);  // Copy the name "root" to the root directory
    root.type = 1;  // Directory type
    root.size = sizeof(Directory);  // Size of the directory structure
    root.start_block = 0;  // Starting block for the root directory

    // Create an Fnode named "home" inside the root directory
    Fnode homeNode;
    memcpy(homeNode.name, "home", namelen);  // Copy the name "home" to the Fnode
    homeNode.type = 0;  // File type
    homeNode.size = sizeof(Fnode);  // Size of the Fnode structure
    homeNode.start_block = 1;  // Starting block for the Fnode
    // You can set the data of the Fnode if needed

    // Add the homeNode to the fnodes vector in the root directory
    root.fnodes.push_back(homeNode);

    // Initialize the manager and write the root directory to disk
    manager.init();
    manager.directory_to_buffer(root);
    manager.buffer_write_disk(0);

    // Close the disk file
    fclose(manager.disk);

    return 0;
}
