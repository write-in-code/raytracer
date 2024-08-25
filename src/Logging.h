#pragma once

#ifdef NDEBUG
    #define INFO(fmt, ...)
    #define ERROR(fmt, ...)
#else
    #define INFO(fmt, ...)  std::format_to(std::ostream_iterator<char>(std::cout), fmt "\n", ##__VA_ARGS__);
    #define ERROR(fmt, ...) std::format_to(std::ostream_iterator<char>(std::cerr), fmt "\n", ##__VA_ARGS__);
#endif
