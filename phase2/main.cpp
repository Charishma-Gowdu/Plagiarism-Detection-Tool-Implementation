#include "plagiarism_checker.hpp"

#include <map>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// You should NOT modify ANYTHING in this file.

void student_t::flag_student(std::shared_ptr<submission_t> __submission) {
    int i = 0;
    while (__submission->codefile[i] != '/') i++;
    i++;
}

void professor_t::flag_professor(std::shared_ptr<submission_t> __submission) {
    int i = 0;
    while (__submission->codefile[i] != '/') i++;
    i++;
    std::cout << __submission->id << std::endl;
}

namespace testcase {
    std::string test_dir;
    std::map<std::string, std::shared_ptr<student_t>> students; 
    std::map<std::string, std::shared_ptr<professor_t>> professors; 
    std::vector<std::shared_ptr<submission_t>> pre_existing_codes;
    void get_students(void);
    void get_professors(void);
    void get_pre_existing_codes(void);
    void execute_testcase();
    double precision_times_recall(std::vector<int> flagged_students, 
                                  std::vector<int> expected_students);
    void print_and_clear(std::vector<int>& flagged_students, 
                         std::vector<int>& expected_students);
    double testcase_wrapper(std::string test_dir, double weightage);
}

void testcase::get_students(void) {
    std::string filename = testcase::test_dir + "/students.txt";
    std::ifstream in(filename);
    std::string student_name;
    while (in >> student_name) {
        testcase::students[student_name] = 
            std::make_shared<student_t>(student_name);
    }
    in.close();
}

void testcase::get_professors(void) {
    std::string filename = testcase::test_dir + "/professors.txt";
    std::ifstream in(filename);
    std::string professor_name;
    while (in >> professor_name) {
        testcase::professors[professor_name] = 
            std::make_shared<professor_t>(professor_name);
    }
    in.close();
}

void testcase::get_pre_existing_codes(void) {
    std::ifstream in(testcase::test_dir + "/originals.txt");
    long id;
    std::string student_name;
    std::string professor_name;
    std::string token;
    std::string code_file;
    while (in >> id >> student_name >> professor_name >> code_file) {
        std::shared_ptr<submission_t> submission = 
            std::make_shared<submission_t>();
        submission->id = id;
        submission->student = testcase::students[student_name];
        submission->professor = testcase::professors[professor_name];
        submission->codefile = testcase::test_dir + "/" + code_file + ".cpp";
        testcase::pre_existing_codes.push_back(submission);
    }
    in.close();
}


void testcase::execute_testcase(void) {
    testcase::get_students();
    testcase::get_professors();
    testcase::get_pre_existing_codes();
    plagiarism_checker_t checker(testcase::pre_existing_codes);
    std::ifstream in(testcase::test_dir + "/submissions.txt");
    double timestamp;
    double old_timestamp = 0.0;
    long id;
    std::string student_name;
    std::string prof_name;
    std::string code_file;
    while (in >> timestamp >> id >> student_name >> prof_name >> code_file) {
        usleep((timestamp - old_timestamp) * 1000000);
        std::shared_ptr<submission_t> submission = 
            std::make_shared<submission_t>();
        submission->id = id;
        submission->student = testcase::students[student_name];
        submission->professor = testcase::professors[prof_name];
        submission->codefile = testcase::test_dir + "/" + code_file + ".cpp";
        checker.add_submission(submission);
        old_timestamp = timestamp;
    }
    in.close();
}

double testcase::precision_times_recall(std::vector<int> flagged_students, 
                                        std::vector<int> expected_students) {
    std::sort(flagged_students.begin(), flagged_students.end());
    std::sort(expected_students.begin(), expected_students.end());
    int true_positives = 0;
    int i = 0, j = 0;
    while (i < flagged_students.size() && j < expected_students.size()) {
        if (flagged_students[i] == expected_students[j]) {
            true_positives++;
            i++;
            j++;
        } else if (flagged_students[i] < expected_students[j]) {
            i++;
        } else {
            j++;
        }
    }
    return static_cast<double>(true_positives * true_positives) / 
           (flagged_students.size() * expected_students.size());
}

void testcase::print_and_clear(std::vector<int>& flagged_students, 
                               std::vector<int>& expected_students) {
    std::cout << "Flagged  submission IDs: ";
    for (int i = 0; i < flagged_students.size(); i++) {
        std::cout << flagged_students[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Expected submission IDs: ";
    for (int i = 0; i < expected_students.size(); i++) {
        std::cout << expected_students[i] << " ";
    }
    std::cout << std::endl;
    int pid;
    if ((pid = fork()) == 0) {
        execlp("rm", "rm", "output.txt", NULL);
        exit(0);
    }
    waitpid(pid, NULL, 0);
}

double testcase::testcase_wrapper(std::string __test_dir, double weightage) {
    std::cout << "Running test case: " << __test_dir << std::endl;
    testcase::test_dir = __test_dir;
    int pid;
    if ((pid = fork()) == 0) {
        FILE* output = fopen("output.txt", "w");
        dup2(fileno(output), 1);
        testcase::execute_testcase();
        fclose(output);
        exit(0);
    }
    waitpid(pid, NULL, 0);
    std::ifstream in("output.txt");
    std::vector<int> flagged_students;
    int id;
    while (in >> id) {
        flagged_students.push_back(id);
    }
    in.close();
    std::ifstream expected(__test_dir + "/expected.txt");
    std::vector<int> expected_students;
    while (expected >> id) {
        expected_students.push_back(id);
    }
    expected.close();
    testcase::print_and_clear(flagged_students, expected_students);
    return weightage * testcase::precision_times_recall(flagged_students, 
                                                       expected_students);
}

int main(void) {
    double score = testcase::testcase_wrapper("hogwarts", 1.5);
    score += testcase::testcase_wrapper("ainur", 1.5);
    score += testcase::testcase_wrapper("jedisith", 1.5);
    score += testcase::testcase_wrapper("long_testcase", 1.5);
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Total score: " << score << std::endl;
    return 0;
}
