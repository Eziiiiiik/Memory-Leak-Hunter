#ifndef LEVELS_H
#define LEVELS_H

#include <string>
#include <vector>

struct Level {
    int id;
    std::string title;
    std::string description;
    std::string code;
    std::vector<int> correct_lines;
    std::string hint;
};

const std::vector<Level> LEVELS = {
    {
        1,
        "Simple Leak",
        "Find where memory was allocated but not freed.",
        "void process() {\n"
        "    char *buffer = malloc(100);\n"
        "    use_buffer(buffer);\n"
        "}",
        {2},
        "Look for malloc without matching free"
    },
    {
        2,
        "Double Leak",
        "There are two memory leaks here. Find both!",
        "void process() {\n"
        "    char *buf1 = malloc(100);\n"
        "    char *buf2 = malloc(200);\n"
        "    use_buffers(buf1, buf2);\n"
        "}",
        {2, 3},
        "Two mallocs, zero frees"
    },
    {
        3,
        "Conditional Leak",
        "Leak only happens under a certain condition.",
        "void process(int mode) {\n"
        "    char *buffer = malloc(100);\n"
        "    if (mode == 1) {\n"
        "        return;\n"
        "    }\n"
        "    free(buffer);\n"
        "}",
        {2, 4},
        "What if mode == 1? buffer is never freed"
    },
    {
        4,
        "Loop Leak",
        "Critical leak - memory leaks on every iteration!",
        "void process(int count) {\n"
        "    for (int i = 0; i < count; i++) {\n"
        "        char *buffer = malloc(100);\n"
        "        use_buffer(buffer);\n"
        "    }\n"
        "}",
        {3},
        "malloc inside loop without free = critical leak!"
    },
    {
        5,
        "Pointer Overwrite",
        "The most sneaky leak - the pointer gets overwritten.",
        "void process() {\n"
        "    char *buffer = malloc(100);\n"
        "    buffer = malloc(200);\n"
        "    free(buffer);\n"
        "}",
        {2, 3},
        "First malloc is lost when buffer is overwritten"
    },
    {
        6,
        "Early Return",
        "Function exits before freeing memory.",
        "void process(char *data) {\n"
        "    char *buffer = malloc(100);\n"
        "    if (!data) {\n"
        "        return;\n"
        "    }\n"
        "    use_buffer(buffer);\n"
        "    free(buffer);\n"
        "}",
        {2, 4},
        "If data == NULL, buffer leaks"
    },
    {
        7,
        "Realloc Leak",
        "realloc can also cause leaks!",
        "void process() {\n"
        "    char *buffer = malloc(100);\n"
        "    buffer = realloc(buffer, 200);\n"
        "    use_buffer(buffer);\n"
        "}",
        {2, 3},
        "If realloc fails, original pointer is lost"
    },
    {
        8,
        "Conditional Free",
        "Memory is only freed under a certain condition.",
        "void process(int flag) {\n"
        "    char *buffer = malloc(100);\n"
        "    if (flag) {\n"
        "        free(buffer);\n"
        "    }\n"
        "}",
        {2},
        "What if flag == 0? buffer leaks"
    },
    {
        9,
        "Loop with Conditional Free",
        "Leak only on certain iterations.",
        "void process(int count) {\n"
        "    for (int i = 0; i < count; i++) {\n"
        "        char *buffer = malloc(100);\n"
        "        if (i % 2 == 0) {\n"
        "            free(buffer);\n"
        "        }\n"
        "    }\n"
        "}",
        {3},
        "Odd iterations never free buffer"
    },
    {
        10,
        "Double Free",
        "Not a leak, but a dangerous bug! Find it.",
        "void process() {\n"
        "    char *buffer = malloc(100);\n"
        "    free(buffer);\n"
        "    free(buffer);\n"
        "}",
        {4},
        "Two frees on one malloc = double free!"
    }
};

#endif