#pragma once
#include <bitset>







// ECS
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 5000;
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;



struct ChunkAddress
{
    int chunkID;
    int index;
    int version;
};


