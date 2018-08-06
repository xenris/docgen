#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <algorithm>
#include <string>

/// # Location
/// Class to temporarily store location information for keywords.<br>
/// Also formats the location information to be used in markdown.
class Location {
    // TODO Save an unformatted version of filePath and heading for error messages.
    std::string _filePath;
    std::string _heading;
    int _lineNumber;

public:

    /// ### Location()
    Location();

    /// ### Location(std::string filePath, std::string heading, int lineNumber)
    Location(const std::string& filePath, const std::string& heading, int lineNumber);

    /// ### std::string filePath()
    /// Get the correctly formatted file path for markdown use.
    std::string filePath();

    const std::string& filePath() const;

    /// ### void filePath(std::string)
    /// Set the file path.<br>
    /// The given path will be formatted.
    void filePath(const std::string& s);

    /// ### std::string heading()
    /// Get the correctly formatted heading for markdown use.
    std::string heading();

    const std::string& heading() const;

    /// ### void heading(std::string)
    /// Set the heading.<br>
    /// The given heading will be formatted.
    void heading(const std::string& s);

    /// ### int lineNumber()
    /// Get the line number.
    int lineNumber() const;

    /// ### void lineNumber(int n)
    /// Set the line number.
    void lineNumber(int n);
};

#endif
