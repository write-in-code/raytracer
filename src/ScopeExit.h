#pragma once

template <typename F>
class ScopeExit
{
public:
    explicit ScopeExit(F &&func)
        : m_func(func) {}
    ~ScopeExit() { m_func(); }

    ScopeExit(const ScopeExit &se) = delete;
    const ScopeExit &operator=(const ScopeExit &se) = delete;

private:
    F m_func;
};

#define _ON_SCOPE_EXIT_CAT_(name, line) name##line
#define _ON_SCOPE_EXIT_CAT(name, line)  _ON_SCOPE_EXIT_CAT_(name, line)
#define ON_SCOPE_EXIT(f)                auto _ON_SCOPE_EXIT_CAT(scopeExit, __COUNTER__) = ScopeExit(f);