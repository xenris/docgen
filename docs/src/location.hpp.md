# class Location
Class to temporarily store location information for keywords.<br>
Also formats the location information to be used in markdown.

### Location()

### Location(std::string filePath, std::string heading, int lineNumber)

### std::string filePath()
Get the correctly formatted file path for markdown use.

### void filePath(std::string)
Set the file path.<br>
The given path will be formatted.

### std::string heading()
Get the correctly formatted heading for markdown use.

### void heading(std::string)
Set the heading.<br>
The given heading will be formatted.

### int lineNumber()
Get the line number.

### void lineNumber(int n)
Set the line number.

