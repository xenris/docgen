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

/// # class {{DocGen}}
/// Class which does all the work of turning the given source files into documentation.
class DocGen {
    // Start of a line of documentation.
    const std::string commentStart;

    // Directory where all the generated documents are placed.
    const std::string outputDir;

    // Wraps words to let docgen know that this is a keyword.
    const std::string keywordDeclerationStart = "{{";
    const std::string keywordDeclerationEnd = "}}";

    // Wraps words to make docgen generate a link to the equivalent keyword.
    const std::string keywordReferenceStart = "[[";
    const std::string keywordReferenceEnd = "]]";

public:

    /// ### DocGen(std::string commentStart, std::string outputDir)
    /// * commentStart: The string docgen looks for at the start of each line to determine if it is a documentation line. (e.g. "///")
    /// * outputDir: The directory to put all the generated documents.
    DocGen(const std::string& commentStart, const std::string& outputDir);

    /// ### bool generate(std::vector\<std::string\> paths)
    /// Generates documentation from the given source files.
    /// * paths: the list of source files to turn into documentation.
    bool generate(const std::vector<std::string>& paths);

private:

    bool gatherKeywords(const std::vector<std::string>& paths, std::map<std::string, Location>* const keywords);

    bool generateDocuments(const std::vector<std::string>& paths, const std::map<std::string, Location>& keywords);

    // Gets the next line and removes any irrelevant content.
    bool getLine(std::fstream* const file, std::string* const line);

    // Generates directories.
    // Assumes the last element is a file if there is no trailing "/" and
    // ignores it. e.g. "a/b/c/" will generate three directories,
    // and "a/b/c" will only generate two.
    bool makeDirectory(const std::string& path);

    bool isDirectory(const std::string& path);

    std::string getMarkdownContents(const std::string& line);

    // Gets the first few relevant characters of a documentation line,
    // e.g. "#", "##", "\*", "  \*", or "" if it is a plain paragraph.
    std::string getMarkdownType(const std::string& line);

    // Makes sure there is a "/" at the end of the string.
    std::string sanitiseDirectory(const std::string& dir);

    static std::string relativePath(std::string to, std::string from);
};

#endif
