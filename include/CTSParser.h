#pragma once

#include "api.h"
#include "CTSLanguage.h"
#include "CTSTree.h"

#include <memory>
#include <string>
#include <vector>

struct CTSParser
{
public:
    CTSParser() = delete;
    CTSParser(const CTSParser&) = delete;
    CTSParser(const CTSParser&&) = delete;
    CTSParser operator= (const CTSParser&) = delete;
    CTSParser operator= (const CTSParser&&) = delete;

    /**
     * Create a new parser.If the language is not successfully loaded, the results
     * of using the parser are undefined.  Be sure to check CTSParser::LanguageSetResults()
     * to validate whether the parser is properly initialized.
     */
    CTSParser(const TSLanguage* language);

    /**
     * Delete the parser, freeing all of the memory that it used.
     */
    ~CTSParser();

    /** Returns the result of the constructor's attempt to set the language for parsing.
     *
     * True means the language assignment succeeded in the constructor. False
     * means there was a version mismatch: the language was generated with an
     * incompatible version of the Tree-sitter CLI. Check the language's version
     * using `CTSLanguage::Version()` and compare it to this library's
     * `TREE_SITTER_LANGUAGE_VERSION` and `TREE_SITTER_MIN_COMPATIBLE_LANGUAGE_VERSION`
     * constants, which can be found in CTSParser::LibraryLanguageVersion() and
     * CTSParser::MinCompatibleLanguageVersion();
     */
    bool LanguageSetResult() const { return m_set_lang_result; }

    /**
     * Get the parser's current language.
     */
    const CTSLanguage* Language() const;

    /**
     * Set the ranges of text that the parser should include when parsing.
     *
     * By default, the parser will always include entire documents. This method
     * allows you to parse only a *portion* of a document but still return a syntax
     * tree whose ranges match up with the document as a whole. You can also pass
     * multiple disjoint ranges.
     *
     * If the length of the ranges vector is zero or the vector is omitted,
     * then the entire document will be parsed. Otherwise, the given ranges must be
     * ordered from earliest to latest in the document, and they must not overlap.
     * That is, the following must hold for all
     * `i` < `(vector length) - 1`: ranges[i].end_byte <= ranges[i + 1].start_byte
     *
     * If this requirement is not satisfied, the operation will fail, the ranges
     * will not be assigned, and this method will return `false`. On success,
     * this method returns `true`
     */
    bool SetIncludedRanges(std::vector<TSRange> ranges = std::vector<TSRange>()) const;

    /**
     * Get the ranges of text that the parser will include when parsing.
     */
    std::vector<TSRange> IncludedRanges() const;

    /**
     * Use the parser to parse some source code and create a syntax tree.
     *
     * If you are parsing this document for the first time, pass nullptr to old_tree
     * or use the overloaded CTSParser::Parse(TSInput input) without a tree pointer.
     *  Otherwise, if you have already parsed an earlier version of this document and
     *  the document has since been edited, pass the previous syntax tree so that the
     *  unchanged parts of it can be reused.  This will save time and memory.
     *
     * For this to work correctly, you must have already edited the old syntax tree
     * using the `CTSTree::Edit()` method in a way that exactly matches the
     * source code changes.
     *
     * The `TSInput` parameter lets you specify how to read the text. It has the
     * following three fields:
     * 1. `read`: A function to retrieve a chunk of text at a given byte offset
     *    and (row, column) position. The function should return a pointer to the
     *    text and write its length to the `bytes_read` pointer. The parser does
     *    not take ownership of this buffer; it just borrows it until it has
     *    finished reading it. The function should write a zero value to the
     *    `bytes_read` pointer to indicate the end of the document.
     * 2. `payload`: An arbitrary pointer that will be passed to each invocation
     *    of the `read` function.
     * 3. `encoding`: An indication of how the text is encoded. Either
     *    `TSInputEncodingUTF8` or `TSInputEncodingUTF16`.
     *
     * This method returns a syntax tree on success, and `NULL` on failure. There
     * are three possible reasons for failure:
     * 1. The parser does not have a language assigned. Check for this using the
     * 	  `CTSParser::Language` method.
     * 2. Parsing was cancelled due to a timeout that was set by an earlier call to
     *    the `CTSParser::SetTimeoutMicros` method. You can resume parsing from
     *    where the parser left out by calling `CTSParser::Parse` again with the
     *    same arguments. Or you can start parsing from scratch by first calling
     *    `CTSParser::Reset`.
     * 3. Parsing was cancelled using a cancellation flag that was set by an
     *    earlier call to `CTSParser::SetCancellationFlag`. You can resume parsing
     *    from where the parser left out by calling `CTSParser::Parse` again with
     *    the same arguments.
     */
    std::shared_ptr<CTSTree> Parse(const std::shared_ptr<CTSTree>& old_tree, TSInput input) const;  

