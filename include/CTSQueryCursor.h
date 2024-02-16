#pragma once

#include "api.h"
#include "CTSQuery.h"
#include "CTSNode.h"

class CTSQueryCursor
{
public:
    CTSQueryCursor(const CTSQueryCursor&) = delete;
    CTSQueryCursor(const CTSQueryCursor&&) = delete;
    CTSQueryCursor operator=(const CTSQueryCursor&) = delete;
    CTSQueryCursor operator=(const CTSQueryCursor&&) = delete;

    /**
     * Create a new cursor for executing a given query.
     *
     * The cursor stores the state that is needed to iteratively search
     * for matches. To use the query cursor, first call `CTSQueryCursor::Exec`
     * to start running a given query on a given syntax node. Then, there are
     * two options for consuming the results of the query:
     * 1. Repeatedly call `CTSQueryCursor::NextMatch` to iterate over all of the
     *    *matches* in the order that they were found. Each match contains the
     *    index of the pattern that matched, and an array of captures. Because
     *    multiple patterns can match the same set of nodes, one match may contain
     *    captures that appear *before* some of the captures from a previous match.
     * 2. Repeatedly call `TSQueryCursor::NextCapture` to iterate over all of the
     *    individual *captures* in the order that they appear. This is useful if
     *    don't care about which pattern matched, and just want a single ordered
     *    sequence of captures.
     *
     * If you don't care about consuming all of the results, you can stop calling
     * `TSQueryCursor::NextMatch` or `TSQueryCursor::NextCapture` at any point.
     *  You can then start executing another query on another node by calling
     *  `TSQueryCursor::Exec` again.
     */
    CTSQueryCursor();

    /**
     * Delete a query cursor, freeing all of the memory that it used.
     */
    ~CTSQueryCursor();

    /**
     * Start running a given query on a given node.
     */
    void Exec(const CTSQuery& query, CTSNode node) const;

    /**
     * Retrieve the maximum number of in-progress matches allowed by this query
     * cursor.
     *
     * Query cursors have an optional maximum capacity for storing lists of
     * in-progress captures. If this capacity is exceeded, then the
     * earliest-starting match will silently be dropped to make room for further
     * matches. This maximum capacity is optional by default, query cursors allow
     * any number of pending matches, dynamically allocating new space for them as
     * needed as the query is executed.
     */

    uint32_t MatchLimit() const;
    /**
     * Set the maximum number of in-progress matches allowed by this query cursor.
     *
     * Query cursors have an optional maximum capacity for storing lists of
     * in-progress captures. If this capacity is exceeded, then the
     * earliest-starting match will silently be dropped to make room for further
     * matches. This maximum capacity is optional by default, query cursors allow
     * any number of pending matches, dynamically allocating new space for them as
     * needed as the query is executed.
     */
    void SetMatchLimit(uint32_t limit) const;

    /**
     * Returns true if the maximum capacity of in-progress captures was exceeded,
     * false otherwise.
     *
     * Query cursors have an optional maximum capacity for storing lists of
     * in-progress captures. If this capacity is exceeded, then the
     * earliest-starting match will silently be dropped to make room for further
     * matches. This maximum capacity is optional by default, query cursors allow
     * any number of pending matches, dynamically allocating new space for them as
     * needed as the query is executed.
     */
    bool DidExceedMatchLimit() const;

    /**
     * Set the range of bytes positions in which the query will be executed.
     */
    void SetByteRange(uint32_t start, uint32_t end) const;

    /**
     * Set the range of (row, column) positions in which the query will be executed.
     */
    void SetPointRange(TSPoint start, TSPoint end) const;

    /**
     * TODO Add Documentation
     */
    void RemoveMatch(uint32_t id) const;

    /**
     * Advance to the next match of the currently running query.
     *
     * Returns 'true' if a match occurred, otherwise returns 'false'.
     *
     * Upon a successful match, the match can be retrieved by calling
     * the 'CTSQueryCursor::GetMatchResult' method.
     */
    bool NextMatch(); // Get result with GetMatchResult()

    /**
     * Advance to the next capture of the currently running query.
     *
     * Returns 'true' if a capture occurred, otherwise returns 'false'.
     *
     * Upon a successful capture, the match can be retrieved by calling
     * the 'CTSQueryCursor::GetMatchResult' method and its offset position
     * can be retrieved by calling 'CTSQueryCursor::GetCaptureIndex'.
     *
     */
    bool NextCapture(); // Get result w/ GetMatchResult() & GetCaptureIndex()

    /**
     * Holds the most recent result found in either the 'CTSQueryCursor:NextMatch'
     * or 'CTSQueryCursor::NextCapture' method.
     */
    TSQueryMatch GetMatchResult() const { return m_last_match; }
    /**
     * Holds the most recent capture offset found in the 'CTSQueryCursor::NextCapture' method.
     */
    uint32_t GetCaptureIndex() const { return m_last_index; }

    /**
     * Returns a reference to the original TSQueryCursor that this CTSQueryCursor object wraps.
     */
    TSQueryCursor& RawCursor() const { return *m_cursor; }

private:
    TSQueryMatch m_last_match{};
    uint32_t m_last_index = 0;

    TSQueryCursor* m_cursor;
};
