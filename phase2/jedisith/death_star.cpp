#include <fstream>
#include <memory>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

template <typename T, class Compare = std::less<T>>
class heap_t {
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

public:
    heap_t();
    ~heap_t();
    heap_t(const heap_t& other);
    heap_t(heap_t&& other) noexcept;
    heap_t& operator = (const heap_t& other);
    heap_t& operator = (heap_t&& other) noexcept;

    void push(const value_type& value);
    value_type pop();
    const_reference top() const;
    size_type size() const;
    bool empty() const;
    void clear();

protected:
    std::vector<value_type> data;
    Compare comp;
    void heapify(size_type i);
    inline size_type parent(size_type i);
    inline size_type left(size_type i);
    inline size_type right(size_type i);
    void build_heap();
};

template <typename T, class Compare>
heap_t<T, Compare>::heap_t() {
    data.clear();
}

template <typename T, class Compare>
heap_t<T, Compare>::~heap_t() {
    data.clear();
}

template <typename T, class Compare>
heap_t<T, Compare>::heap_t(const heap_t& other) {
    data = other.data;
}

template <typename T, class Compare>
heap_t<T, Compare>::heap_t(heap_t&& other) noexcept {
    data = std::move(other.data);
}

template <typename T, class Compare>
heap_t<T, Compare>& heap_t<T, Compare>::operator = (const heap_t& other) {
    data = other.data;
    return *this;
}

template <typename T, class Compare>
heap_t<T, Compare>& heap_t<T, Compare>::operator = (heap_t&& other) noexcept {
    data = std::move(other.data);
    return *this;
}

template <typename T, class Compare>
void heap_t<T, Compare>::push(const value_type& value) {
    data.push_back(value);
    size_type i = data.size() - 1;
    while (i > 0 && comp(data[i], data[parent(i)])) {
        std::swap(data[parent(i)], data[i]);
        i = parent(i);
    }
}

template <typename T, class Compare>
typename heap_t<T, Compare>::value_type heap_t<T, Compare>::pop() {
    if (data.size() == 0) {
        throw std::out_of_range("Heap is empty");
    }
    value_type top = data[0];
    data[0] = data[data.size() - 1];
    data.pop_back();
    this->heapify(0);
    return top;
}

template <typename T, class Compare>
typename heap_t<T, Compare>::const_reference heap_t<T, Compare>::top() const {
    if (data.size() == 0) {
        throw std::out_of_range("Heap is empty");
    }
    return data[0];
}

template <typename T, class Compare>
typename heap_t<T, Compare>::size_type heap_t<T, Compare>::size() const {
    return data.size();
}

template <typename T, class Compare>
bool heap_t<T, Compare>::empty() const {
    return data.size() == 0;
}

template <typename T, class Compare>
void heap_t<T, Compare>::clear() {
    data.clear();
}

template <typename T, class Compare>
void heap_t<T, Compare>::heapify(size_type i) {
    size_type l = left(i);
    size_type r = right(i);
    size_type largest = i;
    if (l < data.size() && comp(data[l], data[i])) {
        largest = l;
    }
    if (r < data.size() && comp(data[r], data[largest])) {
        largest = r;
    }
    if (largest != i) {
        std::swap(data[i], data[largest]);
        heapify(largest);
    }
}

template <typename T, class Compare>
typename heap_t<T, Compare>::size_type heap_t<T, Compare>::parent(size_type i) {
    return (i - 1) / 2;
}

template <typename T, class Compare>
typename heap_t<T, Compare>::size_type heap_t<T, Compare>::left(size_type i) {
    return 2 * i + 1;
}

template <typename T, class Compare>
typename heap_t<T, Compare>::size_type heap_t<T, Compare>::right(size_type i) {
    return 2 * i + 2;
}

template <typename T, class Compare>
void heap_t<T, Compare>::build_heap() {
    for (size_type i = data.size() / 2; i > 0; i--) {
        heapify(i);
    }
    if (data.size() > 0) {
        heapify(0);
    }
}

template <typename T>
struct list_node_t {
    T data;
    std::shared_ptr<list_node_t<T>> next = nullptr;
    std::weak_ptr<list_node_t<T>> prev;

    list_node_t() = default;
    list_node_t(const T& val) : data(val) {}
    ~list_node_t() = default;
};

template <typename T, class Compare>
std::shared_ptr<struct list_node_t<T>> merge_sorted_lists(
        std::vector<std::shared_ptr<struct list_node_t<T>>> lists) {
    heap_t<std::shared_ptr<struct list_node_t<T>>, Compare> heap;
    for (auto& list : lists) {
        if (list) {
            heap.push(list);
        }
    }
    std::shared_ptr<struct list_node_t<T>> head = nullptr;
    std::shared_ptr<struct list_node_t<T>> ptr = nullptr;
    while (!heap.empty()) {
        auto node = heap.top();
        heap.pop();
        if (node->next) {
            heap.push(node->next);
        }
        if (!head) {
            head = std::make_shared<struct list_node_t<T>>(*node);
            ptr = head;
        } else {
            ptr->next = std::make_shared<struct list_node_t<T>>(*node);
            ptr->next->prev = head;
            ptr = ptr->next;
        }
    }
    return head;
}

struct student_t {
    std::string name = "";
    size_t roll_no = 0;
    double height = 0.0;

    student_t() = default;
    student_t(const std::string& new_name, size_t roll, double ht)
        : name(new_name), roll_no(roll), height(ht) {}
    ~student_t() = default;
};

class compare_by_heights {
public:
    bool operator() (
            const std::shared_ptr<struct list_node_t<struct student_t>>& a,
            const std::shared_ptr<struct list_node_t<struct student_t>>& b) {
        return a->data.height < b->data.height;
    }
};