    /**
     * Use the parser to parse some source code and create an initial syntax tree.
     *
     * This is an overloaded method. See other entry for Parse() for full details.
     * */
    std::shared_ptr<CTSTree> Parse(TSInput input) const;

    /**
     * Use the parser to parse some source code stored in one contiguous buffer.
     * The old_tree parameter, when present, is the same as in the `TSParser::Parse` method
     * above. The second parameters is a string containing the contents to be parsed.
     */
    std::shared_ptr<CTSTree> ParseString(const std::shared_ptr<CTSTree>&, const std::string& str) const;  

    /**
     * Use the parser to parse a string containing source code and create an initial syntax tree.
     *
     * This is an overloaded method. See other entry for ParseString() for full details.
     * */
    std::shared_ptr<CTSTree> ParseString(const std::string& str) const;

#ifdef HANDLE_UTF
	TSTree* ParseStringEncoding(const char* str, uint32_t length, TSInputEncoding encoding)
	{
		return ParseStringEncoding(nullptr, str, length, encoding);
	};																												 
	TSTree* ParseStringEncoding(const TSTree* old_tree, const char* str, uint32_t length, TSInputEncoding encoding);  
#endif

    /**
     * Instruct the parser to start the next parse from the beginning.
     *
     * If the parser previously failed because of a timeout or a cancellation, then
     * by default, it will resume where it left off on the next call to
     * `CSTParser::Parse` or other parsing methods. If you don't want to resume,
     * and instead intend to use this parser to parse some other document, you must
     * call `CTSParser::Reset` first.
     */
    void Reset() const;

    /**
     * Set the maximum duration in microseconds that parsing should be allowed to
     * take before halting.
     *
     * If parsing takes longer than this, it will halt early, returning NULL.
     * See `CTSParser:Parse` for more information.
     */
    void SetTimeoutMicros(uint64_t timeout) const;

    /**
     * Get the duration in microseconds that parsing is allowed to take.
     */
    uint64_t TimeoutMicros() const;

    /**
     * Set the parser's current cancellation flag pointer.
     *
     * If a non-null pointer is assigned, then the parser will periodically read
     * from this pointer during parsing. If it reads a non-zero value, it will
     * halt early, returning NULL. See `CTSParser:Parse` for more information.
     */
    void SetCancellationFlag(const size_t* flag) const;

    /**
     * Get the parser's current cancellation flag pointer.
     */
    const size_t* CancellationFlag() const;

    /**
     * Set the logger that a parser should use during parsing.
     *
     * The parser does not take ownership over the logger payload. If a logger was
     * previously assigned, the caller is responsible for releasing any memory
     * owned by the previous logger.
     */
    void SetLogger(TSLogger logger) const;

    /**
     * Get the parser's current logger.
     */
    TSLogger Logger() const;

    /**
     * Set the file descriptor to which the parser should write debugging graphs
     * during parsing. The graphs are formatted in the DOT language. You may want
     * to pipe these graphs directly to a `dot(1)` process in order to generate
     * SVG output. You can turn off this logging by passing a negative number.
     */
    void PrintDotGraphs(int file) const;

    /** Returns the current language version contained in the library macro
     * TREE_SITTER_LANGUAGE_VERSION.  This is used in validating languages set
     * in the constructor.  See 'CTSParser::CTSParser()' for details.
     */
    static uint32_t LibraryLanguageVersion() { return TREE_SITTER_LANGUAGE_VERSION; }

    /** Returns the minimum compatible language version contained in the library
     * macro TREE_SITTER_MIN_COMPATIBLE_LANGUAGE_VERSION. This is used in validating
     *  languages set in the constructor.  See 'CTSParser::CTSParser()' for details.
     */
    static uint32_t MinCompatibleLanguageVersion() { return TREE_SITTER_MIN_COMPATIBLE_LANGUAGE_VERSION; }

private:
    bool SetLanguage(const TSLanguage* language);

    bool m_set_lang_result;

    TSParser* m_self = nullptr;
    CTSLanguage* m_lang;
};
