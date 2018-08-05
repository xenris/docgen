#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <algorithm>
#include <string>

/// # class Location
class Location {
    // TODO Save an unformatted version of filePath and heading for error messages.
    std::string _filePath;
    std::string _heading;
    int _lineNumber;

public:

    Location();

    Location(const std::string& filePath, const std::string& heading, int lineNumber);

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
