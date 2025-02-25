#ifndef FBB5B07F_E120_4218_8ECA_8976E851F0D3
#define FBB5B07F_E120_4218_8ECA_8976E851F0D3

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

std::string encode_tree(std::shared_ptr<node_t> root);
std::shared_ptr<node_t> decode_tree(std::string encoded);

#endif /* FBB5B07F_E120_4218_8ECA_8976E851F0D3 */
