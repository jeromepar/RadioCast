#include "utils.hpp"

void string2char(std::string in, unsigned char *out, int8_t offset_per_iter, uint8_t iter_nb, int8_t nb_visible_chars)
{
    uint8_t nb_slices = (in.size() - nb_visible_chars) / offset_per_iter;

    uint8_t current_slice_nb = iter_nb % nb_slices;

    uint8_t i;
    for (i = 0; (i < nb_visible_chars) && ((current_slice_nb * offset_per_iter+i)<in.size()); i++)
    {
        out[i] = in[current_slice_nb * offset_per_iter+i];
    }
    out[nb_visible_chars+1]=0;//end
}