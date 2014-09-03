#pragma once
class Lump
{
public:
    Lump()
    {
        m_name[0] = 0;
    }
    Lump(const Lump& o);
    Lump(Lump&& o);
    ~Lump()
    {
        delete[] m_content;
    }

    Lump& operator = (const Lump& o);
    Lump& operator = (Lump&& o);
    bool operator == (const Lump& o) const;

    void SetContent(const void* content, size_t size);
    void SetName(const char* name);

private:
    char            m_name[9];
    unsigned char*  m_content = nullptr;
    size_t          m_contentSize = 0;
};

