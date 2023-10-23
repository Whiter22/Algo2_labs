// ALGO2 IS1 212A LAB01 
// Daniel Bolechowicz
// bd53967@zut.edu.pl

#include <iostream>
#include <string>
#include <conio.h>
#include <time.h>
#include <cmath>

struct Book{
    std::string title;
    int book_id;
};

template <typename T>
class List{
    struct List_Node{
        T value;
        List_Node* next; 
        List_Node* prev;
        List_Node(T val) : value(val), next(nullptr), prev(nullptr) {}
    };
    List_Node* head; 
    List_Node* tail;
    int size;

    public:
    List() : head(nullptr), tail(nullptr), size(0) {}

    void add_at_end(T data){
        List_Node* Node = new List_Node(data);
        if(head == nullptr){
            head = Node;
            tail = Node;
        }
        else{
            tail->next = Node;
            Node->prev = tail;
            tail = tail->next;
        }
        size++;
    }

    void add_at_begin(T data){
        List_Node* Node = new List_Node(data);
        if(head == nullptr){
            head = Node;
            tail = Node;
        }
        else{
            head->prev = Node;
            Node->next = head;
            head = head->prev;
        }
        size++;
    }

    void remove_tail(){
        auto temp = tail;
        tail = tail->prev;
        tail->next = nullptr;
        delete temp;
        size--;
    }

    void remove_head(){
        auto temp = head;
        head = head->next;
        head->prev = nullptr;
        delete temp;
        size--;
    }

    List_Node* find_index_val(int index){
        if(index >= size){
            while(true){
                std::cout << "index is bigger than size: " << index << '\n' << "Size: " << size << '\n';
                std::cout << "Enter correct index: "; std::cin >> index; 
                if(index < size)
                    break;
            }
        }
        auto current = head; 
        int temp = 0;
        while(current != nullptr){
            if(temp == index)
                return current;
            temp++;
            current = current->next;
        }
    }

    void replace_on_index(int index, T data){
        auto to_replace = find_index_val(index);
        auto to_raplce_prev = to_replace->prev;
        auto to_replace_next = to_replace->next;
        List_Node* new_element = new List_Node(data);

        if(to_raplce_prev != nullptr)
            to_raplce_prev->next = new_element;
        new_element->prev = to_raplce_prev;

        if(to_replace_next != nullptr)
            to_replace_next->prev = new_element;
        new_element->next = to_replace_next;

        if(to_replace == head)
            head = new_element;
        else if(to_replace == tail)
            tail = new_element;

        delete to_replace;
    }

    List_Node* find_element(T key, int pointer_des){
        auto right = head, left = tail;

        while(right != left){
            if(right->value == key){
                return right;
            }
            if(left->value == key){
                return left;
            }
            right = right->next;
            left = left->prev;
        }
        return right;
    }

