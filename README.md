# MCA CET College Guider

Simple CLI tool that recommends MCA colleges based on your percentile and category.

## Quick start

Build (example):

```bash
g++ -std=c++11 main.cpp CollegeData.cpp -O2 -Wall -Wextra -o mca_guider.exe
```

Run:

```bash
.\mca_guider.exe  # on Windows
```

## Notes

- Keep `mca_cet_clean.csv` in the project root — the program reads this dataset at startup.
- The menu shows recommended and nearby (reach) colleges and can export results to CSV.

## Program files

- `main.cpp`, `CollegeData.cpp`, `CollegeData.h`
- `mca_cet_clean.csv` (data file)
- `README.md`, `LICENSE`

## License

MIT — see the `LICENSE` file.