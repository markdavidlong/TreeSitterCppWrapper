TEMPLATE = lib

CONFIG += c++17 staticlib

TARGET = TSWrapperLib

INCLUDEPATH = \
	include \
	tree-sitter/lib/include \
	tree-sitter/lib/include/tree_sitter 

# Source files
SOURCES = \
	src/CTSWrapperLib.cpp \
	src/CTSNode.cpp \
	src/CTSLanguage.cpp \
	src/CTSParser.cpp \
	src/CTSTree.cpp \
	src/CTSQuery.cpp \
	src/CTSQueryCursor.cpp \
	include/TSWrapperLib.h \
	tree-sitter/src/lib.c 




