#pragma once
#include <vector>
#include <string>
#include <iostream>

template <typename T>
class Binary_Heap{
    std::vector<T>& arr;
    bool fix_down;

    void digging_up(int node_ind, int node_par_ind, bool (*cmp)(T, T)){
        if(node_ind == 0 || cmp(arr[node_par_ind], arr[node_ind]))
            return;

        auto temp = arr[node_ind];
        arr[node_ind] = arr[node_par_ind];
        arr[node_par_ind] = temp;    
        
        node_ind = node_par_ind;
        
        digging_up(node_ind, (node_ind - 1)>>1, cmp);
    }


    void digging_down(int index, bool (*cmp)(T, T)){
        int left_child = 2 * index + 1;
        int right_child = 2 * index + 2;

        int maxIndex = index;

        if(left_child < arr.size() && !cmp(arr[maxIndex], arr[left_child])){
            maxIndex = left_child;
        }

        if(right_child < arr.size() && !cmp(arr[maxIndex], arr[right_child])){
            maxIndex = right_child;
        }

        if(index != maxIndex){
            auto temp = arr[index];
            arr[index] = arr[maxIndex];
            arr[maxIndex] = temp;

            digging_down(maxIndex, cmp);
        }
    }

public:
    
    Binary_Heap() : arr(std::vector<T>()) {}

    Binary_Heap(std::vector<T>& array, bool choice, bool (*cmp)(T, T)) : arr(array), fix_down(choice) {
        for (int i = (arr.size() >> 1 )-1; i >= 0; --i) {
            if (fix_down) {
                digging_down(i, cmp);
            } else {
                digging_up(i, (i - 1) >> 1, cmp);
            }
        }
    }

    // Binary_Heap(std::vector<T>& array, bool choice, bool (*cmp)(T, T)) : arr(array), fix_down(choice) {
    //     auto temp = arr;
    //     arr.clear();
    //     int x=0;
    //     for(T element : temp){
    //         add_element(element, cmp, fix_down);
    //     }
    // }

    // ~Binary_Heap(){ delete arr; }

    void add_element(T data, bool (*cmp)(T, T), bool choice){
        if(choice == 0){
            if(arr.size() == 0){
                //root
                arr.push_back(data);
                return; //index ale wazny tylko w kontekscie heapu index tablicowy ustalany przez tablice
            }
            arr.push_back(data);
            if(cmp(arr[arr.size()-1], arr[((arr.size()-1)-1)>>1])){ //arr->get_at_index(arr->size-1) > arr->get_at_index(((arr->size-1)-1)>>1)
                digging_up(arr.size()-1, ((arr.size()-1)-1)>>1, cmp);
            }
        } else {
            if(arr.size() == 0){
                //root
                arr.push_back(data);
                return;
            }
            arr.insert(arr.begin(), data);
            digging_down(0, cmp);
        }
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

    T remove_greatest(bool (*cmp)(T, T)){
        if(arr.size() == 0)
            return T();

        T max = arr[0];

        arr[0] = arr[arr.size()-1];
        arr.pop_back();

        digging_down(0, cmp);

        return max;
    }

    T remove_greatest1(bool (*cmp)(T, T)){
        if(arr.size() == 0)
            return T();

        T max = arr[0];

        arr[0] = arr[arr.size()-1];
        // arr.pop_back();

        digging_down(0, cmp);

        return max;
    }

    void heap_sort(bool (*cmp)(T,T)){
        // std::vector<T> temp;

        for(int i=0; i<arr.size(); i++){
            // temp.push_back(remove_greatest(cmp));
            T temp = remove_greatest1(cmp);
            arr[arr.size()-i-1] = temp;
        }

        // arr.clear();
        // arr = temp;
    }

    void clear_heap(){ arr.clear(); }

    std::string show_array(std::string (*show)(std::vector<T>*)){ return show(&arr); }
};