#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <iomanip>
#include "CollegeData.h"
using namespace std;

// Simple singly linked list (integer data for DSA demo)
struct Node {
    int data;
    Node* next;
    Node(int v) : data(v), next(nullptr) {}
};

class LinkedList {
public:
    Node* head;
    LinkedList() : head(nullptr) {}
    ~LinkedList() {
        while (head) {
            Node* t = head;
            head = head->next;
            delete t;
        }
    }
    void insertAtEnd(int v) {
        Node* n = new Node(v);
        if (!head) { head = n; return; }
        Node* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = n;
    }
    bool remove(int v) {
        Node* cur = head;
        Node* prev = nullptr;
        while (cur) {
            if (cur->data == v) {
                if (prev) prev->next = cur->next; else head = cur->next;
                delete cur;
                return true;
            }
            prev = cur; cur = cur->next;
        }
        return false;
    }
    void display() {
        if (!head) { cout << "(empty)\n"; return; }
        Node* cur = head;
        while (cur) { cout << cur->data << " "; cur = cur->next; }
        cout << '\n';
    }
};

// Simple stack (int)
class Stack {
    vector<int> data;
public:
    void push(int v) { data.push_back(v); }
    bool pop(int &out) {
        if (data.empty()) return false;
        out = data.back(); data.pop_back(); return true;
    }
    bool top(int &out) {
        if (data.empty()) return false;
        out = data.back(); return true;
    }
    bool empty() { return data.empty(); }
    void display() {
        if (data.empty()) { cout << "(empty)\n"; return; }
        for (auto it = data.rbegin(); it != data.rend(); ++it) cout << *it << " ";
        cout << '\n';
    }
};

// Simple queue (int)
class Queue {
    vector<int> data;
public:
    void enqueue(int v) { data.push_back(v); }
    bool dequeue(int &out) {
        if (data.empty()) return false;
        out = data.front(); data.erase(data.begin()); return true;
    }
    bool empty() { return data.empty(); }
    void display() {
        if (data.empty()) { cout << "(empty)\n"; return; }
        for (auto &v : data) cout << v << " "; cout << '\n';
    }
};



// Utility to show a college
void printCollege(const College &c) {
    cout << "Code: " << c.code << "\n";
    cout << "Name: " << c.name << "\n";
    cout << "City: " << c.city << "\n";
    cout << "Grade: " << c.grade << "\n";
    cout << "Govt: " << (c.isGovt ? "Yes" : "No") << "\n";
    cout << "Open cutoff: " << c.cutoffOpen << "\n";
    cout << "ST cutoff: " << c.cutoffST << "\n";
    cout << "Seats Open/ST: " << c.seatsOpen << "/" << c.seatsST << "\n";
}

void printCollegeSummary(const College &c) {
    cout << c.code << " | " << c.name << " | City: " << c.city << " | Open: " << c.cutoffOpen << " | ST: " << c.cutoffST << "\n";
}

// Forward declarations (defined later)
float getCutoffForCategory(const College &c, const string &category);
string safetyLabel(float margin);
// Round helper (defined later)
static float round2(float v);

// Print a header for recommendation table
void printRecommendationHeader() {
    using std::setw; using std::left; using std::right;
    cout << left << setw(6) << "Code" << ' ' << setw(40) << "Name" << ' ' << setw(15) << "City" << ' ' << right << setw(7) << "Cutoff" << ' ' << setw(7) << "Margin" << ' ' << setw(12) << "Safety" << "\n";
    cout << string(6+1+40+1+15+1+7+1+7+1+12, '-') << "\n";
}

// Global flag for ANSI color output (initialized at runtime in main)
static bool g_useAnsiColors = false;

// Wrap a string with ANSI color codes based on safety
static string colorizeSafety(const string &s) {
    if (!g_useAnsiColors) return s;
    const string RESET = "\033[0m";
    const string RED = "\033[31m";
    const string YELLOW = "\033[33m";
    const string GREEN = "\033[32m";
    if (s.find("Very Safe") != string::npos) return GREEN + s + RESET;
    if (s.find("Safe") != string::npos) return GREEN + s + RESET;
    if (s.find("On Border") != string::npos) return YELLOW + s + RESET;
    if (s.find("Below") != string::npos) return RED + s + RESET;
    if (s.find("Unknown") != string::npos) return RED + s + RESET;
    return s;
}

