#include <iostream>
#include <time.h>
#include <random>
#include <string>
#include <vector>

template <typename T>
struct Node{
    T data;
    int index;
    bool color; // 0 = red, 1 = black
    Node* parent;
    Node* left;
    Node* right;

    Node(T x) : data(x), left(nullptr), right(nullptr), parent(nullptr), color(0) {}
};

template <typename T>
class RBTree{
    Node<T>* root;
    int size;
    std::vector<Node<T>*> nodes;
    std::vector<Node<T>*> nodes_inorder;

    void delete_all_Nodes(Node<T>* node, void (*remove_Node)(Node<T>*)){
        if(node == nullptr)
            return;

        delete_all_Nodes(node->left, remove_Node);
        delete_all_Nodes(node->right, remove_Node);
        remove_Node(node);
        size = 0;
    } 

    left_rotate(Node<T>* node){
        if(node->parent == root)
            root = node;
        else{
            if(node->parent->parent->left == node->parent)
                node->parent->parent->left = node;
            else
                node->parent->parent->right = node;
        }

        node->parent->right = node->left;
        if(node->left != nullptr)
            node->left->parent = node->parent;

        node->left = node->parent;
        
        node->parent = node->parent->parent;
        
        node->parent->parent = node;
    }

    right_rotate(Node<T>* node){
        if(node->parent == root)
            root=node;
        else{
            if(node->parent->parent->left == node->parent)
                node->parent->parent->left = node;
            else
                node->parent->parent->right = node;
        }

        node->parent->left = node->right;
        if(node->right != nullptr)
            node->right->parent = node->parent;

        node->right = node->parent;

        node->parent = node->parent->parent;

        node->parent->parent = node;
    }

    bool get_color(Node<T>* node){
        if(node==nullptr)
            return 1;
        else
            return node->color;
    }

    void balance_tree(Node<T>* x_node){
        std::cout << root->data << std::endl;
        std::cout << x_node->data << std::endl;
        while(x_node != root && get_color(x_node->parent) == 0){
            if(x_node->parent->data < x_node->parent->parent->data){ //tu sie wykrzacza jak rodzic jest root'em wszedzie gdzie jest x_node->parent->parent i dziad jest rootem 
                auto y = x_node->parent->parent->right;
                if(get_color(y) == 0){
                    x_node->parent->color = 1;
                    y->color = 1;
                    x_node->parent->parent->color = 0;
                    x_node = x_node->parent->parent;
                }
                else{
                    if(x_node == x_node->parent->right){
                        left_rotate(x_node);
                        x_node = x_node->parent;
                    }
                    x_node->parent->color = 1;
                    x_node->parent->parent->color = 0;
                    right_rotate(x_node->parent);
                }
            }
            else{
                auto y = x_node->parent->parent->left;
                if(get_color(y) == 0){
                    x_node->parent->color = 1;
                    y->color = 1;
                    x_node->parent->parent->color = 0;
                    x_node = x_node->parent->parent;
                }
                else{
                    if(x_node == x_node->parent->left){
                        right_rotate(x_node);
                        x_node = x_node->parent;
                    }
                    x_node->parent->color = 1;
                    x_node->parent->parent->color = 0;
                    left_rotate(x_node->parent);
                }
            }
            root->color = 1;
        }
    }

public:
    RBTree() : root(nullptr), size(0) {}
    
    void add_Node(T data, bool (*cmp)(T,T)){
        if(root == nullptr){
            root = new Node<T>(data);
            root->index = size;
            root->parent = nullptr;
            root->color = 1;
            size++;
            return;
        }

        auto node = root;
        auto temp = root;
        bool side = false;
        while(node != nullptr){
            temp = node;
            if(cmp(data, node->data)){
                node = node->right;
                side = false;
            }
            else{
                node = node->left;
                side = true;
            }
        }
        node = new Node<T>(data);

        if(!side)
            temp->right = node;
        else
            temp->left = node;

        node->parent = temp;
        node->index = size;

        balance_tree(node);

        size++;
    }

    void preorder_tree(Node<T>* node){
        if(node == nullptr){
            return;   
        }

        nodes.push_back(node);
        preorder_tree(node->left);
        preorder_tree(node->right);
    }

    ///////////////////////////////////////////////////////////////////////

    std::string show_Tree(std::string (*show_tree)(RBTree<T>*)){ 
        this->preorder_tree(root);
        return show_tree(this);
    }

    Node<T>* get_root(){ return root; }

    std::vector<Node<T>*> get_vector(){ return nodes; }

    int get_size(){ return size; }

    int get_height(Node<T>* node){
        if(node == nullptr)
            return 0;
        else{
            int lheight = get_height(node->left);
            int rheight = get_height(node->right);

            if(lheight > rheight)
                return lheight+1;
            else
                return rheight+1;
        }
    }

};

template <typename T>
std::string show_Tree_obj(RBTree<T>* Tree){
    std::string to_return = "binary search tree: \n";
    to_return += "\tsize: " + std::to_string(Tree->get_size()) + "\n";
    to_return += "\theight: " + std::to_string(Tree->get_height(Tree->get_root())) + "\n";
    to_return += "\t{\n";

    if(Tree->get_size()<=1000){
        for(auto node : Tree->get_vector()){
            to_return += "\t\t(" + std::to_string(node->index) + ": ";
            std::string parent_str = (node->parent == nullptr) ? "[p: NULL, " : "p: " + std::to_string(node->parent->index) + ", "; 
            std::string left_str = (node->left == nullptr) ? "l: NULL, " : "l: " + std::to_string(node->left->index) + ", ";
            std::string right_str = (node->right == nullptr) ? "r: NULL], " : "r: " + std::to_string(node->right->index) + "], "; 

            to_return += parent_str + left_str + right_str + "data: " + std::to_string(node->data) + ")\n"; 
        }
        to_return += "\t}";
    } else{
        int i;
        auto node = Tree->get_vector();
        for(i=0;i<50;i++){
            to_return += "\t\t(" + std::to_string(node[i]->index) + ": ";
            std::string parent_str = (node[i]->parent == nullptr) ? "[p: NULL, " : "p: " + std::to_string(node[i]->parent->index) + ", "; 
            std::string left_str = (node[i]->left == nullptr) ? "l: NULL, " : "l: " + std::to_string(node[i]->left->index) + ", ";
            std::string right_str = (node[i]->right == nullptr) ? "r: NULL], " : "r: " + std::to_string(node[i]->right->index) + "], "; 

            to_return += parent_str + left_str + right_str + "data: " + std::to_string(node[i]->data) + ")\n"; 
        }
        to_return += "\t}";
        to_return += "\tShowed first: " + std::to_string(i++) + " elements of " + std::to_string(Tree->get_size()) + " possible";
    }
    return to_return;
}

bool comparator(char a, char b){ return a>b; };

bool comparator(int a, int b){ return a>b; }

int main(){
    std::string res;
    auto T0 = new RBTree<int>();
    std::vector<int> a ={55,69,62,57,35,83,72,74};

    for(int i=0; i<8; i++)
        T0->add_Node(a[i], comparator);

    res = T0->show_Tree(show_Tree_obj);
    std::cout << res << '\n';
    std::cout << "XPP" << std::endl;
    return 0;
}