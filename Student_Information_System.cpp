#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
using namespace std;

// Structure to store student information
struct Student {
    int rollNo;
    char name[50];
    float marks;
};

// Function declarations
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void sortStudents();

const char* FILE_NAME = "students.dat";

int main() {
    int choice;

    do {
        cout << "\n=== Student Record Management System ===\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student by Roll No\n";
        cout << "4. Update Student Record\n";
        cout << "5. Delete Student Record\n";
        cout << "6. Sort Students by Roll No\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: sortStudents(); break;
            case 0: cout << "Exiting program.\n"; break;
            default: cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}

// Function to add a new student
void addStudent() {
    Student s;
    ofstream fout(FILE_NAME, ios::binary | ios::app);

    cout << "Enter Roll No: ";
    cin >> s.rollNo;
    cin.ignore(); // to flush newline
    cout << "Enter Name: ";
    cin.getline(s.name, 50);
    cout << "Enter Marks: ";
    cin >> s.marks;

    fout.write((char*)&s, sizeof(s));
    fout.close();

    cout << "Student record added successfully.\n";
}

// Function to display all students
void displayStudents() {
    Student s;
    ifstream fin(FILE_NAME, ios::binary);

    cout << "\n=== All Student Records ===\n";
    cout << left << setw(10) << "Roll No" << setw(30) << "Name" << "Marks\n";
    cout << "-----------------------------------------------\n";

    while (fin.read((char*)&s, sizeof(s))) {
        cout << left << setw(10) << s.rollNo << setw(30) << s.name << s.marks << "\n";
    }

    fin.close();
}

// Function to search a student by roll number
void searchStudent() {
    Student s;
    int roll;
    bool found = false;

    cout << "Enter Roll No to search: ";
    cin >> roll;

    ifstream fin(FILE_NAME, ios::binary);
    while (fin.read((char*)&s, sizeof(s))) {
        if (s.rollNo == roll) {
            cout << "Student Found:\n";
            cout << "Roll No: " << s.rollNo << "\n";
            cout << "Name: " << s.name << "\n";
            cout << "Marks: " << s.marks << "\n";
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Student with Roll No " << roll << " not found.\n";

    fin.close();
}

// Function to update a student's record
void updateStudent() {
    Student s;
    int roll;
    bool found = false;

    cout << "Enter Roll No to update: ";
    cin >> roll;

    fstream file(FILE_NAME, ios::binary | ios::in | ios::out);
    while (file.read((char*)&s, sizeof(s))) {
        if (s.rollNo == roll) {
            cout << "Existing Record:\n";
            cout << "Name: " << s.name << ", Marks: " << s.marks << "\n";

            cout << "Enter New Name: ";
            cin.ignore();
            cin.getline(s.name, 50);
            cout << "Enter New Marks: ";
            cin >> s.marks;

            file.seekp(-sizeof(s), ios::cur);
            file.write((char*)&s, sizeof(s));
            cout << "Record updated successfully.\n";
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Student not found.\n";

    file.close();
}

// Function to delete a student record
void deleteStudent() {
    Student s;
    int roll;
    bool found = false;

    cout << "Enter Roll No to delete: ";
    cin >> roll;

    ifstream fin(FILE_NAME, ios::binary);
    ofstream fout("temp.dat", ios::binary);

    while (fin.read((char*)&s, sizeof(s))) {
        if (s.rollNo == roll) {
            found = true;
            continue; // skip writing this record
        }
        fout.write((char*)&s, sizeof(s));
    }

    fin.close();
    fout.close();

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found)
        cout << "Record deleted successfully.\n";
    else
        cout << "Student not found.\n";
}

// Function to sort student records by roll number
void sortStudents() {
    Student s[100];
    int n = 0;

    ifstream fin(FILE_NAME, ios::binary);
    while (fin.read((char*)&s[n], sizeof(Student))) {
        n++;
    }
    fin.close();

    // Bubble sort
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (s[j].rollNo > s[j+1].rollNo) {
                Student temp = s[j];
                s[j] = s[j+1];
                s[j+1] = temp;
            }
        }
    }

    ofstream fout(FILE_NAME, ios::binary);
    for (int i = 0; i < n; i++) {
        fout.write((char*)&s[i], sizeof(Student));
    }
    fout.close();

    cout << "Records sorted by Roll No.\n";
}
