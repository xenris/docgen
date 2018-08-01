#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string.h>
#include <sys/stat.h>
#include <vector>

/// # Docgen, the simple and flexible (non-invasive?) documentation generator. {{lakjflfjka}}

/// ## class Main
struct Main {
    class Location {
        // TODO Save an unformatted version of filePath and heading for error messages.
        std::string _filePath;
        std::string _heading;
        int _lineNumber;

    public:

        Location() : _filePath("???"), _heading("???"), _lineNumber(0) {
        }

        Location(const std::string& filePath, const std::string& heading, int lineNumber) {
            this->filePath(filePath);
            this->heading(heading);
            this->lineNumber(lineNumber);
        }

        std::string filePath() {
            return _filePath;
        }

        const std::string& filePath() const {
            return _filePath;
        }

        void filePath(const std::string& s) {
            _filePath = s;
            int i = 0;

            while(true) {
                // TODO Check if symbols need to be removed.
                i = _filePath.find_first_of(' ', i);

                if(i == -1) {
                    break;
                }

                _filePath.erase(i, 1);
                _filePath.insert(i, "%20");
            }
        }

        std::string heading() {
            return _heading;
        }

        const std::string& heading() const {
            return _heading;
        }

        void heading(const std::string& s) {
            _heading = s;

            const auto isSymbol = [](char c) -> bool {
                if((c >= 'a') && (c <= 'z')) {
                    return false;
                } else if((c >= 'A') && (c <= 'Z')) {
                    return false;
                } else if((c >= '0') && (c <= '9')) {
                    return false;
                } else if(c== '-') {
                    return false;
                } else {
                    return true;
                }
            };

            std::replace_if(_heading.begin(), _heading.end(), [](char c) { return c == ' '; }, '-');

            _heading.erase(std::remove_if(_heading.begin(), _heading.end(), isSymbol), _heading.end());

            std::transform(_heading.begin(), _heading.end(), _heading.begin(), ::tolower);
        }

        int lineNumber() const {
            return _lineNumber;
        }

        void lineNumber(int n) {
            _lineNumber = n;
        }
    };

    std::vector<std::string> commentStyles;

    const std::string keywordDeclerationStart = "{{";
    const std::string keywordDeclerationEnd = "}}";

    const std::string keywordReferenceStart = "[[";
    const std::string keywordReferenceEnd = "]]";


    Main() {
        commentStyles.push_back("///");
        commentStyles.push_back("###");
    }

    int main(int argc, char** args) {
        std::vector<std::string> paths;

        for(int i = 1; i < argc; i++) {
            paths.push_back(args[i]);
        }

        std::map<std::string, Location> keywords;

        if(!gatherKeywords(paths, &keywords)) {
            return -1;
        }

        if(!generateDocuments(paths, keywords)) {
            return -1;
        }

        return 0;
    }

    bool gatherKeywords(const std::vector<std::string>& paths, std::map<std::string, Location>* const keywords) {
        for(std::string path : paths) {
            std::fstream file(path, std::fstream::in);

            if(!file) {
                std::cerr << "Error: " << path << " - " << strerror(errno) << std::endl;

                return false;
            }

            std::string line;
            int lineNumber = 1;

            while(getLine(&file, &line)) {
                int start = 0;

                while(true) {
                    start = line.find(keywordDeclerationStart, start);

                    if(start == -1) {
                        break;
                    }

                    start += keywordDeclerationStart.size();

                    const int end = line.find(keywordDeclerationEnd, start);

                    if(end == -1) {
                        break;
                    }

                    const std::string key = line.substr(start, end - start);
                    const std::string heading = getMarkdownContents(line);

                    if(keywords->count(key) == 0) {
                        (*keywords)[key] = Location(path, heading, lineNumber);
                    } else {
                        const Location& original = (*keywords)[key];

                        std::cerr << path << ":" << lineNumber << " warning: keyword \"" << key << "\" redeclared." << std::endl;
                        std::cerr << "  Originally declared here: " << original.filePath() << ":" << original.lineNumber() << ": \"" << original.heading() << "\"" << std::endl;
                    }

                    start = end + keywordDeclerationEnd.size();
                }

                lineNumber += 1;
            }
        }

        return true;
    }

