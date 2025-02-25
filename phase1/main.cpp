#include "match_submissions.hpp"
#include "../tokenizer.hpp"

// You should NOT modify ANYTHING in this file.
extern std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2);

namespace testcase {
    const std::string directory = "fellowship/";
    const std::vector<std::pair<std::string, std::string>> file_pairs = {
        {"gollum.cpp", "smeagol.cpp"},
        {"gollum.cpp", "frodo.cpp"},
        {"gollum.cpp", "aragorn.cpp"},
        {"gollum.cpp", "gandalf.cpp"},
        {"smeagol.cpp", "frodo.cpp"},
        {"smeagol.cpp", "aragorn.cpp"},
        {"smeagol.cpp", "gandalf.cpp"},
        {"frodo.cpp", "aragorn.cpp"},
        {"frodo.cpp", "gandalf.cpp"},
        {"aragorn.cpp", "gandalf.cpp"}
    };
    const std::vector<std::array<int, 5>> correct = {
        {1, 890, 890, 0, 170},
        {0, 0, 30, 820, 1010},
        {0, 0, 30, 750, 70},
        {0, 0, 30, 750, 10},
        {1, 150, 80, 0, 1200},
        {0, 0, 30, 950, 70},
        {0, 0, 50, 970, 220},
        {0, 60, 70, 1010, 0},
        {0, 40, 60, 1010, 210},
        {1, 120, 60, 10, 50}
    };
    double execute_testcase(int index);
    double are_within_thirty(int len1, int len2);
    double compare_lengths(int len1, int len2);
}

double testcase::execute_testcase(int index) {
    std::cout << "Comparing " << testcase::file_pairs[index].first << " and " 
        << testcase::file_pairs[index].second << std::endl;
    tokenizer_t parser_one(testcase::directory + 
            testcase::file_pairs[index].first);
    tokenizer_t parser_two(testcase::directory + 
            testcase::file_pairs[index].second);
    std::vector<int> submission_one = parser_one.get_tokens();
    std::vector<int> submission_two = parser_two.get_tokens();
    std::array<int, 5> results = match_submissions(submission_one, 
            submission_two);
    printf("Results: %4d %4d %4d %4d %4d\n", results[0], results[1], 
            results[2], results[3], results[4]);
    printf("Expected: %3d %4d %4d %4d %4d\n", testcase::correct[index][0], 
            testcase::correct[index][1], testcase::correct[index][2], 
            testcase::correct[index][3], testcase::correct[index][4]);
    double score = (results[0] == testcase::correct[index][0]) ? 0.1 : 0.0;
    score += 0.1 * testcase::compare_lengths(results[1],
            testcase::correct[index][1]);
    score += 0.04 * testcase::compare_lengths(results[2],
            testcase::correct[index][2]);
    score += 0.03 * testcase::compare_lengths(results[3],
            testcase::correct[index][3]);
    score += 0.03 * testcase::compare_lengths(results[4],
            testcase::correct[index][4]);
    return score;
}

double testcase::are_within_thirty(int len1, int len2) {
    return std::max(len1, len2) - std::min(len1, len2) <= 30 ? 1.0 : 0.0;
}

double testcase::compare_lengths(int len1, int len2) {
    if (testcase::are_within_thirty(len1, len2)) {
        return 1.0;
    }
    double __ratio = std::max<double>(len1, len2) / std::min<double>(len1, len2);
    if (__ratio <= 1.25) {
        return 1.0;
    }
    return std::exp(-(__ratio - 1.25));
}

int main(void) {
    double total_score = 0.0;
    for (int i = 0; i < 10; i++) {
        total_score += testcase::execute_testcase(i);
    }
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Total score: " << total_score << std::endl;
    return 0;
}
