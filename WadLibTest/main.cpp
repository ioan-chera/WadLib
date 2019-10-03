//
//  main.cpp
//  WadLibTest
//
//  Created by ioan on 03/10/2019.
//  Copyright © 2019 Ioan Chera. All rights reserved.
//

#include <iostream>
#include "Wad.h"

int main(int argc, const char * argv[])
{
    Wad wad;
    WadLoadResult result = wad.LoadFile(argv[1]);
    if(result.failed())
    {
        fprintf(stderr, "Failed opening %s. %s.\n", argv[1], result.message().c_str());
        return result.type;
    }

    for(const auto &lump : wad.lumps())
        printf("Lump %s size %zu\n", lump.name(), lump.data().size());

    
    return 0;
}
