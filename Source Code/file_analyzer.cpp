#include <iostream>
#include <filesystem>
#include "file_analyzer.h"
using namespace std;

namespace fs = filesystem;

tree get_disk(){
    string dir;
    dir = "/";
    uintmax_t s;
    error_code ec1, ec2;
    tree disk;
    disk.root = new node(dir, 0, true, -1);
    node *old_node, *new_node;
    old_node = disk.root;
    int old_depth = -1, new_depth = 0;
    for(auto e = fs::recursive_directory_iterator(dir,fs::directory_options::skip_permission_denied);
        e != fs::recursive_directory_iterator();
        e.increment(ec1))
    {
        if(ec1.value())
            e.pop();
        else
        {
            new_depth = e.depth();
            s = e->file_size(ec2);
            if (e->is_symlink() || e->path() == "/proc/kcore" || ec2.value())   
                s = 0;
            new_node = new node(e->path().filename().string(), s, e->is_directory(ec2), new_depth);
            for (int i = 0; i < old_depth - new_depth + 1; i++)
                old_node = old_node->parent;
            disk.add_node(old_node, new_node);
            old_depth = new_depth;
            old_node = new_node;
        }
    }
    disk.update_sizes();
    return disk;
}