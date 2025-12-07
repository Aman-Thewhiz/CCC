#include "database.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

Database::Database(const std::string& path) : db_path(path), next_id(1) {}

Database::~Database() {
    save_to_file();
}

void Database::load_from_file() {
    std::ifstream file(db_path);
    if (!file.is_open()) {
        return; // File doesn't exist yet, will be created on save
    }

    std::string line, section;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        if (line == "[ADMINS]") {
            section = "admins";
        } else if (line == "[STUDENTS]") {
            section = "students";
        } else if (section == "admins") {
            size_t pos = line.find(':');
            if (pos != std::string::npos) {
                std::string username = line.substr(0, pos);
                std::string password = line.substr(pos + 1);
                admins[username] = password;
            }
        } else if (section == "students") {
            std::istringstream iss(line);
            Student s;
            char delim;
            if (iss >> s.id >> delim && std::getline(iss, s.name, '|') &&
                std::getline(iss, s.reg_no, '|') && iss >> s.age >> delim &&
                std::getline(iss, s.major)) {
                students.push_back(s);
                if (s.id >= next_id) {
                    next_id = s.id + 1;
                }
            }
        }
    }
    file.close();
}

void Database::save_to_file() {
    std::ofstream file(db_path);
    if (!file.is_open()) {
        std::cerr << "Cannot save database to file" << std::endl;
        return;
    }

    file << "# Student Management System Database\n\n";
    
    file << "[ADMINS]\n";
    for (const auto& pair : admins) {
        file << pair.first << ":" << pair.second << "\n";
    }

    file << "\n[STUDENTS]\n";
    for (const auto& s : students) {
        file << s.id << "|" << s.name << "|" << s.reg_no << "|" 
             << s.age << "|" << s.major << "\n";
    }

    file.close();
}

bool Database::init() {
    load_from_file();
    
    // Add default admin if none exists
    if (admins.empty()) {
        admins["admin"] = "admin123";
        save_to_file();
        std::cout << "Default admin created: username='admin', password='admin123'" << std::endl;
    }

    return true;
}

bool Database::verify_admin(const std::string& username, const std::string& password) {
    auto it = admins.find(username);
    return it != admins.end() && it->second == password;
}

int Database::add_student(const std::string& name, const std::string& reg_no, int age, const std::string& major) {
    // Check if reg_no already exists
    for (const auto& s : students) {
        if (s.reg_no == reg_no) {
            return -1;
        }
    }

    Student s;
    s.id = next_id++;
    s.name = name;
    s.reg_no = reg_no;
    s.age = age;
    s.major = major;
    
    students.push_back(s);
    save_to_file();
    
    return s.id;
}

std::vector<Student> Database::get_all_students() {
    return students;
}

Student* Database::get_student(int id) {
    for (auto& s : students) {
        if (s.id == id) {
            return new Student(s);
        }
    }
    return nullptr;
}

bool Database::update_student(int id, const std::string& name, const std::string& reg_no, int age, const std::string& major) {
    for (auto& s : students) {
        if (s.id == id) {
            // Check if new reg_no conflicts with another student
            for (const auto& other : students) {
                if (other.id != id && other.reg_no == reg_no) {
                    return false;
                }
            }
            
            s.name = name;
            s.reg_no = reg_no;
            s.age = age;
            s.major = major;
            save_to_file();
            return true;
        }
    }
    return false;
}

bool Database::delete_student(int id) {
    auto it = std::remove_if(students.begin(), students.end(),
                             [id](const Student& s) { return s.id == id; });
    
    if (it != students.end()) {
        students.erase(it, students.end());
        save_to_file();
        return true;
    }
    return false;
}

std::vector<Student> Database::search_students(const std::string& query) {
    std::vector<Student> results;
    std::string lower_query = query;
    std::transform(lower_query.begin(), lower_query.end(), lower_query.begin(), ::tolower);
    
    for (const auto& s : students) {
        std::string name_lower = s.name;
        std::string reg_no_lower = s.reg_no;
        std::string major_lower = s.major;
        
        std::transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);
        std::transform(reg_no_lower.begin(), reg_no_lower.end(), reg_no_lower.begin(), ::tolower);
        std::transform(major_lower.begin(), major_lower.end(), major_lower.begin(), ::tolower);
        
        if (name_lower.find(lower_query) != std::string::npos ||
            reg_no_lower.find(lower_query) != std::string::npos ||
            major_lower.find(lower_query) != std::string::npos) {
            results.push_back(s);
        }
    }
    
    return results;
}
