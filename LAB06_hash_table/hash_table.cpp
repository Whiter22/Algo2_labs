#include "double_linked_list.h"
#include "dyn_arr.h"
#include <iostream>
#include <random>
#include <time.h>
#include <cmath>
#include <climits>
#include <string>
#include <conio.h>

template <typename T>
class Hash_Table{
    Dynamic_Arr<List<T>> array;
    int added_elements = 0;

    int hash_key(std::string key, int N){
        int q = key.length();/*N = array.capacity,*/

        long long int sum = 0;
        for(int i=0; i<key.length(); i++){
            --q;
            sum += int(key[i])*pow(31, q);
        }
        return sum % N;
    }

    int key_sum(std::string key){
        int sum=0;
        for(int i=0; i<6; i++)
            sum+=int(key[i]);

        return sum;
    }

    int rehash(){
        Dynamic_Arr<List<T>> array2;
        array2.capacity = array.capacity;
        array2.expand_arr_wa();

        for(int i=0;i<array.capacity;i++){
            if(array.array[i].get_head()!=nullptr){
                // std::cout << array.capacity << "\t" << array2.capacity << std::endl;
                int index = hash_key(array.array[i].get_head()->key, array2.capacity);
                array2.push_on_index(index, array.array[i]);  
            }
        }
        array.remove_all_elements();
        array = array2;
    }

public:

    void add_element(std::string key, T value){
        if(array.size > array.capacity * 0.75){ rehash(); }

        int index = hash_key(key, array.capacity); //, f_key;

        if(array.array[index].get_head()!=nullptr){
            if(!array.array[index].replace_by_key(value, key)){
                array.array[index].add_at_end(value, key);
            }
        }
        else{
            List<T> list;
            list.add_at_end(value, key);
            array.push_on_index(index, list);   
        }    
        added_elements++;
    }

    bool remove_element(std::string key){
        for(int i=0; i<array.capacity; i++){
            if(array.array[i].get_head()!=nullptr){
                auto curr = array.array[i].get_head();
                while(curr!=nullptr){
                    if(key.compare(curr->key) == 0){ 
                        array.array[i].find_and_delete(key);
                        if(array.array[i].get_head() == nullptr)
                            array.size--;
                        return true;
                    }
                    curr = curr->next;
                }
            }
        }
        return false;
    }


    List_Node<T>* find_element(const std::string& key) {
        size_t hash = hash_key(key, array.capacity);
        List_Node<T>* head = array.array[hash].get_head();
        
        if (head == nullptr) {
            return nullptr;
        }

        List_Node<T>* curr = head;
        while (curr != nullptr) {
            if (key.compare(curr->key) == 0) {
                return curr;
            }
            curr = curr->next;
        }

        return nullptr;
    }

    // List_Node<T>* find_element(std::string key){
    //     for(int i=0; i<array.capacity; i++){
    //         if(array.array[i].get_head()!=nullptr){
    //             auto curr = array.array[i].get_head();
    //             while(curr!=nullptr){
    //                 if(key.compare(curr->key) == 0){
    //                     return curr;
    //                 }
    //                 curr = curr->next;
    //             }
    //         }
    //     }
    //     return nullptr;
    // }

    std::string get_stats(){
        std::string stats = "";
        int max = 0, len, min = INT_MAX, all_size = 0;
        for(int i=0;i<array.capacity;i++){
            if(array.array[i].get_head() != nullptr){
                len = array.array[i].get_len();
                if(len > max)
                    max = len;
                if(len < min)
                    min = len;

                all_size += len;
            }
        }

        float avg_size = all_size / array.size;

        stats+= "stats:\n  list min size: " + std::to_string(min) + "\n  list max size: " + std::to_string(max) + "\n  non-null lists: " + std::to_string(array.size) + "\n  list avg size: " + std::to_string(avg_size) + "\n"; 
        return stats;
    }

