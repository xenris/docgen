# class DocGen
Class which does all the work of turning the given source files into documentation.

### DocGen(std::string commentStart)
* commentStart: The string docgen looks for at the start of each line to determine if it is a documentation line. (e.g. "///")

### bool generate(std::vector<std::string> paths)
Generates documentation from the given source files.
* paths: the list of source files to turn into documentation.

