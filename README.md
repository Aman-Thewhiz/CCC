# Student Record Management System (C++)

A simple command-line student record management system built in C++ with an interactive CLI.

## Screenshots

| Login Screen | Student List |
|:------------:|:------------:|
| ![Login](screenshots/login.png) | ![Students](screenshots/students.png) |

## Features

- Admin authentication with username/password
- Add, view, update, and delete student records
- Search students by name, registration number, or major
- SQLite database for persistent storage
- Interactive menu system

## Requirements

- C++17 compiler (g++ or clang++)
- No external dependencies required!

## Build

### Linux/macOS:
```bash
cd cpp
g++ -std=c++17 -o student_manager main.cpp database.cpp
```

### Windows (MinGW/MSYS2/TDM-GCC):
```bash
cd cpp
g++ -std=c++17 -o student_manager.exe main.cpp database.cpp
```

## Usage

### Linux/macOS:
```bash
./student_manager
```

### Windows:
```bash
student_manager.exe
```

### Default Admin Credentials

- Username: `admin`
- Password: `admin123`

## Project Structure

```
cpp/
  main.cpp      - Application entry point and CLI
  database.h    - Database class header
  database.cpp  - SQLite database operations
  student.h     - Student struct definition
```

## Database

The application automatically creates a `students.db` plain text database file on first run. Student records include:

- ID (auto-generated)
- Name
- Registration Number
- Age
- Major

## License

MIT
