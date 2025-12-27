# MCA CET College Guider

Simple CLI tool that recommends MCA colleges based on your percentile and category.

## Quick start

Build (example):

```bash
g++ -std=c++11 main.cpp CollegeData.cpp -O2 -Wall -Wextra -o mca_guider.exe
```

Run:

```bash
.\\mca_guider.exe  # on Windows
```

## Notes

- Keep `mca_cet_clean.csv` in the project root — the program reads this dataset at startup.
- The menu shows recommended and nearby (reach) colleges and can export results to CSV.

## What to keep in the repo

- `main.cpp`, `CollegeData.cpp`, `CollegeData.h`
- `mca_cet_clean.csv` (data file) — optional if you prefer to keep data separate
- `README.md`, `LICENSE`

## Do NOT commit

- Compiled binaries (`*.exe`), object files (`*.o`), virtualenvs (`.venv/`), or temporary exports like `recs.csv`.

## Publish to GitHub

1. Create a new empty repository on GitHub.
2. From this folder run:

```bash
git remote add origin https://github.com/ravikhokle/MCA-College-Guider-CPP.git
git branch -M main
git push -u origin main
```

## License

MIT — see the `LICENSE` file.

If you want a screenshot or CI (GitHub Actions) added, tell me and I’ll add it.
