#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
using namespace std;

class Student {
private:
    int id;
    string name;
    int totalClasses;
    int attendedClasses;

public:
    Student(int i = 0, string n = "", int t = 0, int a = 0) {
        id = i;
        name = n;
        totalClasses = t;
        attendedClasses = a;
    }

    int getId() const { return id; }

    void markAttendance(bool present) {
        totalClasses++;
        if (present) attendedClasses++;
    }

    float getPercentage() const {
        if (totalClasses == 0) return 0;
        return (attendedClasses * 100.0f) / totalClasses;
    }

    void display() const {
        cout << "\nID: " << id
             << "\nName: " << name
             << "\nTotal Classes: " << totalClasses
             << "\nAttended: " << attendedClasses
             << "\nPercentage: " << getPercentage() << "%\n";
    }

    void predictLeave() const {
        cout << "\n--- Prediction ---\n";

        if (totalClasses == 0) {
            cout << "No data available.\n";
            return;
        }

        float percent = getPercentage();

        if (percent >= 75.0) {
            int safeLeaves = (int)((attendedClasses / 0.75) - totalClasses);
            if (safeLeaves < 0) safeLeaves = 0;

            cout << "You can bunk approx " << safeLeaves << " classes safely.\n";
        } else {
            int needed = (int)((0.75 * totalClasses) - attendedClasses);
            if (needed < 0) needed = 0;

            cout << "Attend next " << needed << " classes to reach 75%.\n";
        }
    }

    void save(ofstream &out) const {
        out << id << " " << name << " "
            << totalClasses << " " << attendedClasses << "\n";
    }
bool load(ifstream &in) {
    if (in >> id >> name >> totalClasses >> attendedClasses)
        return true;
    else
        return false;
}
};
int getInt() {
    int x;
    while (true) {
        cin >> x;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Enter number: ";
        } else {
            cin.ignore(1000, '\n');
            return x;
        }
    }
}
bool idExists(const vector<Student>& list, int id) {
    for (const auto &s : list)
        if (s.getId() == id) return true;
    return false;
}

int findIndex(const vector<Student>& list, int id) {
    for (int i = 0; i < list.size(); i++)
        if (list[i].getId() == id) return i;
    return -1;
}
vector<Student> loadStudents() {
    vector<Student> list;
    ifstream in("data.txt");

    if (!in) return list;

    while (true) {
        Student s;
        if (!s.load(in)) break;
        list.push_back(s);
    }

    return list;
}

void saveStudents(const vector<Student>& list) {
    ofstream out("data.txt");
    for (const auto &s : list)
        s.save(out);
}

// ---------- MAIN ----------
int main() {
    vector<Student> students = loadStudents();
    int choice;

    while (true) {
        cout << "\n==== MENU ====\n";
        cout << "1. Add Student\n";
        cout << "2. Mark Attendance\n";
        cout << "3. View Students\n";
        cout << "4. Predict Leave\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";

        choice = getInt();

        if (choice == 1) {
            int id;
            string name;

            cout << "Enter ID: ";
            id = getInt();

            if (idExists(students, id)) {
                cout << "ID already exists!\n";
                continue;
            }

            cout << "Enter Name: ";
            cin >> name;

            students.push_back(Student(id, name));
            cout << "Student added.\n";
        }

        else if (choice == 2) {
            int id;
            cout << "Enter ID: ";
            id = getInt();

            int idx = findIndex(students, id);

            if (idx == -1) {
                cout << "Student not found.\n";
                continue;
            }

            cout << "Present? (1/0): ";
            int p = getInt();

            students[idx].markAttendance(p == 1);
            cout << "Updated.\n";
        }

        else if (choice == 3) {
            for (const auto &s : students)
                s.display();
        }

        else if (choice == 4) {
            int id;
            cout << "Enter ID: ";
            id = getInt();

            int idx = findIndex(students, id);

            if (idx == -1) {
                cout << "Student not found.\n";
                continue;
            }

            students[idx].predictLeave();
        }

        else if (choice == 5) {
            saveStudents(students);
            cout << "Exiting...\n";
            break;
        }

        else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}