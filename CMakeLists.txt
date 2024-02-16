cmake_minimum_required(VERSION 3.0.0)
project(TSWrapperLib VERSION 0.1.0)

cmake_policy(SET CMP0079 NEW)

#include(CTest)
#enable_testing()

include_directories(
    include
tree-sitter/lib/include/tree_sitter 
tree-sitter/lib/include )

file(GLOB tsfiles
    tree-sitter/lib/src/lib.c
)

add_library(TSWrapperLib STATIC 
    src/TSWrapperLib.cpp
    src/CTSLanguage.cpp
    src/CTSNode.cpp 
    src/CTSParser.cpp 
    src/CTSQuery.cpp 
    src/CTSQueryCursor.cpp 
    src/CTSTree.cpp 
    include/TSWrapperLib.h
    ${tsfiles}
)

#add_library(TreeSitter SHARED )


#set_property(TARGET TSWrapperLib PROPERTY CXX_STANDARD 17)
#set_property(TARGET TSWrapperLib PROPERTY CXX_STANDARD_REQUIRED ON)
 
#set_property(TARGET TreeSitter PROPERTY C_STANDARD 11)

#target_link_libraries(TSWrapperLib TreeSitter)
target_link_libraries(TSWrapperLib )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

#include(CPack)
