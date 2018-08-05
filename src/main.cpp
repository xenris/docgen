#include "main.hpp"

int main(int argc, char** argv) {
    Args args;

    if(!args.parse(argc, argv)) {
        return -1;
    }

    if(!DocGen(args.commentStart).generate(args.paths)) {
        return -1;
    }

    return 0;
}
