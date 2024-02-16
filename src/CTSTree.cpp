#include "CTSTree.h"

CTSTree::CTSTree(const TSTree* self)
{
	m_tree = const_cast<TSTree*>(self);
}


CTSTree::~CTSTree()
{
	if (m_tree)
	{
		ts_tree_delete(const_cast<TSTree*>(m_tree));
		m_tree = nullptr;
	}
}

std::shared_ptr<CTSTree> CTSTree::Copy(const CTSTree* source_tree) const
{
	TSTree* new_tstree = ts_tree_copy(source_tree->m_tree);
	return std::make_shared<CTSTree>(new_tstree);
}

CTSNode CTSTree::RootNode() const
{
	return {ts_tree_root_node(m_tree)};
}

const TSLanguage* CTSTree::Language() const
{
	return ts_tree_language(m_tree);
}

void CTSTree::Edit(const TSInputEdit* edit) const
{
	ts_tree_edit(m_tree, edit);
}

std::vector<TSRange> CTSTree::GetChangedRanges(const std::shared_ptr<CTSTree>& new_tree) const
{
	std::vector<TSRange> retval;
	uint32_t length = 0;
	TSRange* items = ts_tree_get_changed_ranges(m_tree, const_cast<TSTree*>(new_tree->m_tree), &length);
	for (uint32_t i = 0; i < length; i++)
	{
		TSRange temp = items[i];
		retval.push_back(temp);
	}
	free(items);
	return retval;
}

void CTSTree::PrintDotGraph(int file_descriptor) const
{
	ts_tree_print_dot_graph(m_tree, file_descriptor);
}

/////////////////////////////////////////////////////////////////////////////

CTSTreeCursor::CTSTreeCursor(CTSNode node) : TSTreeCursor()
{
	// Let the library create a cursor, then hijack its data.
    const TSTreeCursor tmp = ts_tree_cursor_new(node);

	this->tree = tmp.tree;
	this->id = tmp.id;
	this->context[0] = tmp.context[0];
	this->context[1] = tmp.context[1];
}

CTSTreeCursor::~CTSTreeCursor()
{
	// let the library elete the stuff that was allocated in the library.
	ts_tree_cursor_delete(this);
}

void CTSTreeCursor::Reset(CTSNode node) { ts_tree_cursor_reset(this, node); }

CTSNode CTSTreeCursor::CurrentNode() const { return {ts_tree_cursor_current_node(this)}; }

std::string CTSTreeCursor::CurrentFieldName(bool* was_found) const
{
    const auto s = ts_tree_cursor_current_field_name(this);

	if (was_found) { *was_found = (s != nullptr); }
	return s ? std::string(s) : std::string("");
}

TSFieldId CTSTreeCursor::CurrentFieldId() const { return ts_tree_cursor_current_field_id(this); }

bool CTSTreeCursor::GotoParent() { return ts_tree_cursor_goto_parent(this); }
bool CTSTreeCursor::GotoNextSibling() { return ts_tree_cursor_goto_next_sibling(this); }
bool CTSTreeCursor::GotoFirstChild() { return ts_tree_cursor_goto_first_child(this); }

int64_t CTSTreeCursor::GotoFirstChildForByte(int32_t byte) { return ts_tree_cursor_goto_first_child_for_byte(this, byte); }
int64_t CTSTreeCursor::GotoFirstChildForPoint(TSPoint point) { return ts_tree_cursor_goto_first_child_for_point(this, point); }

CTSTreeCursor CTSTreeCursor::Copy() const { return {CurrentNode()}; }
