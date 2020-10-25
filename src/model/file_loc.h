#ifndef _FILE_LOC_H
#define _FILE_LOC_H

#include <assert.h>
#include <iostream>

struct FileLoc {
    const std::shared_ptr<const std::string> filename;
    const int line;
    const int pos;
    const bool fromLib;

    FileLoc(const std::shared_ptr<const std::string>& filename, int line, int pos, bool fromLib)
        : filename(filename), line(line), pos(pos), fromLib(fromLib) {
            assert(filename != nullptr);
        }
};

#endif