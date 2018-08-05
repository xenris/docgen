#ifndef MAIN_HPP
#define MAIN_HPP

#include "args.hpp"
#include "docgen.hpp"

/// # docgen, a simple (both in use and in internal complexity) documentation generator.

/// ## {{Features}}
/// * Reads comments from source code and generates markdown documentation.
/// * Makes linking between parts of source code easy.
/// * Doesn't understand any language constructs so works equally well for all languages.

/// ### int {{main}}(int argc, char\*\* argv)
/// Entry point of program.<br>
/// Parses arguments and generates documentation.
int main(int argc, char** argv);

#endif
