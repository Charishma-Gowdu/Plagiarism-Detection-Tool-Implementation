#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
// Constructor
plagiarism_checker_t::plagiarism_checker_t() 
    : terminate_flag(false) {
    // Start worker threads
    size_t num_threads = std::thread::hardware_concurrency();
    num_threads = (num_threads == 0) ? 2 : num_threads;
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back(&plagiarism_checker_t::worker_thread, this);
    }
}

// Constructor with past submissions
plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions)
    : terminate_flag(false) {
    // Set a low timestamp for past submissions to prevent flagging
    auto low_timestamp = std::chrono::system_clock::from_time_t(0);  // Unix epoch (1970-01-01)

    for (const auto& submission : __submissions) {
        past_submissions.push_back(submission);

        // Set artificially low timestamp for past submissions
        submission_timestamps[submission] = low_timestamp;
        
        process_submission(submission);  // Process but don't flag
    }

    // Start worker threads
    size_t num_threads = std::thread::hardware_concurrency();
    num_threads = (num_threads == 0) ? 2 : num_threads;
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back(&plagiarism_checker_t::worker_thread, this);
    }
}

// Destructor
plagiarism_checker_t::~plagiarism_checker_t() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        terminate_flag = true;
    }
    cv.notify_all();
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

// Add a new submission
void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission) {
    auto timestamp = std::chrono::system_clock::now();
    {
        std::lock_guard<std::mutex> lock(mtx);
        submission_timestamps[__submission] = timestamp;  // Set current timestamp for new submissions
        submission_queue.push(__submission);
    }
    cv.notify_one();
}

// Worker thread function
void plagiarism_checker_t::worker_thread() {
    while (true) {
        std::shared_ptr<submission_t> current_submission;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return !submission_queue.empty() || terminate_flag; });

            if (terminate_flag && submission_queue.empty()) {
                break;
            }

            current_submission = submission_queue.front();
            submission_queue.pop();
        }

        process_submission(current_submission);

        
    }
}

void plagiarism_checker_t::process_submission(std::shared_ptr<submission_t> new_submission) {
    // Tokenize and extract patterns
    tokenizer_t tokenizer(new_submission->codefile);
    auto tokens = tokenizer.get_tokens();
    std::set<std::string> patterns;

    // Create patterns of length 75 or more
    size_t pattern_length = 75;
    for (size_t i = 0; i + pattern_length <= tokens.size(); ++i) {
        std::string pattern;
        for (size_t j = i; j < i + pattern_length; ++j) {
            pattern += std::to_string(tokens[j]);
        }
        patterns.insert(pattern);
    }

    std::unordered_map<std::shared_ptr<submission_t>, int> match_counts;
    bool found_long_pattern = false;
    bool found_match_count = false;

    // Lock around pattern database access
    {
        std::lock_guard<std::mutex> lock(mtx);
        // Count matches of patterns and check for conditions
        for (const auto& pattern : patterns) {
            if (pattern_database.find(pattern) != pattern_database.end()) {
                // Match found, increment match counts
                for (auto& submission : pattern_database[pattern]) {
                    match_counts[submission]++;  // Increment match count for the submission
                }

                // If this pattern's length is >= 75, we mark it as found
                found_long_pattern = true;
            }
        }

        // Check if there are 10 or more matches for any submission
        for (const auto& [submission, count] : match_counts) {
            if (count >= 10) {
                found_match_count = true;
            }
        }

        // Insert this new submission into the pattern database for future matching
        for (const auto& pattern : patterns) {
            pattern_database[pattern].insert(new_submission);
        }
    }

    // If either of the conditions is met, check the time difference and flag
    if (found_long_pattern || found_match_count) {
        // Flag the submissions based on the time difference
        for (const auto& [submission, count] : match_counts) {
            // Get the time difference between submissions
            auto time_diff = std::chrono::duration_cast<std::chrono::seconds>(submission_timestamps[new_submission] - submission_timestamps[submission]).count();
            flag_entities(new_submission, submission);
        }
    }
}

void plagiarism_checker_t::flag_entities(std::shared_ptr<submission_t> later_submission, 
                                         std::shared_ptr<submission_t> earlier_submission) {

    // Check if the current submission is an original submission
    if (later_submission && later_submission->student) {
        // Get the student's name
        std::string student_name = later_submission->student->get_name();

        // Search for the student's name in past submissions
        bool is_original_submission = false;
        for (const auto& submission : past_submissions) {
            if (submission->student && submission->student->get_name() == student_name) {
                is_original_submission = true;
                break;  // Student is found, mark as original submission
            }
        }

        // If this submission is found in past submissions, skip flagging
        if (is_original_submission) {
            return; // Skip flagging original submissions
        }

        // Check if the student has already been flagged
        if (flagged_students.find(student_name) != flagged_students.end()) {
            return; // Student has already been flagged, do not reflag
        }
    }

    // New logic: If the gap between the two submissions is less than 1 second, flag both
    if (later_submission && earlier_submission) {
        auto time_diff = std::chrono::duration_cast<std::chrono::seconds>(submission_timestamps[later_submission] - submission_timestamps[earlier_submission]).count();
        
        if (time_diff < 1) {
            // Flag both submissions if the time difference is less than 1 second
            
            flagged_students.insert(earlier_submission->student->get_name());
            flagged_students.insert(later_submission->student->get_name());
            

            if (earlier_submission->student) {
                earlier_submission->student->flag_student(earlier_submission);
            }

            if (earlier_submission->professor) {
                earlier_submission->professor->flag_professor(earlier_submission);
            }

            if (later_submission->student) {
                later_submission->student->flag_student(later_submission);
            }

            if (later_submission->professor) {
                later_submission->professor->flag_professor(later_submission);
            }


        }
        else{
            if (later_submission->student) {
                later_submission->student->flag_student(later_submission);
            }
            if (later_submission->professor) {
                later_submission->professor->flag_professor(later_submission);
            }
            flagged_students.insert(later_submission->student->get_name());
        }
    }

    // Note: The earlier submission is not flagged as plagiarized in this scenario
}
// End TODO