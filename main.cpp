#include "vector.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // Read and execute commands from stdin
    string command;
    sjtu::vector<int> vec;
    
    while (cin >> command) {
        if (command == "push_back") {
            int value;
            cin >> value;
            vec.push_back(value);
        } else if (command == "pop_back") {
            if (!vec.empty()) {
                vec.pop_back();
            }
        } else if (command == "size") {
            cout << vec.size() << "\n";
        } else if (command == "capacity") {
            cout << vec.capacity() << "\n";
        } else if (command == "empty") {
            cout << (vec.empty() ? "true" : "false") << "\n";
        } else if (command == "clear") {
            vec.clear();
        } else if (command == "at") {
            size_t index;
            cin >> index;
            try {
                cout << vec.at(index) << "\n";
            } catch (const std::out_of_range&) {
                cout << "out_of_range\n";
            }
        } else if (command == "get") {
            size_t index;
            cin >> index;
            if (index < vec.size()) {
                cout << vec[index] << "\n";
            }
        } else if (command == "front") {
            if (!vec.empty()) {
                cout << vec.front() << "\n";
            }
        } else if (command == "back") {
            if (!vec.empty()) {
                cout << vec.back() << "\n";
            }
        } else if (command == "reserve") {
            size_t cap;
            cin >> cap;
            vec.reserve(cap);
        } else if (command == "resize") {
            size_t n;
            cin >> n;
            if (cin.peek() == ' ') {
                int value;
                cin >> value;
                vec.resize(n, value);
            } else {
                vec.resize(n);
            }
        } else if (command == "print") {
            for (size_t i = 0; i < vec.size(); ++i) {
                if (i > 0) cout << " ";
                cout << vec[i];
            }
            cout << "\n";
        } else if (command == "insert") {
            size_t pos;
            int value;
            cin >> pos >> value;
            if (pos <= vec.size()) {
                vec.insert(vec.begin() + pos, value);
            }
        } else if (command == "erase") {
            size_t pos;
            cin >> pos;
            if (pos < vec.size()) {
                vec.erase(vec.begin() + pos);
            }
        }
    }
    
    return 0;
}
