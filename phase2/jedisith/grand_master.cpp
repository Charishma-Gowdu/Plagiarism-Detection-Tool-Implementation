#include <cmath>
#include <iostream>
#include <memory>

class complex_base {
public:
        virtual double get_real() = 0;
        virtual double get_imag() = 0;
        virtual double get_mag() = 0;
        virtual double get_arg() = 0;
        virtual void set_real(double) = 0;
        virtual void set_imag(double) = 0;
        virtual void set_mag(double) = 0;
        virtual void set_arg(double) = 0;
        virtual ~complex_base() = default;
};

class complex_from_real_imag : public complex_base {
        double real;
        double imag;
public:
        complex_from_real_imag() : real(0), imag(0) {}
        complex_from_real_imag(double __r) : real(__r), imag(0) {}
        complex_from_real_imag(double __r, double __i) : real(__r), imag(__i) {}
        ~complex_from_real_imag() {}
        double get_real() { return real; }
        double get_imag() { return imag; }
        double get_mag() { return sqrt(real*real + imag*imag); }
        double get_arg() { return atan2(imag, real); }
        void set_real(double __r) { real = __r; }
        void set_imag(double __i) { imag = __i; }
        void set_mag(double __m) {
                double arg = get_arg();
                real = __m * cos(arg);
                imag = __m * sin(arg);
        }
        void set_arg(double __a) {
                double mag = get_mag();
                real = mag * cos(__a);
                imag = mag * sin(__a);
        }
};

class complex_from_mag_arg : public complex_base {
        double mag;
        double arg;
public:
        complex_from_mag_arg() : mag(1), arg(0) {}
        complex_from_mag_arg(double __m) : mag(__m), arg(0) {}
        complex_from_mag_arg(double __m, double __a) : mag(__m), arg(__a) {}
        ~complex_from_mag_arg() {}
        double get_real() { return mag * cos(arg); }
        double get_imag() { return mag * sin(arg); }
        double get_mag() { return mag; }
        double get_arg() { return arg; }
        void set_real(double __r) {
                mag = sqrt(__r*__r + get_imag()*get_imag());
                arg = atan2(get_imag(), __r);
        }
        void set_imag(double __i) {
                mag = sqrt(get_real()*get_real() + __i*__i);
                arg = atan2(__i, get_real());
        }
        void set_mag(double __m) { mag = __m; }
        void set_arg(double __a) { arg = __a; }
};

class complex_t {
        class complex_base* ptr = nullptr;
public:
        complex_t() {
                if (this->ptr) delete this->ptr;
                this->ptr = new complex_from_real_imag();
        }
        complex_t(double __r) {
                if (this->ptr) delete this->ptr;
                this->ptr = new complex_from_real_imag(__r);
        }
        complex_t(double __r, double __i, bool __mag_arg = false) {
                if (this->ptr) delete this->ptr;
                if (__mag_arg) this->ptr = new complex_from_mag_arg(__r, __i);
                else this->ptr = new complex_from_real_imag(__r, __i);
        }
        complex_t(class complex_base* __ptr) {
                if (this->ptr) delete this->ptr;
                this->ptr = __ptr;
        }
        class complex_base* operator -> () { return ptr; }
        complex_t(complex_t &__rhs) {
                if (this->ptr) delete this->ptr;
                this->ptr = new complex_from_real_imag(
                                __rhs->get_real(), __rhs->get_imag());
        }
        complex_t operator = (complex_t &__rhs) {
                class complex_base* new_ptr = new complex_from_real_imag(
                                __rhs->get_real(), __rhs->get_imag());
                if (this->ptr) delete this->ptr;
                this->ptr = new_ptr;
                return *this;
        }
        complex_t operator += (complex_t &__rhs) {
                class complex_base* new_ptr = new complex_from_real_imag(
                                this->ptr->get_real() + __rhs->get_real(),
                                this->ptr->get_imag() + __rhs->get_imag());
                if (this->ptr) delete this->ptr;
                this->ptr = new_ptr;
                return *this;
        }
        complex_t operator -= (complex_t &__rhs) {
                class complex_base* new_ptr = new complex_from_real_imag(
                                this->ptr->get_real() - __rhs->get_real(),
                                this->ptr->get_imag() - __rhs->get_imag());
                if (this->ptr) delete this->ptr;
                this->ptr = new_ptr;
                return *this;
        }
        complex_t operator *= (complex_t &__rhs) {
                class complex_base* new_ptr = new complex_from_mag_arg(
                                this->ptr->get_mag() * __rhs->get_mag(),
                                this->ptr->get_arg() + __rhs->get_arg());
                if (this->ptr) delete this->ptr;
                this->ptr = new_ptr;
                return *this;
        }
        complex_t operator /= (complex_t &__rhs) {
                class complex_base* new_ptr = new complex_from_mag_arg(
                                this->ptr->get_mag() / __rhs->get_mag(),
                                this->ptr->get_arg() - __rhs->get_arg());
                if (this->ptr) delete this->ptr;
                this->ptr = new_ptr;
                return *this;
        }
        ~complex_t() { if (this->ptr) delete this->ptr; }
};

bool operator == (complex_t lhs, complex_t rhs) {
        return lhs->get_real() == rhs->get_real() && 
                        lhs->get_imag() == rhs->get_imag();
}

complex_t operator + (complex_t lhs, complex_t rhs) {
        return complex_t(lhs->get_real() + rhs->get_real(), 
                        lhs->get_imag() + rhs->get_imag(), false);
}

complex_t operator - (complex_t lhs, complex_t rhs) {
        return complex_t(lhs->get_real() - rhs->get_real(), 
                        lhs->get_imag() - rhs->get_imag(), false);
}

complex_t operator - (complex_t rhs) {
        return complex_t(-rhs->get_real(), -rhs->get_imag(), false);
}

complex_t operator * (complex_t lhs, complex_t rhs) {
        return complex_t(lhs->get_mag() * rhs->get_mag(), 
                        lhs->get_arg() + rhs->get_arg(), true);
}

complex_t operator / (complex_t lhs, complex_t rhs) {
        return complex_t(lhs->get_mag() / rhs->get_mag(), 
                        lhs->get_arg() - rhs->get_arg(), true);
}

int main() {
        complex_t a(4, 3);
        complex_t b(5, 0.9272952180016123, true);
        // That number is arcsin(4/5).
        complex_t c = a + b;
        complex_t d = a - b;
        complex_t e = -a;
        complex_t f = a * b;
        complex_t g = a / b;
        std::cout << c->get_real() << " + " << c->get_imag() << "i\n";
        std::cout << d->get_real() << " + " << d->get_imag() << "i\n";
        std::cout << e->get_real() << " + " << e->get_imag() << "i\n";
        std::cout << f->get_real() << " + " << f->get_imag() << "i\n";
        std::cout << g->get_real() << " + " << g->get_imag() << "i\n";
        return 0;
}
