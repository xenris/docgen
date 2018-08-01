# Docgen, the simple and flexible (non-invasive?) documentation generator. lakjflfjka

## class Main

### bool getLine(std::fstream\* file, std::string\* line)
Gets the next line with documentation text only.

### bool makeDirectory(std::string path)
Generates directories.<br>
Assumes the last element is a file if there is no trailing "/" and
ignores it. e.g. "a/b/c/" will generate three directories,
and "a/b/c" will only generate two.

### int main(int argc, char\*\* args)
Main function.

