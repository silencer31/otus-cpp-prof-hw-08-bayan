#pragma once

#include <vector>
#include <string>
#include <list>

using ui_vector = std::vector<uint8_t>; // One hash block

using hb_list_vector = std::list<ui_vector>; // List of hash blocks

using fp_list = std::list<std::string>; // List of file paths

using str_vector = std::vector<std::string>;

using str_pair = std::pair<std::string, std::string>;
