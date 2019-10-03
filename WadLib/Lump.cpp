#include "stdafx.h"
#include "Lump.h"

//
// Loads a lump from a stream, given an address and size. Shall keep the same position on exit.
//
bool Lump::load(FILE *f, const char *name, int address, int size)
{
    long curpos = ftell(f);
    if(curpos == -1)
        return false;

    if(fseek(f, address, SEEK_SET) == -1)
        return false;

    mData.resize(size);
    if(fread(mData.data(), 1, size, f) < size)
    {
        fseek(f, curpos, SEEK_SET);
        return false;
    }

    mName = name;
    return fseek(f, curpos, SEEK_SET) == 0;
}
