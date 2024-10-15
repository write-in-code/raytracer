#!/usr/bin/env python3

import subprocess
import os
import sys
import shutil
import argparse

PROJECT_FOLDER = os.path.dirname(os.path.realpath(__file__))
BUILD_FOLDER = os.path.join(PROJECT_FOLDER, 'build')
DEFAULT_COMPILER_FLAGS = '-O3 -Wall -Wextra' if sys.platform != 'win32' else '/O3'        # Enforce optimization even in debug mode
DEFAULT_BUILD_TYPE = 'Debug'
CMAKE_CMD = 'cmake'
CMAKE_CXX_FLAGS = f'-DCMAKE_CXX_FLAGS={DEFAULT_COMPILER_FLAGS}'
CMAKE_BUILD_TYPE = f'-DCMAKE_BUILD_TYPE={DEFAULT_BUILD_TYPE}'

parser = argparse.ArgumentParser(
        prog='cmake_configure.py',
        description='Utility command line tool to configure CMake')

parser.add_argument('--clean', help='Remove build directory and reconfigure', action='store_true')
args = parser.parse_args()


print("""
 *******       **     **    ** ********** *******       **       ******  ******** *******  
/**////**     ****   //**  ** /////**/// /**////**     ****     **////**/**///// /**////** 
/**   /**    **//**   //****      /**    /**   /**    **//**   **    // /**      /**   /** 
/*******    **  //**   //**       /**    /*******    **  //** /**       /******* /*******  
/**///**   **********   /**       /**    /**///**   **********/**       /**////  /**///**  
/**  //** /**//////**   /**       /**    /**  //** /**//////**//**    **/**      /**  //** 
/**   //**/**     /**   /**       /**    /**   //**/**     /** //****** /********/**   //**
//     // //      //    //        //     //     // //      //   //////  //////// //     //                                                         
""")

print()
print(f'Project folder: {PROJECT_FOLDER}')
print(f'  Build folder: {BUILD_FOLDER}')
print(f'    Build type: {DEFAULT_BUILD_TYPE}')
print(f'Compiler flags: {DEFAULT_COMPILER_FLAGS}')
print()

if args.clean:
    print(f' - Discarding {BUILD_FOLDER}')
    if os.path.exists(BUILD_FOLDER):
        shutil.rmtree(BUILD_FOLDER)

    os.mkdir(BUILD_FOLDER)

print()

cmd_line = [CMAKE_CMD, 
            CMAKE_BUILD_TYPE,
            CMAKE_CXX_FLAGS,
            '-DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE',
            f'-S{PROJECT_FOLDER}',
            f'-B{BUILD_FOLDER}']

print(f'> {cmd_line}')
print('')

result = subprocess.call(cmd_line, cwd=PROJECT_FOLDER)
if result != 0:
    print(f'Error: CMake configuration failed with exit code {result}')
    sys.exit(result)