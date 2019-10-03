//
//  Level.cpp
//  WadLib
//
//  Created by ioan on 03/10/2019.
//  Copyright © 2019 Ioan Chera. All rights reserved.
//

#include <string.h>
#include "Level.h"

enum LumpOffset
{
    LumpHeader,
    LumpOThings,
    LumpOLines,
    LumpOSides,
    LumpOVerts,
    LumpOSegs,
    LumpOSS,
    LumpONodes,
    LumpOSectors,
    LumpORej,
    LumpOBlock,
    LumpOMAX,
};

enum ItemSize
{
    ISLine = 14,
    ISSide = 30,
    ISVert = 4,
    ISSector = 26
};

//
// Check if a given lump is a level header
//
static bool isLevel(const std::vector<Lump> &lumps, int index)
{
    if(index < 0 || index + LumpOMAX >= lumps.size() + 1)
        return false;
    if(strcasecmp(lumps[index + LumpOThings].name(), "things") ||
       strcasecmp(lumps[index + LumpOLines].name(), "linedefs") ||
       strcasecmp(lumps[index + LumpOSides].name(), "sidedefs") ||
       strcasecmp(lumps[index + LumpOVerts].name(), "vertexes") ||
       strcasecmp(lumps[index + LumpOSegs].name(), "segs") ||
       strcasecmp(lumps[index + LumpOSS].name(), "ssectors") ||
       strcasecmp(lumps[index + LumpONodes].name(), "nodes") ||
       strcasecmp(lumps[index + LumpOSectors].name(), "sectors") ||
       strcasecmp(lumps[index + LumpORej].name(), "reject") ||
       strcasecmp(lumps[index + LumpOBlock].name(), "blockmap"))
    {
        return false;
    }
    return true;
}

//
// Tries loading a linedef from given data
//
bool Linedef::load(const uint8_t *data, const std::vector<Vertex> &vertices,
                   const std::vector<int> &sides, const std::vector<Sector> &sectors)
{
    int v1 = *reinterpret_cast<const int16_t *>(data);
    int v2 = *reinterpret_cast<const int16_t *>(data + 2);
    int s1 = *reinterpret_cast<const int16_t *>(data + 10);
    int s2 = *reinterpret_cast<const int16_t *>(data + 12);
    int numVerts = static_cast<int>(vertices.size());
    int numSides = static_cast<int>(sides.size());
    if(v1 < 0 || v1 >= numVerts || v2 < 0 || v2 >= numVerts || s1 < -1 || s1 >= numSides ||
       s2 < -1 || s2 >= numSides)
    {
        return false;
    }
    int numSectors = static_cast<int>(sectors.size());
    if(s1 >= 0 && (sides[s1] < 0 || sides[s1] >= numSectors))
        return false;
    if(s2 >= 0 && (sides[s2] < 0 || sides[s2] >= numSectors))
        return false;
    v[0] = &vertices[v1];
    v[1] = &vertices[v2];
    s[0] = s1 >= 0 ? &sectors[sides[s1]] : nullptr;
    s[1] = s2 >= 0 ? &sectors[sides[s2]] : nullptr;
    return true;
}

//
// Convenience to get all levels from a set
//
std::vector<Level> Level::getLevels(const std::vector<Lump> &lumps)
{
    std::vector<Level> result;
    Level level;
    for(int index = -1; (index = level.load(lumps, index)) != -1;)
        result.push_back(level);
    return result;
}

//
// Loads a level from a lump list, after a given index
//
int Level::load(const std::vector<Lump> &lumps, int pastIndex)
{
    if(pastIndex < -1)
        return -1;

    mVertices.clear();
    mSectors.clear();
    std::vector<int> sides;
    mLinedefs.clear();

    for(int i = pastIndex + 1; i < lumps.size(); ++i)
    {
        if(!isLevel(lumps, i))
            continue;

        const Lump &verticesLump = lumps[i + LumpOVerts];
        size_t numVertices = verticesLump.data().size() / ISVert;
        mVertices.reserve(numVertices);
        for(size_t j = 0; j < numVertices; ++j)
            mVertices.push_back(Vertex(verticesLump.data().data() + ISVert * j));

        const Lump &sectorsLump = lumps[i + LumpOSectors];
        size_t numSectors = sectorsLump.data().size() / ISSector;
        mSectors.reserve(numSectors);
        for(size_t j = 0; j < numSectors; ++j)
            mSectors.push_back(Sector(sectorsLump.data().data() + ISSector * j));

        // need sides
        const Lump &sidesLump = lumps[i + LumpOSides];
        size_t numSides = sidesLump.data().size() / ISSide;
        sides.reserve(numSides);
        for(size_t j = 0; j < numSides; ++j)
        {
            sides.push_back(*reinterpret_cast<const int16_t *>(sidesLump.data().data() +
                                                               ISSide * j + 28));
        }

        const Lump &linesLump = lumps[i + LumpOLines];
        size_t numLines = linesLump.data().size() / ISLine;
        mLinedefs.reserve(numLines);
        for(size_t j = 0; j < numLines; ++j)
        {
            Linedef line;
            if(!line.load(linesLump.data().data() + ISLine * j, mVertices, sides, mSectors))
                return -1;
            mLinedefs.push_back(line);
        }
        mName = lumps[i].name();
        return i;
    }
    return -1;
}
