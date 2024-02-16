//#include "pch.h"
// ReSharper disable CppClangTidyClangDiagnosticShorten64To32
#include "CTSNode.h"

CTSNode::CTSNode() : TSNode()
{
	for (unsigned int& i : context)
    {
        i = 0;
	}
	id = nullptr;
	tree = nullptr;
}
CTSNode::CTSNode(TSNode node) : TSNode()
{
	for (int i = 0; i < 4; i++)
	{
		context[i] = node.context[i];
	}
	id = node.id;
	tree = node.tree;
}


std::string CTSNode::Type() const { return { ts_node_type(*this) }; }

TSSymbol CTSNode::Symbol() const { return ts_node_symbol(*this); }

uint32_t CTSNode::StartByte() const { return ts_node_start_byte(*this); }
TSPoint CTSNode::StartPoint() const { return ts_node_start_point(*this); }
uint32_t CTSNode::EndByte() const { return ts_node_end_byte(*this); }
TSPoint CTSNode::EndPoint() const { return ts_node_end_point(*this); }

std::string CTSNode::String() const
{
	char* free_me_after_use = ts_node_string(*this);
	auto retval = std::string(free_me_after_use);
	free(free_me_after_use);
	return retval;
}

bool CTSNode::IsNull() const { return ts_node_is_null(*this); }
bool CTSNode::IsNamed() const { return ts_node_is_named(*this); }
bool CTSNode::IsMissing() const { return ts_node_is_missing(*this); }
bool CTSNode::IsExtra() const { return ts_node_is_extra(*this); }

bool CTSNode::HasChanges() const { return ts_node_has_changes(*this); }
bool CTSNode::HasError() const { return ts_node_has_error(*this); }

CTSNode CTSNode::Parent() const { return {ts_node_parent(*this)}; }
CTSNode CTSNode::Child(uint32_t index) const { return {ts_node_child(*this, index)}; }
CTSNode CTSNode::NamedChild(uint32_t index) const
{
	return ts_node_named_child(*this, index);
}

std::string CTSNode::FieldNameForChild(uint32_t index, bool* was_found) const
{
	auto s = ts_node_field_name_for_child(*this, index);
	if (was_found) { *was_found = (s != nullptr); }
	if (s) return {s}; else return {};
}

uint32_t CTSNode::ChildCount() const { return ts_node_child_count(*this); }
uint32_t CTSNode::NamedChildCount() const { return ts_node_named_child_count(*this); }

CTSNode CTSNode::ChildByFieldName(const std::string& field_name) const
{
	return { ts_node_child_by_field_name(*this, field_name.c_str(), field_name.length()) };
}

CTSNode CTSNode::ChildByFieldId(TSFieldId cid) const { return {ts_node_child_by_field_id(*this, cid)}; }

CTSNode CTSNode::NextSibling() const { return {ts_node_next_sibling(*this)}; }
CTSNode CTSNode::PrevSibling() const { return {ts_node_prev_sibling(*this)}; }

CTSNode CTSNode::NextNamedSibling() const { return {ts_node_next_named_sibling(*this)}; }
CTSNode CTSNode::PrevNamedSibling() const { return {ts_node_prev_named_sibling(*this)}; }

CTSNode CTSNode::FirstChildForByte(uint32_t loc) const { return {ts_node_first_child_for_byte(*this, loc)}; }
CTSNode CTSNode::FirstNamedChildForByte(uint32_t loc) const { return {ts_node_first_named_child_for_byte(*this, loc)}; }

CTSNode CTSNode::DescendantForByteRange(uint32_t start, uint32_t end) const { return {ts_node_descendant_for_byte_range(*this, start, end)
}; }
CTSNode CTSNode::DescendantForPointRange(TSPoint start, TSPoint end) const { return {ts_node_descendant_for_point_range(*this, start, end)
}; }

CTSNode CTSNode::NamedDescendantForByteRange(uint32_t start, uint32_t end) const { return {
    ts_node_named_descendant_for_byte_range(*this, start, end)
}; }
CTSNode CTSNode::NamedDescendantForPointRange(TSPoint start, TSPoint end) const { return {
    ts_node_named_descendant_for_point_range(*this, start, end)
}; }

void CTSNode::Edit(const TSInputEdit* edit) { ts_node_edit(this, edit); }



bool CTSNode::operator==(CTSNode other) const { return Eq(*this, other); }


