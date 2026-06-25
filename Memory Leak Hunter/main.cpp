#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <cstdlib>
#include "levels.h"

struct Scores {
    int xp = 0;
    int coins = 0;
    std::vector<int> completed_levels;
    int hints_used = 0;
};

const std::string SCORES_FILE = "scores.json";

Scores load_scores() {
    Scores scores;
    std::ifstream file(SCORES_FILE);
    if (!file.is_open()) return scores;

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("\"xp\"") != std::string::npos) {
            auto pos = line.find(":");
            if (pos != std::string::npos)
                scores.xp = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"coins\"") != std::string::npos) {
            auto pos = line.find(":");
            if (pos != std::string::npos)
                scores.coins = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"hints_used\"") != std::string::npos) {
            auto pos = line.find(":");
            if (pos != std::string::npos)
                scores.hints_used = std::stoi(line.substr(pos + 1));
        }
    }

    file.clear();
    file.seekg(0);
    std::stringstream buf;
    buf << file.rdbuf();
    std::string content = buf.str();

    auto start = content.find("\"completed_levels\"");
    if (start != std::string::npos) {
        start = content.find("[", start);
        auto end = content.find("]", start);
        if (start != std::string::npos && end != std::string::npos) {
            std::string levels_str = content.substr(start + 1, end - start - 1);
            std::stringstream ss(levels_str);
            std::string item;
            while (std::getline(ss, item, ',')) {
                item.erase(std::remove(item.begin(), item.end(), ' '), item.end());
                if (!item.empty()) {
                    try { scores.completed_levels.push_back(std::stoi(item)); }
                    catch (...) {}
                }
            }
        }
    }
    return scores;
}

