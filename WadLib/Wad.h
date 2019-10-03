#ifndef WAD_H_
#define WAD_H_

#include "Lump.h"

enum WadLoadResultType
{
   WadLoadOK,
    WadLoadBadArg,
   WadLoadCannotOpenErrno,
   WadLoadBadType,
   WadLoadBadSizes,
    WadLoadBadLumpIndex,
};

struct WadLoadResult
{
    WadLoadResultType type;
    int index;

    bool failed() const
    {
        return type != WadLoadOK;
    }

    std::string message() const;
};

enum WadType
{
   WadUndefined,
   WadInternal,
   WadPatch,
};

//
// DOOM WAD
//
class Wad
{
   WadType  mType = WadUndefined;
    std::vector<Lump> mLumps;
public:
   WadLoadResult LoadFile(const char* filename);
    const std::vector<Lump> &lumps() const
    {
        return mLumps;
    }
};

#endif
