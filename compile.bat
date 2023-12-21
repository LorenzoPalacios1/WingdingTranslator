clang -fstack-protector-strong -Og -pedantic -Wall -Werror -Wextra -Wfloat-equal -Wformat=2 -Wimplicit-atomic-properties -Wmissing-declarations -Wmissing-prototypes -Woverlength-strings -Wshadow -D_FORTIFY_SOURCE=2 Translator.c "C-MyBasics\MyBasics.c" -o Translator

clang -fstack-protector-strong -Og -pedantic -Wall -Werror -Wextra -Wfloat-equal -Wformat=2 -Wimplicit-atomic-properties -Wmissing-declarations -Wmissing-prototypes -Woverlength-strings -Wshadow -D_FORTIFY_SOURCE=2 "Miscellaneous\wdanalysis.h" -o analysis
