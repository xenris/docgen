#include "location.hpp"

Location::Location() : _filePath("???"), _heading("???"), _lineNumber(0) {
}

Location::Location(const std::string& filePath, const std::string& heading, int lineNumber) {
    this->filePath(filePath);
    this->heading(heading);
    this->lineNumber(lineNumber);
}

std::string Location::filePath() {
    return _filePath;
}

const std::string& Location::filePath() const {
    return _filePath;
}

void Location::filePath(const std::string& s) {
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

    _filePath += ".md";
}

std::string Location::heading() {
    return _heading;
}

const std::string& Location::heading() const {
    return _heading;
}

void Location::heading(const std::string& s) {
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

    std::transform(_heading.begin(), _heading.end(), _heading.begin(), tolower);
}

int Location::lineNumber() const {
    return _lineNumber;
}

void Location::lineNumber(int n) {
    _lineNumber = n;
}
