# Docgen, a simple (both in use and in internal complexity) documentation generator.

## Features
* Reads comments from source code and generates markdown documentation.
* Makes linking between parts of documentation easy.
* Doesn't understand any language constructs so works equally well for all languages.

### int main(int argc, char\*\* argv)
Entry point of the program.<br>
Parses arguments and generates documentation, using [Args](args.hpp.md#args), and [DocGen](docgen.hpp.md#class-docgen).
