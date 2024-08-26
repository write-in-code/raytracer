#pragma once

template <typename To, typename From>
inline To narrow_cast(From value)
{
    To res = static_cast<From>(value);
    assert(static_cast<From>(res) == value);

    return res;
}
