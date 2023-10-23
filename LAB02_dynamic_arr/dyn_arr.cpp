#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <conio.h>

struct Book{
    int book_id;
    char title;
};

template <typename T>
struct Dynamic_Arr{
    T* array;
    int size;
    int capacity;

    Dynamic_Arr() :  capacity(1), array(new T[1]), size(0) {}

    void push_back(T val){   
        if(size == capacity)
            expand_arr();

        array[size] = val;
        size++;
    }

    void expand_arr(){
        auto temp = new T[capacity<<1];
        capacity = capacity<<1;

        for(int i=0; i<size; i++){
            temp[i] = array[i];
        }

        delete[] array;
        array = temp;
    }

    T find_element(int index){
        int r=0, l=size-1;
        while(r<l){
            if(array[r] == array[index])
                return array[r];
            else if(array[l] == array[index])
                return array[l];
            ++r;
            --l;
        }
        if(r==l && array[r] == array[index])
            return array[r];
    }

    void replace_element(int i, T new_val){ array[i] = new_val; }  

    void remove_all_elements(){ 
        delete[] array;
        array = NULL;
        capacity = 1;
        size = 0;  
    }

    void remove_all_elements(Book* x){ 
        for(int i=0; i<size; i++){
            delete array[i];
        }
        delete[] array;
        array = NULL;
        capacity = 1;
        size = 0;  
    }

    void bubble_sort_arr(bool (*cmp)(T,T)){
        bool flag;
        for(int i=0; i<size; i++){
            flag = false;
            for(int j=0; j<size-i-1; j++){
                if(cmp(array[j], array[j+1])){ //array[j]>array[j+1]
                    T temp = array[j];
                    array[j] = array[j+1];
                    array[j+1] = temp;
                    flag = true;
                }
            }
            if(flag == false) break;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string show_arr(){
        std::string to_return = "";

        for(int i=0;i<size;i++){
            to_return += std::to_string(array[i])+" ";
        }
        return to_return;
    }

    std::string show_arr(Book* x){
        std::string to_return = "";

        for(int i=0;i<size;i++){
            to_return += std::to_string(array[i]->book_id)+" ";
        }
        to_return += "\n\tSize: " + std::to_string(size) + "\tCapacity: " + std::to_string(capacity);
        return to_return;
    }

    T get_at_index(int index){ return array[index]; }

    int get_capacity(){ return capacity; };
    int get_size(){ return size; };
};

bool comparator(int a, int b){ return a>b; }
bool comparator(Book* a, Book* b){ return a->book_id>b->book_id; }

int main(){
    srand(static_cast<unsigned int>(time(NULL)));
    std::string letters = "abcdefghijklmnopqrstuvwxyz";
    std::string elements;
    Dynamic_Arr<Book*>* da2 = new Dynamic_Arr<Book*>();

    const int order = 7;
    const int n = pow(10, order);
    clock_t t1 = clock();
    double max_time_per_element = 0.0;
    for(int i=0; i<n; ++i){
        Book* book = new Book;
        book->book_id = rand();
        int rand_index = rand() % letters.length();
        book->title = letters[rand_index];

        clock_t t1_element = clock();
        da2->push_back(book);
        clock_t t2_element = clock();

        double time_per_element = double(t2_element-t1_element)/double(CLOCKS_PER_SEC);
        if(time_per_element > max_time_per_element){
            max_time_per_element = time_per_element;
            std::cout << "New worst time on adding index array[" << da2->size - 1<< "] : " << max_time_per_element*1000 << " ms" << '\n';
        }
    }
    clock_t t2 = clock();

    elements = da2->show_arr(da2->array[0]);
    double full_time = double(t2-t1)/double(CLOCKS_PER_SEC);
    std::cout << "Full time spent on adding to array: " << full_time*1000 << " ms" << std::endl;

    da2->remove_all_elements(da2->array[0]);
    delete da2;

    // for(int i=0; i<9; ++i){
    //     Book* book = new Book;
    //     book->book_id = rand();
    //     int rand_index = rand() % letters.length();
    //     book->title = letters[rand_index];
    //     da2->push_back(book);
    // }

    // elements = da2->show_arr(da2->get_at_index(0));
    // std::cout << elements << "\n\n";
    // // std::cout << "\tCapacity: " << da2->get_capacity() << '\n' << "\tSize: " << da2->get_size() << std::endl;

    // da2->bubble_sort_arr(comparator);
    // elements = da2->show_arr(da2->get_at_index(0));
    // std::cout << elements << "\n\n";
    // // std::cout << "\tCapacity: " << da2->get_capacity() << '\n' << "\tSize: " << da2->get_size() << std::endl;

    // Book* bookxD = new Book;
    // bookxD->book_id = 0;
    // // int rand_index = rand() % letters.length();
    // bookxD->title = 'J';
    // da2->replace_element(4, bookxD);

    // da2->bubble_sort_arr(comparator);
    // elements = da2->show_arr(da2->get_at_index(0));
    // std::cout << elements << "\n\n";
    // // std::cout << "\tCapacity: " << da2->get_capacity() << '\n' << "\tSize: " << da2->get_size() << std::endl;

    // da2->remove_all_elements(da2->get_at_index(0));

    // Dynamic_Arr<int>* da1 = new Dynamic_Arr<int>();
    // Dynamic_Arr<int> da0;
    // std::string elements;
    // std::string letters = "abcdefghijklmnopqrstuvwxyz";
    
    // for(int i=5;i>-1;i--){
    //     da0.push_back(i);
    // }

    // elements = da0.show_arr();
    // std::cout << elements << "\n\n";
    // std::cout << "\tCapacity: " << da0.get_capacity() << '\n' << "\tSize: " << da0.get_size() << std::endl;
    
    // try{
    //     int index = 3;
    //     if(index < da0.get_size())
    //         std::cout << "Value found on index " << index << ": " << da0.find_element(4) << std::endl;
    //     else
    //         throw index;
    // }
    // catch(int i){
    //     std::cout << "Index is larger than size!!!\tIndex: " << i << std::endl; 
    // }

    // da0.replace_element(4, 1000);
    // elements = da0.show_arr();
    // std::cout << elements << "\n\n";
    // std::cout << "\tCapacity: " << da0.get_capacity() << '\n' << "\tSize: " << da0.get_size() << std::endl;
    // std::cout << "I array[1]: " << da0.get_at_index(1) << std::endl;

    // da0.bubble_sort_arr(comparator);
    // elements = da0.show_arr();
    // std::cout << elements << "\n\n";
    // std::cout << "\tCapacity: " << da0.get_capacity() << '\n' << "\tSize: " << da0.get_size() << std::endl;
    // // std::cout << "I array[1]: " << da0.get_at_index(1) << std::endl;
    
    // da0.remove_all_elements();

    // // elements = da0.show_arr();
    // // std::cout << elements << "\n\n";
    // // std::cout << "\tCapacity: " << da0.get_capacity() << '\n' << "\tSize: " << da0.get_size() << std::endl;

    return 0;
}