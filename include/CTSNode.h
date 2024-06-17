#pragma once
#include "api.h"
#include <string>

class CTSNode : public TSNode
{
public:
    /**
     * Creates an empty node
     */
    CTSNode();

    /**
     * Creates a new CTSNode wrapping and taking ownership of the TSNode node.
     */
    CTSNode(TSNode node);

    /**
     * Get the node's type as a std::string.
     */
    std::string Type() const;

    /**
     * Get the node's type as a numerical id.
     */
    TSSymbol Symbol() const;

    /**
     * Get the node's start byte.
     */
    uint32_t StartByte() const;
    /**
     * Get the node's start position in terms of rows and columns.
     */
    TSPoint StartPoint() const;
    /**
     * Get the node's end byte.
     */
    uint32_t EndByte() const;
    /**
     * Get the node's end position in terms of rows and columns.
     */
    TSPoint EndPoint() const;

    /**
     * Get an S-expression representing the node as a string.
     */
    std::string String() const;

    /**
     * Check if the node is null. methods like `CTSNode::Child` and
     * `CTSNode::NextSibling` will return a null node to indicate that no such node
     * was found.
     */
    bool IsNull() const;

    /**
     * Check if the node is *named*. Named nodes correspond to named rules in the
     * grammar, whereas *anonymous* nodes correspond to string literals in the
     * grammar.
     */
    bool IsNamed() const;

    /**
     * Check if the node is *missing*. Missing nodes are inserted by the parser in
     * order to recover from certain kinds of syntax errors.
     */
    bool IsMissing() const;

    /**
     * Check if the node is *extra*. Extra nodes represent things like comments,
     * which are not required the grammar, but can appear anywhere.
     */
    bool IsExtra() const;

    /**
     * Check if a syntax node has been edited.
     */
    bool HasChanges() const;

    /**
     * Check if the node is a syntax error or contains any syntax errors.
     */
    bool HasError() const;

    /**
     * Get the node's immediate parent.
     */
    CTSNode Parent() const;

    /**
     * Get the node's child at the given index, where zero represents the first
     * child.
     */
    CTSNode Child(uint32_t index = 0) const;

    /**
     * Get the node's *named* child at the given index.
     *
     * See also `CTSNode::IsNamed`.
     */
    CTSNode NamedChild(uint32_t index = 0) const;

    /**
     * Get the field name for node's child at the given index, where zero represents
     * the first child. If given, was_found is set to true if the field was found or
     * false if the field was not found
     */
    std::string FieldNameForChild(uint32_t index = 0, bool* was_found = nullptr) const;

    /**
     * Get the node's number of children.
     */
    uint32_t ChildCount() const;

    /**
     * Get the node's number of *named* children.
     *
     * See also `CTSNode::IsNamed`.
     */
    uint32_t NamedChildCount() const;

    /**
     * Get the node's child with the given field name.
     */
    CTSNode ChildByFieldName(const std::string& field_name) const;

    /**
     * Get the node's child with the given numerical field id.
     *
     * You can convert a field name to an id using the
     * `CTSLanguage::FieldIdForName` method.
     */
    CTSNode ChildByFieldId(TSFieldId id) const;

    /**
     * Get the node's next sibling.
     */
    CTSNode NextSibling() const;

    /**
     * Get the node's previous sibling.
     */
    CTSNode PrevSibling() const;

    /**
     * Get the node's next *named* sibling.
     */
    CTSNode NextNamedSibling() const;

    /**
     * Get the node's previous *named* sibling.
     */
    CTSNode PrevNamedSibling() const;

    /**
     * Get the node's first child that extends beyond the given byte offset.
     */
    CTSNode FirstChildForByte(uint32_t loc) const;

    /**
     * Get the node's first named child that extends beyond the given byte offset.
     */
    CTSNode FirstNamedChildForByte(uint32_t loc) const;

    /**
     * Get the smallest node within this node that spans the given range of bytes.
     */
    CTSNode DescendantForByteRange(uint32_t start, uint32_t end) const;

    /**
     * Get the smallest node within this node that spans the given range of
     * (row, column) positions.
     */
    CTSNode DescendantForPointRange(TSPoint start, TSPoint end) const;

    /**
     * Get the smallest named node within this node that spans the given range of
     * bytes.
     */
    CTSNode NamedDescendantForByteRange(uint32_t start, uint32_t end) const;

    /**
     * Get the smallest named node within this node that spans the given range of
     * (row, column) positions.
     */
    CTSNode NamedDescendantForPointRange(TSPoint start, TSPoint end) const;

    /**
     * Edit the node to keep it in-sync with source code that has been edited.
     *
     * This method is only rarely needed. When you edit a syntax tree with the
     * `CTSTree::Edit` method, all of the nodes that you retrieve from the tree
     * afterward will already reflect the edit. You only need to use `CTSNode::Edit`
     * when you have a `CTSNode` instance that you want to keep and continue to use
     * after an edit.
     */
    void Edit(const TSInputEdit* edit);

    /**
     * Check if this node is equal to another.
     */
    bool operator==(CTSNode other) const;

    /**
     * Check if two nodes are identical.
     */
    static bool Eq(CTSNode n1, CTSNode n2) { return ts_node_eq(n1, n2); }
};
