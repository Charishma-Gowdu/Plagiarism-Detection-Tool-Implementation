#include <iostream>
#include <utility>
#include <stdexcept>

void pass(void) {return;}
#define AT(N) __get_val<N>()

template <size_t Net, size_t N, typename... Args>
struct tuple_leaf;

template <size_t Net, typename... Args> requires (sizeof...(Args) == 0)
struct tuple_leaf<Net, 0, Args...> {
    virtual ~tuple_leaf() = default;
    tuple_leaf() = default;
    tuple_leaf(const tuple_leaf&) = default;
    tuple_leaf(tuple_leaf&&) = default;
    tuple_leaf& operator = (const tuple_leaf&) = default;
    tuple_leaf& operator = (tuple_leaf&&) = default;
};

template <size_t Net, size_t N, typename T, typename... Args> requires (N > 0) && (N <= sizeof...(Args) + 1)
struct tuple_leaf<Net, N, T, Args...> : public tuple_leaf<Net, N - 1, Args...> {
protected:
    T __value;
public:
    virtual ~tuple_leaf() = default;
    tuple_leaf() : tuple_leaf<Net, N - 1, Args...>(), __value() {}
    tuple_leaf(T __arg, Args... __args) : tuple_leaf<Net, N - 1, Args...>(__args...), __value(__arg) {}
    tuple_leaf(const tuple_leaf&) = default;
    tuple_leaf(tuple_leaf&&) = default;
    tuple_leaf& operator = (const tuple_leaf&) = default;
    tuple_leaf& operator = (tuple_leaf&&) = default;
    template <size_t Q> auto& __get() {
        if constexpr (Q == Net - N) {
            return __value;
        } else {
            return tuple_leaf<Net, N - 1, Args...>::template __get<Q>();
        }
    }
};

template <typename... Args>
struct tuple : public tuple_leaf<sizeof...(Args), sizeof...(Args), Args...> {
    virtual ~tuple() = default;
    tuple() = default;
    tuple(Args... args) : tuple_leaf<sizeof...(Args), sizeof...(Args), Args...>(args...) {}
    tuple(const tuple&) = default;
    tuple(tuple&&) = default;
    tuple& operator = (const tuple& oth) = default;
    tuple& operator = (tuple&& oth) = default;
    template <size_t Q> auto& __get_val() {
        static_assert(Q < sizeof...(Args), "Tuple index out of range");
        return tuple_leaf<sizeof...(Args), sizeof...(Args), Args...>::template __get<Q>();
    }
};

int main(void) {
    tuple<int, int, double> t;
    t.AT(0) = 10;
    t.AT(2) = 3.14;
    std::cout << t.AT(0) << ' ' << t.AT(1) << ' ' << t.AT(2) << " " << sizeof(t) << std::endl;
    return 0;
}
