cmake_minimum_required(VERSION 3.10)

project(json_parser VERSION 0.0.1 LANGUAGES CXX)

if(MSVC)
    add_compile_options("/utf-8")
endif()



#--------------------cmake settings start--------------------
if (NOT CMAKE_BUILD_TYPE)
    message("CMAKE_BUILD_TYPE not found. Default value: Release")
    set(CMAKE_BUILD_TYPE Release)
endif()
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
#--------------------cmake settings end--------------------



#--------------------options start--------------------
option(OPT_NO_EXAMPLES "Build without example programs" OFF)
option(OPT_ENABLE_ASAN "Enable the address sanitizer" OFF)
option(OPT_ENABLE_FLEX_BISON_DEBUG "Enable the debug option for flex and bison" OFF)

message("")
message("OPTIONS:")
get_cmake_property(_options VARIABLES)
list (FILTER _options INCLUDE REGEX ^OPT_)
foreach (_optionName ${_options})
    message("${_optionName}=${${_optionName}}")
endforeach()
message("")
#--------------------options end--------------------



#--------------------compiler start--------------------
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall)
add_compile_options(-Wextra)

if(ENABLE_ASAN)
    add_compile_options(-fno-omit-frame-pointer)
    add_compile_options(-fsanitize=address)
endif()
#--------------------compiler end--------------------



#--------------------public include directories start--------------------
set(PUBLIC_DEPENDENCIES ${PROJECT_SOURCE_DIR}/include)
include_directories("${PUBLIC_DEPENDENCIES}")
#--------------------public include directories end--------------------



#--------------------dependencies start--------------------
find_package(FLEX REQUIRED)
find_package(BISON 3.2 REQUIRED)
#--------------------dependencies end--------------------



#--------------------macro definitions start--------------------
#--------------------macro definitions end--------------------



#--------------------lib start--------------------
set (FLEX_FLAGS "")
if (WIN32)
    set (FLEX_FLAGS "${FLEX_FLAGS} --wincompat")
endif()
if (ENABLE_FLEX_BISON_DEBUG)
    set (FLEX_FLAGS "${FLEX_FLAGS} -d")
endif()
flex_target(LEXER 
${PROJECT_SOURCE_DIR}/src/scanner.l 
${PROJECT_SOURCE_DIR}/src/scanner.cpp 
COMPILE_FLAGS ${FLEX_FLAGS})

set (BISON_FLAGS "")
if (ENABLE_FLEX_BISON_DEBUG)
    set (BISON_FLAGS "${BISON_FLAGS} --debug")
endif()
bison_target(PARSER 
${PROJECT_SOURCE_DIR}/src/parser.y 
${PROJECT_SOURCE_DIR}/src/parser.cpp
DEFINES_FILE ${PROJECT_SOURCE_DIR}/src/parser.h
COMPILE_FLAGS "${BISON_FLAGS}")

add_custom_target(gen_flex_bison DEPENDS ${PROJECT_SOURCE_DIR}/src/scanner.cpp ${PROJECT_SOURCE_DIR}/src/parser.cpp)

add_flex_bison_dependency(LEXER PARSER)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

set(LIB_DEPENDENCIES ${PROJECT_SOURCE_DIR}/third_party ${PROJECT_SOURCE_DIR}/src)

add_library(json_parser_objlib OBJECT ${SOURCES} ${PROJECT_SOURCE_DIR}/src/scanner.cpp ${PROJECT_SOURCE_DIR}/src/parser.cpp)
target_include_directories(json_parser_objlib PUBLIC "${LIB_DEPENDENCIES}")
set_target_properties(json_parser_objlib PROPERTIES 
POSITION_INDEPENDENT_CODE TRUE
ADDITIONAL_CLEAN_FILES ${PROJECT_SOURCE_DIR}/src/location.hh)

add_library(json_parser_shared SHARED $<TARGET_OBJECTS:json_parser_objlib>)
set_target_properties(json_parser_shared PROPERTIES 
OUTPUT_NAME json_parser)

add_library(json_parser_static STATIC $<TARGET_OBJECTS:json_parser_objlib>)
set_target_properties(json_parser_static PROPERTIES 
OUTPUT_NAME json_parser 
POSITION_INDEPENDENT_CODE TRUE)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
DESTINATION ${PROJECT_SOURCE_DIR}/release)

install(TARGETS json_parser_shared
LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/release/lib/
)
install(TARGETS json_parser_static
ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/release/lib/
)
#--------------------lib end--------------------



#--------------------examples start--------------------
if(NOT NO_EXAMPLES)
    add_executable(example1 ${PROJECT_SOURCE_DIR}/example/1.cpp)
    target_include_directories(example1 PUBLIC ${PROJECT_SOURCE_DIR}/example/include)
    target_compile_options(example1 PUBLIC -Wno-unused-parameter)
    target_link_libraries(example1 json_parser_static)

    add_executable(example2 ${PROJECT_SOURCE_DIR}/example/2.cpp)
    target_include_directories(example2 PUBLIC ${PROJECT_SOURCE_DIR}/example/include)
    target_compile_options(example2 PUBLIC -Wno-unused-parameter)
    target_link_libraries(example2 json_parser_static)
endif()
#--------------------examples end--------------------