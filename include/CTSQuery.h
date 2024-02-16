#pragma once

#include "api.h"
#include <string>
#include <vector>

#include "CTSLanguage.h"


struct  CTSQuery
{
public:

    CTSQuery() = delete;
    CTSQuery(const CTSQuery&) = delete;
    CTSQuery(const CTSQuery&&) = delete;
    CTSQuery operator= (const CTSQuery&) = delete;
    CTSQuery operator= (const CTSQuery&&) = delete;

    /**
     * Create a new query from a string containing one or more S-expression
     * patterns. The query is associated with a particular language, and can
     * only be run on syntax nodes parsed with that language.
     *
     * If all of the given patterns are valid, 'CTSQuery::IsValid' will be set
     * to true, otherwise it will be set to false and the query will not be usable.
     *
     * If a pattern is invalid, information about the problem can be found by
     * looking at the 'CTSQuery::GetErrorOffset'  and the 'CTSQuery::GetErrorCode'
     * methods.
     */

    CTSQuery(const CTSLanguage* lang, const char* oldstr);

    // CTSQuery(const CTSLanguage *lang, std::string &pattern);

    /**
     * Delete a query, freeing all of the memory that it used.
     */
    ~CTSQuery();

    /**
     * Get the number of patterns in the query.
     */
    uint32_t PatternCount() const;

    /**
     * Get the number of captures in the query.
     */
    uint32_t CaptureCount() const;

    /**
     * Get the number of string literals in the query.
     */
    uint32_t StringCount() const;

    /**
     * Get the byte offset where the given pattern starts in the query's source.
     *
     * This can be useful when combining queries by concatenating their source
     * code strings.
     */
    uint32_t StartByteForPattern(uint32_t offset) const;

    /**
     * Get all of the predicates for the given pattern in the query.
     *
     * The predicates are represented as vector of steps. There are three
     * types of steps in this array, which correspond to the three legal values for
     * the `type` field:
     * - `TSQueryPredicateStepTypeCapture` - Steps with this type represent names
     *    of captures. Their `value_id` can be used with the
     *   `CTSQuery::CaptureNameForId` method to obtain the name of the capture.
     * - `TSQueryPredicateStepTypeString` - Steps with this type represent literal
     *    strings. Their `value_id` can be used with the
     *    `CTSQuery::StringValueForId` method to obtain their string value.
     * - `TSQueryPredicateStepTypeDone` - Steps with this type are *sentinels*
     *    that represent the end of an individual predicate. If a pattern has two
     *    predicates, then there will be two steps with this `type` in the array.
     */
    std::vector<TSQueryPredicateStep> PredicatesForPattern(uint32_t pattern_index) const;

    /**
     *  TODO Add documentation
     */
    bool IsPatternRooted(uint32_t pattern_index) const;

    /**
     *  TODO Add documentation
     */
    bool IsPatternGuaranteedAtStep(uint32_t byte_offset) const;

    /**
     * Get the name of one of the query's captures, or one of the query's string
     * literals. Each capture and string is associated with a numeric id based
     * on the order that it appeared in the query's source.
     */
    std::string CaptureNameForId(uint32_t id) const;

    /**
     * Get the quantifier of one of the query's captures. Each capture is associated
     * with a numeric id based on the order that it appeared in the query's source.
     */
    TSQuantifier CaptureQuantifierForId(uint32_t pattern_id, uint32_t capture_id) const;

    /**
     * Get the string value of one of the query's captures. Each capture is associated
     * with a numeric id based on the order that it appeared in the query's source.
     */
    std::string StringValueForId(uint32_t id) const;

    /**
     * Disable a certain capture within a query.
     *
     * This prevents the capture from being returned in matches, and also avoids
     * any resource usage associated with recording the capture. Currently, there
     * is no way to undo this.
     */
    void DisableCapture(const std::string& name) const;

    /**
     * Disable a certain pattern within a query.
     *
     * This prevents the pattern from matching and removes most of the overhead
     * associated with the pattern. Currently, there is no way to undo this.
     */
    void DisablePattern(uint32_t pattern_index) const;

    /**
     * Returns true if the query was constructed with a valid pattern, false otherwise.
     */
    bool IsValid() const { return m_query != nullptr; }

    /**
     * Returns the byte offset of the error when the constructor has been
     * called with an invalid pattern.
     */
    uint32_t GetErrorOffset() const { return m_error_offset; }

    /**
     * Returns the type of error when the constructor has been  called with
     *  an invalid pattern.
     */
    TSQueryError GetErrorCode() const { return m_error_code; }


    static std::string DecodeQueryError(TSQueryError errnum)
    {
        switch (errnum)
        {
        case TSQueryErrorNone: return { "No Error (TSQueryErrorNone)" };
        case TSQueryErrorSyntax: return { "Syntax Error (TSQueryErrorSyntax)" };
        case TSQueryErrorNodeType: return { "NodeType Error (TSQueryErrorNodeType)" };
        case TSQueryErrorField: return { "Field Error (TSQueryErrorField)" };
        case TSQueryErrorCapture: return { "Capture Error (TSQueryErrorCapture)" };
        case TSQueryErrorStructure: return { "Structure Error (TSQueryErrorStructure)" };
        case TSQueryErrorLanguage: return { "Language Error (TSQueryErrorLanguage)" };
        }
        return {};
    }


    std::string DecodeLastErrorCode() const { return DecodeQueryError(GetErrorCode()); }


    friend class CTSQueryCursor;

    TSQuery* RawQuery() const { return m_query; }
private:
    uint32_t m_error_offset = 0;
    TSQueryError m_error_code = TSQueryErrorNone;
    TSQuery* m_query = nullptr;
};
