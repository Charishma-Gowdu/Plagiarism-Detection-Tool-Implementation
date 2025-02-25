#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#define MAX_SIZE 9

struct node_t {
    long val = 0;
    virtual ~node_t() {}
};

struct n_ary_node_t : public node_t {
    class std::vector<std::shared_ptr<node_t>> children;
};

template <size_t N = 0>
struct fixed_node_t : public node_t {
    static_assert(N <= MAX_SIZE && N >= 1);
    class std::array<std::shared_ptr<node_t>, N> children = {nullptr};
};

namespace decoder {
    std::shared_ptr<node_t> decode_n_ary_tree(std::string &encoded,
                                                        int &index);
    template <size_t N> std::shared_ptr<node_t> decode_fixed_tree(
                                            std::string &encoded, int &index);
    template <size_t N> typename 
    std::enable_if<std::is_lteq(N <=> MAX_SIZE), std::shared_ptr<node_t>>::type
    decode_fixed_tree_tester(std::string &encoded, int &index, size_t n);
    template <size_t N> typename
    std::enable_if<std::is_gt(N <=> MAX_SIZE), std::shared_ptr<node_t>>::type
    decode_fixed_tree_tester(std::string &encoded, int &index, size_t n);
};

std::shared_ptr<struct node_t> decoder::decode_n_ary_tree(std::string &encoded, 
                                                    int &index) {
    std::string num = "";
    if (encoded[index] == '-') num += encoded[index++];
    while (encoded[index] >= '0' && encoded[index] <= '9') 
        num += encoded[index++];
    std::shared_ptr<n_ary_node_t> root = std::make_shared<n_ary_node_t>();
    root->val = std::stol(num);
    while (encoded[index] != ']') {
        if (encoded[index++] != '[') throw std::invalid_argument("Bad input");
        root->children.push_back(decoder::decode_n_ary_tree(encoded, index));
    }
    index++;
    return std::dynamic_pointer_cast<node_t>(root);
}

template <size_t N> std::shared_ptr<node_t> decoder::decode_fixed_tree(
                                            std::string &encoded, int &index) {
    if (encoded[index] == 'N') {
        if (encoded[++index] != 'U') throw std::invalid_argument("Bad input");
        if (encoded[++index] != 'L') throw std::invalid_argument("Bad input");
        if (encoded[++index] != 'L') throw std::invalid_argument("Bad input");
        if (encoded[++index] != ')') throw std::invalid_argument("Bad input");
        index++;
        return nullptr;
    }
    std::string num = "";
    if (encoded[index] == '-') num += encoded[index++];
    while (encoded[index] >= '0' && encoded[index] <= '9') 
        num += encoded[index++];
    std::shared_ptr<fixed_node_t<N>> root = std::make_shared<fixed_node_t<N>>();
    root->val = std::stol(num);
    for (size_t i = 0; i < N; i++) {
        if (encoded[index++] != '(') throw std::invalid_argument("Bad input");
        root->children[i] = decoder::decode_fixed_tree<N>(encoded, index);
    }
    if (encoded[index++] != ')') throw std::invalid_argument("Bad input");
    return std::dynamic_pointer_cast<node_t>(root);
}

template <size_t N> typename
std::enable_if<std::is_lteq(N <=> MAX_SIZE), std::shared_ptr<node_t>>::type
decoder::decode_fixed_tree_tester(std::string &encoded, int &index, size_t n) {
    if (n > N) {
        return decoder::decode_fixed_tree_tester<N + 1>(encoded, index, n);
    } else {
        return decoder::decode_fixed_tree<N>(encoded, index);
    }
}

template <size_t N> typename
std::enable_if<std::is_gt(N <=> MAX_SIZE), std::shared_ptr<node_t>>::type
decoder::decode_fixed_tree_tester(std::string &encoded, int &index, size_t n) {
    throw std::invalid_argument("Invalid tree");
}

namespace encoder {
    std::string encode_n_ary_tree(std::shared_ptr<node_t> root);
    template <size_t N> std::string encode_fixed_tree(
                                    std::shared_ptr<node_t> root);
    template <size_t N> 
    typename std::enable_if<std::is_lteq(N <=> MAX_SIZE), std::string>::type
    encode_fixed_tree_tester(std::shared_ptr<node_t> root);
    template <size_t N> 
    typename std::enable_if<std::is_gt(N <=> MAX_SIZE), std::string>::type
    encode_fixed_tree_tester(std::shared_ptr<node_t> root);
};

