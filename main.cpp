#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int THREAD_COUNT_1 = 3;
const int THREAD_COUNT_2 = 4;

void read_set(const string& path, vector<unsigned long>& set) {
    string line;
    ifstream in(path);
    if (in.is_open())
    {
        while (getline(in, line)) {
            set.push_back(stoul(line));
        }
    }
    in.close();
}

bool contains(const vector<unsigned long>& set, unsigned long value) {
    return find(set.begin(), set.end(), value) != set.end();
}

vector<unsigned long> intersection_(const vector<unsigned long>& set_1, const vector<unsigned long>& set_2) {
    vector<unsigned long> res;
    for (unsigned long i : set_1) {
        if (contains(set_2, i)) {
            res.push_back(i);
        }
    }
    return res;
}

vector<unsigned long> union_(const vector<unsigned long>& set_1, const vector<unsigned long>& set_2) {
    vector<unsigned long> res;
    for (unsigned long i : set_1)
        res.push_back(i);

    vector<unsigned long> intersection = intersection_(set_1, set_2);
    for (unsigned long i : set_2) {
        if (!contains(intersection, i)) {
            res.push_back(i);
        }
    }

    return res;
}

vector<unsigned long> difference_(const vector<unsigned long>& set_1, const vector<unsigned long>& set_2) {
    vector<unsigned long> res;
    for (unsigned long i : set_1) {
        if (!contains(set_2, i))
            res.push_back(i);
    }
    return res;
}

void is_intersection(const vector<unsigned long>& set_1, const vector<unsigned long>& set_2, const vector<unsigned long>& set_3, bool& result) {
    vector<unsigned long> intersection = intersection_(set_1, set_2);

    if (intersection.size() != set_3.size()) {
        result = false;
        return;
    }

    for (unsigned long i : intersection) {
        if (!contains(set_3, i)) {
            result = false;
            return;
        }
    }
}

void is_union_(const vector<unsigned long>& set_1, const vector<unsigned long>& set_2, const vector<unsigned long>& set_3, bool& result) {
    vector<unsigned long> un = union_(set_1, set_2);

    if (un.size() != set_3.size()) {
        result = false;
        return;
    }

    for (unsigned long i : un) {
        if (!contains(set_3, i)) {
            result = false;
            return;
        }
    }
}

void is_difference(const vector<unsigned long>& set_1, const vector<unsigned long>& set_2, const vector<unsigned long>& set_3, bool& result) {
    vector<unsigned long> difference = difference_(set_1, set_2);

    if (difference.size() != set_3.size()) {
        result = false;
        return;
    }

    for (unsigned long i : difference) {
        if (!contains(set_3, i)) {
            result = false;
            return;
        }
    }
}

int main(int argc, char *argv[]) {
    vector<unsigned long> set_A, set_B, set_C;
    vector<thread> read_threads, solve_threads;
    bool results[] = {true, true, true, true};

    read_threads.emplace_back(read_set, argv[1], ref(set_A));
    read_threads.emplace_back(read_set, argv[2], ref(set_B));
    read_threads.emplace_back(read_set, argv[3], ref(set_C));

    for (size_t i = 0; i < THREAD_COUNT_1; ++i)
        read_threads[i].join();

    solve_threads.emplace_back(is_intersection, set_A, set_B, set_C, ref(results[0]));
    solve_threads.emplace_back(is_union_, set_A, set_B, set_C, ref(results[1]));
    solve_threads.emplace_back(is_difference, set_A, set_B, set_C, ref(results[2]));
    solve_threads.emplace_back(is_difference, set_B, set_A, set_C, ref(results[3]));

    for (size_t i = 0; i < THREAD_COUNT_2; ++i)
        solve_threads[i].join();

    ofstream out;
    out.open(argv[4]);
    if (out.is_open())
    {
        if (results[0])
            out << "+ C = A ∩ B" << endl;
        else
            out << "- C = A ∩ B" << endl;
        if (results[1])
            out << "+ C = A U B" << endl;
        else
            out << "- C = A U B" << endl;
        if (results[2])
            out << "+ C = A \\ B" << endl;
        else
            out << "- C = A \\ B" << endl;
        if (results[3])
            out << "+ C = B \\ A" << endl;
        else
            out << "- C = B \\ A" << endl;
    }
    out.close();

    return 0;
}
