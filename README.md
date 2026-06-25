<div align="center">

# 🎯 Memory Leak Hunter

### *Find the leaks. Save the memory. Become a reverse engineer.*

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue?logo=c%2B%2B)](https://isocpp.org/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)]()
[![Levels](https://img.shields.io/badge/levels-10-orange)]()

[Features](#-features) • [Installation](#-installation) • [How to Play](#-how-to-play) • [Levels](#-levels) • [Roadmap](#-roadmap)

</div>

---

## 📖 About

**Memory Leak Hunter** is an educational terminal game designed for aspiring reverse engineers and C/C++ developers. You play as a reverse engineer analyzing suspicious code — your mission is to find every memory leak hidden within.

Inspired by real-world patterns found in malware analysis, this game teaches you to spot the subtle bugs that even experienced developers miss.

> 💡 *"In reverse engineering, a forgotten `free()` isn't just a bug — it's a clue."*

---

## ✨ Features

- 🧠 **10 hand-crafted levels** covering all major memory leak patterns
- 🎨 **Colorful terminal UI** with ASCII art
- 💾 **Progress saving** — your XP and coins persist between sessions
- 💡 **Hint system** — stuck? Use a hint (but it costs XP!)
- 🏆 **Scoring system** — earn XP and coins for every leak you find
- 🔄 **Replayable** — jump back to any level anytime
- 🌐 **Cross-platform** — works on Windows, Linux, and macOS
- 📚 **Educational** — each level teaches a real-world memory bug pattern

---


---

## 🚀 Installation

### Prerequisites

- A C++17 compatible compiler (GCC, Clang, or MSVC)
- `make` (optional, for Linux/macOS)

### Build from source

**Linux / macOS:**
```bash
git clone https://github.com/YOUR_USERNAME/memory-leak-hunter.git
cd memory-leak-hunter
make
./memory-leak-hunter
```

Windows (MinGW):
```bash
git clone https://github.com/YOUR_USERNAME/memory-leak-hunter.git
cd memory-leak-hunter
g++ -std=c++17 main.cpp -o memory-leak-hunter.exe
memory-leak-hunter.exe
```
Windows (MSVC):
```bash
git clone https://github.com/YOUR_USERNAME/memory-leak-hunter.git
cd memory-leak-hunter
cl /EHsc /std:c++17 main.cpp
memory-leak-hunter.exe
```

🎮 How to Play

    Run the game and select "Start game" from the menu
    You'll be shown a piece of C/C++ code with numbered lines
    Analyze the code and find lines where memory leaks occur
    Enter the line numbers separated by commas (e.g., 2, 4, 7)
    Use hint if you're stuck, skip to see the answer, or quit to exit

Example
1: void process() {
2:     char *buffer = malloc(100);
3:     use_buffer(buffer);
4: }

Your answer: 2 — because malloc on line 2 has no matching free.
