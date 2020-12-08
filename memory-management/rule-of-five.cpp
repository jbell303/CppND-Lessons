// g++ -stc=c++11 -fno-elide-constructors rule-of-five.cpp
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

    MyMoveableClass(MyMoveableClass &&source) { // move constructor
        std::cout << "Moving (c'tor) instance " << &source \
            << " to instance " << this << std::endl;
        _size = source._size;
        _data = source._data;

        source._data = nullptr;
        source._size = 0;
    }

    MyMoveableClass &operator=(MyMoveableClass &&source) { // move assignment operator
        std::cout << "MOVING (assign) instance " << &source << \
            " to " << this << std::endl;
        if (this == &source)
            return *this;
        delete[] _data;
        _size = source._size;
        _data = source._data;

        source._data = nullptr;
        source._size = 0;

        return *this;
    }
};

int main() {
    MyMoveableClass obj1(100); // constructor
    obj1 = MyMoveableClass(200); // move assignment operator
    MyMoveableClass obj2 = MyMoveableClass(300); // move constructor

    return 0;
}

