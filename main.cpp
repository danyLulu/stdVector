#include "vector.h"


int main(){
    MyVector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    std::cout<<"Элементы вектора"<<std::endl;
    for(int i = 0; i < v.getSize(); ++i){
        std::cout<<v[i]<<std::endl;
    }

    std::cout<<"Размер вектора: "<<v.getSize()<<std::endl;
    std::cout<<"Емкость вектора: "<<v.getCapacity()<<std::endl;

    v.pop_back();
    std::cout<<"Размер после pop_back: "<<v.getSize()<<std::endl;

    std::cout<<"empty: "<<v.empty()<<std::endl;

    std::cout<<"front: "<<v.front()<<std::endl;
    std::cout<<"back: "<<v.back()<<std::endl;

    std::cout<<"at(1): "<<v.at(1)<<std::endl;


    std::cout<<"range-based for: ";
    for(auto x : v){
        std::cout<<x<<" ";
    }
    std::cout<<std::endl;


    // resize
    v.resize(5);
    std::cout<<"После resize(5): ";
    for(auto x : v){
        std::cout<<x<<" ";
    }
    std::cout<<std::endl;

    // shrink_to_fit
    v.shrink_to_fit();
    std::cout<<"Capacity после shrink_to_fit: "<<v.getCapacity()<<std::endl;

    // конструктор копирования
    MyVector<int> v2 = v;
    std::cout<<"v2 (копия): ";
    for(auto x : v2){
        std::cout<<x<<" ";
    }
    std::cout<<std::endl;

    // оператор присваивания
    MyVector<int> v3;
    v3 = v;
    std::cout<<"v3 (присваивание): ";
    for(auto x : v3){
        std::cout<<x<<" ";
    }
    std::cout<<std::endl;


    return 0;
}