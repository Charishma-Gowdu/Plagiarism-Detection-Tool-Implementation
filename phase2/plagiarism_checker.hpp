#include "structures.hpp"
// -----------------------------------------------------------------------------
#include <unordered_map>
#include <queue>
#include <condition_variable>
#include <thread>
#include <set>
#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <chrono>
#include <unordered_set>
#include <algorithm>
#include <shared_mutex>
// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and classes here

class plagiarism_checker_t {
    // You should NOT modify the public interface of this class.
public:
    plagiarism_checker_t(void);
    plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> 
                            __submissions);
    ~plagiarism_checker_t(void);
    void add_submission(std::shared_ptr<submission_t> __submission);

protected:
    // TODO: Add members and function signatures here
       std::vector<std::shared_ptr<submission_t>> past_submissions;
    std::queue<std::shared_ptr<submission_t>> submission_queue;
    std::mutex mtx;
    std::condition_variable cv;
    bool terminate_flag;
    std::vector<std::thread> workers;
    std::set<std::string> flagged_students;

    // Helper Functions
    void worker_thread();
    void process_submission(std::shared_ptr<submission_t> new_submission);
    bool check_plagiarism(std::shared_ptr<submission_t> new_submission, std::shared_ptr<submission_t> old_submission);
   
    std::vector<std::string> tokenize(const std::string& content);

    // Utility
    void update_pattern_database(const std::vector<std::string>& patterns, std::shared_ptr<submission_t> submission);

    // Thread Management
    void start_worker_threads(size_t num_threads);
    void stop_worker_threads();
    void clear();

    // Timestamps of submissions
    std::unordered_map<std::string, std::unordered_set<std::shared_ptr<submission_t>>> pattern_database;
    std::unordered_map<std::string, std::unordered_set<std::shared_ptr<submission_t>>> pattern_database10;
    std::unordered_map<std::shared_ptr<submission_t>, std::chrono::time_point<std::chrono::high_resolution_clock>> submission_timestamps;
    std::shared_mutex db_mutex; // Mutex for database operations

    // Private/Protected Functions
    void flag_entities(std::shared_ptr<submission_t> later_submission, std::shared_ptr<submission_t> earlier_submission);
    // End TODO
};
