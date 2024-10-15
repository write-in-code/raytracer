#pragma once
#include "FuncName.h"

#define _profblock()      ProfBlock profBlock(nullptr, __FUNCTION_NAME__);
#define _profblockn(name) ProfBlock profBlock((name), __FUNCTION_NAME__);

class ProfBlock : private NonCopyable
{
public:
    ProfBlock(const char *blockName, const char *funcName)
        : m_blockName(blockName), m_funcName(funcName), m_start(std::chrono::high_resolution_clock::now()) {}
    ~ProfBlock()
    {
        auto duration = std::chrono::high_resolution_clock::now() - m_start;
        auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
        if (m_blockName)
        {
            INFO("{} - {}: {} ms", m_blockName, m_funcName, durationMs);
        }
        else
        {
            INFO("{}: {} ms", m_funcName, durationMs);
        }
    }

private:
    const char *m_blockName;
    const char *m_funcName;
    std::chrono::high_resolution_clock::time_point m_start;
};