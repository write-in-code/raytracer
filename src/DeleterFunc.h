#pragma once

template <auto F>
struct DeleterFunc
{
    template <typename T>
    constexpr void operator()(T *arg) const
    {
        F(arg);
    }
};