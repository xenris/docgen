#include "args.hpp"

bool Args::parse(int argc, char** argv) {
    if(argc <= 1) {
        std::cout << helpText;

        return false;
    }

    for(int i = 1; i < argc; i++) {
        const char* arg = argv[i];

        if(arg[0] == '-') {
            if(strequ(arg, "-c")) {
                i++;

                if(i >= argc) {
                    std::cerr << "Error: No comment style specified after \"-c\". (Try using double quotes \"\".)" << std::endl;

                    return false;
                }

                commentStart = argv[i];
            } else if(strequ(arg, "-h")) {
                std::cout << helpText;

                return true;
            } else {
                std::cerr << "Error: Unrecognised command \"" << arg << "\". Try \"-h\"." << std::endl;

                return false;
            }
        } else {
            paths.push_back(arg);
        }
    }

    return true;
}

bool Args::strequ(const char*const a, const char*const b) {
    int i = 0;

    while(true) {
        if(a[i] != b[i]) {
            return false;
        }

        if(a[i] == '\0') {
            return true;
        }

        i++;
    }
}
