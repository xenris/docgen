# class DocGen
Class which does all the work of turning the given source files into documentation.

### DocGen(std::string commentStart, std::string outputDir)
* commentStart: The string docgen looks for at the start of each line to determine if it is a documentation line. (e.g. "///")
* outputDir: The directory to put all the generated documents.

### bool generate(std::vector\<std::string\> paths)
Generates documentation from the given source files.
* paths: the list of source files to turn into documentation.
