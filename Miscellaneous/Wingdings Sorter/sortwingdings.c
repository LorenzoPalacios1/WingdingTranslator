/*
 * Used to generate a file containing a line of the given Wingdings in a sorted order
 * that can simply be copied and pasted as a pointer to an array of const char
 * (const char * const wingdings[]).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_WINGDINGS (sizeof(wingdings) / sizeof(*wingdings))

static const char *wingdings[] = {
    "✏︎", "✂︎", "✁︎", "👓︎", "🕭︎", "🕮︎", "🕯︎", "🕿︎", "✆︎", "🖂︎", "🖃︎", "📪︎", "📫︎", "📬︎", "📭︎", "📁︎", "📂︎", "📄︎", "🗏︎", "🗐︎", "🗄︎", "⌛︎", "🖮︎", "🖰︎", "🖲︎", "🖳︎", "🖴︎", "🖫︎", "🖬︎", "✇︎", "✍︎", "@", "✌︎", "👌︎", "👍︎", "👎︎", "☜︎", "☞︎", "☝︎", "☟︎", "✋︎", "☺︎", "😐︎", "☹︎", "💣︎", "☠︎", "⚐︎", "🏱︎", "✈︎", "☼︎", "💧︎", "❄︎", "🕆︎", "✞︎", "🕈︎", "✠︎", "✡︎", "☪︎", "☯︎", "ॐ︎", "☸︎", "☸︎", "♈︎", "♉︎", "♋︎", "♌︎", "♍︎", "♎︎", "♏︎", "♐︎", "♑︎", "♒︎", "♓︎", "🙰", "🙵", "●︎", "❍︎", "■︎", "□︎", "◻︎", "❑︎", "❒︎", "⬧︎", "⧫︎", "◆︎", "❖︎", "⬥︎", "⌧︎", "⍓︎", "⌘︎", "❀︎", "✿︎", "❝︎", "❞︎"};

void print_wingdings_bytes(void)
{
    FILE *out = fopen("sortedwingdings.txt", "w");
    fputs("static const char *const sorted_wingdings[] = ", out);
    for (size_t i = 0; i < NUM_WINGDINGS; i++)
        fprintf(out, "\"%s\", ", wingdings[i]);
    fputc('}', out);
    fputc(';', out);

    fclose(out);
}

int cmp(const void *const a, const void *const b)
{
    return strcmp(a, b);
}

int main(void)
{
    qsort(wingdings, NUM_WINGDINGS, sizeof(*wingdings), cmp);
    print_wingdings_bytes();
    return 0;
}
