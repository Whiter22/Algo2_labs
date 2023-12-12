#include "binary_heap.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <conio.h>
#include <random>
#include <time.h>

double rand_double(){
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    double randomDouble = distribution(gen);
    return randomDouble;
}

int rand_int(){
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distribution(0, pow(10,7));
    int randomInt = distribution(gen);
    return randomInt;
}

struct Wagon{
    int wagon_num;
    double weight;
};

void counting_sort(std::vector<int>& arr, int max){
    int n = arr.size();
    std::vector<int> counts;

    for(int i=0; i<=max; i++){
        counts.push_back(0);
    }

    for(int i=0;i<n;i++){
        counts[arr[i]]+=1;
    }

    int k = 0;
    for(int i=0; i<=max; i++){
        for(int j=0; j<counts[i]; j++){
            arr[k++]=i;
        }
    }
}

template <typename T>
void insertion_sort(std::vector<T>& arr, bool (*cmp)(T, T)) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        T key = arr[i];
        int j = i - 1;

        while (j >= 0 && cmp(arr[j], key)) {
            arr[j + 1] = arr[j];
            --j;
        }

        arr[j + 1] = key;
    }
}


void bucket_sort(std::vector<int>& arr, double n, double m){
    int bucket_ind;
    double w = m/n;

    std::vector<std::vector<int>> buckets(n);

    for(int i=0;i<arr.size();i++){
        bucket_ind = double(arr[i])/w;
        buckets[bucket_ind].push_back(arr[i]);
    }

    for(auto& array : buckets){
        std::stable_sort(array.begin(), array.end());
    }

    arr.clear();
    for(auto& array : buckets){
        arr.insert(arr.end(), array.begin(), array.end());
    }
}

template <typename T>
void bucket_sort(std::vector<T>& arr, double n, double m, double (*get_val)(T), bool (*cmp)(T,T)){
    int bucket_ind;
    double w = m/n;

    std::vector<std::vector<T>> buckets(n);

    for(int i=0;i<arr.size();i++){
        bucket_ind = get_val(arr[i])/w;
        buckets[bucket_ind].push_back(arr[i]);
    }

    for(auto& array : buckets){
        insertion_sort(array, cmp);
    }

    arr.clear();
    for(auto& array : buckets){
        arr.insert(arr.end(), array.begin(), array.end());
    }
}

bool comparator(int a, int b){ return a>=b; }
bool comparator(Wagon w1, Wagon w2){ return w1.weight >= w2.weight; }
bool comparator1(Wagon w1, Wagon w2){ return w1.weight < w2.weight; }

double getKey(Wagon w){ return w.weight; }

// void bucket_sort(std::vector<int>& arr, double n, double m){
//     int bucket_ind;
//     double w = m/n;

//     std::vector<std::vector<int>> buckets(n);

//     for(int i=0;i<arr.size();i++){
//         bucket_ind = double(arr[i])/w;
//         buckets[bucket_ind].push_back(arr[i]);
//     }

//     for(auto& array : buckets){
//         std::stable_sort(array.begin(), array.end());
//     }

//     arr.clear();
//     for(auto& array : buckets){
//         arr.insert(arr.end(), array.begin(), array.end());
//     }
// }

// void counting_sort(int* arr, int n, int m){
//     int* counts = new int[m];

//     for(int i=0; i<=m; i++){
//         counts[i] = 0;
//     }

//     for(int i=0;i<n;i++){
//         counts[arr[i]]++;
//     }

//     int k = 0;
//     for(int i=0; i<=m; i++){
//         for(int j=0; j<counts[i]; j++){
//             arr[k++]=i;
//         }
//     }
//     delete[] counts;
// }

