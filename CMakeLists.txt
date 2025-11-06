cmake_minimum_required(VERSION 3.14)

project(ParaCL)

enable_testing()

option(ENABLE_LOGGING "Enable logging" OFF)
set(ENABLE_LOGGING ${ENABLE_LOGGING} CACHE BOOL "Enable logging" FORCE)

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
	-Wswitch-enum
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
	-Wlarger-than=8192
	-fPIE
	-Werror=vla
	-Wno-pre-c++17-compat
	-Wno-pre-c++20-compat
	-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
)


file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/parser)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

set(BISON_INPUT ${CMAKE_SOURCE_DIR}/parse_gen/parser.yy)
set(FLEX_INPUT ${CMAKE_SOURCE_DIR}/parse_gen/lexer.ll)

BISON_TARGET(Parser ${BISON_INPUT} ${CMAKE_BINARY_DIR}/parser/parser.cpp
    COMPILE_FLAGS "--defines=${CMAKE_BINARY_DIR}/parser/parser.hh")

FLEX_TARGET(Lexer ${FLEX_INPUT} ${CMAKE_BINARY_DIR}/parser/lexer.cpp)

ADD_FLEX_BISON_DEPENDENCY(Lexer Parser)


add_executable(paracl.x ./main.cpp
    ${FLEX_Lexer_OUTPUTS}
    ${BISON_Parser_OUTPUTS}
)

target_include_directories(paracl.x PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/parser
	${CMAKE_SOURCE_DIR}/utils/include
)

if(ENABLE_LOGGING)
    target_compile_definitions(paracl.x PRIVATE ENABLE_LOGGING)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(paracl.x PRIVATE DEBUG)

	target_compile_definitions(paracl.x PRIVATE YY_FLEX_DEBUG=1 YYDEBUG=1)

    target_compile_options(paracl.x PRIVATE ${DEBUG_COMPILE_OPTIONS})
    target_link_options(paracl.x PRIVATE
        -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
    )
else()
    target_compile_options(paracl.x PRIVATE ${RELEASE_COMPILE_OPTIONS})
	target_compile_definitions(paracl.x PRIVATE YY_FLEX_DEBUG=0 YYDEBUG=0)
endif()