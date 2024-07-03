#include "utils.hpp"
#include "defines.h"
#include <math.h>

// #include <Arduino.h>

void string2char(String in, char *out, int8_t offset_per_iter, uint8_t iter_cpt, int8_t nb_visible_chars)
{
    // char dbg[500];
    // sprintf(dbg, "Start iter_cpt %d; nb_char in %d; nb_char_visibles %d", iter_cpt, in.length(), nb_visible_chars);
    // Serial.println(dbg);

    uint8_t nb_iter_fixed = 3;

    uint8_t nb_slices = ceil(MAX((int8_t)in.length() - nb_visible_chars, 0) / (float)offset_per_iter) + 1;
    if (nb_slices>1)
        nb_slices+=2; //to scroll a bit past

    uint8_t current_slice_nb = iter_cpt % (nb_slices + nb_iter_fixed);

    if (current_slice_nb < nb_iter_fixed)
    {
        current_slice_nb = 0;
    }
    else
    {
        current_slice_nb -= nb_iter_fixed;
    }
    // sprintf(dbg, "Current slice %d / %d", current_slice_nb, nb_slices);
    // Serial.println(dbg);

    // sprintf(dbg, "first index %d", current_slice_nb * offset_per_iter);
    // Serial.println(dbg);

    uint8_t i;
    for (i = 0; (i < nb_visible_chars) && ((current_slice_nb * offset_per_iter + i) < in.length()); i++)
    {
        out[i] = in[current_slice_nb * offset_per_iter + i];
    }

    out[i] = 0; // end
}