#include "CTSQueryCursor.h"

CTSQueryCursor::CTSQueryCursor()
{
	m_cursor = ts_query_cursor_new();
}

CTSQueryCursor::~CTSQueryCursor()
{
	ts_query_cursor_delete(m_cursor);
}

void CTSQueryCursor::Exec(const CTSQuery& query, CTSNode node) const
{
	const TSQuery* q = query.RawQuery();
	ts_query_cursor_exec(m_cursor, q, node);
}

bool CTSQueryCursor::DidExceedMatchLimit() const
{
	return ts_query_cursor_did_exceed_match_limit(m_cursor);
}

uint32_t CTSQueryCursor::MatchLimit() const
{
	return ts_query_cursor_match_limit(m_cursor);
}

void CTSQueryCursor::SetMatchLimit(uint32_t limit) const
{
	ts_query_cursor_set_match_limit(m_cursor, limit);
}

void CTSQueryCursor::SetByteRange(uint32_t start, uint32_t end) const
{
	ts_query_cursor_set_byte_range(m_cursor, start, end);
}

void CTSQueryCursor::SetPointRange(TSPoint start, TSPoint end) const
{
	ts_query_cursor_set_point_range(m_cursor, start, end);
}

bool CTSQueryCursor::NextMatch()
{
	return ts_query_cursor_next_match(m_cursor, &m_last_match);
}

void CTSQueryCursor::RemoveMatch(uint32_t id) const
{
	ts_query_cursor_remove_match(m_cursor, id);
}

bool CTSQueryCursor::NextCapture()
{
	return ts_query_cursor_next_capture(m_cursor, &m_last_match, &m_last_index);
}
