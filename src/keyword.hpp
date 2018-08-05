#ifndef KEYWORD_HPP
#define KEYWORD_HPP

#include <algorithm>
#include <string>

/// # class Keyword
/// Class to temporarily store information about keywords.
class Keyword {
    // TODO Save an unformatted version of filePath and heading for error messages.
    std::string _filePath;
    std::string _heading;
    int _lineNumber;

public:

    Keyword();

    Keyword(const std::string& filePath, const std::string& heading, int lineNumber);

    std::string filePath();

    const std::string& filePath() const;

    void filePath(const std::string& s);

    std::string heading();

    const std::string& heading() const;

    void heading(const std::string& s);

    int lineNumber() const;

    void lineNumber(int n);
};

#endif