    List_Node* find_element(T key, Book* pointer_des){
        auto right = head, left = tail;

        while(right != left){
            if(right->value->book_id == key->book_id){
                return right;
            }
            if(left->value->book_id == key->book_id){
                return left;
            }
            right = right->next;
            left = left->prev;
        }
        return right;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool find_and_delete(T key, List_Node* (List<T>::*find_fnc)(T,T)){
        if(size == 0)
            return false;
        else if(size == 1){
            head = nullptr;
            delete head;
            size--;
            return true;
        }

        List_Node* to_remove = (this->*find_fnc)(key, head->value);
        if(to_remove == nullptr){
            return false;
        }

        if(to_remove == head){
            auto temp = head;
            head = head->next;
            head->prev = nullptr;
            temp = nullptr;
            delete to_remove;
            size--;
            return true;
        }
        else if(to_remove == tail){
            tail = tail->prev;
            tail->next = nullptr;
            delete to_remove; 
            size--;
            return true;
        }
        if(to_remove->value == key){
            auto to_remove_next = to_remove->next;
            auto to_remove_prev = to_remove->prev;

            to_remove_prev->next = to_remove_next;
            to_remove_next->prev = to_remove_prev;

            delete to_remove;
            size--;
            return true;
        }
        // delete to_remove;
        return false; 
    }

    int get_size(){
        return size;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void add_by_key(T data, int (*comparator)(T, T)){
        auto Node = new List_Node(data);
        if(comparator(head->value, Node->value) >= 0){
            head->prev = Node;
            Node->next = head;
            head = head->prev;
            size++;
            return;
        }

        auto current = head->next;
        while(current != nullptr){
            if(comparator(current->value, Node->value) >= 0){
                auto curr_prev = current->prev;
                curr_prev->next = Node;
                Node->prev = curr_prev;
                Node->next = current;
                size++;
                return;
            }
            current = current->next;
        }
        tail->next = Node;
        Node->prev = tail;
        tail = tail->next;
        size++;
    }

    std::string print_list(){
        std::string res = "";
        auto current = head;
        while(current != nullptr){
            res += "Address Node: " + std::to_string(reinterpret_cast<std::intptr_t>(current)) + " Value: " + std::to_string(current->value->book_id) + "\n";
            // std::cout << "Title: " << current->value->title << " book_id: " << current->value->book_id << '\n';
            current = current->next;
        }
        res += "\t\tSize: " + std::to_string(size); 
        return res;
    }

    std::string print_list_obj(){
        std::string res = "";
        auto current = head;
        while(current != nullptr){
            res += "Address Node: " + std::to_string(reinterpret_cast<std::intptr_t>(current)) + " Value: " + std::to_string(current->value) + "\n";
            // std::cout << current->value << ' ';
            current = current->next;
        }
        res += "\tSize: " + std::to_string(size);
        return res;
    }

    void remove_every_node(){
        while(head != nullptr){
            auto to_delete = head;
            head = head->next;
            delete to_delete;
            size--;
        }
    }

    bool is_empty() const {
        return head == nullptr;
    }

};

int compare_value(int a, int b){
        return a-b;
}

int compare_value(Book* a, Book* b){
    return a->book_id - b->book_id;
}

int main(){
    // srand(static_cast<unsigned int>(time(NULL)));
    // List<int>* num_list = new List<int>();
    // for(int i = 0; i<100; i++){
    //     num_list->add_at_end(rand());
    // }
    // num_list->print_list_obj();
    // _getch();
    // for(int i=0; i<1000; i++){
    //     num_list->find_and_delete(rand(), List<int>::find_element);
    //     if(num_list->get_size() == 0){
    //         std::cout << num_list->get_size() << '\n';
    //         break;
    //     }
    // }
    // num_list->print_list_obj();
    // _getch();

    // std::cout << "\n\n New: \n";
    // num_list->print_list_obj();

    // std::cout << "Is empty? " << num_list->is_empty();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // List<Book*>* book_list = new List<Book*>(); 

    // for(int i=0; i<4; ++i){
    //     Book* book = new Book;
    //     std::cout << "Title: "; std::cin >> book->title;
    //     std::cout << "book_id: "; std::cin >> book->book_id;
    //     book_list->add_at_end(book);
    // }

    // Book* book_p = new Book;
    // book_p->title = "ZYX";
    // book_p->book_id = 5;
    // // book_list->replace_on_index(0, book_p);
    // book_list->add_by_key(book_p, compare_value);

    // book_list->print_list();
    // book_list->find_and_delete(book_p, List<Book*>::find_element);
    // std::cout << '\n';
    // book_list->print_list();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    std::string letters = "abcdefghijklmnopqrstuvwxyz";
    std::string result;
    List<Book*>* book_list = new List<Book*>(); 
    // srand(static_cast<unsigned int>(time(NULL)));
    // for(int i=0; i<4; ++i){
    //     Book* book = new Book;
    //     book->book_id = rand();
    //     int rand_index = rand() % letters.length();
    //     book->title = letters[rand_index];
    //     book_list->add_at_end(book);
    // }
    // result = book_list->print_list();
    // std::cout << "\n\n" << result << "\n\n";

    // Book* book_p = new Book;
    // book_p->title = "ZYX";
    // book_p->book_id = 20000;
    // // book_list->replace_on_index(0, book_p);
    // book_list->add_by_key(book_p, compare_value);

    // result = book_list->print_list();
    // std::cout << result << '\n';
    // book_list->find_and_delete(book_p, List<Book*>::find_element);
    // std::cout << '\n';
    // result = book_list->print_list();
    // std::cout << result << '\n';

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    const int MAX_ORDER = 6;
    for(int o=0; o<=MAX_ORDER; o++){
        
        const int n = pow(10, o);
        std::cout << "Rozmiar danych: " << n << " " << o << std::endl;

        clock_t t1 = clock();
        for(int i=0; i<n; i++){
            Book* book = new Book;
            book->book_id = rand();
            int rand_index = rand() % letters.length();
            book->title = letters[rand_index];
            book_list->add_at_end(book);
        }
        clock_t t2 = clock();
        // num_list->print_list_obj();
        double spent_time = double(t2-t1)/double(CLOCKS_PER_SEC);
        std::cout << "Time spent on adding items to the list: " << spent_time*1000 << " ms" << std::endl;
        const int m = pow(10, 4);
        // std::cout << "m: " << m << '\n';
        t1 = clock();
        for(int i=0; i<m; i++){
            Book* book = new Book;
            book->book_id = rand();
            int rand_index = rand() % letters.length();
            book->title = letters[rand_index];
            book_list->add_at_end(book);
            book_list->find_and_delete(book, List<Book*>::find_element);
        }
        t2 = clock();

        // num_list->print_list_obj();
        double spent_time2 = double(t2-t1)/double(CLOCKS_PER_SEC);
        std::cout << "Time spent on finding items to the list: " << spent_time2*1000 << " ms" << std::endl;
        _getch();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // srand(static_cast<unsigned int>(time(NULL)));

    // const int MAX_ORDER = 6;
    // List<int>* num_list = new List<int>();
    // for(int o=0; o<=MAX_ORDER; o++){
        
    //     const int n = pow(10, o);
    //     std::cout << "Rozmiar danych: " << n << " " << o << std::endl;

    //     clock_t t1 = clock();
    //     for(int i=0; i<n; i++){
    //         num_list->add_at_end(rand());
    //     }
    //     clock_t t2 = clock();
    //     // num_list->print_list_obj();
    //     double spent_time = double(t2-t1)/double(CLOCKS_PER_SEC);
    //     std::cout << "Time spent on adding items to the list: " << spent_time*1000 << " ms" << std::endl;
    //     const int m = pow(10, 4);
    //     // std::cout << "m: " << m << '\n';
    //     t1 = clock();
    //     for(int i=0; i<m; i++){
    //         num_list->find_and_delete(rand(), List<int>::find_element);
    //     }
    //     t2 = clock();

    //     // num_list->print_list_obj();
    //     double spent_time2 = double(t2-t1)/double(CLOCKS_PER_SEC);
    //     std::cout << "Time spent on finding items to the list: " << spent_time2*1000 << " ms" << std::endl;
    //     _getch();
    // }

    // delete num_list;
    return 0;
}   