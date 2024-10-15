#pragma once

class NonCopyable
{
protected:
    NonCopyable() = default;

public:
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
};