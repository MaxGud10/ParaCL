cmake_minimum_required(VERSION 3.14)

project(ParaCL)

enable_testing()

option(ENABLE_LOGGING "Enable logging" OFF)
set(ENABLE_LOGGING ${ENABLE_LOGGING} CACHE BOOL "Enable logging" FORCE)

option(ENABLE_GRAMMAR_LOG "Enable logging" OFF)
set(ENABLE_GRAMMAR_LOG ${ENABLE_GRAMMAR_LOG} CACHE BOOL "Enable logging" FORCE)

option(ENABLE_BD_TESTS "Enables big data tests" OFF)
set(ENABLE_BD_TESTS ${ENABLE_BD_TESTS} CACHE BOOL "Enables big data tests" FORCE)

set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Build type")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(RELEASE_COMPILE_OPTIONS
	-O2
	-fstack-protector
	-fPIE
	-fstrict-overflow
	-Wformat-security
	-Wformat=2
	-Wall
	-Wextra
	-Wnon-virtual-dtor
	-Woverloaded-virtual
	-Wcast-align
	-Wpointer-arith
	-Wunreachable-code
	-Wno-pre-c++17-compat
)

set(DEBUG_COMPILE_OPTIONS
	-D_DEBUG
	-ggdb3
	-O0
	-Wall
	-Wextra
	-Wc++14-compat
	-Wmissing-declarations
	-Wcast-align
	-Wcast-qual
	-Wchar-subscripts
	-Wconversion
	-Wctor-dtor-privacy
	-Wempty-body
	-Wfloat-equal
	-Wformat-security
	-Winline
	-Wnon-virtual-dtor
	-Woverloaded-virtual
	-Wpacked
	-Wpointer-arith
	-Winit-self
	-Wredundant-decls
	-Wshadow
	-Wsign-conversion
	-Wsign-promo
	-Wstrict-overflow=2
	-Wsuggest-override
	-Wswitch-default
	-Wundef
	-Wunreachable-code
	-Wunused
	-Wvariadic-macros
	-Wno-missing-field-initializers
	-Wno-narrowing
	-Wno-old-style-cast
	-Wno-varargs
	-Wstack-protector
	-fcheck-new
	-fsized-deallocation
	-fstack-protector
	-fstrict-overflow
	-fno-omit-frame-pointer
	-fPIE
	-Werror=vla
	-fsanitize=address,alignment,bool,bounds,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
)


file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/parser)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(BISON_INPUT ${CMAKE_SOURCE_DIR}/grammar/parser.y)
set(FLEX_INPUT ${CMAKE_SOURCE_DIR}/grammar/lexer.l)

BISON_TARGET(Parser ${BISON_INPUT} ${CMAKE_BINARY_DIR}/parser/parser.cpp
    COMPILE_FLAGS "--defines=${CMAKE_BINARY_DIR}/parser/parser.hpp")

FLEX_TARGET(Lexer ${FLEX_INPUT} ${CMAKE_BINARY_DIR}/parser/lexer.cpp)

ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)

set_source_files_properties(
    ${FLEX_Lexer_OUTPUTS}
    ${BISON_Parser_OUTPUTS}
    PROPERTIES GENERATED TRUE
)

add_library(paracl_core
    ${FLEX_Lexer_OUTPUTS}
    ${BISON_Parser_OUTPUTS}
)

target_include_directories(paracl_core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/detail
    ${CMAKE_BINARY_DIR}/parser
    ${CMAKE_SOURCE_DIR}/utils
)

if(ENABLE_LOGGING)
    target_compile_definitions(paracl_core PUBLIC ENABLE_LOGGING)
endif()

if(ENABLE_GRAMMAR_LOG)
    target_compile_definitions(paracl_core PUBLIC YY_FLEX_DEBUG=1 YYDEBUG=1)
else()
    target_compile_definitions(paracl_core PUBLIC YY_FLEX_DEBUG=0 YYDEBUG=0)
endif()

add_executable(paracl main.cpp)

target_link_libraries(paracl PRIVATE paracl_core)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(paracl PRIVATE DEBUG)
    target_compile_options(paracl PRIVATE ${DEBUG_COMPILE_OPTIONS})
    target_link_options(paracl PRIVATE
        -fsanitize=address,alignment,bool,bounds,float-cast-overflow,float-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
    )
else()
    target_compile_options(paracl PRIVATE ${RELEASE_COMPILE_OPTIONS})
endif()

add_subdirectory(unit_tests/)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/dumps)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/dumps/png)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/dumps/dot)

target_include_directories(paracl PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/detail
    ${CMAKE_BINARY_DIR}/parser
	${CMAKE_SOURCE_DIR}/utils
)

if(ENABLE_LOGGING)
    target_compile_definitions(paracl PRIVATE ENABLE_LOGGING)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(paracl PRIVATE DEBUG)

    target_compile_options(paracl PRIVATE ${DEBUG_COMPILE_OPTIONS})
    target_link_options(paracl PRIVATE
        -fsanitize=address,alignment,bool,bounds,float-cast-overflow,float-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
    )
else()
    target_compile_options(paracl PRIVATE ${RELEASE_COMPILE_OPTIONS})
endif()

if(ENABLE_GRAMMAR_LOG)
	target_compile_definitions(paracl PRIVATE YY_FLEX_DEBUG=1 YYDEBUG=1)
else()
	target_compile_definitions(paracl PRIVATE YY_FLEX_DEBUG=0 YYDEBUG=0)
endif()