    bool generateDocuments(const std::vector<std::string>& paths, const std::map<std::string, Location>& keywords) {
        for(const std::string path : paths) {

            std::fstream file(path, std::fstream::in);

            if(!file) {
                std::cerr << "Error: " << path << " - " << strerror(errno) << std::endl;

                return false;
            }

            const std::string outPath = "docs/" + path + ".md";

            if(!makeDirectory(outPath)) {
                return false;
            }

            std::ofstream outFile(outPath);

            if(!outFile) {
                std::cerr << "Error: " << outPath << " - " << "Failed to create file" << std::endl;

                return false;
            }

            std::string line;
            bool blankAllowed = false;
            int lineNumber = 1;

            while(getLine(&file, &line)) {
                int start = 0;

                while(true) {
                    start = line.find(keywordReferenceStart, start);

                    if(start == -1) {
                        break;
                    }

                    line.erase(start, keywordReferenceStart.size());

                    const int end = line.find(keywordReferenceEnd, start);

                    if(end == -1) {
                        break;
                    }

                    line.erase(end, keywordReferenceEnd.size());

                    const std::string key = line.substr(start, end - start);

                    if(keywords.count(key) != 0) {
                        const Location& location = keywords.at(key);

                        // TODO Make path relative to current file.
                        const std::string linkPath = (location.filePath() == path) ? "" : location.filePath();

                        const std::string linkStart = "[";
                        const std::string linkEnd = "](" + linkPath + "#" + location.heading() + ")";

                        line.insert(start, linkStart);
                        line.insert(end + 1, linkEnd);

                        start = end + linkStart.size() + linkEnd.size();
                    } else {
                        std::cerr << path << ":" << lineNumber << " warning: keyword \"" << key << "\" does not exist." << std::endl;

                        start = end + keywordReferenceEnd.size();
                    }
                }

                start = 0;

                while(true) {
                    start = line.find(keywordDeclerationStart, start);

                    if(start == -1) {
                        break;
                    }

                    line.erase(start, keywordDeclerationStart.size());

                    const int end = line.find(keywordDeclerationEnd, start);

                    if(end == -1) {
                        break;
                    }

                    line.erase(end, keywordDeclerationEnd.size());

                    start = end;
                }

                if((line == "") && blankAllowed) {
                    outFile << line << std::endl;

                    blankAllowed = false;
                } else if(line != "") {
                    outFile << line << std::endl;

                    blankAllowed = true;
                }

                lineNumber += 1;
            }
        }

        return true;
    }

    /// ### bool getLine(std::fstream\* file, std::string\* line)
    /// Gets the next line with documentation text only.
    bool getLine(std::fstream* const file, std::string* const line) {
        if(!std::getline(*file, *line)) {
            return false;
        }

        const int i = line->find_first_not_of(' ');

        *line = line->substr(std::max(0, i));

        int docLineStart = 0;

        for(std::string s : commentStyles) {
            docLineStart = line->find(s);

            if(docLineStart != -1) {
                break;
            }
        }

        if(docLineStart == 0) {
            if(line->size() == 3) {
                *line = "";
            } else {
                *line = line->substr(4);
            }
        } else {
            *line = "";
        }

        return true;
    }

    /// ### bool makeDirectory(std::string path)
    /// Generates directories.<br>
    /// Assumes the last element is a file if there is no trailing "/" and
    /// ignores it. e.g. "a/b/c/" will generate three directories,
    /// and "a/b/c" will only generate two.
    bool makeDirectory(const std::string& path) {
        const int i = path.find_last_of('/');

        if(i == -1) {
            return true;
        }

        const std::string directoryPath = path.substr(0, i);

        const std::string command = "mkdir -p " + directoryPath;

        const int error = system(command.c_str());

        if(error == -1) {
            std::cerr << strerror(errno) << std::endl;
        }

        return error != -1;
    }

    // /// ### std::string getElementType(std::string line)
    // /// Gets the first few relevant characters, e.g. "#", "##", "*", "  *". Or "" if it is a plain paragraph.
    // std::string getElementType(const std::string& line) {
    //     int i = line.find_first_not_of(' ');

    //     if(i == -1) {
    //         return "";
    //     }

    //     i = line.find_first_of(' ', i);

    //     if(i == -1) {
    //         return "";
    //     }

    //     const std::string result = line.substr(0, i);

    //     const std::string result = line.substr(result.find_first_not_of(' '));

    //     // std::cout << "\"" << line << "\" " << "\"" << result << "\"" << std::endl;
    //     // std::cout << "\"" << result << "\"" << std::endl;

    //     if(result == "#") {
    //         return result;
    //     } else if(result == "*") {
    //     }

    //     return result;
    // }

    std::string getMarkdownContents(const std::string& line) {
        std::string result = line;

        int i = result.find_first_not_of(' ');

        if(i == -1) {
            return result;
        }

        i = result.find_first_of(' ', i);

        if(i == -1) {
            return result;
        }

        i = result.find_first_not_of(' ', i);

        if(i == -1) {
            return result;
        }

        result = result.substr(i);

        return result;
    }

    std::string getMarkdownType(const std::string& line) {
        std::string result = line;

        int i = result.find_first_not_of(' ');

        if(i == -1) {
            return result;
        }

        i = result.find_first_of(' ', i);

        if(i == -1) {
            return result;
        }

        result = result.substr(0, i);

        return result;
    }
};

/// ### int {{main}}(int argc, char\*\* args)
/// Main function.
int main(int argc, char** args) {
    return Main().main(argc, args);
}
