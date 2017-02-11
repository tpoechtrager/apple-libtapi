/* gcc 4.7 does not have std::map::emplace */

#define STD_MAP_EMPLACE(KEY_TYPE, VALUE_TYPE, set, _first, _second) \
({ \
    std::pair<decltype(set)::iterator, bool> ret; \
    bool f = false; \
    for (decltype(set)::iterator it = set.begin(); it != set.end(); ++it) if (it->first == _first) \
    { \
        ret = std::pair<decltype(set)::iterator, bool>(it, false); \
        f = true; \
        break; \
    } \
    if (!f) ret = set.insert(std::pair<KEY_TYPE, VALUE_TYPE>(_first, _second)); \
    ret; \
})