std::string encoder::encode_n_ary_tree(std::shared_ptr<node_t> root) {
    std::string result = "[" + std::to_string(root->val);
    std::shared_ptr<n_ary_node_t> __root = 
            std::dynamic_pointer_cast<n_ary_node_t>(root);
    for (std::shared_ptr<struct node_t> child : __root->children) {
        result += encoder::encode_n_ary_tree(child);
    }
    result += "]";
    return result;
}

template <size_t N> std::string encoder::encode_fixed_tree(
                                    std::shared_ptr<node_t> root) {
    if (root == nullptr) return "(NULL)";
    std::shared_ptr<fixed_node_t<N>> __root = 
            std::dynamic_pointer_cast<fixed_node_t<N>>(root);
    std::string result = "(" + std::to_string(root->val);
    for (std::shared_ptr<struct node_t> child : __root->children)
    {
        result += encoder::encode_fixed_tree<N>(child);
    }
    result += ")";
    return result;
}

template <size_t N> 
typename std::enable_if<std::is_lteq(N <=> MAX_SIZE), std::string>::type 
encoder::encode_fixed_tree_tester(std::shared_ptr<node_t> root) {
    if (std::dynamic_pointer_cast<fixed_node_t<N>>(root) == nullptr) {
        return encoder::encode_fixed_tree_tester<N + 1>(root);
    } else {
        return std::to_string(N) + encoder::encode_fixed_tree<N>(root);
    }
}

template <size_t N> 
typename std::enable_if<std::is_gt(N <=> MAX_SIZE), std::string>::type
encoder::encode_fixed_tree_tester(std::shared_ptr<node_t> root) {
    throw std::invalid_argument("Invalid tree");
}

struct court_t {
    int plaintiff;
    int defendant;
    base_t() : plaintiff(0), defendant(0) {
        std::cout << "base_t constructor with plaintiff = " << plaintiff << 
            ", and defendant = " << defendant << std::endl;
    }
    base_t(int __plaintiff, int __defendant) : plaintiff(__plaintiff), defendant(__defendant) {
        std::cout << "base_t constructor with plaintiff = " << plaintiff << 
            ", and defendant = " << defendant << std::endl;
    }
    virtual ~base_t() {
        std::cout << "base_t destructor with plaintiff = " << plaintiff << 
            ", and defendant = " << defendant << std::endl;
    }
};

std::string encode_tree(std::shared_ptr<node_t> root) {
    if (std::dynamic_pointer_cast<n_ary_node_t>(root)) {
        return encoder::encode_n_ary_tree(root);
    } else {
        return encoder::encode_fixed_tree_tester<1>(root);
    }
}

std::shared_ptr<node_t> decode_tree(std::string encoded) {
    int index = 0;
    if (encoded[index++] == '[') {
        return decoder::decode_n_ary_tree(encoded, index);
    }
    size_t n = encoded[0] - '0';
    if (encoded[index++] != '(') {
        throw std::invalid_argument("Bad input");
    }
    return decoder::decode_fixed_tree_tester<1>(encoded, index, n);
}

template <typename T, size_t... Dims>
class ndarray {
public:
    ndarray(size_t __size, size_t __dims[sizeof...(Dims)]) : size(__size) {
        for (int i = 0; i < sizeof...(Dims); i++) {
            dims[i] = __dims[i];
        }
        strides[sizeof...(Dims) - 1] = 1;
        for (int i = sizeof...(Dims) - 2; i >= 0; i--) {
            strides[i] = strides[i + 1] * dims[i + 1];
        }
        data = std::make_unique<T[]>(size);
    }

    T& operator()(size_t indices[sizeof...(Dims)]) {
        size_t index = 0;
        for (int i = 0; i < sizeof...(Dims); i++) {
            index += indices[i] * strides[i];
        }
        return data[index];
    }

    T& operator()(std::span<size_t> indices) {
        size_t index = 0;
        for (int i = 0; i < sizeof...(Dims); i++) {
            index += indices[i] * strides[i];
        }
        return data[index];
    }

    size_t get_size(void) {
        return size;
    }

    size_t get_dim(int i) {
        return dims[i];
    }

    size_t get_stride(int i) {
        return strides[i];
    }
protected:
    std::unique_ptr<T[]> data;
    size_t size;
    size_t dims[sizeof...(Dims)];
    size_t strides[sizeof...(Dims)];
};
