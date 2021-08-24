#include <iostream>

class Array;
class Int;
class Iterator;

class MyArray{
    friend Int;
    friend Iterator;

    const int dim;      // �� ���� �迭����
    int* size;          // size[0]*size[1]*...*size[dim-1]

    struct Address{
        int level;      // 0 ~ N-1 level
        void* next;     // ���� address�� �����ּ� or ������ �迭�� �����ּҸ� ����Ŵ
    };
    Address* top;
public:
    // Iterator
    class Iterator{
        // arr[1][2][3] => location = {1, 2, 3}
	    int* location;  // � ���Ҹ� ����Ű�� �ִ����� ���� ����
	    MyArray* myarr;
        friend Int;
    public:
        Iterator(MyArray* myarr, int* loc = NULL) : myarr(myarr){
            location = new int[myarr->dim];
            for(int i=0; i < myarr->dim; i++)
                location[i] = (loc != NULL ? loc[i] : 0);
        }
        Iterator(const Iterator& itr) : myarr(itr.myarr){
            location = new int[myarr->dim];
            for(int i=0; i<myarr->dim; i++)
                location[i] = itr.location[i];
        }
        ~Iterator(){
            delete[] location;
        }
        Iterator& operator++(){
            if(location[0] >= myarr->size[0]) return *this;

            bool carry = false;     // �޾� �ø��� �ִ���
            int i = myarr->dim - 1;
        
            do{
                location[i]++;
                // if(i==0) : {0 , 0, 0}�� �Ǿ���� �� ���� => iterator�� ó���� ����
                if(location[i] >= myarr->size[i] && i >= 1){
                    location[i] -= myarr->size[i];
                    carry = true;
                    i--;
                }
                else{
                    carry = false;
                }
                }while(i>=0 && carry);
            return *this;
        }
        Iterator operator++(int){
            Iterator itr(*this);
            ++(*this);
            return itr;
        }
        bool operator!=(const Iterator& itr){
            if(itr.myarr->dim != myarr->dim)
                return true;
            for(int i=0; i<myarr->dim; i++){
                if(itr.location[i] != location[i])
                    return true;
            }

            return false;
        }
        Int operator*();
    };
    // ������
    MyArray(int dim, int*array_size) : dim(dim) {
        size = new int[dim];
        for(int i=0; i<dim; i++)
            size[i] = array_size[i];
        
        top = new Address;
        top->level = 0;

        initialize_address(top);
    }
    // ���� ������
    MyArray(const MyArray& arr) : dim(arr.dim) {
        size = new int[dim];
        for(int i=0; i<dim; i++)
            size[i] = arr.size[i];
        
        top = new Address;
        top->level = 0;

        initialize_address(top);

        // ���� ����
        copy_address(top, arr.top);
    }
    // address�� �ʱ�ȭ �ϴ� �Լ�, ��� ȣ��� ����
    void initialize_address(Address *current){
        if(!current) return;
        if(current->level == dim - 1){
            current->next = new int[size[current->level]];
            return;
        }
        current->next = new Address[size[current->level]];
        for(int i=0; i<size[current->level]; i++){
            (static_cast<Address *>(current->next) + i)->level = current->level + 1;
            initialize_address(static_cast<Address *>(current->next) + i);
        }
    }
    // address �Ҹ��Ű�� �Լ�, ��� ȣ��� ����
    void delete_address(Address *current){
        if(!current) return;
        for(int i=0; current->level < dim - 1 && i < size[current->level]; i++)
            delete_address(static_cast<Address *>(current->next) + i);

        if(current->level == dim - 1)
            delete[] static_cast<int *>(current->next);
        
        delete[] static_cast<Address *>(current->next);
    }
    // ���빰 ���� �Լ�
    void copy_address(Address *dst, Address *src){
        if(dst->level == dim -1){
            for(int i=0; i<size[dst->level]; i++)
                static_cast<int *>(dst->next)[i] = static_cast<int *>(src->next)[i];
            return;
        }
        for(int i=0; i<size[dst->level]; i++){
            Address *new_dst = static_cast<Address *>(dst->next);
            Address *new_src = static_cast<Address *>(src->next);
            copy_address(new_dst, new_src);
        }
    }
    // operator [], MyArray ma[2][3][4] ���� ma[2]�� ���⼭ ����
    Int operator[](const int index);
    Iterator begin(){
        int *arr = new int[dim];
        for (int i = 0; i < dim; i++)
            arr[i] = 0;

        Iterator temp(this, arr);
        delete[] arr;

        return temp;
    }
    Iterator end(){
        int *arr = new int[dim];
        arr[0] = size[0];
        for (int i = 1; i < dim; i++)
            arr[i] = 0;

        Iterator temp(this, arr);
        delete[] arr;

        return temp;
    }

    // �Ҹ���
    ~MyArray(){
        delete_address(top);
        delete[] size;
    }
};

class Int{
    void* data;

    int level;
    MyArray* myarr;
public:
    // ������
    Int(int index, int _level = 0, void* _data = NULL, MyArray* _array = NULL) : level(_level), data(_data), myarr(_array){
        if(_level < 1 || index >= myarr->size[_level -1]){
            data = NULL;
            return;
        }
        if(level == myarr->dim){
            data = static_cast<void *>(static_cast<int *>(static_cast<MyArray::Address *>(data)->next) + index);
        }
        else{
            data = static_cast<void *>(static_cast<MyArray::Address *>(static_cast<MyArray::Address *>(data)->next)+index);
        }
    }
    // ���� ������
    Int(const Int& i) : data(i.data), level(i.level), myarr(i.myarr) {}
    // Ÿ�� ��ȯ ������
    operator int(){
        if(data)
            return *static_cast<int *>(data);
        return 0;
    }
    // operator =
    Int& operator=(const int& a){
        if(data)
            *static_cast<int *>(data) = a;
        return *this;
    }
    // operator []
    Int operator[](const int index){
        if(!data) return 0;
        return Int(index, level+1, data, myarr);
    }
};

Int MyArray::operator[](const int index){
    return Int(index, 1, static_cast<void*>(top), this);
}

Int MyArray::Iterator::operator*(){
    Int start = myarr->operator[](location[0]);
    for(int i=1; i<=myarr->dim-1; i++)
        start = start.operator[](location[i]);
    return start;
}

int main(){

}