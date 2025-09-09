#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Student {
    int id;
    vector<int> preferences;
    vector<int> pre_preferences; // Student's satisfaction towards colleges
    bool matched = false;
    int currentProposal = 0;

    Student(int id, vector<int> preferences, vector<int> pre_preferences)
        : id(id), preferences(preferences), pre_preferences(pre_preferences) {}
};

struct College {
    int id;
    int capacity;
    vector<int> preferences;
    vector<int> enrolledStudents;

    College(int id, int capacity, vector<int> preferences)
        : id(id), capacity(capacity), preferences(preferences) {}

    int getPreferenceRank(int studentId) {
        return preferences[studentId - 1];
    }

    bool prefers(int studentId1, int studentId2) {
        return getPreferenceRank(studentId1) > getPreferenceRank(studentId2);
    }

    void sortEnrolledStudents() {
        sort(enrolledStudents.begin(), enrolledStudents.end(), [this](int s1, int s2) {
            return getPreferenceRank(s1) > getPreferenceRank(s2);
        });
    }
};

int returnCollegeID(const vector<int>& preferences, const vector<int>& pre_preferences, int currentProposal) {
    for (size_t i = 0; i < pre_preferences.size(); i++) {
        if (pre_preferences[i] == preferences[currentProposal]) {
            return i;
        }
    }
    return -1;
}

int main() {
    int N, M;
    cin >> N >> M;

    vector<int> cap(M);
    for (int i = 0; i < M; i++) {
        cin >> cap[i];
    }

    vector<Student> students;
    for (int i = 0; i < N; i++) {
        vector<int> preferences(M);
        vector<int> pre_preferences(M);
        for (int j = 0; j < M; j++) {
            cin >> preferences[j];
            pre_preferences[j] = preferences[j];
        }

        sort(preferences.begin(), preferences.end(), greater<int>());
        students.emplace_back(i, preferences, pre_preferences);
    }

    vector<College> colleges;
    for (int i = 0; i < M; i++) {
        vector<int> c_preferences(N);
        for (int j = 0; j < N; j++) {
            cin >> c_preferences[j];
        }
        colleges.emplace_back(i, cap[i], c_preferences);
    }

    queue<Student*> freeStudents;
    for (auto& student : students) {
        freeStudents.push(&student);
    }

    while (!freeStudents.empty()) {
        Student* student = freeStudents.front();
        freeStudents.pop();

        while (student->currentProposal < M) {
            int collegeId = returnCollegeID(student->preferences, student->pre_preferences, student->currentProposal);
            College& college = colleges[collegeId];

            if (college.enrolledStudents.size() < college.capacity && college.preferences[student->id] > 0) {
                college.enrolledStudents.push_back(student->id + 1);
                student->matched = true;
                college.sortEnrolledStudents();
                break;
            } else {
                if (college.preferences[student->id] < 0) {
                    freeStudents.push(student);
                    student->matched = false;
                    student->currentProposal++;
                    break;
                } else {
                    int currentWorstStudentId = college.enrolledStudents.back();
                    if (college.prefers(student->id + 1, currentWorstStudentId)) {
                        college.enrolledStudents.pop_back();
                        college.enrolledStudents.push_back(student->id + 1);
                        student->matched = true;

                        freeStudents.push(&students[currentWorstStudentId - 1]);
                        college.sortEnrolledStudents();
                        students[currentWorstStudentId - 1].currentProposal++;
                        break;
                    } else {
                        freeStudents.push(student);
                        student->matched = false;
                        student->currentProposal++;
                        break;
                    }
                }
            }
        }

        if (!student->matched && student->currentProposal == M) {
            student->matched = true;
        }
    }

    for (const auto& college : colleges) {
        cout << college.enrolledStudents.size() << " ";
        for (int studentId : college.enrolledStudents) {
            cout << studentId << " ";
        }
        cout << endl;
    }

    return 0;
}