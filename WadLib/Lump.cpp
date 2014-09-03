#include "stdafx.h"
#include "Lump.h"

Lump::Lump(const Lump& o) : m_contentSize(o.m_contentSize)
{
    strcpy(m_name, o.m_name);
    if (m_contentSize)
    {
        m_content = new unsigned char[m_contentSize];
        memcpy(m_content, o.m_content, m_contentSize);
    }
    else
        m_content = nullptr;
}

Lump::Lump(Lump&& o) : m_content(o.m_content), m_contentSize(o.m_contentSize)
{
    strcpy(m_name, o.m_name);
    o.m_content = nullptr;
    o.m_contentSize = 0;
}

Lump& Lump::operator = (const Lump& o)
{
    if (&o == this)
        return *this;
    strcpy(m_name, o.m_name);
    delete[] m_content;
    m_contentSize = o.m_contentSize;
    if (m_contentSize)
    {
        m_content = new unsigned char[m_contentSize = o.m_contentSize];
        memcpy(m_content, o.m_content, m_contentSize);
    }
    else
        m_content = nullptr;
    return *this;
}

Lump& Lump::operator = (Lump&& o)
{
    if (&o != this)
    {
        strcpy(m_name, o.m_name);
        m_content = o.m_content;
        o.m_content = nullptr;
        m_contentSize = o.m_contentSize;
        o.m_contentSize = 0;
    }
    return *this;
}

bool Lump::operator == (const Lump& o) const
{
    if (this == &o)
        return true;

    return m_contentSize == o.m_contentSize && strcmp(m_name, o.m_name) == 0 && (m_contentSize > 0 ? memcmp(m_content, o.m_content, m_contentSize) : true);
}

void Lump::SetContent(const void* content, size_t size)
{
    delete[] m_content;
    if (size > 0)
    {
        m_content = new unsigned char[size];
        memcpy(m_content, content, size);
    }
    else
        m_content = nullptr;
    m_contentSize = size;
    
}

void Lump::SetName(const char* name)
{
    unsigned i = 0;
    while (*name && i < 8)
    {
        m_name[i] = toupper(*name++);
        ++i;
    }
    m_name[i] = 0;
}