// Print a recommendation row with safety margin and label (formatted)
void printRecommendationRow(const College &c, float userPercentile, const string &category) {
    using std::setw; using std::left; using std::right; using std::setprecision; using std::fixed;
    float rawCutoff = getCutoffForCategory(c, category);
    float cutoff = (rawCutoff > 0.0f) ? round2(rawCutoff) : 0.0f; // round to 2 decimals for display and matching
    float margin = (cutoff > 0.0f) ? (userPercentile - cutoff) : 0.0f;
    string safety = (cutoff > 0.0f) ? safetyLabel(margin) : string("Unknown");
    string name = c.name; if (name.size() > 40) name = name.substr(0,37) + "...";
    string safetyColored = colorizeSafety(safety);
    cout << left << setw(6) << c.code << ' ' << setw(40) << name << ' ' << setw(15) << c.city << ' ';
    if (cutoff > 0.0f) cout << right << setw(7) << fixed << setprecision(2) << cutoff << ' ' << setw(7) << fixed << setprecision(2) << margin << ' ';
    else cout << right << setw(7) << "N/A" << ' ' << setw(7) << "N/A" << ' ';
    cout << setw(12) << safetyColored << "\n";
}

// Export recommendations to CSV
#include <fstream>
void exportRecommendationsCSV(const vector<int> &idxs, const string &filename, float userPercentile, const string &category) {
    std::ofstream out(filename);
    if (!out.is_open()) { cout << "Failed to open " << filename << " for writing.\n"; return; }
    out << "code,name,city,grade,isGovt,cutoff,margin,safety\n";
    for (int idx : idxs) {
        const College &c = colleges[idx];
        float rawCutoff = getCutoffForCategory(c, category);
        float cutoff = (rawCutoff > 0.0f) ? round2(rawCutoff) : 0.0f;
        float margin = (cutoff > 0.0f) ? (userPercentile - cutoff) : 0.0f;
        string safety = (cutoff > 0.0f) ? safetyLabel(margin) : string("Unknown");
        // escape quotes in name
        string name = c.name;
        for (auto &ch : name) if (ch == '"') ch = '\'';
        out << c.code << ",\"" << name << "\"," << c.city << "," << c.grade << "," << (c.isGovt?1:0) << "," << cutoff << "," << margin << "," << safety << "\n";
    }
    out.close();
    cout << "Exported " << idxs.size() << " recommendations to " << filename << "\n";
}

void menu() {
    cout << "\n==== MCA CET College Guider ====" << endl;
    cout << "1) Show recommendations (based on current percentile & category)" << endl;
    cout << "2) Nearest matches (just above my percentile)" << endl;
    cout << "3) List all colleges" << endl;
    cout << "4) Change percentile/category" << endl;
    cout << "5) Exit" << endl;
    cout << "6) Simulate percentile increase (see colleges become available)" << endl;
    cout << "7) Toggle ANSI color output (currently: " << (g_useAnsiColors?"ON":"OFF") << ")" << endl;
    cout << "Enter choice: ";
}

// Recommendation utilities
#include <algorithm>
#include <cctype>
#include <limits>
#include <sstream>
#include <cmath> // for floor

// Round float to 2 decimal places (works reliably across compilers)
static float round2(float v) {
    return std::floor(v * 100.0f + 0.5f) / 100.0f;
}


// Get cutoff for a college for a given category token
float getCutoffForCategory(const College &c, const string &category) {
    if (category == "ST") return c.cutoffST;
    if (category == "SC") return c.cutoffSC;
    if (category == "OBC") return c.cutoffOBC;
    if (category == "EWS") return c.cutoffEWS;
    if (category == "PWD") return c.cutoffPWD;
    if (category == "NT") return c.cutoffNT;
    // default: Open
    return c.cutoffOpen;
}

// Safety label based on margin
string safetyLabel(float margin) {
    if (margin >= 5.0f) return "Very Safe";
    if (margin >= 1.0f) return "Safe";
    if (margin >= 0.0f) return "On Border";
    return "Below cutoff";
}

vector<int> recommendColleges(float percentile, const string &category, int limit = 10) {
    struct R { float score; float cutoff; int idx; };
    vector<R> matches;
    vector<int> unknowns;
    for (int i = 0; i < collegeCount; ++i) {
        float rawCutoff = getCutoffForCategory(colleges[i], category);
        if (rawCutoff > 0.0f) {
            // Round cutoff to 2 decimals for matching/display consistency
            float cutoff = std::round(rawCutoff * 100.0f) / 100.0f;
            if (percentile >= cutoff) {
                float score = cutoff; // sort by cutoff (higher is better)
                matches.push_back({score, cutoff, i});
            }
        } else {
            // unknown cutoff, keep separately
            unknowns.push_back(i);
        }
    }
    sort(matches.begin(), matches.end(), [](const R &a, const R &b) {
        if (a.score != b.score) return a.score > b.score;
        return a.cutoff > b.cutoff;
    });

    vector<int> result;
    for (auto &r : matches) {
        result.push_back(r.idx);
        if ((int)result.size() >= limit) return result;
    }
    // append some unknown-cutoff colleges if we still need more
    for (int idx : unknowns) {
        result.push_back(idx);
        if ((int)result.size() >= limit) break;
    }
    return result;
}

