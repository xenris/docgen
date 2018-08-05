#include "help.hpp"

const char* helpText =
    "-c [comment style] -> sets the style of comment which leads documentation, e.g. \"///\" (default), \"###\", etc...\n"
    "-h -> Show this help text.\n"
    "-d -> Delete docs directory before generating new documents.\n"
    "-o [output directory] -> Set directory to put documents in.\n"
    "[file list] -> File paths to include.\n"
    "Example: docgen -c \"###\" main.hpp other.hpp\n"
    "Wildcard example: docgen $(find src/ -name '*.hpp')\n";
