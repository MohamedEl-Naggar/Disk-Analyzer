#include <vector>
#include <string>
using namespace std;

class node {
    public:
        node(string, uintmax_t, bool, int);
        ~node();
        string name;
        uintmax_t size;
        bool is_directory;
        int num_children = 0;
        int depth;
        node* parent = NULL;
        vector <node*> children;
        node* get_next_child();
        int cur_child = 0;
};

class tree {
    public:
        tree();
        ~tree();
        node* root;
        void add_node(node*, node*);
        void update_sizes();
        node* dfs_iterator();
    private:
        void update_sizes(node*);
};