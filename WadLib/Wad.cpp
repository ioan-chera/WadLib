#include <stdio.h>
#include <string.h>
#include "Wad.h"

WadLoadResult Wad::LoadFile(const char* filename)
{
   // open wad and cache everything, in one place
   FILE* f = fopen(filename, "rb");

   if (!f)
      return WadLoadCannotOpen;

   char sign_header[4];
   WadType new_type;

   fread(sign_header, 1, 4, f);
   if (!memcmp(sign_header, "PWAD", 4))
      new_type = WadPatch;
   else if (!memcmp(sign_header, "IWAD", 4))
      new_type = WadInternal;
   else
      return WadLoadBadType;

   fseek(f, 0, SEEK_END);
   long filesize = ftell(f);
   fseek(f, 4, SEEK_SET);

   int numlumps, infotableofs;
   fread(&numlumps, 4, 1, f);
   fread(&infotableofs, 4, 1, f);
   if (numlumps < 0 || infotableofs < 12 || infotableofs + numlumps * 16 > filesize)
      return WadLoadBadSizes;

   // data to prepare:
   // - m_names: an array of units of 8-length strings
   // - m_datas: an array of int-sized buffers
   int* new_lumpsizes = new int[numlumps];
   char** new_names = new char*[numlumps];
   new_names[0] = new char[8 * numlumps];
   fseek(f, infotableofs, SEEK_SET);
   int i;
   int filepos, totalsize = 0;
   for (i = 0; i < numlumps; ++i)
   {
      fread(&filepos, 4, 1, f);
      fread(&new_lumpsizes[i], 4, 1, f);
       if (new_lumpsizes[i] < 0 || (new_lumpsizes[i] > 0 && (filepos < 12 || filepos + new_lumpsizes[i] > filesize)))
      {
         delete[] new_lumpsizes;
         delete[] new_names[0];
         delete[] new_names;
         return WadLoadBadSizes;
      }
      totalsize += new_lumpsizes[i];
      new_names[i] = new_names[0] + 8 * i;
      fread(new_names[i], 1, 8, f);
   }
   char** new_datas = new char*[numlumps];
   new_datas[0] = new char[totalsize];
   fseek(f, infotableofs, SEEK_SET);
   for (i = 0; i < numlumps; ++i)
   {
      fread(&filepos, 4, 1, f);

   }

   return WadLoadOK;
}
