#pragma once

//
// WAD data element
//
class Lump
{
public:
    bool load(FILE *f, const char *name, int address, int size);
    bool operator == (const Lump& o) const
    {
        return mName == o.mName && mData == o.mData;
    }

    void SetContent(const void* content, size_t size)
    {
        auto byteContent = static_cast<const uint8_t *>(content);
        mData.assign(byteContent, byteContent + size);
    }
    void SetName(const char* name)
    {
        mName = name;
    }

private:
    std::string mName;
    std::vector<uint8_t> mData;
};
