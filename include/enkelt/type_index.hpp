#pragma once
/// \file type_index.hpp
/// \author Johnathan Lee
/// \date 07/11/18
///
/// \brief A replacement for <typeindex> that gives "operators" for type_indexes.


#include <typeindex>

// for typeindex<T>()
template<typename T>
constexpr auto typeindex() {
    return std::type_index(typeid (T));
}

// for typeindex(variable)
template<typename T>
constexpr auto typeindex(const T&) {
    return std::type_index(typeid (T));
}
