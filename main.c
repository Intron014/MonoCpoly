#include "common.h"
#include "basics.h"
#include "game.h"

int main() {
    srand(time(NULL));
    int cnt;
    char ans;
    FILE *file;

    reload:
    file = fopen("card.bin", "rb");
    if (file == NULL) {
        card_file_gen(0);
        file = fopen("card.bin", "rb");
        if (file == NULL) {
            perror("Error creating and reading card.bin");
            return 1;
        }
    }

    struct Card card[40];
    size_t num_read = fread(&card, sizeof(struct Card), 40, file);
    if (num_read != 40) {
        perror("Error reading file, file is probably corrupted");
        fclose(file);
        return 1;
    }
    fclose(file);

    for (cnt = 0; cnt < 40; cnt++) {
        printf("%i - %s %s loaded\n", cnt, cardTypeToString(card[cnt].type), card[cnt].nombre);
    }

    do {
        printf("\nWelcome to Monopoly\n----------\nd - Debug menu\ns - Start the game\nq - quit\n--> ");
        clearInputBuffer();
        scanf(" %c", &ans);
        clearInputBuffer();

        if (ans == 'd') {
            printf("Debug menu\n----------\ne - Edit card.bin\nr - Reload card.bin\nq - Exit debug menu\n--> ");
            clearInputBuffer();
            scanf(" %c", &ans);
            clearInputBuffer();
            if (ans == 'e') {
                card_file_gen(1);
            } else if (ans == 'r') {
                printf("Reloading card.bin\n");
                goto reload;
            } else if (ans == 'q') {
                printf("Exiting debug menu\n");
            } else {
                printf("Unknown command\n");
            }
        } else if (ans == 'q') {
            printf("Exiting\n");
        } else if (ans == 's') {
            monocpoly(card);
        } else {
            printf("Unknown command\n");
        }
    } while (ans != 'q');

    return 0;
}
