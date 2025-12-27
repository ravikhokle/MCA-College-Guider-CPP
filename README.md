# MCA CET College Guider

A simple command-line tool that recommends MCA colleges based on your percentile and category.

## Quick start

Prerequisites:
- A C++ compiler (g++, MSVC, etc.)
- `mca_cet_clean.csv` in the project root (the program reads this dataset at startup)

Build (example, MinGW):

    g++ -std=c++11 main.cpp CollegeData.cpp -O2 -Wall -Wextra -o mca_guider.exe

Run:

    .\mca_guider.exe

Follow the interactive prompts: enter your percentile and category, then pick a menu option.

## Repository contents (what to keep)

- main.cpp — program entry and UI
- CollegeData.cpp / CollegeData.h — data loader and models
- mca_cet_clean.csv — dataset used by the program (keep if you want full data)
- README.md, LICENSE — documentation and license

## What NOT to commit

- Compiled binaries and object files: `*.exe`, `*.o`, `*.i`
- Virtual environments: `.venv/` or `venv/`
- Export files you create during runs (e.g., `recs.csv`)
These are already ignored by `.gitignore`.

## Features

- Shows recommended colleges and nearby "reach" colleges.
- Export recommendations to CSV from the menu.
- Console-friendly color output if the terminal supports ANSI colors.

## Contributing

Bugs and small improvements are welcome. Please open an issue or a pull request and include a short description of your change.

## Publish to GitHub (one-time)

1. Create an empty repository on GitHub (do not add a README there).
2. On your machine, run these commands from the project folder:

    git remote add origin https://github.com/ravikhokle/MCA-College-Guider-CPP.git
    git branch -M main
    git push -u origin main

If `git push` asks for credentials, use your GitHub username/password or a Personal Access Token (PAT).

## License

This project is licensed under the MIT License — see the `LICENSE` file.

---

If you want, I can push the repo to GitHub now (I will use the URL you provided). If push needs credentials I will show any errors and next steps.