vector<int> nearestMatches(float percentile, const string &category, float range = 5.0f, int limit = 10) {
    struct N { float diff; int idx; };
    vector<N> near;
    for (int i = 0; i < collegeCount; ++i) {
        float cutoff = getCutoffForCategory(colleges[i], category);
        if (cutoff > percentile && cutoff - percentile <= range) {
            near.push_back({cutoff - percentile, i});
        }
    }
    sort(near.begin(), near.end(), [](const N &a, const N &b) {
        if (a.diff != b.diff) return a.diff < b.diff;
        return a.idx < b.idx;
    });
    vector<int> res;
    for (auto &n : near) {
        res.push_back(n.idx);
        if ((int)res.size() >= limit) break;
    }
    return res;
}

// Read a line from the console even if stdin is redirected from a pipe/file.
// On Windows use CONIN$, on POSIX use /dev/tty.
#include <cstdio>
#include <stdio.h>
#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#else
#include <unistd.h>
#endif

static bool stdinIsATty() {
    // Check whether stdin is a terminal (file descriptor 0)
    return isatty(0);
}

static string readLineConsole(const string &prompt) {
    // If stdin is a TTY, just use standard getline
    if (stdinIsATty()) {
        cout << prompt;
        string s; std::getline(cin, s);
        return s;
    }
    // Otherwise, open console device
#ifdef _WIN32
    FILE *f = fopen("CONIN$", "r");
#else
    FILE *f = fopen("/dev/tty", "r");
#endif
    if (!f) {
        // fallback to standard input
        cout << prompt;
        string s; std::getline(cin, s);
        return s;
    }
    cout << prompt;
    char buf[512];
    if (!fgets(buf, sizeof(buf), f)) {
        fclose(f);
        return string();
    }
    fclose(f);
    string s(buf);
    // strip trailing newlines
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) s.pop_back();
    return s;
}

// Read a token (single word) from console
static string readTokenConsole(const string &prompt) {
    string line = readLineConsole(prompt);
    std::istringstream iss(line);
    string token; iss >> token; return token;
} 

// read category/caste input (Open/SC/ST/OBC/EWS/PWD/NT)
string readCategory() {
    cout << "Enter your caste/category (Open/SC/ST/OBC/EWS/PWD/NT): ";
    string cat; cin >> cat;
    for (auto &ch : cat) ch = toupper((unsigned char)ch);
    if (cat == "ST") return "ST";
    if (cat == "SC") return "SC";
    if (cat == "OBC") return "OBC";
    if (cat == "EWS") return "EWS";
    if (cat == "PWD") return "PWD";
    if (cat == "NT") return "NT";
    return "OPEN";
}

