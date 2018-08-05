#include "main.hpp"

int main(int argc, char** argv) {
    // TODO Use argv to determine comment style.
    return DocGen("///").generate(argc, argv);
}
