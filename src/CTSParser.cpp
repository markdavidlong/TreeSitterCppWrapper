// ReSharper disable CppClangTidyClangDiagnosticShorten64To32
#include "CTSParser.h"

using namespace std;


CTSParser::CTSParser(const TSLanguage *language) : m_lang(nullptr)
{
    m_self            = ts_parser_new();
    m_set_lang_result = SetLanguage(language);
}

CTSParser::~CTSParser()
{
    if (m_self)
    {
        ts_parser_delete(m_self);
        m_self = nullptr;
    }

    if (m_lang)
    {
        delete m_lang;
        m_lang = nullptr;
    }
}

bool CTSParser::SetLanguage(const TSLanguage *language)
{
    const auto retval = ts_parser_set_language(m_self, language);

    m_lang = new CTSLanguage(ts_parser_language(m_self));

    return retval;
}

const CTSLanguage * CTSParser::Language() const
{
    return m_lang;
}

bool CTSParser::SetIncludedRanges(std::vector<TSRange>ranges) const
{
    const TSRange *items = ranges.data();

    return ts_parser_set_included_ranges(m_self, items, ranges.size());
}

std::vector<TSRange>CTSParser::IncludedRanges() const
{
    auto retval       = std::vector<TSRange>();
    uint32_t   len    = 0;
    const auto ranges = ts_parser_included_ranges(m_self, &len);

    for (uint32_t idx = 0; idx < len; idx++)
    {
        retval.push_back(ranges[idx]);
    }
    return retval;
}

std::shared_ptr<CTSTree>CTSParser::Parse(const std::shared_ptr<CTSTree>& /*old_tree*/,
                                         TSInput input) const
{
    return std::make_shared<CTSTree>(ts_parser_parse(m_self,
                                                     /*old_tree->GetEncapsulatedTSTree()*/ nullptr,
                                                     input));
}

std::shared_ptr<CTSTree>CTSParser::Parse(TSInput input) const
{
    return std::make_shared<CTSTree>(ts_parser_parse(m_self, nullptr, input));
}

std::shared_ptr<CTSTree>CTSParser::ParseString(const std::string& str) const
{
    return std::make_shared<CTSTree>(ts_parser_parse_string(m_self,
                                                            nullptr,
                                                            str.c_str(),
                                                            str.length()));
}

std::shared_ptr<CTSTree>CTSParser::ParseString(const std::shared_ptr<CTSTree>& old_tree,
                                               const std::string             & str) const
{
    return make_shared<CTSTree>(ts_parser_parse_string(m_self,
                                                       old_tree->m_tree,
                                                       str.c_str(),
                                                       str.length()));
}

#ifdef HANDLE_UTF
TSTree * CTSParser::ParseStringEncoding(const TSTree   *old_tree,
                                        const char     *str,
                                        uint32_t        length,
                                        TSInputEncoding encoding)
{
    return ts_parser_parse_string_encoding(m_self, old_tree, str, length, encoding);
}

#endif // ifdef HANDLE_UTF

void CTSParser::Reset() const
{
    ts_parser_reset(m_self);
}

void CTSParser::SetTimeoutMicros(uint64_t timeout) const
{
    ts_parser_set_timeout_micros(m_self, timeout);
}

uint64_t CTSParser::TimeoutMicros() const
{
    return ts_parser_timeout_micros(m_self);
}

void CTSParser::SetCancellationFlag(const size_t *flag) const
{
    ts_parser_set_cancellation_flag(m_self, flag);
}

const size_t * CTSParser::CancellationFlag() const
{
    return ts_parser_cancellation_flag(m_self);
}

void CTSParser::SetLogger(TSLogger logger) const
{
    ts_parser_set_logger(m_self, logger);
}

TSLogger CTSParser::Logger() const
{
    return ts_parser_logger(m_self);
}

void CTSParser::PrintDotGraphs(int file) const
{
    ts_parser_print_dot_graphs(m_self, file);
}
