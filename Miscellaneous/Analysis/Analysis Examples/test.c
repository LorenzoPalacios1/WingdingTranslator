#include "../wdanalysis.h"

int main(void)
{
    const char ls = '\n';
    const char ws = ' ';
    print_wingdings(fopen(WINGDINGS_OUTPUT_FILENAME, "w"), ws);
    print_wingdings_sorted(fopen(SORTED_WINGDINGS_OUTPUT_FILENAME, "w"), ws);
    print_wingdings_bytes(fopen(WINGDINGS_BYTES_OUTPUT_FILENAME, "w"), ls, ws);
    print_wingdings_bytes_sorted(fopen(WINGDINGS_BYTES_SORTED_OUTPUT_FILENAME, "w"), ls, ws);

    return 0;
}
