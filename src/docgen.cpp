#include "docgen.hpp"

DocGen::DocGen(const std::string& commentStart, const std::string& outputDir) : commentStart(commentStart), outputDir(sanitiseDirectory(outputDir)) {
}

bool DocGen::generate(const std::vector<std::string>& paths) {
    std::map<std::string, Location> keywords;

    if(!gatherKeywords(paths, &keywords)) {
        return false;
    }

    if(!generateDocuments(paths, keywords)) {
        return false;
    }

    return true;
}

bool DocGen::gatherKeywords(const std::vector<std::string>& paths, std::map<std::string, Location>* const keywords) {
    for(std::string path : paths) {
        if(isDirectory(path)) {
            continue;
        }

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

bool DocGen::generateDocuments(const std::vector<std::string>& paths, const std::map<std::string, Location>& keywords) {
    for(const std::string path : paths) {
        if(isDirectory(path)) {
            continue;
        }

        std::fstream file(path, std::fstream::in);

        if(!file) {
            std::cerr << "Error: " << path << " - " << strerror(errno) << std::endl;

            return false;
        }

        const std::string outPath = outputDir + path + ".md";

        if(!makeDirectory(outPath)) {
            return false;
        }

        std::ofstream outFile(outPath);

        if(!outFile) {
            std::cerr << "Error: " << outPath << " - " << "Failed to create file" << std::endl;

            return false;
        }

        std::string line;
        bool blankLineRequired = false;
        bool hasMarkdown = false;
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

                    const std::string linkPath = relativePath(location.filePath(), path);

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

            if(line != "") {
                hasMarkdown = true;

                if(blankLineRequired) {
                    outFile << std::endl;

                    blankLineRequired = false;
                }

                outFile << line << std::endl;
            } else if(hasMarkdown) {
                blankLineRequired = true;
            }

            lineNumber += 1;
        }

        if(!hasMarkdown) {
            outFile.close();

            deleteFile(outPath);
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

bool DocGen::isDirectory(const std::string& path) {
    return path.back() == '/';
}

bool DocGen::deleteFile(const std::string& filePath) {
    const std::string command = "rm " + filePath;

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

std::string DocGen::sanitiseDirectory(const std::string& dir) {
    if(*dir.end() == '/') {
        return dir;
    } else {
        return dir + "/";
    }
}

std::string DocGen::relativePath(std::string to, std::string from) {
    int i = 0;

    while(true) {
        if(to[i] == from[i]) {
            if(to[i] == '/') {
                to = to.substr(i + 1);
                from = from.substr(i + 1);

                i = 0;
            } else if(to[i] == '\0') {
                to = "";
                from = "";

                break;
            } else {
                i++;
            }
        } else {
            break;
        }
    }

    for(const char c : from) {
        if(c == '/') {
            to = "../" + to;
        }
    }

    return to;
}
