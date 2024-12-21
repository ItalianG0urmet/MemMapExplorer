#ifndef GUIMANAGER_H
#define GUIMANAGER_H
#include <string>
#include <unordered_map>


class GuiManager {
public:
    GuiManager(int *currentLine, int *maxLine, std::unordered_map<int, std::string> *strings);
    std::unordered_map<int, std::string> *strings;
    void run() const;
private:
    static void printLineNumber(int number);
    std::string getPathByLine(int line) const;
    static void printCurrentLineNumber(int number);
    void createBox() const;
    void loadLines() const;
    int *currentLine;
    int *maxLine;
};



#endif //GUIMANAGER_H
