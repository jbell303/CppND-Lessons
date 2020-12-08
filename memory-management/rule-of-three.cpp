#include <iostream>
#include <stdlib.h>

class MyMoveableClass {
private:
    int _size;
    int* _data;

public:
    MyMoveableClass (size_t size) { // constructor
        _size = size;
        _data = new int[_size];
        std::cout << "CREATING instance of MyMovableClass at " \
            << this << " allocated with size = " << _size*sizeof(int) \
            << " bytes" << std::endl;
    }

    ~MyMoveableClass() { // destructor
        std::cout << "DELETING instance of MyMovable Class at " << \
            this << std::endl;
        delete[] _data;
    }

    MyMoveableClass(const MyMoveableClass &source) { // copy constructor
        _size = source._size;
        _data = new int[_size];
        *_data = *source._data;
        std::cout << "COPYING content of instance " << &source << \
            " to instance " << this << std::endl;
    }

    MyMoveableClass &operator=(const MyMoveableClass &source) { // copy assignment operator
        std::cout << "ASSIGNING content of instance " << &source << \
            " to instance " << this << std::endl;
        if (this == &source) {
            return *this;
        }
        delete[] _data;
        _data = new int[source._size];
        *_data = *source._data;
        _size = source._size;
        return *this;
    }
};

int main() {
    MyMoveableClass obj1(10); // regular constructor
    MyMoveableClass obj2(obj1); // copy constructor
    obj2 = obj1; // copy assignment operator

    return 0;
}

