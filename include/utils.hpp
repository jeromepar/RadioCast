#pragma once

#include <cstdint>
#include <string>

void string2char(std::string in, unsigned char *out, int8_t offset_per_iter, uint8_t iter_nb, int8_t nb_visible_chars);
