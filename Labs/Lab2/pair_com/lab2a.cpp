#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Student {
    int id;
    int current; 
    vector<pair<int, int>> preferences; 
};

struct College {
    int id;
    int capacity;
    int current_capacity; 
    vector<pair<int, int>> preferences; 
    vector<int> students; 
};

int main() {
    int N, M;
    cin >> N >> M;

    vector<College> colleges(M);
    vector<Student> students(N);

    for (int i = 0; i < M; i++) {
        cin >> colleges[i].capacity;
        colleges[i].id = i;
        colleges[i].current_capacity = 0;
    }

    for (int i = 0; i < N; i++) {
        students[i].id = i;
        students[i].current = -1; 
        students[i].preferences.resize(M);
        for (int j = 0; j < M; j++) {
            int preferenceValue;
            cin >> preferenceValue;
            students[i].preferences[j] = {j, preferenceValue};
        }
        sort(students[i].preferences.begin(), students[i].preferences.end(),
             [](const pair<int, int>& a, const pair<int, int>& b) {
                 return a.second > b.second;
             });
    }

    for (int i = 0; i < M; i++) {
        colleges[i].preferences.resize(N);
        for (int j = 0; j < N; j++) {
            int preferenceValue;
            cin >> preferenceValue;
            colleges[i].preferences[j] = {j, preferenceValue};
        }
        sort(colleges[i].preferences.begin(), colleges[i].preferences.end(),
             [](const pair<int, int>& a, const pair<int, int>& b) {
                 return a.second > b.second;
             });
    }

    bool allMatched = false;
    while (!allMatched) {
        allMatched = true;
        for (int s = 0; s < N; s++) {
            for (int c = 0; c < M; c++) {
                if (students[s].current == c) {
                    continue;
                }
                bool prefersCollege = false;
                for (const auto& pref : students[s].preferences) {
                    if (pref.first == c) {
                        if (students[s].current == -1) {
                            prefersCollege = (pref.second > 0);
                        } else {
                            int currentCollegePreference = -1;
                            for (const auto& p : students[s].preferences) {
                                if (p.first == students[s].current) {
                                    currentCollegePreference = p.second;
                                    break;
                                }
                            }
                            prefersCollege = (pref.second > currentCollegePreference);
                        }
                        break;
                    }
                }
                if (!prefersCollege) {
                    continue;
                }
                bool PrefersStudent = false;
                if (colleges[c].current_capacity < colleges[c].capacity) {
                    int studentPreference = -1;
                    for (const auto& pref : colleges[c].preferences) {
                        if (pref.first == s) {
                            studentPreference = pref.second;
                            break;
                        }
                    }
                    if (studentPreference > 0) {
                        PrefersStudent = true;
                    }
                }else {
                    int studentSPreference = -1;              
                    for (const auto& pref : colleges[c].preferences) {
                        if (pref.first == s) {
                            studentSPreference = pref.second;
                            break;
                        }
                    }
                    for (int studentId : colleges[c].students) {
                        int currentStudentPreference = -1;
                        for (const auto& pref : colleges[c].preferences) {
                            if (pref.first == studentId) {
                                currentStudentPreference = pref.second;
                                break;
                            }
                        }
                        if (studentSPreference > currentStudentPreference) {
                            PrefersStudent = true;
                            break;
                        }
                    }
                }
                if (prefersCollege && PrefersStudent) {
                    allMatched = false; 
                    break;
                }
            }
            if (!allMatched) {
                break;
            }
        }
        if (!allMatched) {
            for (int i = 0; i < N; i++) {
                Student& student = students[i];
                if (student.current == -1) {
                    for (const auto& pref : student.preferences) {
                        int collegeId = pref.first;
                        int studentPreferenceValue = pref.second;
                        if (studentPreferenceValue < 0) {
                            continue;
                        }
                        College& college = colleges[collegeId];

                        bool isNegativePreference = false;
                        for (const auto& collegePref : college.preferences) {
                            if (collegePref.first == student.id && collegePref.second < 0) {
                                isNegativePreference = true;
                                break;
                            }
                        }

                        if (isNegativePreference) {
                            continue;
                        }

                        if (college.current_capacity < college.capacity) {
                            college.students.push_back(student.id);
                            college.current_capacity++;
                            student.current = collegeId;
                            break;
                        } else {
                            int worstStudentId = college.students[0];
                            int worstStudentPreference = college.preferences[0].second;      
                            for (int studentId : college.students) {
                                for (const auto& pref : college.preferences) {
                                    if (pref.first == studentId) {
                                        if (pref.second < worstStudentPreference) {
                                            worstStudentId = studentId;
                                            worstStudentPreference = pref.second;
                                        }
                                        break;
                                    }
                                }
                            }
                            int studentPreference = -1;
                            for (const auto& pref : college.preferences) {
                                if (pref.first == student.id) {
                                    studentPreference = pref.second;
                                    break;
                                }
                            }
                            if (studentPreference > worstStudentPreference) {
                                auto it = find(college.students.begin(), college.students.end(), worstStudentId);
                                if (it != college.students.end()) {
                                    college.students.erase(it);
                                }
                                college.students.push_back(student.id);
                                student.current = collegeId;
                                students[worstStudentId].current = -1;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < M; i++) {
        cout << colleges[i].students.size() << " ";
        for (int studentId : colleges[i].students) {
            cout << studentId + 1 << " "; 
        }
        cout << endl;
    }

    return 0;
}