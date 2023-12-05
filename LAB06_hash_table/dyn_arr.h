#pragma once
#include <iostream>
#include <string>
#include <random>
#include <time.h>
#include <vector>

// struct Book{
//     int book_id;
//     char title;
// };

template <typename T>
struct Dynamic_Arr{
    T* array;
    int size;
    int capacity;
    // std::vector<int> indexes;

    Dynamic_Arr() :  capacity(1), array(new T[1]), size(0) {}

    void push_back(T val){
        if(size == capacity)
            expand_arr();

        array[size] = val;
        size++;
    }

    void push_on_index(int index, T val){
        array[index] = val;
        size++;
        // indexes.push_back(index);
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

    void expand_arr_wa(){
        auto temp = new T[capacity<<1];
        capacity = capacity<<1;
        delete[] array;
        array = temp;
    }

    T find_element(int index){ return array[index]; }

    void replace_element(int i, T new_val){ array[i] = new_val; }  

    void remove_all_elements(){ 
        delete[] array;
        array = NULL;
        capacity = 1;
        size = 0;  
    }

    void clear_all_elements(){
        delete[] array;
        capacity = 1;
        array = new T[capacity];
        size = 0;  
    }

    void pop_last(){
        if (size > 0)
            --size;
        // auto temp = new T[capacity];

        // for(int i=0;i<size-1;i++){
        //     temp[i] = array[i];
        // }

        // delete[] array;
        // --size;
        // array = temp;
    }

    // void remove_all_elements(Book* x){ 
    //     for(int i=0; i<size; i++){
    //         delete array[i];
    //     }
    //     delete[] array;
    //     array = NULL;
    //     capacity = 1;
    //     size = 0;  
    // }

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

    // bool is_index(int index){
    //     std::sort(indexes.begin(), indexes.end());
    //     int r = indexes[0], l = indexes.back();
    //     while(r<=l){
    //         if(r==index || l==index)
    //             return true;
    //         r++;
    //         l--;
    //     }
    //     return false;
    // }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string show_arr(std::string (*show_array)(Dynamic_Arr<T>*)){ return show_array(this); }

    T get_at_index(int index){ return array[index]; }

    int get_capacity(){ return capacity; };
    int get_size(){ return size; };
    // auto get_vector(){ return indexes; }
};

template <typename T>
std::string show_arr(Dynamic_Arr<T>* dyn_arr){
    std::string to_return = "";

    for(int i=0;i<dyn_arr->get_size();i++){
        to_return += std::to_string(dyn_arr->array[i])+" ";
    }
    return to_return;
}

// std::string show_arr(Dynamic_Arr<Book*>* dyn_arr){
//     std::string to_return = "";

//     for(int i=0;i<dyn_arr->get_size();i++){
//         to_return += std::to_string(dyn_arr->array[i]->book_id)+" ";
//     }
//     to_return += "\n\tSize: " + std::to_string(dyn_arr->get_size()) + "\tCapacity: " + std::to_string(dyn_arr->get_capacity());
//     return to_return;
// }
template <typename T>
bool comparator(T a, T b){ return a>b; }

// bool comparator(Book* a, Book* b){ return a->book_id>b->book_id; }