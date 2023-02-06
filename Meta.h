#include <type_traits>

namespace MBUtility
{
    template<template<typename...> class, typename...>
    struct IsInstantiation : public std::false_type {};

    template<template<typename...> class T, typename... I>
    struct IsInstantiation<T, T<I...>> : public std::true_type {};
};
