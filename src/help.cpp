#include "help.hpp"

const char* helpText =
    "-c [comment style] -> sets the style of comment which leads documentation, e.g. \"///\" (default) or \"###\"\n"
    "-h -> Show help\n"
    "[file list] -> File paths to include.\n"
    "e.g. docgen -c \"###\" main.hpp other.hpp\n";
