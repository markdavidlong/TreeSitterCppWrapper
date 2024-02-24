TEMPLATE = lib

CONFIG += c++17 staticlib

TARGET = TSWrapperLib

INCLUDEPATH = \
	include \
	tree-sitter/lib/include \
    tree-sitter/lib/include/tree_sitter \
    tree-sitter/lib/src

# Source files
SOURCES = \
	src/CTSNode.cpp \
	src/CTSLanguage.cpp \
	src/CTSParser.cpp \
	src/CTSTree.cpp \
	src/CTSQuery.cpp \
	src/CTSQueryCursor.cpp \
    tree-sitter/lib/src/lib.c


INCLUDES = \
    include/CTSNode.h \
    include/CTSLanguage.h \
    include/CTSParser.h \
    include/CTSTree.h \
    include/CTSQuery.h \
    include/CTSQueryCursor.h

