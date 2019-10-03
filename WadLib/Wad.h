#ifndef WAD_H_
#define WAD_H_

#include "Lump.h"

enum WadLoadResultType
{
   WadLoadOK,
   WadLoadCannotOpenErrno,
   WadLoadBadType,
   WadLoadBadSizes,
    WadLoadBadLumpIndex,
};

struct WadLoadResult
{
    WadLoadResultType type;
    int index;
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
};

#endif
