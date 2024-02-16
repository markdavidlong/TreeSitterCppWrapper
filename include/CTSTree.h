#pragma once

#include "api.h"
#include "CTSNode.h"
#include <string>
#include <vector>
#include <memory>

class CTSTree;

class CTSTreeCursor : public TSTreeCursor
{
public:
    CTSTreeCursor(const CTSTreeCursor&) = delete;
    CTSTreeCursor(const CTSTreeCursor&&) = delete;
    CTSTreeCursor operator=(const CTSTreeCursor&) = delete;
    CTSTreeCursor operator=(const CTSTreeCursor&&) = delete;

    /**
     * Deletes the tree cursor, freeing all of the memory that it used.
     */
    ~CTSTreeCursor();

    /**
     * Re-initialize a tree cursor to start at a different node.
     */
    void Reset(CTSNode node);

    /**
     * Get the tree cursor's current node.
     */
    CTSNode CurrentNode() const;

    /**
     * Get the field name of the tree cursor's current node.
     *
     * This returns an empty string if the current node does not have a field name.
     *
     * If was_found is provided, it will be set to true if a field was found and
     * false if it was not.
     *
     * See also `CTSNode::ChildByFieldName`.
     */
    std::string CurrentFieldName(bool* was_found) const;

    /**
     * Get the field id of the tree cursor's current node.
     *
     * This returns zero if the current node does not have a field.
     * See also `CTSNode::ChildByFieldId`, `CTSLanguage::FieldIdForName`.
     */
    TSFieldId CurrentFieldId() const;

    /**
     * Move the cursor to the parent of its current node.
     *
     * This returns `true` if the cursor successfully moved, and returns `false`
     * if there was no parent node (the cursor was already on the root node).
     */
    bool GotoParent();
    /**
     * Move the cursor to the next sibling of its current node.
     *
     * This returns `true` if the cursor successfully moved, and returns `false`
     * if there was no next sibling node.
     */
    bool GotoNextSibling();
    /**
     * Move the cursor to the first child of its current node.
     *
     * This returns `true` if the cursor successfully moved, and returns `false`
     * if there were no children.
     */
    bool GotoFirstChild();

    /**
     * Move the cursor to the first child of its current node that extends beyond
     * the given byte offset.
     *
     * This returns the index of the child node if one was found, and returns -1
     * if no such child was found.
     */
    int64_t GotoFirstChildForByte(int32_t byte);
    /**
     * Move the cursor to the first child of its current node that extends beyond
     * the given point.
     *
     * This returns the index of the child node if one was found, and returns -1
     * if no such child was found.
     */
    int64_t GotoFirstChildForPoint(TSPoint point);

    /**
     * Returns a copy of the current cursor
     */
    CTSTreeCursor Copy() const;

    friend class CTSTree;

private:
    CTSTreeCursor(CTSNode node);
};

class CTSTree
{
public:
    CTSTree() = delete;
    CTSTree(const CTSTree&) = delete;
    CTSTree(const CTSTree&&) = delete;
    CTSTree operator=(const CTSTree&) = delete;
    CTSTree operator=(const CTSTree&&) = delete;

    /**
     * Constructs a new CTSTree object around a pre-existing TSTree object.
     * The CTSTree class takes ownership of the TSTree object.
     *
     * This constructor generally should not be called manually.  CTSTree
     * objects are created either from the CTSParse class or through
     * the 'CTSTree::Copy' method.
     */
    CTSTree(const TSTree* self);

    /***
     * Overloaded constructor
     */
    CTSTree(TSTree* self) { m_tree = self; }
    /**
     * Delete the syntax tree, freeing all of the memory that it used.
     */
    ~CTSTree();

    /**
     * Create a shallow copy of the syntax tree. This is very fast.
     *
     * You need to copy a syntax tree in order to use it on more than one thread at
     * a time, as syntax trees are not thread safe.
     */
    std::shared_ptr<CTSTree> Copy(const CTSTree* source_tree) const;

    /**
     * Get the root node of the syntax tree.
     */
    CTSNode RootNode() const;

    /**
     * Get the language that was used to parse the syntax tree.
     */
    const TSLanguage* Language() const;

    /**
     * Edit the syntax tree to keep it in sync with source code that has been
     * edited.
     *
     * You must describe the edit both in terms of byte offsets and in terms of
     * (row, column) coordinates.
     */
    void Edit(const TSInputEdit* edit) const;

    /**
     * Compare an old edited syntax tree to a new syntax tree representing the same
     * document, returning an array of ranges whose syntactic structure has changed.
     *
     * For this to work correctly, the old syntax tree must have been edited such
     * that its ranges match up to the new tree. Generally, you'll want to call
     * this method right after calling one of the `TSParser::Parse` methods.
     * You need to pass the old tree that was passed to parse, as well as the new
     * tree that was returned from that method.
     */
    std::vector<TSRange> GetChangedRanges(const std::shared_ptr<CTSTree>& new_tree) const;

    /**
     * Write a DOT graph describing the syntax tree to the given file.
     */
    void PrintDotGraph(int file_descriptor) const;

    /**
     * Create a new tree cursor starting from the tree's root node.
     *
     * A tree cursor allows you to walk a syntax tree more efficiently than is
     * possible using the `TSNode` functions. It is a mutable object that is always
     * on a certain syntax node, and can be moved imperatively to different nodes.
     */
    CTSTreeCursor GetCursor() const { return {RootNode()}; }

    /**
     * Create a new tree cursor starting from the given node.
     *
     * A tree cursor allows you to walk a syntax tree more efficiently than is
     * possible using the `TSNode` functions. It is a mutable object that is always
     * on a certain syntax node, and can be moved imperatively to different nodes.
     */
    static CTSTreeCursor GetCursorAtNode(CTSNode node) { return {node}; }

    //	const TSTree *GetEncapsulatedTSTree() { return const_cast<const TSTree *>(m_tree); }

    friend struct CTSParser;

private:
    TSTree* m_tree = nullptr;
};
