#!/usr/bin/env python3

import subprocess
import os
import sys
import argparse

PROJECT_FOLDER = os.path.dirname(os.path.realpath(__file__))
SOURCE_FOLDER = os.path.join(PROJECT_FOLDER, 'src')
CLANG_FORMAT_CONFIG_PATH = os.path.join(PROJECT_FOLDER, '.clang-format')
CLANG_FORMAT = 'clang-format'
GIT = 'git'
FILE_EXTENSIONS = ['.h', '.cpp']


def clang_format(filename):
    cmd_line = [CLANG_FORMAT,
            '-i',
            f'--style=file:{CLANG_FORMAT_CONFIG_PATH}',
            filename]

    subprocess.call(cmd_line)
    print(filename)

def collect_modified_files():
    cmd_line = [
        GIT,
        'status',
        '-u',
        '--porcelain']
    
    p = subprocess.Popen(cmd_line, cwd=PROJECT_FOLDER, stdout=subprocess.PIPE)
    output, err = p.communicate()
    if err:
        print(f'Git error: {err}')
        sys.exit(1)

    for line in output.splitlines():
        # status = line[:2]
        filename = line[3:].decode()
        if any(map(lambda ext: filename.endswith(ext), FILE_EXTENSIONS)):
            full_name = os.path.join(PROJECT_FOLDER, filename)
            if os.path.exists(full_name):
                yield full_name

def collect_all_files():
    for subdir, _, files in os.walk(SOURCE_FOLDER):
        for file in files:
            if any(map(lambda ext: file.endswith(ext), FILE_EXTENSIONS)):
                yield os.path.join(subdir, file)


def main():
    parser = argparse.ArgumentParser(
        prog='format.py',
        description='Format source files')

    parser.add_argument('-a', '--all', help='Format all files in source folder', action='store_true')
    args = parser.parse_args()

    if not os.path.exists(CLANG_FORMAT_CONFIG_PATH):
        print(f'Cannot find .clang_format file, expected path: "{CLANG_FORMAT_CONFIG_PATH}"')

    print(f'Project folder: {PROJECT_FOLDER}')
    print(f'Source folder: {SOURCE_FOLDER}')
    print(f'.clang_format: {CLANG_FORMAT_CONFIG_PATH}')

    print()

    for file in collect_all_files() if args.all else collect_modified_files():
        clang_format(file)


if __name__ == "__main__":
    main()