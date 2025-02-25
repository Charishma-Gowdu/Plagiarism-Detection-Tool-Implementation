#include <iostream>

struct base_t {
    int x;
    int y;
    base_t() : x(0), y(0) {
        std::cout << "base_t constructor with x = " << x << " y = " << y << std::endl;
    }
    base_t(int __x, int __y) : x(__x), y(__y) {
        std::cout << "base_t constructor with x = " << x << " y = " << y << std::endl;
    }
    virtual ~base_t() {
        std::cout << "base_t destructor with x = " << x << " y = " << y << std::endl;
    }
    void print() {
        std::cout << "base_t x = " << x << " y = " << y << std::endl;
    }
};

struct derived_t : virtual public base_t {
    int y;
    int z;
    derived_t() : base_t(0, 0), y(0), z(0) {
        std::cout << "derived_t constructor with y = " << y << " z = " << z << std::endl;
    }
    derived_t(int __y, int __z) : base_t(0, 0), y(__y), z(__z) {
        std::cout << "derived_t constructor with y = " << y << " z = " << z << std::endl;
    }
    derived_t(int __x, int __y_base, int __y, int __z) : base_t(__x, __y_base), y(__y), z(__z) {
        std::cout << "derived_t constructor with y = " << y << " z = " << z << std::endl;
    }
    ~derived_t() {
        std::cout << "derived_t destructor with y = " << y << " z = " << z << std::endl;
    }
    void print() {
        std::cout << "derived_t y = " << y << " z = " << z << std::endl;
    }
};

int main(void) {
    base_t* base = new derived_t(1, 2, 3, 4);
    std::cout << base->x << " " << base->y << std::endl;
    if (derived_t* derived = dynamic_cast<derived_t*>(base)) {
        std::cout << derived->y << " " << derived->z << std::endl;
        derived->print();
    }
    base->print();
    delete base;
}
