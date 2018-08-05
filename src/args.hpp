#ifndef ARGS_HPP
#define ARGS_HPP

#include <iostream>
#include <string>
#include <vector>

#include "help.hpp"

/// # class {{Args}}
/// Helper class which parses command line arguments and turns them into useful values.
struct Args {
    /// ### std::string commentStart
    /// String which indicates the start of a line of documentation.<br>
    /// Defaults to "///".
    std::string commentStart = "///";

    /// ### std::vector<std::string> paths
    /// List of input files to generate documentation from.
    std::vector<std::string> paths;

    /// ### bool {{Args::parse}}(int argc, char\*\* argv)
    /// Reads the given arguments, sets appropriate member variables,
    /// and returns true if all arguments were valid.
    bool parse(int argc, char** argv);

    /// ### bool {{Args::strequ}}(const char\* a, const char\* b)
    /// Compares two char arrays and returns true if they are the same.<br>
    /// Faster than "strcmp(a, b) == 0", because it doesn't always need to read the entire array.<br>
    /// Also easier to understand than "strcmp(a, b) == 0".
    bool strequ(const char*const a, const char*const b);
};

#endif
