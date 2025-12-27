# MCA CET College Guider

A small command-line tool that recommends MCA colleges based on your percentile and category.

## Quick start

1. Build (Windows / MinGW):

   g++ -std=c++11 main.cpp CollegeData.cpp -O2 -Wall -Wextra -o mca_guider.exe

2. Run:

   .\mca_guider.exe

3. Follow the interactive prompts: enter your percentile and category, then pick a menu option.

## Features

- Shows recommended colleges and nearby "reach" colleges.
- Export recommendations to CSV from the menu.
- Works with the `mca_cet_clean.csv` dataset (keep it in the project root).

## Contributing

- Fixes and small improvements welcome. Create a branch and open a Pull Request with a short description.

## Publishing to GitHub

1. Create an empty repository on GitHub.
2. From your project directory run:

   git remote add origin https://github.com/<your-username>/<repo>.git
   git branch -M main
   git push -u origin main

(Replace the URL with your repo URL.)

## License

This project is released under the MIT License — see `LICENSE`.

---

If you want, I can add the `LICENSE` file and a short `CONTRIBUTING.md` now.