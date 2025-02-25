#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <cmath>
// -----------------------------------------------------------------------------
#include <unordered_set>
#include<unordered_map>
#include<utility>
// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here

std::vector<int> hash_function(const std::vector<int>& s1, int k, int base = 31, int mod = 2147483647) {
    int n = s1.size();
    std::vector<int> hash_vector((n + k - 1) / k);
    int index = 0;
    for (int i = 0; i < n; i += k) {
        int hash_ = 0;
        for (int j = i; j < i + k && j < n; j++) {
            hash_ = (1LL * hash_ * base + s1[j]) % mod;
        }
        hash_vector[index++] = hash_;
    }
    
    return hash_vector;
}
std::vector<std::pair<int, int>> winnowing(const std::vector<int>& hash_vector, int window) {
    int n = hash_vector.size();
    std::vector<std::pair<int, int>> fingerprints;


    if (n < window) return fingerprints;


    int min_index = 0;
    for (int i = 1; i < window; i++) {
        if (hash_vector[i] < hash_vector[min_index]) {
            min_index = i;
        }
    }
    fingerprints.push_back({hash_vector[min_index], min_index});

    
    for (int i = 1; i <= n - window; i++) {
        int new_element_index = i + window - 1;

        if (min_index < i) {
            min_index = i;
            for (int j = i; j <= new_element_index; j++) {
                if (hash_vector[j] < hash_vector[min_index]) {
                    min_index = j;
                }
            }
            fingerprints.push_back({hash_vector[min_index], min_index});
        } else {
            
            if (hash_vector[new_element_index] <= hash_vector[min_index]) {
                min_index = new_element_index;
                fingerprints.push_back({hash_vector[min_index], min_index});
            }
        }
        
    }

    return fingerprints;
}

// int match_length(const std::vector<int>& s1, const std::vector<int>& s2,int k, std::pair<int,int> f1,std::pair<int,int> f2){

//     int length = 0;

//     int idx1 = f1.second*k;
//     int idx2 = f2.second*k;
//     int i1 = idx1;
//     int i2 = idx2;
//     int n1 = s1.size();
//     int n2 = s2.size();

//     while(i1 < n1 && i2 < n2 && s1[i1] == s2[i2]){
//             i1++;
//             i2++;
//             length++;
//     }

//     i1 = idx1 - 1;
//     i2 = idx2 - 1;

//      while(i1 >= 0 && i2 >= 0 && s1[i1] == s2[i2]){
//             i1--;
//             i2--;
//             length++;
//     }

//     return length;

// }


std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> getMatchingPairsHashing(
        const std::vector<std::pair<int, int>>& vec1, 
        const std::vector<std::pair<int, int>>& vec2) {

    // Create a hash map to store elements from vec1, using first as the key
    std::unordered_map<int, std::pair<int, int>> hashMap;
    for (const auto& p : vec1) {
        hashMap[p.first] = p;
    }

    // Find matching pairs by checking if first elements of vec2 exist in the hash map
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> matchingPairs;
    for (const auto& p : vec2) {
        auto it = hashMap.find(p.first);
        if (it != hashMap.end()) {
            matchingPairs.push_back({it->second, p});
        }
    }

    return matchingPairs;
}


int match_length_with_tracking(const std::vector<int>& s1, const std::vector<int>& s2, int k, 
                               std::pair<int, int> f1, std::pair<int, int> f2, 
                               std::vector<bool>& matched1, std::vector<bool>& matched2) {
    int length = 0;

    // Calculate starting indices based on hash positions
    int idx1 = f1.second * k;
    int idx2 = f2.second * k;
    int n1 = s1.size();
    int n2 = s2.size();

    // Match forward and mark positions as matched
    int i1 = idx1, i2 = idx2;
    while (i1 < n1 && i2 < n2 && s1[i1] == s2[i2]) {
        if (!matched1[i1] && !matched2[i2]) { // Only count if not previously matched
            matched1[i1] = true;
            matched2[i2] = true;
            length++;
        }
        i1++;
        i2++;
    }

    // Match backward and mark positions as matched
    i1 = idx1 - 1;
    i2 = idx2 - 1;
    while (i1 >= 0 && i2 >= 0 && s1[i1] == s2[i2]) {
        if (!matched1[i1] && !matched2[i2]) { // Only count if not previously matched
            matched1[i1] = true;
            matched2[i2] = true;
            length++;
        }
        i1--;
        i2--;
    }

    return length;
}


int total_length(std::vector<int> &s1, std::vector<int> &s2, std::vector<std::pair<int,int>> &f1, std::vector<std::pair<int,int>> &f2, int k) {
    int total_length = 0; 

    auto pairs = getMatchingPairsHashing(f1, f2);
    std::vector<bool> counted_s1(s1.size(), false);
    std::vector<bool> counted_s2(s2.size(), false);

    for (auto &p : pairs) {
        int match_len = match_length_with_tracking(s1, s2, k, p.first, p.second,counted_s1,counted_s2);
        
        total_length += match_len;
    }

    return total_length;
}


int final_result1(std::vector<int> &s1, 
        std::vector<int> &s2){

        int n1 = s1.size();
        int n2 = s2.size();
        
        int k = 3;
        int t = 10;

        std::vector<int> hash1 = hash_function(s1,k);
        std::vector<int> hash2 = hash_function(s2,k);

        int window = t - k + 1;
        std::vector<std::pair<int,int>> f1 = winnowing(hash1,window);
        std::vector<std::pair<int,int>> f2 = winnowing(hash2,window);

        return total_length(s1,s2,f1,f2,k);

}

// Function to find the longest fuzzy match and its starting indices
std::tuple<int, int, int> findLongestVectorMatch(
    const std::vector<int>& doc1,
    const std::vector<int>& doc2,
    double threshold = 0.8)
{
    size_t n = doc1.size();
    size_t m = doc2.size();
    int longestMatchLength = 0;
    int startIndexDoc1 = -1;
    int startIndexDoc2 = -1;

    // Compare each token in doc1 with each token in doc2
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            // Calculate similarity between single tokens
            double similarity = (i < n && j < m) ? (doc1[i] == doc2[j] ? 1.0 : 0.0) : 0.0;
            if (similarity >= threshold) {
                int length = 1; // Start counting matches
                int k = 1;

                // Check for consecutive matches
                while (i + k < n && j + k < m &&
                       (doc1[i + k] == doc2[j + k])) {
                    length++;
                    k++;
                }

                // Update longest match if the current is longer
                if (length > longestMatchLength) {
                    longestMatchLength = length;
                    startIndexDoc1 = i;
                    startIndexDoc2 = j;
                }
            }
        }
    }

    return std::make_tuple(startIndexDoc1, startIndexDoc2, longestMatchLength);
}


std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
        std::array<int, 5> result = {0, 0, 0, 0, 0};
        int n1 = submission1.size();
        int n2 = submission2.size();

        result[1] = final_result1(submission1,submission2);
        double threshold = 0.80;

        auto result_ = findLongestVectorMatch(submission1, submission2, threshold);
        result[2] = std::get<2>(result_);
        result[3] = std::get<0>(result_);
        result[4] = std::get<1>(result_);
       double combinedScore = (result[1] * 100.0) / (n1 + n2 - result[1]);
    combinedScore += (result[2] * 100.0) / (n1 + n2); // Normalizing longest match

        result[0] = combinedScore > 30.0 ? 1 : 0;
    return result; 
  
}