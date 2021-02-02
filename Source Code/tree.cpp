#include "tree.h"

node::node(string n, uintmax_t s, bool is, int d){
    name = n;
    size = s;
    is_directory = is;
    depth = d;
}

node* node::get_next_child()
{
    if (num_children == cur_child) 
        return NULL;
    cur_child++;
    return children[cur_child - 1];
}

node::~node(){}

tree::tree(){}

void tree::add_node(node* parent, node* child){
    parent->children.push_back(child);
    parent->num_children++;
    child->parent = parent;
}

void tree::update_sizes(){
    update_sizes(root);
}

void tree::update_sizes(node* cur_node){
    if(!cur_node->is_directory)
        return;
    for (int i = 0; i < cur_node->num_children; i++)
    {
        update_sizes(cur_node->children[i]);
        cur_node->size += cur_node->children[i]->size;
        if (cur_node->children[i]->is_directory)
            cur_node->size += 4100; //directory overhead size
    }
}

node* tree::dfs_iterator()
{
    static node* cur_node = root;
    node* next_node;
    next_node = cur_node->get_next_child();
    while (next_node == NULL && cur_node != root)
    {
        cur_node = cur_node->parent;
        next_node = cur_node->get_next_child();
    }
    cur_node = next_node;
    return cur_node;
}

tree::~tree(){}