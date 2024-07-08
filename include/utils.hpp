#pragma once

#include <cstdint>
#include <WString.h>

void string2char(String in, char *out, int8_t offset_per_iter, uint8_t iter_cpt, int8_t nb_visible_chars);
