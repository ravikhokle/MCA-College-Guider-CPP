#ifndef COLLEGE_DATA_H
#define COLLEGE_DATA_H

#include <string>
using namespace std;

// College structure definition
struct College {
    int code;
    string name;
    string city;
    string grade;      // "A++", "A+", "A", "Other"
    bool isGovt;
    float cutoffOpen;       // last year Open percentile
    float cutoffSC;         // last year SC percentile
    float cutoffST;         // last year ST percentile
    float cutoffNT;         // last year NT percentile
    float cutoffOBC;        // last year OBC percentile
    float cutoffEWS;        // last year EWS percentile
    float cutoffPWD;        // last year PWD percentile
    int seatsOpen;
    int seatsST;
};

const int MAX_COLLEGES = 5000;
extern College colleges[MAX_COLLEGES];
extern int collegeCount;

// Function to initialize college data (parses allCollegeData.txt if present)
void initColleges();

#endif
