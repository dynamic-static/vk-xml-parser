
/*
==========================================
  Copyright (c) 2020 Dynamic_Static
    Patrick Purcell
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#pragma once

#include "dynamic_static/core/defines.hpp"

#include <map>
#include <ostream>
#include <string>
#include <utility>

namespace dst {
namespace vk {
namespace xml {

/**
TODO : Documentation
*/
class Element
{
public:
    template <typename ElementType>
    class Manifest;

    /**
    TODO : Documentation
    */
    virtual ~Element() = 0;

    std::string name;         //!< TODO : Documentation
    std::string compileGuard; //!< TODO : Documentation
    std::string vendor;       //!< TODO : Documentation
    std::string alias;        //!< TODO : Documentation
    std::string extension;    //!< TODO : Documentation
};

inline Element::~Element()
{
}

/**
TODO : Documentation
*/
template <typename ElementType>
class Element::Manifest
    : public std::map<std::string, ElementType>
{
public:
    /**
    TODO : Documentation
    */
    using base = std::map<std::string, ElementType>;

    /**
    Constructs an instance of manifest
    */
    Manifest() = default;

    /**
    Destroys this instance of Element::Manifest
    */
    virtual ~Manifest() = 0;

    /**
    TODO : Documentation
    */
    inline void insert(const ElementType& element)
    {
        if (!element.name.empty()) {
            base::insert({ element.name, element });
        }
    }
};

template <typename ElementType>
inline Element::Manifest<ElementType>::~Manifest()
{
}

} // namespace xml
} // namespace vk
} // namespace dst
