#ifndef WAD_H_
#define WAD_H_

enum WadLoadResult
{
   WadLoadOK,
   WadLoadCannotOpen,
   WadLoadBadType,
   WadLoadBadSizes,
};

enum WadType
{
   WadUndefined,
   WadInternal,
   WadPatch,
};

class Wad
{
   char**   m_names;
   char**   m_datas;
   int*     m_lumpsizes;
   int      m_numlumps;
   WadType  m_type;
public:
   WadLoadResult LoadFile(const char* filename);
   Wad()
   {
      m_names = nullptr;
      m_datas = nullptr;
      m_lumpsizes = nullptr;
      m_numlumps = 0;
      m_type = WadUndefined;
   }
   virtual ~Wad()
   {
      if (m_names)
         delete[] m_names[0];
      delete[] m_names;
      if (m_datas)
         delete[] m_datas[0];
      delete[] m_datas;
      delete[] m_lumpsizes;
   }
};

#endif
