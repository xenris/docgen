#ifndef DOCGEN_HPP
#define DOCGEN_HPP

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <string.h>
#include <vector>

#include "location.hpp"

/// # class DocGen
class DocGen {
    const std::string commentStart;

    const std::string keywordDeclerationStart = "{{";
    const std::string keywordDeclerationEnd = "}}";

    const std::string keywordReferenceStart = "[[";
    const std::string keywordReferenceEnd = "]]";

public:

    /// ### DocGen(std::string commentStart)
    DocGen(const std::string& commentStart);

    int generate(int argc, char** args);

private:

    bool gatherKeywords(const std::vector<std::string>& paths, std::map<std::string, Location>* const keywords);

    bool generateDocuments(const std::vector<std::string>& paths, const std::map<std::string, Location>& keywords);

    /// ### bool getLine(std::fstream\* file, std::string\* line)
    /// Gets the next line with documentation text only.
    bool getLine(std::fstream* const file, std::string* const line);

    /// ### bool makeDirectory(std::string path)
    /// Generates directories.<br>
    /// Assumes the last element is a file if there is no trailing "/" and
    /// ignores it. e.g. "a/b/c/" will generate three directories,
    /// and "a/b/c" will only generate two.
    /// [[lakjflfjka]]
    bool makeDirectory(const std::string& path);

    std::string getMarkdownContents(const std::string& line);

    /// ### std::string getMarkdownType(std::string line)
    /// Gets the first few relevant characters of a documentation line,
    /// e.g. "#", "##", "\*", "  \*", or "" if it is a plain paragraph.
    std::string getMarkdownType(const std::string& line);
};

#endif
