#pragma once

// data
#include <set>
#include <vector>
#include <array>
#include <map>
#include <string>
#include <atomic>
#include <queue>

// other
#include <math.h>
#include <algorithm>
#include <iostream>
#include <unistd.h>
#include <chrono>

// boost
#include <boost/asio.hpp>
#include <boost/thread.hpp>

// macro used to get std::vector<T>::size_type of variable or variable reference
#define VINDEX(vector) typename std::remove_reference<decltype(vector)>::type::size_type
#define VINDEX_TYPE(type) typename std::vector<type>::size_type
