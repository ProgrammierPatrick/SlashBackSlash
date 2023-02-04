#ifndef _FILE_LOC_H
#define _FILE_LOC_H

#include <string_view>

struct FileLoc {
    std::string_view filename;
    int line;
    int pos;
    bool fromLib;

    FileLoc(std::string_view filename, int line, int pos, bool fromLib)
        : filename(filename), line(line), pos(pos), fromLib(fromLib) { }
};

#endif