//
//  Level.hpp
//  WadLib
//
//  Created by ioan on 03/10/2019.
//  Copyright © 2019 Ioan Chera. All rights reserved.
//

#ifndef Level_hpp
#define Level_hpp

#include <stdint.h>
#include <vector>
#include "Lump.h"

struct Vertex
{
    int x, y;

    Vertex() = default;

    explicit Vertex(const uint8_t *data) :
    x(*reinterpret_cast<const int16_t *>(data)),
    y(*reinterpret_cast<const int16_t *>(data + 2))
    {
    }
};

struct Sector
{
    int floorheight;
    int ceilingheight;

    Sector() = default;

    explicit Sector(const uint8_t *data) :
    floorheight(*reinterpret_cast<const int16_t *>(data)),
    ceilingheight(*reinterpret_cast<const int16_t *>(data + 2))
    {
    }
};

struct Linedef
{
    const Vertex *v[2];
    const Sector *s[2];

    bool load(const uint8_t *data, const std::vector<Vertex> &vertices,
              const std::vector<int> &sides, const std::vector<Sector> &sectors);
};

class Level
{
    std::string mName;
    std::vector<Vertex> mVertices;
    std::vector<Sector> mSectors;
    std::vector<Linedef> mLinedefs;

public:
    static std::vector<Level> getLevels(const std::vector<Lump> &lumps);
    int load(const std::vector<Lump> &lumps, int pastIndex);

    const char *name() const
    {
        return mName.c_str();
    }
    const std::vector<Vertex> &vertices() const
    {
        return mVertices;
    }
    const std::vector<Sector> &sectors() const
    {
        return mSectors;
    }
    const std::vector<Linedef> &linedefs() const
    {
        return mLinedefs;
    }
};

#endif /* Level_hpp */
