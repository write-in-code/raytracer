#pragma once

template <>
struct std::formatter<glm::vec2> : std::formatter<std::string>
{
    template <typename FormatContext>
    auto format(const glm::vec2 &v, FormatContext &ctx) const
    {
        return std::format_to(ctx.out(), "{} {} {}", v.x, v.y);
    }
};

template <>
struct std::formatter<glm::vec3> : std::formatter<std::string>
{
    template <typename FormatContext>
    auto format(const glm::vec3 &v, FormatContext &ctx) const
    {
        return std::format_to(ctx.out(), "{} {} {}", v.x, v.y, v.z);
    }
};

template <>
struct std::formatter<glm::vec4> : std::formatter<std::string>
{
    template <typename FormatContext>
    auto format(const glm::vec4 &v, FormatContext &ctx) const
    {
        return std::format_to(ctx.out(), "{} {} {}", v.x, v.y, v.z, v.w);
    }
};