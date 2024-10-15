#pragma once

#ifndef __FUNCTION_NAME__
    #ifdef WIN32
        #define __FUNCTION_NAME__ __FUNCTION__
    #else
        #define __FUNCTION_NAME__ __func__
    #endif
#endif