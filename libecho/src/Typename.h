#ifndef ECHOCONNECT_TYPENAME_H
#define ECHOCONNECT_TYPENAME_H

#include <QMetaEnum>
#include <QString>

#if (__GNUC__ && __cplusplus && __GNUC__ >= 3)
#include <cxxabi.h>

inline QString demangle(const char *name) {
    int status = 0;
    QString result = "";

    char *realname = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    realname == nullptr ? result = name : result = realname;
    // NOLINTNEXTLINE(cppcoreguidelines-no-malloc,-warnings-as-errors)
    free(realname);

    return result;
}
#else
inline QString demangle(const char *name) {
    return name;
}
#endif

template <class T>
QString type(const T &t) {
    return demangle(typeid(t).name());
}

template <typename QEnum>
std::string EnumName(const QEnum value) {
    return std::string(QMetaEnum::fromType<QEnum>().valueToKey(value));
}

#endif  // ECHOCONNECT_TYPENAME_H