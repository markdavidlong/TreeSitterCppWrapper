// ReSharper disable CppClangTidyClangDiagnosticShorten64To32
#include "CTSQuery.h"
#include "CTSLanguage.h"

CTSQuery::CTSQuery(const CTSLanguage* lang, const char* oldstr)
{
    const auto source = std::string(oldstr);
	m_query = ts_query_new(lang->GetTSLanguage(), source.c_str(), static_cast<uint32_t>(source.length()), &m_error_offset, &m_error_code);
}


//CTSQuery::CTSQuery(const CTSLanguage *lang, std::string &source)
//{
//    m_query = ts_query_new(lang->GetTSLanguage(), source.c_str(), source.length(), &m_error_offset, &m_error_code);
//}

CTSQuery::~CTSQuery()
{
	if (m_query)
		ts_query_delete(m_query);
}

uint32_t CTSQuery::PatternCount() const
{
	if (m_query)
	{
		return ts_query_pattern_count(m_query);
	}
	else
	{
		return 0;
	}
}

uint32_t CTSQuery::CaptureCount() const
{
	if (m_query)
	{
		return ts_query_capture_count(m_query);
	}
	else
	{
		return 0;
	}
}

uint32_t CTSQuery::StringCount() const
{
	if (m_query)
	{
		return ts_query_string_count(m_query);
	}
	else
	{
		return 0;
	}
}

uint32_t CTSQuery::StartByteForPattern(uint32_t offset) const
{
	if (m_query)
	{
		return ts_query_start_byte_for_pattern(m_query, offset);
	}
	else
	{
		return 0;
	}
}

std::vector<TSQueryPredicateStep> CTSQuery::PredicatesForPattern(uint32_t pattern_index) const
{
	std::vector<TSQueryPredicateStep> retval;
	uint32_t count = 0;
    const auto results = ts_query_predicates_for_pattern(m_query, pattern_index, &count);
	for (uint32_t i = 0; i < count; i++)
	{
		retval.push_back(results[i]);
	}
	return retval;
}

bool CTSQuery::IsPatternRooted(uint32_t pattern_index) const
{
	if (m_query)
	{
		return ts_query_is_pattern_rooted(m_query, pattern_index);
	}
	else
	{
		return false;
	}
}

bool CTSQuery::IsPatternGuaranteedAtStep(uint32_t byte_offset) const
{
	if (m_query)
	{
		return ts_query_is_pattern_guaranteed_at_step(m_query, byte_offset);
	}
	else
	{
		return false;
	}
}

std::string CTSQuery::CaptureNameForId(uint32_t id) const
{
	std::string retval;
	if (m_query)
	{
		uint32_t size = 0;
        const auto tmp = ts_query_capture_name_for_id(m_query, id, &size);
		retval = retval.assign(tmp, size);
	}
	return retval;
}

TSQuantifier CTSQuery::CaptureQuantifierForId(uint32_t pattern_id, uint32_t capture_id) const
{
	if (m_query)
	{
		return ts_query_capture_quantifier_for_id(m_query, pattern_id, capture_id);
	}
	else
	{
		return TSQuantifier::TSQuantifierZero;
	}
}

std::string CTSQuery::StringValueForId(uint32_t id) const
{
	std::string retval;
	if (m_query)
	{
		uint32_t size = 0;
        const auto tmp = ts_query_string_value_for_id(m_query, id, &size);
		retval = retval.assign(tmp, size);
	}
	return retval;
}

void CTSQuery::DisableCapture(const std::string& name) const
{
	if (m_query)
	{
		ts_query_disable_capture(m_query, name.c_str(), static_cast<uint32_t>(name.length()));
	}
}

void CTSQuery::DisablePattern(uint32_t pattern_index) const
{
	if (m_query)
	{
		ts_query_disable_pattern(m_query, pattern_index);
	}
}
