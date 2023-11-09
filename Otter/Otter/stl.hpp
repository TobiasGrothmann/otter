#pragma once

// data
#include <set>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <memory>

// other
#include <math.h>
#include <algorithm>
#include <iostream>
#include <functional>
#include <utility>

// macro used to get std::vector<T>::size_type of variable or variable reference
#define VINDEX(vector) typename std::remove_reference<decltype(vector)>::type::size_type
#define VINDEX_TYPE(type) typename std::vector<type>::size_type

