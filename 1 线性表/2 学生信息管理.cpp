#include <iostream>
#include <vector>
#include <string>
#include <sstream>

struct Student {
    std::string number;
    std::string name;
};

// 自定义函数，将整数转换为字符串（C++98不支持std::to_string）
std::string intToString(int n) {
    std::ostringstream ss;
    ss << n;
    return ss.str();
}

int main(){
    int n;
    std::cin >> n;
    std::vector<Student> students;
    students.reserve(10000); // 预留空间，避免频繁分配内存
    for (int i = 0; i < n; ++i) {
        Student s;
        std::cin >> s.number >> s.name;
        students.push_back(s);
    }

    std::vector<std::string> outputs; // 存储输出结果
    std::string line;
    std::getline(std::cin, line); // 读取上一行的换行符

    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "insert") {
            int position;
            Student new_student;
            iss >> position >> new_student.number >> new_student.name;

            if (position < 1 || position > static_cast<int>(students.size()) + 1) {
                outputs.push_back("-1");
            } else {
                students.insert(students.begin() + position - 1, new_student);
                outputs.push_back("0");
            }
        } else if (command == "remove") {
            int position;
            iss >> position;

            if (position < 1 || position > static_cast<int>(students.size())) {
                outputs.push_back("-1");
            } else {
                students.erase(students.begin() + position - 1);
                outputs.push_back("0");
            }
        } else if (command == "check") {
            std::string type;
            iss >> type;

            if (type == "name") {
                std::string name_to_find;
                iss >> name_to_find;
                bool found = false;

                for (size_t i = 0; i < students.size(); ++i) {
                    if (students[i].name == name_to_find) {
                        std::string output = intToString(static_cast<int>(i) + 1) + " " + students[i].number + " " + students[i].name;
                        outputs.push_back(output);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    outputs.push_back("-1");
                }
            } else if (type == "no") {
                std::string number_to_find;
                iss >> number_to_find;
                bool found = false;

                for (size_t i = 0; i < students.size(); ++i) {
                    if (students[i].number == number_to_find) {
                        std::string output = intToString(static_cast<int>(i) + 1) + " " + students[i].number + " " + students[i].name;
                        outputs.push_back(output);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    outputs.push_back("-1");
                }
            }
        } else if (command == "end") {
            outputs.push_back(intToString(static_cast<int>(students.size())));
            break;
        }
    }

    // 统一输出结果（避免使用C++11的范围for循环）
    for (std::vector<std::string>::size_type i = 0; i < outputs.size(); ++i) {
        std::cout << outputs[i] << std::endl;
    }

    return 0;
}