int main(){

    //TEST INTS:

    // srand(0);
    const int MAX_ORDER = 7;
    const int m = pow(10,7);

    for(int o=1;o<=MAX_ORDER;o++){
        const int n = pow(10, o);
        std::cout << "\n\t\t--FOR SIZE: " << n << std::endl;

        std::vector<int> vec0;
        for(int i=0;i<n;i++){
            vec0.push_back(rand_int());
        }
        auto vec1 = vec0;
        auto vec2 = vec0;

        std::cout << "\t\tBefore sort: \n";
        for(int i=0;i<10;i++){
            std::cout << vec0[i] << ", ";
        } 
        std::cout << std::endl;

        auto max = std::max_element(vec0.begin(), vec0.end()); //returns iterator/pointer
        //Counting_sort
        clock_t t1 = clock();
        counting_sort(vec0, *max);
        clock_t t2 = clock();

        std::cout << "\t\nCouting sort: \n";
        for(int i=0;i<10;i++){
            std::cout << vec0[i] << ", ";
        } 
        std::cout << std::endl;
        double full_time = double(t2-t1)/double(CLOCKS_PER_SEC);
        std::cout << "Counting_Sort time: " << full_time*1000 << " ms" << std::endl;
        
        std::cout << "\n---------------------------------------------------------------------------------------------------------------" << std::endl;
        
        Binary_Heap<int> BH(vec1, true, comparator);
        t1 = clock();
        BH.heap_sort(comparator);
        t2 = clock();

        std::cout << "\t\nHeap sort: \n";
        for(int i=vec0.size()-1;i>vec1.size()-11;i--){
            std::cout << vec1[i] << ", ";
        } 
        std::cout << std::endl;
        full_time = double(t2-t1)/double(CLOCKS_PER_SEC);
        std::cout << "Heap_Sort time: " << full_time*1000 << " ms" << std::endl;
        
        std::cout << "\n---------------------------------------------------------------------------------------------------------------" << std::endl;

        t1 = clock();
        bucket_sort(vec2, vec2.size(), m);
        t2 = clock();

        std::cout << "\t\nBucket sort: \n";
        for(int i=0;i<10;i++){
            std::cout << vec2[i] << ", ";
        } 
        std::cout << std::endl;
        full_time = double(t2-t1)/double(CLOCKS_PER_SEC);
        std::cout << "Bucket time: " << full_time*1000 << " ms" << std::endl;
        
        std::cout << "\n---------------------------------------------------------------------------------------------------------------" << std::endl;

        std::reverse(vec1.begin(), vec1.end());

        for (size_t i = 0; i < vec0.size(); ++i) {
            if (vec0[i] != vec1[i] || vec1[i] != vec2[i] || vec0[i] != vec2[i]) {
                std::cout << "SAME: FALSE" << std::endl;
                break;
            }
            else if(i == vec0.size()-1){
                std::cout << "SAME: TRUE" << std::endl;
            }
        }

        _getch();
    }


    std::cout << "\n\n\t\t\tStarting sort for objects\n\n\n----------------------------------------------------------------------------------------------------" << std::endl;
    _getch();


    //TEST some objects

    for(int o=1;o<=MAX_ORDER;o++){
        const int n = pow(10, o);
        
        std::cout << "\n\t\t--FOR SIZE: " << n << std::endl;

        std::vector<Wagon> some_vec;
        for(int i=0;i<n;i++){
            Wagon wagon;
            wagon.wagon_num = rand();
            wagon.weight = rand_double();
            some_vec.push_back(wagon);
        }
        auto some_vec1 = some_vec;

        std::cout << "\t\tBefore sort: \n";
        for(int i=0;i<10;i++){
            std::cout << some_vec[i].weight << ", ";
        } 
        std::cout << std::endl;
        Binary_Heap<Wagon> BH(some_vec, true, comparator);
        clock_t t1 = clock();
        BH.heap_sort(comparator);
        clock_t t2 = clock();

        std::cout << "\t\nHeap sort: \n";
        for(int i=some_vec1.size()-1;i>some_vec1.size()-11;i--){
            std::cout << some_vec[i].weight << ", ";
        } 
        std::cout << std::endl;
        double full_time = double(t2-t1)/double(CLOCKS_PER_SEC);
        std::cout << "Heap_Sort time: " << full_time*1000 << " ms" << std::endl;
        
        std::cout << "\n---------------------------------------------------------------------------------------------------------------" << std::endl;

        t1 = clock();
        bucket_sort(some_vec1, double(some_vec1.size()), 1.0, getKey, comparator);
        t2 = clock();

        std::cout << "\t\nBucket sort: \n";
        for(int i=0;i<10;i++){
            std::cout << some_vec1[i].weight << ", ";
        } 
        std::cout << std::endl;
        full_time = double(t2-t1)/double(CLOCKS_PER_SEC);
        std::cout << "Bucket time: " << full_time*1000 << " ms" << std::endl;
        
        std::cout << "\n---------------------------------------------------------------------------------------------------------------" << std::endl;

        std::reverse(some_vec.begin(), some_vec.end());

        for (size_t i = 0; i < some_vec.size(); ++i) {
            if (some_vec[i].weight != some_vec1[i].weight) {
                std::cout << "SAME: FALSE" << std::endl;
                break;
            }
            else if(i == some_vec.size()-1){
                std::cout << "SAME: TRUE" << std::endl;
            }
        }

        _getch();
    }

    

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // std::vector<int> vec;
    // for(int i=0;i<pow(10,7);i++){
    //     vec.push_back((rand()<<15)+rand());
    // }

    // std::cout << "Adding done to vec" << std::endl;
    // Binary_Heap<int> BH(vec, false, comparator);
    // std::cout << "Adding done to Heap" << std::endl;
    // BH.heap_sort(comparator);
    // std::cout << "After H Sort:\n";
    // for(int i=0;i<50;i++){
    //     std::cout << vec[i] << ", ";
    // }
    // std::cout << std::endl;


    // std::vector<Wagon> some_vec;
    // for(int i=0;i<pow(10,7);i++){
    //     Wagon wagon;
    //     wagon.wagon_num = rand();
    //     wagon.weight = rand_double();
    //     some_vec.push_back(wagon);
    // }
    // auto some_vec1 = some_vec;

    // std::cout << "\t\tBefore sort: \n";
    // for(int i=0;i<50;i++){
    //     std::cout << some_vec[i].weight << ", ";
    // } 
    // std::cout << std::endl;

    // Binary_Heap<Wagon> BH(some_vec, true, comparator);

    // BH.heap_sort(comparator);

    // std::cout << "After Heap sort:\n";
    // for(int i=0;i<50;i++){
    //     std::cout << some_vec[i].weight << ", ";
    // } 
    // std::cout << std::endl;

    // bucket_sort(some_vec1, double(some_vec1.size()), 1.0, getKey, comparator1);

    // std::cout << "After Bucket sort:\n";
    // for(int i = some_vec1.size() - 50; i < some_vec1.size(); ++i){
    //     std::cout << some_vec1[i].weight << ", ";
    // } 
    // std::cout << std::endl;


    //double
    // std::vector<Wagon> some_vec;
    // for(int i=0;i<20;i++){
    //     Wagon wagon;
    //     wagon.wagon_num = rand();
    //     wagon.weight = rand_double();
    //     some_vec.push_back(wagon);
    // }
    // auto some_vec1 = some_vec;

    // std::cout << "\t\tBefore sort: \n";
    // for(Wagon elements : some_vec){
    //     std::cout << elements.weight << ", ";
    // } 
    // std::cout << std::endl;


    // Binary_Heap<Wagon> BH(some_vec, true, comparator);

    // BH.heap_sort(comparator);

    // std::cout << "After Heap sort:\n";
    // for(Wagon elements : some_vec){
    //     std::cout << elements.weight << ", ";
    // } 
    // std::cout << std::endl;

    // bucket_sort(some_vec1, 20.0, 1.0, getKey, comparator1);

    // std::cout << "After Bucket sort:\n";
    // for(Wagon elements : some_vec1){
    //     std::cout << elements.weight << ", ";
    // } 
    // std::cout << std::endl;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //inty
    // Heap-Sort
    // std::vector<int> some_vec = {45, 12, 78, 34, 56, 89, 23, 67, 90, 14, 27, 90, 50, 73, 19, 62, 8, 41, 95, 30, 55};
    // auto some_vec1 = some_vec;
    // auto some_vec2 = some_vec;
    // std::cout << "Size: " << some_vec.size() << std::endl;
    // std::cout <<"\t\tBEFORE SORTING\n";
    //     for(int elements : some_vec){
    //     std::cout << elements << ", ";
    // }
    // std::cout << "\n---------------------------------------------------------------------------------------------------------------" << std::endl;

    // Binary_Heap<int> BH(some_vec, true, comparator);

    // BH.heap_sort(comparator);    
    
    // std::cout << "After H Sort:\n";
    // for(int elements : some_vec){
    //     std::cout << elements << ", ";
    // }
    // std::cout << std::endl;

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // //Counting-Sort
    // auto max = std::max_element(some_vec1.begin(), some_vec1.end()); //returns iterator/pointer
    // std::cout << "MAX: " << *max << std::endl;
    // counting_sort(some_vec1, *max);

    // std::cout << "Counting sort:\n";
    // for(int elements : some_vec1){
    //     std::cout << elements << ", ";
    // }
    // std::cout << std::endl;

    // std::cout << "after some_vec1 size: " << some_vec1.size() << std::endl;

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Bucket-Sort
    // std::cout << "\nBucket sort:\n";
    // std::cout <<"\t\tBEFORE SORTING\n";
    //     for(int elements : some_vec2){
    //     std::cout << elements << ", ";
    // }
    // bucket_sort(some_vec2, some_vec2.size(), 100);

    // std::cout << "\nBucket error not sort:\n";
    // for(int elements : some_vec2){
    //     std::cout << elements << ", ";
    // }
    // std::cout << std::endl;

    // int *arr1 = new int[20];
    // for(int i=0;i<20;i++){ arr1[i] = some_vec1[i]; }
    // std::cout << "Before: \n"; 
    // for(int i=0;i<20;i++){std::cout << arr1[i] << ", ";}
    // std::cout << std::endl;

    // counting_sort(arr1, 20, 95);

    // std::cout << "After: \n"; 
    // for(int i=0;i<20;i++){std::cout << arr1[i] << ", ";}    

    return 0;
}