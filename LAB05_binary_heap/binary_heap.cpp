#include "dyn_arr.h"
#include <iostream>
#include <random>
#include <time.h>
#include <conio.h>

struct Book{
    int book_id;
    char title;
};

template <typename T>
class Binary_Heap{
    Dynamic_Arr<T>* arr;

    void digging_up(int node_ind, int node_par_ind, bool (*cmp)(T, T)){
        if(node_ind == 0 || cmp(arr->get_at_index(node_par_ind), arr->get_at_index(node_ind)))
            return;

        auto temp = arr->array[node_ind];
        arr->array[node_ind] = arr->array[node_par_ind];
        arr->array[node_par_ind] = temp;    
        
        node_ind = node_par_ind;
        
        digging_up(node_ind, (node_ind - 1)>>1, cmp);
    }


    void digging_down(int index, bool (*cmp)(T, T)){
        int left_child = 2 * index + 1;
        int right_child = 2 * index + 2;

        int maxIndex = index;

        if(left_child < arr->size && cmp(arr->array[maxIndex], arr->array[left_child])){
            maxIndex = left_child;
        }

        if(right_child < arr->size && cmp(arr->array[maxIndex], arr->array[right_child])){
            maxIndex = right_child;
        }

        if(index != maxIndex){
            auto temp = arr->array[index];
            arr->array[index] = arr->array[maxIndex];
            arr->array[maxIndex] = temp;

            digging_down(maxIndex, cmp);
        }
    }

    // void digging_down(int node_ind, int node_ind_child, bool (*cmp)(T, T)) {
    //     if (node_ind >= arr->size || node_ind_child >= arr->size || cmp(arr->get_at_index(node_ind), arr->get_at_index(node_ind_child)))
    //         return;

    //     auto temp = arr->array[node_ind];
    //     arr->array[node_ind] = arr->array[node_ind_child];
    //     arr->array[node_ind_child] = temp;

    //     node_ind = node_ind_child;

    //     int left_child = (node_ind << 1) + 1;
    //     int right_child = (node_ind << 1) + 2;

    //     if (left_child < arr->size && (right_child >= arr->size || cmp(arr->get_at_index(left_child), arr->get_at_index(right_child))))
    //         node_ind_child = left_child;
    //     else
    //         node_ind_child = right_child;

    //     digging_down(node_ind, node_ind_child, cmp);
    // }

public:

    Binary_Heap() : arr(new Dynamic_Arr<T>()) {}
    
    ~Binary_Heap(){
        this->arr->remove_all_elements();
        delete arr;
    }

    void add_element(T data, bool (*cmp)(T, T)){
        if(arr->get_size() == 0){
            //root
            arr->push_back(data);
            return; //index ale wazny tylko w kontekscie heapu index tablicowy ustalany przez tablice
        }
        arr->push_back(data);
        if(cmp(arr->get_at_index(arr->size-1), arr->get_at_index(((arr->size-1)-1)>>1))){ //arr->get_at_index(arr->size-1) > arr->get_at_index(((arr->size-1)-1)>>1)
            digging_up(arr->size-1, ((arr->size-1)-1)>>1, cmp);
        }
    }

    void remove_greatest(bool (*cmp)(T, T)){
        // std::cout << "Rozmiar: " << arr->size << std::endl;
        if(arr->size == 0)
            return;

        arr->array[0] = arr->array[arr->size-1];
        arr->pop_last();

        digging_down(0, cmp);
        // if(cmp(arr->array[1], arr->array[2]))
        //     digging_down(0, 1, cmp); 
        // else
        //     digging_down(0, 2, cmp);
    }

    void clear_heap(){ arr->clear_all_elements(); }

    Dynamic_Arr<T>* get_arr(){ return arr; }
};

template <typename T>
std::string show_arr_array(Dynamic_Arr<T>* dyn_arr){
    std::string to_return = "";
    if(dyn_arr->size > 50){
        for(int i=0;i<50;i++){
            to_return += std::to_string(dyn_arr->array[i])+" ";
        }
        to_return += "\n\tSize: " + std::to_string(dyn_arr->get_size()) + "\tCapacity: " + std::to_string(dyn_arr->get_capacity()) + "\n\tShowed first 50 out of " + std::to_string(dyn_arr->get_size()) + " possible\n";
        return to_return;
    } 
    else {
        for(int i=0;i<dyn_arr->get_size();i++){
            to_return += std::to_string(dyn_arr->array[i])+" ";
        }
        to_return += "\n\tSize: " + std::to_string(dyn_arr->get_size()) + "\tCapacity: " + std::to_string(dyn_arr->get_capacity()) + "\n\tShowed first 50 out of " + std::to_string(dyn_arr->get_size()) + " possible\n";
        return to_return;
    }
}

