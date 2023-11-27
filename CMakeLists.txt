cmake_minimum_required(VERSION 3.26)
project(math_eval)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES math_eval.c ast.c eval.c lexer.c libs/list.c token.c
        libs/linenoise.c)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
