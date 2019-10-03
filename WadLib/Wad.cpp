#include "stdafx.h"
#include "Wad.h"

//
// Check the result and form a string
//
std::string WadLoadResult::message() const
{
    switch (type)
    {
        case WadLoadBadArg:
            return "Bad argument";
        case WadLoadBadLumpIndex:
            return std::string("Bad lump at index ") + std::to_string(index);
        case WadLoadBadSizes:
            return "Bad data block sizes";
        case WadLoadBadType:
            return "Bad WAD type in header (not IWAD or PWAD)";
        case WadLoadCannotOpenErrno:
            return std::string("Cannot open file, error ") + std::to_string(index);
        case WadLoadOK:
            return "Successful";
    }
}

WadLoadResult Wad::LoadFile(const char* filename)
{
    if(!filename)
        return { WadLoadBadArg };

    FILE* f = fopen(filename, "rb");

   if (!f)
       return { WadLoadCannotOpenErrno, errno };

    char sign_header[4] = {};

   fread(sign_header, 1, 4, f);
   if (!memcmp(sign_header, "PWAD", 4))
      mType = WadPatch;
   else if (!memcmp(sign_header, "IWAD", 4))
      mType = WadInternal;
   else
   {
       fclose(f);
       return { WadLoadBadType };
   }

   fseek(f, 0, SEEK_END);
   long filesize = ftell(f);
   fseek(f, 4, SEEK_SET);

   int numlumps, infotableofs;
   fread(&numlumps, 4, 1, f);
   fread(&infotableofs, 4, 1, f);
   if (numlumps < 0 || infotableofs < 12 || infotableofs + numlumps * 16 > filesize)
   {
       fclose(f);
       return { WadLoadBadSizes };
   }

    fseek(f, infotableofs, SEEK_SET);
    int filepos;
    int size;
    char lumpname[9];
    mLumps.reserve(numlumps);
    for(int i = 0; i < numlumps; ++i)
    {
        fread(&filepos, 4, 1, f);
        fread(&size, 4, 1, f);
        if (size < 0 || (size > 0 && (filepos < 12 || filepos + size > filesize)))
        {
            fclose(f);
            return { WadLoadBadSizes };
        }
        fread(lumpname, 1, 8, f);
        lumpname[8] = 0;
        Lump lump;
        if(!lump.load(f, lumpname, filepos, size))
        {
            fclose(f);
            return { WadLoadBadLumpIndex, i };
        }
        mLumps.push_back(lump);
    }

    fclose(f);
    return { WadLoadOK };
}
