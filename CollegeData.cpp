#include "CollegeData.h"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cctype>
#include <map>

College colleges[MAX_COLLEGES];
int collegeCount = 0;

void initColleges() {
    // Reset count (safe if called multiple times)
    collegeCount = 0;

    // 1) Try loading cleaned CSV produced by extract_mca_cutoffs.py
    std::ifstream csvFile("mca_cet_clean.csv");
    if (csvFile.is_open()) {
        // Small utilities
        auto trim = [](std::string &s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch){ return !std::isspace(ch); }));
            s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch){ return !std::isspace(ch); }).base(), s.end());
        };

        // Simple CSV splitter that handles quoted fields with commas
        auto splitCSV = [&](const std::string &ln) {
            std::vector<std::string> out;
            std::string cur;
            bool inQuotes = false;
            for (size_t i = 0; i < ln.size(); ++i) {
                char c = ln[i];
                if (c == '"') {
                    // check for double quote escape
                    if (inQuotes && i + 1 < ln.size() && ln[i+1] == '"') { cur.push_back('"'); ++i; }
                    else { inQuotes = !inQuotes; }
                } else if (c == ',' && !inQuotes) {
                    std::string tmp = cur; trim(tmp); out.push_back(tmp); cur.clear();
                } else {
                    cur.push_back(c);
                }
            }
            std::string tmp = cur; trim(tmp); out.push_back(tmp);
            return out;
        };

        std::string header;
        if (std::getline(csvFile, header)) {
            std::string ln;
            while (std::getline(csvFile, ln)) {
                if (ln.empty()) continue;
                auto cols = splitCSV(ln);
                // Expecting at least 16 columns as per extractor
                if (cols.size() < 16) continue;
                try {
                    College c{};
                    c.code = std::stoi(cols[0]);
                    c.name = cols[1];
                    c.city = cols[2];
                    c.grade = cols[3];
                    c.isGovt = (cols[4].size() && std::stoi(cols[4]) != 0);
                    c.cutoffOpen = (cols[6].size() ? std::stof(cols[6]) : 0.0f);
                    c.cutoffSC = (cols[7].size() ? std::stof(cols[7]) : 0.0f);
                    c.cutoffST = (cols[8].size() ? std::stof(cols[8]) : 0.0f);
                    c.cutoffNT = (cols[9].size() ? std::stof(cols[9]) : 0.0f);
                    c.cutoffOBC = (cols[10].size() ? std::stof(cols[10]) : 0.0f);
                    c.cutoffEWS = (cols[11].size() ? std::stof(cols[11]) : 0.0f);
                    c.cutoffPWD = (cols[12].size() ? std::stof(cols[12]) : 0.0f);
                    c.seatsOpen = (cols[13].size() ? std::stoi(cols[13]) : 0);
                    c.seatsST = (cols[14].size() ? std::stoi(cols[14]) : 0);

                    // Sanity: default grade if missing
                    if (c.grade.empty()) c.grade = "Other";

                    if (collegeCount < MAX_COLLEGES) {
                        colleges[collegeCount++] = c;
                    } else break;
                } catch (...) {
                    // Skip malformed row
                    continue;
                }
            }
        }
        csvFile.close();

        // Summary debug
        int withOpen = 0, withSC = 0, withST = 0;
        for (int i = 0; i < collegeCount; ++i) {
            if (colleges[i].cutoffOpen > 0.0f) ++withOpen;
            if (colleges[i].cutoffSC > 0.0f) ++withSC;
            if (colleges[i].cutoffST > 0.0f) ++withST;
        }
        std::cout << "Loaded " << collegeCount << " colleges from mca_cet_clean.csv (" << withOpen << " Open, " << withSC << " SC, " << withST << " ST).\n";
        return;
    }

    // If CSV not available, require user to generate it. Keep one minimal fallback so the
    // program still runs and can be tested without the CSV file.
    std::ifstream file("allCollegeData.txt");
    if (!file.is_open()) {
        std::cerr << "Warning: mca_cet_clean.csv not found; using minimal fallback. Please run extract_mca_cutoffs.py to generate CSV.\n";
        // Minimal fallback record (keeps behavior predictable)
        if (collegeCount < MAX_COLLEGES) {
            colleges[collegeCount++] = {
                3012,
                "Veermata Jijabai Technological Institute (VJTI), Matunga, Mumbai",
                "Mumbai",
                "A++",
                true,
                99.51f, 0.0f, 98.32f, 0.0f, 0.0f, 0.0f, 0.0f,
                60, 0
            };
        }
        return;
    }

    // Raw-text parsing removed; CSV-only mode.
    file.close();
    std::cout << "CSV not found or raw parsing disabled; minimal fallback used (run extract_mca_cutoffs.py to generate full CSV).\n";
    return;
}
