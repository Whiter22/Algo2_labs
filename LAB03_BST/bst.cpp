#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <conio.h>
#include <vector>
#include <math.h>

struct Book{
    int book_id;
    std::string title;
};

template <typename T>
struct Node{
    T data;
    int index;
    Node* parent;
    Node* left;
    Node* right;

    Node(T x) : data(x), left(nullptr), right(nullptr), parent(nullptr) {}
};

template <typename T>
class Tree{
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
public:
    Tree() : root(nullptr), size(0) {}
    
    void add_Node(T data, bool (*cmp)(T,T)){
        if(root == nullptr){
            root = new Node<T>(data);
            root->index = size;
            root->parent = nullptr;
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

    std::string show_Tree(std::string (*show_tree)(Tree<T>*)){ 
        this->preorder_tree(root);
        return show_tree(this);
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

    void clear_Tree(void (*remove_Node)(Node<T>*)){
        delete_all_Nodes(root, remove_Node);

        nodes.clear();
        nodes_inorder.clear();
    }

    void delete_Node(Node<T>* node, bool (*cmp)(T,T)){
        if(node->left == nullptr && node->right == nullptr){
            if(cmp(node->data, node->parent->data))
                node->parent->right = nullptr;
            else
                node->parent->left = nullptr;

            size--;
            delete node;
        }
        else if(node->left == nullptr && node->right != nullptr){
            node->right->parent = node->parent;
            if(cmp(node->data, node->parent->data))
                node->parent->right = node->right;
            else
                node->parent->left = node->right;
            
            size--;
            delete node;
        }
        else if(node->left != nullptr && node->right == nullptr){
            node->left->parent = node->parent;
            if(cmp(node->data, node->parent->data))
                node->parent->right = node->left;
            else
                node->parent->left = node->left;

            size--;
            delete node;
        }
        else{
            auto temp = node;
            node = node->right;
            while(node->left != nullptr)
                node = node->left;

            if(temp == root && node->right != nullptr){
                node->right->parent = node->parent;
                node->parent->left = node->right;

                node->left = root->left;
                root->left->parent = node;

                node->right = root->right;
                root->right->parent = node;

                delete root;
                root = nullptr;
                root = node;

                size--;
            } 
            else if(temp == root && node->right == nullptr) {
                node->left = root->left;
                node->right = root->right;

                root->right->parent = node;
                root->left->parent = node;

                node->parent->left = nullptr;

                delete root;
                root = nullptr;
                root = node;

                size--;
            } 
            else {
                if(node == temp->right){
                    node->left = temp->left;
                    node->parent = temp->parent;
                    if(cmp(node->data, root->data))
                        temp->parent->right = node;
                    else
                        temp->parent->left = node;

                    temp->left->parent = node;
                    temp->right = nullptr;

                    delete temp;
                    size--;
                }
                else if(node->right != nullptr){
                    node->parent->left = node->right;
                    node->right->parent = node->parent;

                    node->parent = temp->parent;
                    if(cmp(node->data, root->data))
                        temp->parent->right = node;
                    else
                        temp->parent->left = node;

                    node->right = temp->right;
                    temp->right->parent = node;

                    node->left = temp->left;
                    temp->left->parent = node;

                    delete temp;
                    size--;
                } else{
                    node->parent->left = nullptr;

                    node->parent = temp->parent;
                    if(cmp(node->data, root->data))
                        temp->parent->right = node;
                    else
                        temp->parent->left = node;

                    node->right = temp->right;
                    temp->right->parent = node;
                    
                    node->left = temp->left;
                    temp->left->parent = node;

                    delete temp;
                    size--;
                }
            }
        }
    }

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

    Node<T>* get_root(){ return root; }
    
    std::vector<Node<T>*> get_vector(){ return nodes; }

    std::vector<Node<T>*> get_vector_inorder(){ std::cout << "wtf" << std::endl; return nodes_inorder; }

    int get_size(){ return size; }
}; 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool comparator(char a, char b){ return a>b; };

bool comparator(int a, int b){ return a>b; }

bool comparator(Book* a, Book* b){ return a->book_id>b->book_id; }

void remove_Node(Node<int>* node){ delete node; }

void remove_Node(Node<Book*>* node){ 
    delete node->data; 
    delete node;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
std::string show_Tree_obj(Tree<T>* Tree){
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

std::string show_Tree_obj(Tree<Book*>* Tree){
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

            to_return += parent_str + left_str + right_str + "data: " + node->data->title + ")\n"; 
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

            to_return += parent_str + left_str + right_str + "data: " + node[i]->data->title + ")\n"; 
        }
        to_return += "\t}";
        to_return += "\tShowed first: " + std::to_string(i++) + " elements of " + std::to_string(Tree->get_size()) + " possible";
    }
    return to_return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double height_to_dsize(int height, int size){ return height/size; }

double height_to_log_size(int height, int log_p2){ return height/log_p2; }

int main(){
    //adding
    std::string result;
    const int MAX_ORDER = 7;
    auto T0 = new Tree<int>();

    for(int o=1; o<=MAX_ORDER; o++){
        const int n = pow(10, o);

        clock_t t1 = clock();
        for(int i=0; i<n; i++)
            T0->add_Node((rand()<<15)+rand(), comparator);
        clock_t t2 = clock();
        // T0->preorder_tree(T0->get_root());
        result = T0->show_Tree(show_Tree_obj);
        std::cout << result << std::endl;

        double full_time = double(t2-t1)/double(CLOCKS_PER_SEC);
        std::cout << "Full time spent on adding to Tree: " << full_time*1000 << " ms" << std::endl;
        std::cout << "\n";
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
    }
    
    T0->clear_Tree(remove_Node);
    delete T0;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // std::string answer;
    // auto T1 = new Tree<char>();
    // std::string letters = "abcdefghijklmnopqrstuvwxyz";

    // for(int i=0;i<10;i++){
    //     T1->add_Node(letters[rand()%27], comparator);
    // }
    // answer = T1->show_Tree(show_Tree_obj);
    // std::cout << answer << std::endl;


    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // auto T0 = new Tree<int>();
    // auto T0 = new Tree<Book*>;
    // std::string str_tree;
    //  std::string letters = "abcdefghijklmnopqrstuvwxyz";
    // for(int i=0;i<10;i++){
    //     Book* book = new Book;
    //     book->book_id = rand();
    //     int rand_index = rand() % letters.length();
    //     book->title = letters[rand_index];
    //     T0->add_Node(book, comparator);
    // }

    // // T0->preorder_tree(T0->get_root());
    // str_tree = T0->show_Tree(show_Tree_obj);
    // std::cout << str_tree << std::endl;

    // T0->clear_Tree(remove_Node);

    // std::cout << "po usunieciu" << "\n\n";
    // str_tree = T0->show_Tree(show_Tree_obj);
    // std::cout << str_tree << std::endl;

    // std::cout << "dziala?" << std::endl;

    // T0->inorder_tree(T0->get_root());
    // auto xpp = T0->get_vector_inorder();
    // for(auto x : xpp){
    //     std::cout << x->data->title << '\n';
    // }
    // std::cout << "xpp" << std::endl;

    // std::cout << "xD0" << std::endl;
    // std::vector<int> nums = {10,15,12,5,30,25,35,-2,33,7};//{10,15,12,5,30,25,35,8,-2,33,26,6,7};//{9,15,5,10,3,7};
    // for(int x : nums)
    //     T0->add_Node(x, comparator);
    // std::cout << "xD9" << std::endl;
    // T0->delete_Node(T0->find_Node(30, comparator), comparator);
    // std::cout << "xD!" << std::endl;


    // clock_t t1 = clock();
    // for(int i=0; i<10000000; i++){
    //     T0->add_Node((rand()<<15)+rand(), comparator);
    // }
    // clock_t t2 = clock();

    // double spent_time = double(t2-t1)/double(CLOCKS_PER_SEC);
    // std::cout << "Time spent on adding items to the Tree: " << spent_time*1000 << " ms" << std::endl;
    // std::cout << "xD1" << std::endl;
    // T0->preorder_tree(T0->get_root());
    // std::cout << "xD2" << std::endl;

    // // auto xd = T0->get_vector();
    // // for(auto c : xd)
    // //     std::cout << c->data << '\n';  
    // str_tree = T0->show_Tree(show_Tree_obj);

    // auto node = T0->find_Node();
    // std::cout << "Found: " << node->data << std::endl;

    // std::cout << str_tree << std::endl;
    
    // // std::cout << T0->get_height(T0->get_root()) << std::endl;
    std::cout << "End" << std::endl;
    _getch();
    return 0;
}