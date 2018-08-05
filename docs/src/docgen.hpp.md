# class DocGen

### DocGen(std::string commentStart)

### bool getLine(std::fstream\* file, std::string\* line)
Gets the next line with documentation text only.

### bool makeDirectory(std::string path)
Generates directories.<br>
Assumes the last element is a file if there is no trailing "/" and
ignores it. e.g. "a/b/c/" will generate three directories,
and "a/b/c" will only generate two.
[lakjflfjka](src/main.hpp#docgen-the-simple-and-flexible-non-invasive-documentation-generator-lakjflfjka)

### std::string getMarkdownType(std::string line)
Gets the first few relevant characters of a documentation line,
e.g. "#", "##", "\*", "  \*", or "" if it is a plain paragraph.