std::string show_arr_array(Dynamic_Arr<Book*>* dyn_arr){
    std::string to_return = "";
    if(dyn_arr->size > 50){
        for(int i=0;i<50;i++){
            to_return += std::to_string(dyn_arr->array[i]->book_id)+" ";
        }
        to_return += "\n\tSize: " + std::to_string(dyn_arr->get_size()) + "\tCapacity: " + std::to_string(dyn_arr->get_capacity()) + "\n\tShowed first 50 out of " + std::to_string(dyn_arr->get_size()) + " possible\n";
        return to_return;
    } 
    else{
        for(int i=0;i<dyn_arr->get_size();i++){
            to_return += std::to_string(dyn_arr->array[i]->book_id)+" ";
        }
        to_return += "\n\tSize: " + std::to_string(dyn_arr->get_size()) + "\tCapacity: " + std::to_string(dyn_arr->get_capacity());
        return to_return;
    }
}

bool comparator(int a, int b){ return a>=b; }

bool comparator(Book* a, Book* b){ return a->book_id >= b->book_id; }

int main(){
    srand(static_cast<unsigned int>(time(NULL)));
    std::string letters = "abcdefghijklmnopqrstuvwxyz";
    const int MAX_ORDER = 7;

    auto BH0 = new Binary_Heap<Book*>();
    for(int o=1; o<=MAX_ORDER; o++){
        const int n = pow(10, o);

        //adding to heap
        clock_t t1 = clock();
        for(int i=0; i<n; i++){
            auto book = new Book;
            book->book_id = (rand()<<15)+rand();
            int rand_index = rand() % letters.length();
            book->title = letters[rand_index];
            BH0->add_element(book, comparator);
        }
        clock_t t2 = clock();

        std::cout << "\n\n--------------------------------------------------------------------------\n" << "Adding:\nArray after adding: " << std::endl;

        std::string str_tab = BH0->get_arr()->show_arr(show_arr_array);
        std::cout << str_tab << '\n';

        double full_time = double(t2-t1)/double(CLOCKS_PER_SEC);
        double average_time = full_time/n;

        std::cout << "\nFull time: " << full_time*1000 << " ms\t" << "Average time: " << average_time*1000 << " ms" << std::endl;

        _getch();

        //deleting largest
        t1 = clock();
        for(int i=0; i<n; i++){
            BH0->remove_greatest(comparator);
            // str_tab = BH0->get_arr()->show_arr(show_arr_array);
            // std::cout << str_tab << '\n';
        }
        t2 = clock();

        std::cout << "\n--------------------------------------------------------------------------\n" << "Deletion:\nArray after deletion: " << std::endl;
        str_tab = BH0->get_arr()->show_arr(show_arr_array);
        std::cout << str_tab << '\n';

        full_time = double(t2-t1)/double(CLOCKS_PER_SEC);
        average_time = full_time/n;

        std::cout << "\nFull time: " << full_time*1000 << " ms\t" << "Average time: " << average_time*1000 << " ms" << std::endl;

        BH0->clear_heap();
        std::cout << "Done" << std::endl;

        _getch();
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // srand(static_cast<unsigned int>(time(NULL)));
    // auto BH = new Binary_Heap<Book*>();
    // std::string letters = "abcdefghijklmnopqrstuvwxyz";
    // // int arr[11] = {25, 20, 22, 15, 6, 7, 17, 5, 13 , 1, 24};
    // // for(int x : arr){
    // //     BH->add_element(x, comparator);
    // //     std::string sa1 = BH->get_arr()->show_arr(show_arr_array);
    // //     std::cout << sa1 << std::endl;
    // //     std::string sa0 = BH->get_arr()->show_arr(show_arr_array);
    // //     std::cout << sa0 << std::endl;
    // // }

    // for(int i=0;i<100;i++){
    //     // BH->add_element((rand()<<15)+rand(), comparator);
    //     Book* book = new Book;
    //     book->book_id = (rand()<<15)+rand();
    //     int rand_index = rand() % letters.length();
    //     book->title = letters[rand_index];
    //     BH->add_element(book, comparator);
    // }

    // // std::cout << "xpp" << std::endl;
    // std::string sa = BH->get_arr()->show_arr(show_arr_array);
    // std::cout << sa << std::endl;

    // BH->remove_greatest(comparator);
    // std::string sa2 = BH->get_arr()->show_arr(show_arr_array);
    // std::cout << sa2 << std::endl;

    // BH->clear_heap();

    // std::string sa3 = BH->get_arr()->show_arr(show_arr_array);
    // std::cout << sa3 << std::endl;
    
    // for(int i=0;i<10;i++){
    //     // BH->add_element((rand()<<15)+rand(), comparator);
    //     Book* book = new Book;
    //     book->book_id = (rand()<<15)+rand();
    //     int rand_index = rand() % letters.length();
    //     book->title = letters[rand_index];
    //     BH->add_element(book, comparator);
    // }

    // sa3 = BH->get_arr()->show_arr(show_arr_array);
    // std::cout << sa3 << std::endl;

    // delete BH;

    // std::cout << "xpp" << std::endl;
    delete BH0;
    return 0;
}