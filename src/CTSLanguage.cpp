// ReSharper disable CppClangTidyClangDiagnosticShorten64To32
#include "CTSLanguage.h"


CTSLanguage::CTSLanguage(const TSLanguage *lang)
{
    m_language = lang;
}

inline uint32_t CTSLanguage::SymbolCount() const
{
    return m_language ? ts_language_symbol_count(m_language) : 0;
}

std::string CTSLanguage::SymbolName(TSSymbol symbol)  const
{
    return m_language ? std::string(ts_language_symbol_name(m_language,
                                                            symbol)) :
           "ERROR: No Language Initialized!";
}

TSSymbol CTSLanguage::SymbolForName(const std::string& name, bool is_named) const
{
    return m_language ? ts_language_symbol_for_name(m_language,
                                                    name.c_str(),
                                                    name.length(),
                                                    is_named) : 0;
}

inline uint32_t CTSLanguage::FieldCount()  const
{
    return m_language ? ts_language_field_count(m_language) : 0;
}

inline std::string CTSLanguage::FieldNameForId(TSFieldId id)  const
{
    return m_language ? std::string(ts_language_field_name_for_id(m_language,
                                                                  id)) :
           "Error: No Language Initialized!";
}

TSFieldId CTSLanguage::FieldIdForName(const std::string& name) const
{
    return m_language ? ts_language_field_id_for_name(m_language, name.c_str(), name.length()) : 0;
}

TSSymbolType CTSLanguage::SymbolType(TSSymbol symbol) const
{
    return m_language ? ts_language_symbol_type(m_language, symbol) : TSSymbolTypeRegular;
}
