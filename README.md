MCA CET College Guider
======================

Small console app to recommend MCA colleges based on percentile and category.

Build
-----
Requires a C++ compiler (g++ / MSVC). Example build command with MinGW g++:

    g++ -std=c++11 main.cpp CollegeData.cpp -O2 -Wall -Wextra -o mca_guider.exe

Run
---

    .\mca_guider.exe

Data
----
Keep `mca_cet_clean.csv` in the same folder; it's the dataset used by the program.

License
-------
Add a license if you plan to publish this repository publicly.
