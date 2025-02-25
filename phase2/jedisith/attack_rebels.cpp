#include <iostream>

class base_t {
    int a;
public:
    base_t() : a(0) {}
    base_t(int __a) : a(__a) {}
    virtual ~base_t() = default;
    int get_a() { return this->a; }
    void set_a(int __a) { this->a = __a; }
};

class derived_t : public base_t {
    int b;
public:
    derived_t() : base_t(), b(0) {}
    derived_t(int __a, int __b) : base_t(__a), b(__b) {}
    int get_b() { return this->b; }
    void set_b(int __b) { this->b = __b; }
};

class derived_virt_t : virtual public base_t {
    int c;
public:
    derived_virt_t() : base_t(), c(0) {}
    derived_virt_t(int __a, int __c) : base_t(__a), c(__c) {}
    int get_c() { return this->c; }
    void set_c(int __c) { this->c = __c; }
};

int main(void) {
    class base_t* base = new base_t;
    base->set_a(10);
    std::cout << (dynamic_cast<class derived_t*>(base))->get_a() << std::endl;
    delete base;
    return 0;
}
