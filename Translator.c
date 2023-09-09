#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "MyBasics.h"

// The number of Wingdings characters.
#define SIZE_OF_WINGDINGS_PTR 94
#define WINGDINGS_START 33

#define DEL_CHAR_VALUE 127
int main(void)
{
    
    // ASCII mapped to their respective Wingding representation.
    // Used to translate from ASCII to Wingdings.
    const char *wingdings[] = {
        // Symbols 1 (!, ", #, $, %, &, ', (, ), *, +, ',' , -, ., /) (15 total)
        "✏︎", "✂︎", "✁︎", "👓︎", "🕭︎", "🕮︎", "🕯︎", "🕿︎", "✆︎", "🖂︎", "🖃︎", "📪︎",
        "📫︎", "📬︎", "📭︎",

        // Numerical characters (0-9) (10 total)
        "📁︎", "📂︎", "📄︎", "🗏︎", "🗐︎", "🗄︎", "⌛︎", "🖮︎", "🖰︎", "🖲︎",

        // Symbols 2 (:, ;, <, =, >, ?, @) (7 total)
        // '@' has no Wingding equivalent; it's just here for the sake of simplicity
        "🖳︎", "🖴︎", "🖫︎", "🖬︎", "✇︎", "✍︎", "@",

        // Uppercase alphabetical characters (A-Z) (26 total)
        "✌︎", "👌︎", "👍︎", "👎︎", "☜︎", "☞︎", "☝︎", "☟︎", "✋︎", "☺︎", "😐︎", "☹︎",
        "💣︎", "☠︎", "⚐︎", "🏱︎", "✈︎", "☼︎", "💧︎", "❄︎", "🕆︎", "✞︎", "🕈︎", "✠︎",
        "✡︎", "☪︎",

        // Symbols 3 ([, \, ], ^, _, `) (6 total)
        "☯︎", "ॐ︎", "☸︎", "☸︎", "♈︎", "♉︎",

        // Lowercase alphabetical characters (a-z) (26 total)
        "♋︎", "♌︎", "♍︎", "♎︎", "♏︎", "♐︎", "♑︎", "♒︎", "♓︎", "🙰", "🙵", "●︎",
        "❍︎", "■︎", "□︎", "◻︎", "❑︎", "❒︎", "⬧︎", "⧫︎", "◆︎", "❖︎", "⬥︎", "⌧︎",
        "⍓︎", "⌘︎",

        // Symbols 4 ({, |, }, ~) (4 total)
        "❀︎", "✿︎", "❝︎", "❞︎"};

    char *input;
    while (1)
    {
        input = NULL;
        // Input to be translated into Wingdings
        printf("Input a string to convert (CTRL-C to quit): ");
        const int inputlength = getStrStdin(&input, -1) - 1;

        // Outputting the Wingding counterpart to the user inputted string
        FILE *const out = fopen("out.txt", "w");
        for (int i = 0; i < inputlength; i++)
        {
            const unsigned char charDec = input[i];
            // The character's ASCII value should represent its respective Wingding character
            // within the pointer to array of strings, 'wingdings'.
            // Avoids trying to convert anything equal to a space's ASCII value and below, or above
            // the character DEL
            if (charDec > ' ' && charDec < DEL_CHAR_VALUE)
            {
                fprintf(out, wingdings[charDec - WINGDINGS_START]);
            }
            else
            {
                // If the character is not a Wingdings convertible ASCII character, print it back instead of
                // trying to convert it to its Wingding counterpart
                fprintf(out, "%c", charDec);
            }
        }
        free(input);
        fclose(out);
    }

    return 0;
}
