#pragma once

#define DEFINE_CLASS_PTR(T)            \
    class T;                           \
    using T##Ptr = std::shared_ptr<T>; \
    using T##UPtr = std::unique_ptr<T>;