int main() {
    initColleges();

    // Initialize ANSI color support depending on whether stdout is a TTY
    // Use file descriptor 1 (stdout) to detect TTY without fileno() portability issues
    g_useAnsiColors = isatty(1);

    // Use ASCII hyphen to avoid console encoding issues on Windows
    cout << "Welcome to MCA CET College Guider" << endl;
    // Read percentile interactively (works even if stdin is piped)
    float userPercentile = -1.0f;
    while (true) {
        string pct = readLineConsole("Enter your percentile (0-100): ");
        if (pct.empty()) continue;
        try {
            userPercentile = std::stof(pct);
        } catch (...) { userPercentile = -1.0f; }
        if (userPercentile >= 0.0f && userPercentile <= 100.0f) break;
        cout << "Please enter a valid percentile (0-100).\n";
    }
    string userCategory = "OPEN";
    while (true) {
        string cat = readLineConsole("Enter your caste/category (Open/SC/ST/OBC/EWS/PWD/NT) [default Open]: ");
        if (cat.empty()) { userCategory = "OPEN"; break; }
        for (auto &ch : cat) ch = toupper((unsigned char)ch);
        if (cat == "ST" || cat == "SC" || cat == "OBC" || cat == "EWS" || cat == "PWD" || cat == "NT") { userCategory = cat; break; }
        if (cat == "OPEN") { userCategory = "OPEN"; break; }
        cout << "Invalid category. Please enter one of Open/SC/ST/OBC/EWS/PWD/NT.\n";
    }

    int choice = 0;
    while (true) {
        menu();
        string choiceStr = readTokenConsole("");
        // If user just presses Enter, default to choice 1 (show recommendations)
        if (choiceStr.empty()) choiceStr = "1";
        try { choice = std::stoi(choiceStr); } catch (...) { cout << "Invalid choice input.\n"; continue; }
        if (choice == 1) {
            string kStr = readLineConsole("How many recommendations do you want? (default 10): "); int k = 10; if (!kStr.empty()) { try { k = std::stoi(kStr); } catch(...) { k = 10; } }

            vector<int> rec = recommendColleges(userPercentile, userCategory, k);
            if (rec.empty()) cout << "No matches found.\n";
            else {
                cout << "Recommended colleges for percentile " << userPercentile << " (category: " << userCategory << "):\n";
                printRecommendationHeader();
                for (int idx : rec) printRecommendationRow(colleges[idx], userPercentile, userCategory);

                // Also show some "reach" colleges just above the percentile
                vector<int> reach = nearestMatches(userPercentile, userCategory, 5.0f, 10);
                if (!reach.empty()) {
                    cout << "\nReach colleges (just above your percentile):\n";
                    printRecommendationHeader();
                    for (int idx : reach) printRecommendationRow(colleges[idx], userPercentile, userCategory);
                }

                // Offer export of the main recommendations only
                string ex = readTokenConsole("Export these recommendations to CSV? (y/N): ");
                if (!ex.empty() && (ex[0]=='y' || ex[0]=='Y')) {
                    string fname = readLineConsole("Enter filename (default recs.csv): "); if (fname.empty()) fname = "recs.csv";
                    exportRecommendationsCSV(rec, fname, userPercentile, userCategory);
                }
            }
        } else if (choice == 2) {
            string rStr = readLineConsole("Show nearest matches within how many percentile points? (default 5): "); float r = 5.0f; if (!rStr.empty()) { try { r = std::stof(rStr); } catch(...) { r = 5.0f; } }
            vector<int> near = nearestMatches(userPercentile, userCategory, r, 20);
            if (near.empty()) cout << "No nearby matches found within " << r << " points.\n";
            else {
                cout << "Colleges just above your percentile (within " << r << "):\n";
                printRecommendationHeader();
                for (int idx : near) printRecommendationRow(colleges[idx], userPercentile, userCategory);
                string ex = readTokenConsole("Export these nearby matches to CSV? (y/N): ");
                if (!ex.empty() && (ex[0]=='y' || ex[0]=='Y')) {
                    string fname = readLineConsole("Enter filename (default near.csv): "); if (fname.empty()) fname = "near.csv"; exportRecommendationsCSV(near, fname, userPercentile, userCategory);
                }
            }
        } else if (choice == 3) {
            vector<int> allIdx;
            for (int i = 0; i < collegeCount; ++i) allIdx.push_back(i);
            if (allIdx.empty()) cout << "No colleges available.\n";
            else { cout << "Listing colleges:\n"; for (int idx : allIdx) printCollegeSummary(colleges[idx]); }
        } else if (choice == 4) {
            // Update percentile interactively
            while (true) {
                string pct = readLineConsole("Enter your percentile (0-100): ");
                if (pct.empty()) continue;
                try { userPercentile = std::stof(pct); } catch(...) { userPercentile = -1.0f; }
                if (userPercentile >= 0.0f && userPercentile <= 100.0f) break;
                cout << "Please enter a valid percentile (0-100).\n";
            }
            userCategory = readCategory();
            cout << "Updated percentile = " << userPercentile << ", category = " << userCategory << "\n";
        } else if (choice == 5) {
            cout << "Exiting.\n";
            break;
        } else if (choice == 6) {
            string dStr = readLineConsole("Simulate increase by how many percentile points? (e.g., 2.5): "); float d = 0.0f; try { d = std::stof(dStr); } catch(...) { cout << "Invalid input\n"; continue; }
            float newP = userPercentile + d;
            cout << "Simulating percentile = " << newP << " (category: " << userCategory << ")\n";
            vector<int> before = recommendColleges(userPercentile, userCategory, 100);
            vector<int> after = recommendColleges(newP, userCategory, 100);
            // compute newly available
            std::sort(before.begin(), before.end());
            std::sort(after.begin(), after.end());
            vector<int> newly;
            for (int idx : after) if (!binary_search(before.begin(), before.end(), idx)) newly.push_back(idx);
            if (newly.empty()) cout << "No additional colleges become available with +" << d << " points.\n";
            else {
                cout << "Newly available colleges with +" << d << " points:\n";
                printRecommendationHeader();
                for (int idx : newly) printRecommendationRow(colleges[idx], newP, userCategory);
            }
        } else if (choice == 7) {
            g_useAnsiColors = !g_useAnsiColors;
            cout << "ANSI color output is now " << (g_useAnsiColors?"ON":"OFF") << ".\n";
        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}


