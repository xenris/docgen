#include "main.hpp"

int main(int argc, char** argv) {
    Args args;

    if(!args.parse(argc, argv)) {
        return -1;
    }

    if(args.deleteDocs) {
        const std::string command = "rm -rf " + args.outputDir;

        const int error = system(command.c_str());

        if(error != 0) {
            std::cerr << strerror(errno) << std::endl;
        }
    }

    if(!DocGen(args.commentStart, args.outputDir).generate(args.paths)) {
        return -1;
    }

    return 0;
}