    std::string show_table(){
        int count = 0;
        std::string res = "hash table:\ncurrent_size: " + std::to_string(added_elements) +"\n"+ "max_size: " + std::to_string(array.capacity) + "\ntable:\n{\n";
        for(int i=0; i<array.capacity; i++){
            if(count == 8)
                break;
            if(array.array[i].get_head()!=nullptr){
                auto curr = array.array[i].get_head();
                res += "\t" + std::to_string(i) + ": ";
                while(curr != nullptr){
                    res += curr->key + " -> " + std::to_string(curr->value) + "; ";
                    curr = curr -> next;
                }
                res += "\n";
                count++;
            }
        }
        res+="\t...\n}\n";
        res+=get_stats();
        return res;
    }


    void clear_table(){
        added_elements = 0; 
        array.clear_all_elements(); 
    }

    int get_size(){ return array.size; }
    int get_capacity() { return array.capacity; }
    auto get_array(){ return array; }
};

int main(){
    srand(static_cast<unsigned int>(time(NULL)));
    std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const int MAX_ORDER = 7;
    Hash_Table<int> HT;

    for(int o=1; o<=MAX_ORDER; o++){
        const int n = pow(10, o);

        std::cout << "For data: " << n << std::endl;
 
        std::string key = "";
        clock_t t1 = clock();
        for(int i=0; i<n; i++){
            key = "";
            for(int j=0;j<6;j++){ key += letters[rand()%52]; }
            HT.add_element(key, ((rand()<<15)+rand())%1000000);
        }
        clock_t t2 = clock();

        double spent_time = double(t2-t1)/double(CLOCKS_PER_SEC);
        std::string table = HT.show_table();
        std::cout << table << '\n';

        std::cout << "Time spent on adding: " << spent_time << "ms" << std::endl;
        
        const int m = pow(10, 4);
        int hits = 0;
        t1 = clock();
        for(int i=0; i<m; i++){
            key = "";
            for(int j=0;j<6;j++){ key += letters[rand()%52]; }
            auto x = HT.find_element(key);
            if(x!=nullptr)
                hits++;
        }
        t2 = clock();

        spent_time = double(t2-t1)/double(CLOCKS_PER_SEC);
        std::cout << "Time spent on searching: " << spent_time << "ms\nHits: " << hits << std::endl;
        std::string stats = HT.get_stats();

        _getch();

        std::cout << "\n\n";
        HT.clear_table();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    // Hash_Table<int> HT;
    // std::cout << "Size: " << HT.get_size() << "\tCapacity: " << HT.get_capacity() << std::endl; 
    // HT.add_element("hgjvcx", 9);
    // HT.add_element("hgjvcx", 7); //
    // HT.add_element("kjsdae", 9);
    // HT.add_element("ludwsb", 4);
    // HT.add_element("abcdef", 1); //
    // HT.add_element("dfklhs", 5);
    // HT.add_element("vxhgol", 12);
    // HT.add_element("rpotui", 9);
    // HT.add_element("gdjcbf", 32);
    // HT.add_element("luqrzn", 10);
    // HT.add_element("mynlfo", 78);
    // HT.add_element("abcdef", 90);
    // HT.add_element("poiuyg", 23);
    // HT.add_element("aaaaaa", 0); 


    // std::string x = "hgjvcx";
    // HT.remove_element(x);

    // if(HT.get_array().array[0].get_head()!=nullptr){
    //     std::cout << HT.get_array().array[15].get_head()->key << std::endl;
    // }

    // std::string key;
    // for(int i=0; i<pow(10, 4); i++){
    //     key = "";
    //     for(int j=0;j<6;j++){ key += letters[rand()%52]; }
    //     HT.add_element(key, ((rand()<<15)+rand())%1000000);
    // }

    // for(auto i : HT.get_array().get_vector()){ std::cout << i <<'\n'; }

    // std::cout << "Size: " << HT.get_size() << "\tCapacity: " << HT.get_capacity() << std::endl; 

    // if(HT.get_array().array[15].get_head()!=nullptr){
    //     std::cout << HT.get_array().array[15].get_head()->key << std::endl;
    // }
    // std::cout << "Size: " << HT.get_size() << "\tCapacity: " << HT.get_capacity() << std::endl; 

    // for(int i=0;i<HT.get_capacity();i++){
    //     auto begin = HT.get_array().array[i].get_head();
    //     std::cout << "dla i " << i << std::endl;
    //     while(begin != nullptr){
    //         std::cout << " Klucz " << begin->key << '\t' << "Wartosc: " << begin->value << '\n';
    //         begin = begin->next;
    //         // std::cout << "halo kurwa" << '\n';
    //     }
    // }
    // int inde;
    // int max = 0, len;
    // for(int i=0;i<HT.get_array().capacity;i++){
    //     if(HT.get_array().array[i].get_head() != nullptr){
    //         len = HT.get_array().array[i].get_len();
    //         if(len > max){
    //             max = len;
    //             inde = i;
    //         }
    //     }
    // }


    // std::string stats = HT.show_table();
    // std::cout << stats << std::endl;
    // _getch();

    // std::cout << "\n\n\nNajdluzsza lista: " << max << std::endl;

    // HT.get_array().array[inde].show_all_elem();

    // HT.remove_element("poiuyg");

    // auto searched = HT.find_elemnt("zzzzzz");
    // if(searched!=nullptr)
    //     std::cout << "searched_key: " << searched->key << "\tvalue: " << searched->value << std::endl;

    // std::cout << "\n\n";
    // HT.get_array().array[inde].show_all_elem();

    // HT.clear_table();

    // std::string key1;
    // for(int i=0; i<pow(10, 7); i++){
    //     key1 = "";
    //     for(int j=0;j<6;j++){ key1 += letters[rand()%52]; }
    //     HT.add_element(key1, ((rand()<<15)+rand())%1000000);
    // }

    // std::string stats1 = HT.show_table(); 
    // std::cout << stats1 << std::endl;
    // HT.clear_table();

    // HT.add_element("hgjvcx", 9);
    // HT.add_element("hgjvcx", 7); //
    // HT.add_element("kjsdae", 9);
    // HT.add_element("ludwsb", 4);
    // HT.add_element("abcdef", 1); //
    // HT.add_element("dfklhs", 5);
    // HT.add_element("vxhgol", 12);
    // HT.add_element("rpotui", 9);
    // HT.add_element("gdjcbf", 32);
    // HT.add_element("luqrzn", 10);
    // HT.add_element("mynlfo", 78);
    // HT.add_element("abcdef", 90);
    // HT.add_element("poiuyg", 23);
    // HT.add_element("aaaaaa", 0);


    // auto searched = HT.find_elemnt("luqrzn");
    // if(searched!=nullptr)
    //     std::cout << "searched_key: " << searched->key << "\tvalue: " << searched->value << std::endl;

    // for(int i=0;i<HT.get_capacity();i++){
    //     auto begin = HT.get_array().array[i].get_head();
    //     std::cout << "dla i " << i << std::endl;
    //     while(begin != nullptr){
    //         std::cout << " Klucz " << begin->key << '\t' << "Wartosc: " << begin->value << '\n';
    //         begin = begin->next;
    //         // std::cout << "halo kurwa" << '\n';
    //     }
    // }

    // inde=0;
    // max = 0; len=0;
    // for(int i=0;i<HT.get_array().capacity;i++){
    //     if(HT.get_array().array[i].get_head() != nullptr){
    //         len = HT.get_array().array[i].get_len();
    //         if(len > max){
    //             max = len;
    //             inde = i;
    //         }
    //     }
    // }

    // std::cout << "Najdluzsza lista: " << max << std::endl;

    // HT.get_array().array[15].add_at_end(17, "xD");

    // if(HT.get_array().array[15].get_head()!=nullptr){
    //     std::cout << HT.get_array().array[15].get_head()->key << std::endl;
    // }

    // std::cout << "Size: " << HT.get_size() << "\tCapacity: " << HT.get_capacity() << std::endl; 
    // std::cout << "nara" << std::endl;
    return 0;
}