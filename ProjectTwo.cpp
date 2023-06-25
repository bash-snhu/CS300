// Brooklen Ashleigh
// CS300 Project Two
// June 14th, 2023

#include <iostream>
#include <fstream>
#include <sstream>
#include <String>
#include <Vector>
#include <algorithm>
using namespace std;

class Course {
private:
    string id;
    string name;
    vector<string> prereqs;

public:
    // Empty constructor
    Course() {}

    // Constructor with initialization
    Course(string uniquecourseId, string courseName, vector<string> prerequisites) {
        id = uniquecourseId;
        name = courseName;
        prereqs = prerequisites;
    }

    // ID Getter
    string getId() {
        return id;
    }

    // Course id comparator override
    bool operator<(const Course course) const {
        return id < course.id;
    }

    // Print simple course information for catalog
    void printInfo() {
        cout << id << ", " << name << endl;
    }

    // Print detailed course information with prerequisites
    void printWithReqs() {
        cout << endl;
        
        // Call default printInfo
        printInfo();

        cout << "Prerequisites: ";

        // Print list of prereqs from vector
        for (int i = 0; i < prereqs.size(); i++) {
            
            if (i != 0) cout << ", ";
            cout << prereqs.at(i);
        }

        // Print None if no prereqs exist
        if (prereqs.size() == 0) {
            cout << "None";
        }

        cout << endl;
    }
};

// Converts comma-separated string into a vector
vector<string> stringToVector(string input) {
    vector<string> inputv;

    stringstream ss(input);
    
    // Splits and appends comma delimeted items
    while (ss.good()) {
        string partial;
        getline(ss, partial, ',');
        inputv.push_back(partial);
    }
    return inputv;
}

// Read input text and store it in vector data structure
vector<Course> readFromFile() {
    // Initialize reading object
    fstream readFile;
    // Vector all courses will be appended to
    vector<Course> courses;

    // Open file
    readFile.open("ABCU_Advising_Program_Input.txt", ios::in);

    // If opening the file is successful
    if (readFile.is_open()) {
        string line;

        // Read line
        while (getline(readFile, line)) {
            // Stores current line
            vector<string> temp = stringToVector(line);
            // Vector for storing additional prerequisites if they exist
            vector<string> prereqs;

            // If the line only includes course ID and name
            if (temp.size() == 2) {
                // Create course object with id, name, and empty prereq vector
                Course tempcourse(temp.at(0), temp.at(1), prereqs);
                // Append course object to vector
                courses.push_back(tempcourse);
            }
            // If the line also includes prerequisites
            else if (temp.size() > 2) {
                // For every additional item after ID and name
                for (int i = 2; i < temp.size(); i++) {
                    // Append prereq
                    prereqs.push_back(temp.at(i));
                }

                // Create course object with id, name, and list of prereqs
                Course tempcourse(temp.at(0), temp.at(1), prereqs);
                // Append course object
                courses.push_back(tempcourse);
            }
        }
        // Close file
        readFile.close();
    }
    return courses;
}

// Prints all courses in the vector using the Course class method
void printCourses(vector<Course> courses) {
    for (int i = 0; i < courses.size(); i++) {
        courses.at(i).printInfo();
    }
}

// Search for course using binary search algorithm
int binarySearch(vector<Course> courses, string searchTerm) {
    int mid = 0;
    int high = courses.size() - 1;
    int low = 0;

    while (high >= low) {
        // Set midpoint indice
        mid = (high + low) / 2;

        // If checked course is earlier
        if (courses.at(mid).getId() < searchTerm) {
            low = mid + 1;
        }
        // If checked course is later
        else if (courses.at(mid).getId() > searchTerm) {
            high = mid - 1;
        }
        else {
            return mid;
        }
    }
    // If failed to find
    return -1;
}

// Partition method for recursive QuickSort algorithm
int partition(vector<Course>& course, int left, int right) {

    // Select pivot point in middle of vector
    int pivotIndex = left + (right - left) / 2;
    string pivotValue = course.at(pivotIndex).getId();

    Course temp;

    while (left <= right) {
        // While low index is smaller than pivot
        while (course.at(left).getId() < pivotValue) {
            left++;
        }

        // While high index is larger than pivot
        while (course.at(right).getId() > pivotValue) {
            right--;
        }

        // If left index is smaller than right, swap
        if (left <= right) {
            // Swap left and right elements
            temp = course.at(left);
            course.at(left) = course.at(right);
            course.at(right) = temp;

            // Updated indexes
            left++;
            right--;
        }
    }
    return left;
}

// Quicksorts vector by course ID
void quicksort(vector<Course>& courses, int left, int right) {
    if (left < right) {
        // Create midpoint index
        int pivotIndex = partition(courses, left, right);

        // Call quicksort recursively
        quicksort(courses, left, pivotIndex - 1);
        quicksort(courses, pivotIndex, right);
    }
}

// Main
int main()
{
    // Course-containing data structure
    vector<Course> courses;
    // Input variables
    string courseChoice;
    int choice = 0;
  
    cout << "\nWelcome to the course planner." << endl;
    
    while (choice != 4) {
        // Print Menu
        cout << endl;
        cout << "    1. Load Data Structure" << endl;
        cout << "    2. Print Course List" << endl;
        cout << "    3. Print Course" << endl;
        cout << "    4. Exit" << endl;

        // Input menu choice from user
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {
        case 1:
            // Input all data into vector structure
            courses = readFromFile();
            break;

        case 2:
            cout << "Here is a sample schedule:\n" << endl;

            // Sort vector alphanumerically with quicksort
            quicksort(courses, 0, courses.size() - 1);

            // Print sample schedule of all input without prerequisites
            printCourses(courses);

            break;

        case 3:
            int searchResult;

            // Input search term from user
            cout << "What course do you want to know about? ";
            cin >> courseChoice;
            
            // Transform input to all caps for comparison
            transform(courseChoice.begin(), courseChoice.end(), courseChoice.begin(), ::toupper);

            // Find index of sought cource object in courses vector using Binary Search
            searchResult = binarySearch(courses, courseChoice);

            // Check if search term was found (-1 = failed)
            if (searchResult != -1) {
                // Print course information with prerequisite information
                courses.at(searchResult).printWithReqs();
            }
            else {
                cout << "Sorry, course not found!" << endl;
            }

            break;

        case 4:
            // Exit condition
            break;

        // If none of the options, entry was out of scope
        default:
            cout << choice << " is not a valid option." << endl;
        }
    }

    cout << "Goodbye!" << endl;
}