void save_scores(const Scores& scores) {
    std::ofstream file(SCORES_FILE);
    file << "{\n";
    file << "  \"xp\": " << scores.xp << ",\n";
    file << "  \"coins\": " << scores.coins << ",\n";
    file << "  \"hints_used\": " << scores.hints_used << ",\n";
    file << "  \"completed_levels\": [";
    for (size_t i = 0; i < scores.completed_levels.size(); ++i) {
        file << scores.completed_levels[i];
        if (i + 1 < scores.completed_levels.size()) file << ", ";
    }
    file << "]\n}\n";
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void print_header() {
    std::cout << "\n";
    std::cout << "+-----------------------------------------------------------+\n";
    std::cout << "|                                                           |\n";
    std::cout << "|           [*]  MEMORY LEAK HUNTER  v1.0  [*]              |\n";
    std::cout << "|                                                           |\n";
    std::cout << "|      Find all memory leaks in suspicious code!            |\n";
    std::cout << "|                                                           |\n";
    std::cout << "+-----------------------------------------------------------+\n";
    std::cout << "\n";
}

void print_stats(const Scores& scores) {
    std::cout << "[*] Stats:\n";
    std::cout << "    XP: " << scores.xp << "\n";
    std::cout << "    Coins: " << scores.coins << "\n";
    std::cout << "    Completed: " << scores.completed_levels.size()
              << "/" << LEVELS.size() << "\n";
    std::cout << "    Hints used: " << scores.hints_used << "\n";
    std::cout << "\n";
}

void print_level(const Level& level) {
    std::cout << "============================================================\n";
    std::cout << " Level " << level.id << ": " << level.title << "\n";
    std::cout << "============================================================\n";
    std::cout << "\n";
    std::cout << "[*] Task: " << level.description << "\n";
    std::cout << "\n";
    std::cout << "[*] Code:\n";
    std::cout << "------------------------------------------------------------\n";

    std::istringstream stream(level.code);
    std::string line;
    int num = 1;
    while (std::getline(stream, line)) {
        if (num < 10) std::cout << " ";
        std::cout << num << " | " << line << "\n";
        num++;
    }

    std::cout << "------------------------------------------------------------\n";
    std::cout << "\n";
}

// Special return codes
const int CMD_HINT = -1;
const int CMD_SKIP = -2;
const int CMD_QUIT = -3;

std::vector<int> get_user_answer() {
    std::cout << "[*] Enter line numbers with leaks (comma separated)\n";
    std::cout << "    Example: 3, 7, 12\n";
    std::cout << "    Commands: hint, skip, quit\n";
    std::cout << "\n";

    while (true) {
        std::cout << "Your answer: ";
        std::string input;
        std::getline(std::cin, input);

        // Trim spaces
        input.erase(std::remove(input.begin(), input.end(), ' '), input.end());

        // To lowercase
        std::string lower_input = input;
        std::transform(lower_input.begin(), lower_input.end(), lower_input.begin(), ::tolower);

        if (lower_input == "hint") return {CMD_HINT};
        if (lower_input == "skip") return {CMD_SKIP};
        if (lower_input == "quit") return {CMD_QUIT};

        try {
            std::vector<int> lines;
            std::stringstream ss(input);
            std::string item;

            while (std::getline(ss, item, ',')) {
                if (item.empty()) continue;
                int num = std::stoi(item);
                if (num < 1 || num > 100) {
                    std::cout << "[!] Lines must be 1-100\n";
                    lines.clear();
                    break;
                }
                lines.push_back(num);
            }

            if (!lines.empty()) {
                std::sort(lines.begin(), lines.end());
                return lines;
            }
        } catch (...) {
            std::cout << "[!] Enter numbers separated by commas, or a command\n";
        }
    }
}

bool is_level_completed(const Scores& scores, int level_id) {
    return std::find(scores.completed_levels.begin(),
                     scores.completed_levels.end(),
                     level_id) != scores.completed_levels.end();
}

bool play_level(const Level& level, Scores& scores) {
    if (is_level_completed(scores, level.id)) {
        std::cout << "[+] Level " << level.id << " already completed!\n";
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return true;
    }

    clear_screen();
    print_header();
    print_stats(scores);
    print_level(level);

    while (true) {
        std::vector<int> answer = get_user_answer();

        if (answer[0] == CMD_HINT) {
            std::cout << "\n";
            std::cout << "[HINT] " << level.hint << "\n";
            std::cout << "       (-10 XP)\n";
            scores.xp -= 10;
            scores.hints_used++;
            std::cout << "\n";
            continue;
        }

        if (answer[0] == CMD_SKIP) {
            std::cout << "\n[SKIP] Level skipped.\n";
            std::cout << "       Correct answer: ";
            for (size_t i = 0; i < level.correct_lines.size(); ++i) {
                std::cout << level.correct_lines[i];
                if (i + 1 < level.correct_lines.size()) std::cout << ", ";
            }
            std::cout << "\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            return true;
        }

        if (answer[0] == CMD_QUIT) {
            return false; // signal quit
        }

        // Check answer
        std::set<int> user_set(answer.begin(), answer.end());
        std::set<int> correct_set(level.correct_lines.begin(), level.correct_lines.end());

        if (user_set == correct_set) {
            std::cout << "\n";
            std::cout << "[SUCCESS] All leaks found!\n";
            std::cout << "          +30 XP, +50 Coins\n";
            scores.xp += 30;
            scores.coins += 50;
            scores.completed_levels.push_back(level.id);
            std::sort(scores.completed_levels.begin(), scores.completed_levels.end());
            save_scores(scores);
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            return true;
        } else {
            std::cout << "\n[FAIL] Not quite...\n";

            std::set<int> missing, extra;
            std::set_difference(correct_set.begin(), correct_set.end(),
                               user_set.begin(), user_set.end(),
                               std::inserter(missing, missing.begin()));
            std::set_difference(user_set.begin(), user_set.end(),
                               correct_set.begin(), correct_set.end(),
                               std::inserter(extra, extra.begin()));

            if (!extra.empty()) {
                std::cout << "       You marked extra lines.\n";
            }
            if (!missing.empty()) {
                std::cout << "       You missed some leaks.\n";
            }
            std::cout << "       Try again or type 'hint'\n\n";
        }
    }
}

void show_menu() {
    std::cout << "[*] Menu:\n";
    std::cout << "    1. Start game\n";
    std::cout << "    2. Select level\n";
    std::cout << "    3. Reset progress\n";
    std::cout << "    4. Quit\n";
    std::cout << "\n";
}

int main() {
    Scores scores = load_scores();

    while (true) {
        clear_screen();
        print_header();
        print_stats(scores);
        show_menu();

        std::cout << "Choose option: ";
        std::string choice;
        std::getline(std::cin, choice);

        // Trim
        choice.erase(std::remove(choice.begin(), choice.end(), ' '), choice.end());

        if (choice == "1") {
            int start = 0;
            for (size_t i = 0; i < LEVELS.size(); ++i) {
                if (!is_level_completed(scores, LEVELS[i].id)) {
                    start = i;
                    break;
                }
            }

            bool quit = false;
            for (size_t i = start; i < LEVELS.size(); ++i) {
                if (!play_level(LEVELS[i], scores)) {
                    quit = true;
                    break;
                }
            }

            if (!quit) {
                std::cout << "\n[CONGRATS] You completed all levels!\n";
                std::cout << "Press Enter to return to menu...";
                std::cin.get();
            }

        } else if (choice == "2") {
            std::cout << "\n[*] Available levels:\n";
            for (const auto& level : LEVELS) {
                bool done = is_level_completed(scores, level.id);
                std::cout << "    " << level.id << ". " << level.title;
                if (done) std::cout << " [DONE]";
                std::cout << "\n";
            }
            std::cout << "\nEnter level number (0 to cancel): ";
            std::string lvl_str;
            std::getline(std::cin, lvl_str);

            try {
                int num = std::stoi(lvl_str);
                if (num == 0) continue;

                auto it = std::find_if(LEVELS.begin(), LEVELS.end(),
                    [num](const Level& l) { return l.id == num; });

                if (it != LEVELS.end()) {
                    play_level(*it, scores);
                } else {
                    std::cout << "[!] Level not found!\n";
                    std::cout << "Press Enter to continue...";
                    std::cin.get();
                }
            } catch (...) {
                std::cout << "[!] Invalid input!\n";
                std::cout << "Press Enter to continue...";
                std::cin.get();
            }

        } else if (choice == "3") {
            std::cout << "\n[!] Are you sure? (y/n): ";
            std::string confirm;
            std::getline(std::cin, confirm);

            if (confirm == "y" || confirm == "Y") {
                scores = Scores();
                save_scores(scores);
                std::cout << "[+] Progress reset!\n";
                std::cout << "Press Enter to continue...";
                std::cin.get();
            }

        } else if (choice == "4") {
            std::cout << "\n[*] Goodbye!\n";
            break;

        } else {
            std::cout << "[!] Invalid option!\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
        }
    }

    return 0;
}