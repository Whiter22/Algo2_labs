#include <iostream>
#include <time.h>
#include <random>
#include <string>
#include <vector>
#include <conio.h>

struct Book{
    int book_id;
    std::string title;
};

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

    void left_rotate(Node<T>* x) {
        Node<T>* y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void right_rotate(Node<T>* x) {
        Node<T>* y = x->left;
        x->left = y->right;
        if (y->right != nullptr) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->right = x;
        x->parent = y;
    } 

    void balance_Tree(Node<T>* node) {
        while (node != root && node->parent->color == 0) {
            if (node->parent == node->parent->parent->left) {
                auto y = node->parent->parent->right;
                if (y != nullptr && y->color == 0) {
                    node->parent->color = 1;
                    y->color = 1;
                    node->parent->parent->color = 0;
                    node = node->parent->parent;
                } 
                else {
                    if (node == node->parent->right) {
                        node = node->parent;
                        left_rotate(node);
                    }
                    node->parent->color = 1;
                    node->parent->parent->color = 0;
                    right_rotate(node->parent->parent);
                }
            } 
            else {
                auto y = node->parent->parent->left;
                if (y != nullptr && y->color == 0) {
                    node->parent->color = 1;
                    y->color = 1;
                    node->parent->parent->color = 0;
                    node = node->parent->parent;
                } 
                else {
                    if (node == node->parent->left) {
                        node = node->parent;
                        right_rotate(node);
                    }
                    node->parent->color = 1;
                    node->parent->parent->color = 0;
                    left_rotate(node->parent->parent);
                }
            }
        }
        root->color = 1;
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

        balance_Tree(node);

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

    void inorder_tree(Node<T>* node){
        if(node==nullptr){
            return;
        }

        inorder_tree(node->left);
        nodes_inorder.push_back(node);
        inorder_tree(node->right);
    }

    void clear_Tree(void (*remove_Node)(Node<T>*)){
        delete_all_Nodes(root, remove_Node);

        nodes.clear();
        nodes_inorder.clear();
        root = nullptr;
    }

    Node<T>* find_Node(T key, bool (*cmp)(T,T)){
        auto node = root;
        while(node != nullptr && node->data != key){
            if(cmp(key, node->data))
                node = node->right;
            else
                node = node->left;
        }
        if(node == nullptr)
            return nullptr;
        return node;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

    bool get_color(Node<T>* node){
        if(node==nullptr)
            return 1;
        else
            return node->color;
    }

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
std::string show_Tree_obj(RBTree<T>* Tree){
    std::string to_return = "binary search tree: \n";
    to_return += "\tsize: " + std::to_string(Tree->get_size()) + "\n";
    to_return += "\theight: " + std::to_string(Tree->get_height(Tree->get_root())) + "\n";
    to_return += "\t\t\t\tRED 0 : BLACK 1\n";
    to_return += "\t{\n";

    if(Tree->get_size()<=1000){
        for(auto node : Tree->get_vector()){
            to_return += "\t\t(" + std::to_string(node->index) + ": [" + std::to_string(node->color) + ", ";
            std::string parent_str = (node->parent == nullptr) ? "p: NULL, " : "p: " + std::to_string(node->parent->index) + ", "; 
            std::string left_str = (node->left == nullptr) ? "l: NULL, " : "l: " + std::to_string(node->left->index) + ", ";
            std::string right_str = (node->right == nullptr) ? "r: NULL], " : "r: " + std::to_string(node->right->index) + "], "; 

            to_return += parent_str + left_str + right_str + "data: " + std::to_string(node->data) + ")\n"; 
        }
        to_return += "\t}";
    } else{
        int i;
        auto node = Tree->get_vector();
        for(i=0;i<50;i++){
            to_return += "\t\t(" + std::to_string(node[i]->index) + ": [" + std::to_string(node[i]->color) + ", ";
            std::string parent_str = (node[i]->parent == nullptr) ? "p: NULL, " : "p: " + std::to_string(node[i]->parent->index) + ", "; 
            std::string left_str = (node[i]->left == nullptr) ? "l: NULL, " : "l: " + std::to_string(node[i]->left->index) + ", ";
            std::string right_str = (node[i]->right == nullptr) ? "r: NULL], " : "r: " + std::to_string(node[i]->right->index) + "], "; 

            to_return += parent_str + left_str + right_str + "data: " + std::to_string(node[i]->data) + ")\n"; 
        }
        to_return += "\t}";
        to_return += "\tShowed first: " + std::to_string(i++) + " elements of " + std::to_string(Tree->get_size()) + " possible";
    }
    return to_return;
}

std::string show_Tree_obj(RBTree<Book*>* Tree){
    std::string to_return = "binary search tree: \n";
    to_return += "\tsize: " + std::to_string(Tree->get_size()) + "\n";
    to_return += "\theight: " + std::to_string(Tree->get_height(Tree->get_root())) + "\n";
    to_return += "\t\t\t\tRED 0 : BLACK 1\n";
    to_return += "\t{\n";
    if(Tree->get_size()<=1000){
        for(auto node : Tree->get_vector()){
            to_return += "\t\t(" + std::to_string(node->index) + ": [" + std::to_string(node->color) + ", ";;
            std::string parent_str = (node->parent == nullptr) ? "[p: NULL, " : "p: " + std::to_string(node->parent->index) + ", "; 
            std::string left_str = (node->left == nullptr) ? "l: NULL, " : "l: " + std::to_string(node->left->index) + ", ";
            std::string right_str = (node->right == nullptr) ? "r: NULL], " : "r: " + std::to_string(node->right->index) + "], "; 

            to_return += parent_str + left_str + right_str + "data: " + node->data->title + ")\n"; 
        }
        to_return += "\t}";
    } else{
        int i;
        auto node = Tree->get_vector();
        for(i=0;i<50;i++){
            to_return += "\t\t(" + std::to_string(node[i]->index) + ": [" + std::to_string(node[i]->color) + ", ";;
            std::string parent_str = (node[i]->parent == nullptr) ? "[p: NULL, " : "p: " + std::to_string(node[i]->parent->index) + ", "; 
            std::string left_str = (node[i]->left == nullptr) ? "l: NULL, " : "l: " + std::to_string(node[i]->left->index) + ", ";
            std::string right_str = (node[i]->right == nullptr) ? "r: NULL], " : "r: " + std::to_string(node[i]->right->index) + "], "; 

            to_return += parent_str + left_str + right_str + "data: " + node[i]->data->title + ")\n"; 
        }
        to_return += "\t}";
        to_return += "\tShowed first: " + std::to_string(i++) + " elements of " + std::to_string(Tree->get_size()) + " possible";
    }
    return to_return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void remove_Node(Node<int>* node){ delete node; }

void remove_Node(Node<Book*>* node){ 
    delete node->data; 
    delete node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool comparator(char a, char b){ return a>b; };

bool comparator(int a, int b){ return a>b; }

bool comparator(Book* a, Book* b){ return a->book_id>b->book_id; }

double height_to_dsize(int height, int size){ return static_cast<double>(height)/size; }

double height_to_log_size(int height, int log_p2){ return static_cast<double>(height)/log_p2; }

int main(){ 
    // for ints
    std::string result;
    const int MAX_ORDER = 7;
    auto T0 = new RBTree<int>();

    for(int o=1; o<=MAX_ORDER; o++){
        // auto T0 = new RBTree<int>();
        const int n = pow(10, o);
        //adding
        clock_t t1 = clock();
        for(int i=0; i<n; i++)
            T0->add_Node((rand()<<15)+rand(), comparator);
        clock_t t2 = clock();
        // T0->preorder_tree(T0->get_root());
        result = T0->show_Tree(show_Tree_obj);
        std::cout << result << '\n' << "\tFor size: " << n << std::endl;

        double full_time = double(t2-t1)/double(CLOCKS_PER_SEC);
        std::cout << "\n";
        std::cout << "Full time spent on adding to Tree: " << full_time*1000 << " ms" << std::endl;
        
        //searching
        const int m = pow(10, 4);
        int hits = 0;
        int rand_num;
        t1=clock();
        for(int i=0;i<m;i++){
            rand_num = rand();
            auto result = T0->find_Node(rand_num, comparator);
            if(result != nullptr)
                hits++;
        }
        t2=clock();
        double full_time1 = double(t2-t1)/double(CLOCKS_PER_SEC);

        std::cout << "Full time spent on searching: " << full_time1*1000 << " ms" << std::endl;
        std::cout << "Hits: " << hits << std::endl;

        double height_size = height_to_dsize(T0->get_height(T0->get_root()), T0->get_size());
        double log2_size = log2(T0->get_size());
        double height_logdsize = height_to_log_size(T0->get_height(T0->get_root()), log2_size);

        std::cout << "Height to size ratio: " << height_size << '\n';
        std::cout << "Log2 data size: " << log2_size << '\n';
        std::cout << "Height to log2 data size: " << height_logdsize << '\n';

        _getch();
        T0->clear_Tree(remove_Node); 
        // delete T0;
    }
    // T0->clear_Tree(remove_Node);
    delete T0;
    std::cout << "END" << std::endl;
    return 0;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // auto T2 = new RBTree<char>;
    // std::string str_tree;
    // std::string letters = "abcdefghijklmnopqrstuvwxyz";

    // for(int i=0; i<10; i++)
    //     T2->add_Node(letters[rand()%26], comparator);

    // str_tree = T2->show_Tree(show_Tree_obj);
    // std::cout << str_tree << std::endl;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //for Books
    // auto T1 = new RBTree<Book*>;
    // std::string str_tree;
    //  std::string letters = "abcdefghijklmnopqrstuvwxyz";
    // for(int i=0;i<10;i++){
    //     Book* book = new Book;
    //     book->book_id = (rand()<<15)+rand();
    //     int rand_index = rand() % letters.length();
    //     book->title = letters[rand_index];
    //     T1->add_Node(book, comparator);
    // }

    // str_tree = T1->show_Tree(show_Tree_obj);
    // std::cout << str_tree << std::endl;


    // const int MAX_ORDER = 7;
    // std::string result;
    // // auto T0 = new RBTree<int>();

    // for(int o=1; o<=MAX_ORDER; o++){
    //     const int n = pow(10, o);

    //     clock_t t1 = clock();
    //     for(int i=0;i<n;i++){
    //         Book* book = new Book;
    //         book->book_id = rand();
    //         int rand_index = rand() % letters.length();
    //         book->title = letters[rand_index];
    //         T1->add_Node(book, comparator);
    //     }
    //     // for(int i=0; i<n; i++)
    //     //     T0->add_Node((rand()<<15)+rand(), comparator);
    //     clock_t t2 = clock();
    //     // T0->preorder_tree(T0->get_root());
    //     result = T1->show_Tree(show_Tree_obj);
    //     std::cout << result << '\n' << "\tFor size: " << n << std::endl;

    //     double full_time = double(t2-t1)/double(CLOCKS_PER_SEC);
    //     std::cout << "\n";
    //     std::cout << "Full time spent on adding to Tree: " << full_time*1000 << " ms" << std::endl;
        
    //     //searching
    //     const int m = pow(10, 4);
    //     int hits = 0;
    //     int rand_num;
    //     t1=clock();
    //     for(int i=0;i<m;i++){
    //         Book* book = new Book;
    //         book->book_id = (rand()<<15)+rand();
    //         int rand_index = rand() % letters.length();
    //         auto result = T1->find_Node(book, comparator);
    //         if(result != nullptr)
    //             hits++;
    //         delete book;
    //     }
    //     t2=clock();
    //     double full_time1 = double(t2-t1)/double(CLOCKS_PER_SEC);

    //     std::cout << "Full time spent on searching: " << full_time1*1000 << " ms" << std::endl;
    //     std::cout << "Hits: " << hits << std::endl;

    //     double height_size = height_to_dsize(T1->get_height(T1->get_root()), T1->get_size());
    //     double log2_size = log2(T1->get_size());
    //     double height_logdsize = height_to_log_size(T1->get_height(T1->get_root()), log2_size);

    //     std::cout << "Height to size ratio: " << height_size << '\n';
    //     std::cout << "Log2 data size: " << log2_size << '\n';
    //     std::cout << "Height to log2 data size: " << height_logdsize << '\n';

    //     _getch();
        // T1->clear_Tree(remove_Node); 
    // }
    // T1->clear_Tree(remove_Node);
    // delete T1;
    // std::cout << "END" << std::endl;
    // return 0;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // std::string res;
    // auto T0 = new RBTree<int>();
    // std::vector<int> a ={55,69,62,57,35,83,72,74};

    // for(int i=0; i<8; i++)
    //     T0->add_Node(a[i], comparator);

    // res = T0->show_Tree(show_Tree_obj);
    // std::cout << res << '\n';
    // std::cout << "XPP" << std::endl;

    // auto x = T0->get_vector();

    // for(auto j : x){
    //     std::cout << j->data << " ";
    // }
    // std::cout << std::endl;

    // auto result = T0->find_Node(83, comparator);
    // std::cout << result->data <<std::endl;

    // T0->clear_Tree(remove_Node);
    // delete T0;
    // return 0;
    // std::cout << "END" << std::endl;
}