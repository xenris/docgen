#include "docgen.hpp"

DocGen::DocGen(const std::string& commentStart) : commentStart(commentStart) {
}

bool DocGen::generate(const std::vector<std::string>& paths) {
    std::map<std::string, Keyword> keywords;

    if(!gatherKeywords(paths, &keywords)) {
        return false;
    }

    if(!generateDocuments(paths, keywords)) {
        return false;
    }

    return true;
}

bool DocGen::gatherKeywords(const std::vector<std::string>& paths, std::map<std::string, Keyword>* const keywords) {
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
                    (*keywords)[key] = Keyword(path, heading, lineNumber);
                } else {
                    const Keyword& original = (*keywords)[key];

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

bool DocGen::generateDocuments(const std::vector<std::string>& paths, const std::map<std::string, Keyword>& keywords) {
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
                    const Keyword& location = keywords.at(key);

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

bool DocGen::getLine(std::fstream* const file, std::string* const line) {
    if(!std::getline(*file, *line)) {
        return false;
    }

    const int i = line->find_first_not_of(' ');

    *line = line->substr(std::max(0, i));

    const int docLineStart = line->find(commentStart);

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

bool DocGen::makeDirectory(const std::string& path) {
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

std::string DocGen::getMarkdownContents(const std::string& line) {
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

std::string DocGen::getMarkdownType(const std::string& line) {
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
