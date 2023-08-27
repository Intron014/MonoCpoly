#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define HAS_OWNER_DEFAULT -1
#define IS_MORTGAGED_DEFAULT 0
#define HOUSE_DEFAULT 0

//TODO: Fix the double write error
//TODO:

enum CardType {
    PROPERTY,
    TRAIN_STATION,
    SPECIAL,
    NULLIVER
};

enum PropertyColor {
    BROWN,
    LBLUE,
    PINK,
    ORANGE,
    RED,
    YELLOW,
    GREEN,
    DBLUE
};

const char* cardTypeToString(enum CardType type) {
    switch (type) {
        case PROPERTY:
            return "Property";
        case TRAIN_STATION:
            return "Train Station";
        case SPECIAL:
            return "Special";
        case NULLIVER:
            return "NULL";
        default:
            return "Unknown Type";
    }
}
const char* colorToString(enum PropertyColor color){
    switch (color) {
        case BROWN: return "Brown";
        case LBLUE: return "Light Blue";
        case PINK: return "Pink";
        case ORANGE: return "Orange";
        case RED: return "Red";
        case YELLOW: return "Yellow";
        case GREEN: return "Green";
        case DBLUE: return "Dark Blue";
    }
}
union CardProperties {
    struct Property {
        int has_owner;
        int precio;
        enum PropertyColor color;
        int mortgage;
        int is_mortgaged;
        int houses; // 0 - No house // 1 to 4 - House // 5 - Hotel // 
        int precioXcasa;
        int rent[6]
    } Property;
    struct Train_station{ 
    /* The prices will be determined by the 
    train station flow, looking at the flags
    found on the players */
        int has_owner;
    } Train_station;
    struct Special{
    /*
    0 - Salida
    1 - Carcel (Visita) [if here check in_jail player flag]
    2 - Suerte
    3 - Caja de la comunidad
    4 - Impuesto pre-salida
    5 - Impuesto pos-salida
    6 - Go to jail
    7 - Parking gratuito
    8 - Servicio de agua
    9 - Servicio electrico
    */
        int type;
    } Special;
};

struct Card {
    enum CardType type;
    union CardProperties properties;
    char nombre[50];
};

struct Ccandl {
    int type; // 0 - Suerte // 1 - Caja de la Comunidad //
    char description[250];
} Ccandl;

struct Players { // Players
    char nombre[50];
    int money;
    int pos;
    // Flags
    bool in_jail;
    bool has_water;
    bool has_electric;
    bool is_bankrupt;
} Players;
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
int random_number(int max, int low){
    int result = (rand() % (max - low + 1)) + low;
    return result;
}
int card_file_gen(int why) {
    // why 0 - file not found // why 1 - edit struct //
    struct Card card[40];
    int cnt, aux;
    char ans;
    if(why==0){
        printf("File not found, opening editor\n");
        for(cnt = 0; cnt<40 ; cnt++){
            strcpy(card[cnt].nombre, "NULL");
            card[cnt].type = NULLIVER;
        }
    }
    else {
        FILE *file = fopen("card.bin", "rb");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        };

        size_t num_read = fread(&card, sizeof(struct Card), 40, file);
        if (num_read != 40) {
            perror("Error reading file");
            fclose(file);
            return 1;
        };

        fclose(file);
    }
    printf("Master card.bin Editor\n");
    for(cnt = 0; cnt<40 ; cnt++){
        printf("\nEditando la casilla %i\nNombre: %s\nTipo: %s\n",cnt, card[cnt].nombre, cardTypeToString(card[cnt].type));
        if(card[cnt].type == PROPERTY && strcmp(card[cnt].nombre, "NULL")!=0){
            printf("Precio: %i\nMortgage: %i\nColor: %s\nPrecio por casa: %i\nAlquileres: %i %i %i %i %i %i\n",
                   card[cnt].properties.Property.precio, card[cnt].properties.Property.mortgage, colorToString(card[cnt].properties.Property.color),
                   card[cnt].properties.Property.precioXcasa, card[cnt].properties.Property.rent[0], card[cnt].properties.Property.rent[1],
                   card[cnt].properties.Property.rent[2], card[cnt].properties.Property.rent[3], card[cnt].properties.Property.rent[4],
                   card[cnt].properties.Property.rent[5]);
        }
        printf("Introduzca el tipo de casilla\n1 - Propiedad\n2 - Estacion de tren\n3 - Especial");
        if(card[cnt].type == PROPERTY || card[cnt].type == TRAIN_STATION && strcmp(card[cnt].nombre, "NULL")!=0){
            printf("\n4 - Edit name");
        }
        printf("\n+ - Skip\n! - Skip somewhere\n* - Exit\n--> ");
            clearInputBuffer();
            scanf("%c", &ans);
        if(ans=='+') {
            printf("Skipping\n");
            continue;
        } else if(ans=='!') {
            int skip;
            printf("Skip to somewhere? (number/n): ");
            fflush(stdin);
            scanf("%i", &skip);
            if(skip!='n') {
                cnt=skip-1;
            }
        } else if(ans=='*') {
            printf("Exiting\n");
            break;
        } else if(ans=='1') {
             card[cnt].type = PROPERTY;
             card[cnt].properties.Property.has_owner = HAS_OWNER_DEFAULT;
             card[cnt].properties.Property.is_mortgaged = IS_MORTGAGED_DEFAULT;
             card[cnt].properties.Property.houses = HOUSE_DEFAULT;
             printf("Nombre: ");
             fflush(stdin);
             scanf(" %49[^\n]", card[cnt].nombre);
             printf("Precio: ");
             fflush(stdin);
             scanf("%i", &card[cnt].properties.Property.precio);
             printf("Mortgage: ");
             fflush(stdin);
             scanf("%i", &card[cnt].properties.Property.mortgage);
             do {
                 printf("Color: ");
                 fflush(stdin);
                 scanf("%i", &aux);
             } while (aux < 0 || aux > 7);
             switch (aux) {
                 case 0: card[cnt].properties.Property.color = BROWN; break;
                 case 1: card[cnt].properties.Property.color = LBLUE; break;
                 case 2: card[cnt].properties.Property.color = PINK; break;
                 case 3: card[cnt].properties.Property.color = ORANGE; break;
                 case 4: card[cnt].properties.Property.color = RED; break;
                 case 5: card[cnt].properties.Property.color = YELLOW; break;
                 case 6: card[cnt].properties.Property.color = GREEN; break;
                 case 7: card[cnt].properties.Property.color = DBLUE; break;
             };
             printf("Precio por casa: ");
             fflush(stdin);
             scanf("%i", &card[cnt].properties.Property.precioXcasa);
             for (aux = 0; aux < 6; aux++) {
                 printf("Alquiler %i: ", aux);
                 fflush(stdin);
                 scanf("%i", &card[cnt].properties.Property.rent[aux]);
             };
         } else if(ans=='2'){
            card[cnt].type = TRAIN_STATION;
            card[cnt].properties.Train_station.has_owner = HAS_OWNER_DEFAULT;
            printf("Nombre: ");
            fflush(stdin);
            scanf(" %49[^\n]", card[cnt].nombre);
         } else if(ans=='3'){
            card[cnt].type = SPECIAL;
            printf("Editando la casilla especial %i\nIntroduzca el tipo de casilla\n0 - Salida\n1 - Carcel (Visita)\n2 - Suerte\n3 - Caja de la comunidad\n4 - Impuesto pre-salida\n5 - Impuesto pos-salida\n6 - Go to jail\n7 - Parking gratuito\n8 - Agua\n9 - Electricidad\n--> ", cnt);
            scanf("%i", &card[cnt].properties.Special.type);
            fflush(stdin);
             switch (card[cnt].properties.Special.type) {
                 case 0: strcpy(card[cnt].nombre, "Salida"); break;
                 case 1: strcpy(card[cnt].nombre, "Carcel (Visita)"); break;
                 case 2: strcpy(card[cnt].nombre, "Suerte"); break;
                 case 3: strcpy(card[cnt].nombre, "Caja de la comunidad"); break;
                 case 4: strcpy(card[cnt].nombre, "Impuesto pre-salida"); break;
                 case 5: strcpy(card[cnt].nombre, "Impuesto pos-salida"); break;
                 case 6: strcpy(card[cnt].nombre, "Go to jail"); break;
                 case 7: strcpy(card[cnt].nombre, "Parking gratuito"); break;
                 case 8: strcpy(card[cnt].nombre, "Servicio de agua"); break;
                 case 9: strcpy(card[cnt].nombre, "Servicio electrico"); break;
             };
        } else if(ans=='4'){
            printf("Nombre: ");
            fflush(stdin);
            scanf(" %49[^\n]", card[cnt].nombre);
        };
    };

    FILE *file = fopen("card.bin", "wb");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    };

    size_t num_written = fwrite(&card, sizeof(struct Card), 40, file);
    if (num_written != 40) {
        perror("Error writing to file");
        fclose(file);
        return 1;
    };

    fclose(file);

    printf("Card data saved to card.bin\n");
    return 0;
};

int monocpoly(struct Card card[40]){
    int pcount;
    do{
        printf("How many players are here today? (2 - 7) --> ");
        scanf("%i", &pcount);
        if(pcount<2 || pcount>8){
            printf("Invalid number of players\n");
        }
    } while(pcount<2 || pcount>8);
    struct Players players[pcount];
    int player_non_bankrupt = pcount;
    for(int cnt=0 ; cnt<pcount ; cnt++){
        printf("Nombre del jugador %i: ", cnt+1);
        fflush(stdin);
        scanf(" %49[^\n]", players[cnt].nombre);
        players[cnt].money = 1500;
        players[cnt].pos = 0;
        players[cnt].in_jail = false;
        players[cnt].has_water = false;
        players[cnt].has_electric = false;
    };
    int order[pcount];
    do{ // Main game loop
        // Main loop
        for(int cnt=0 ; cnt<pcount ; cnt++){
            order[cnt] = cnt;
        };
        for(int cnt=0 ; cnt<pcount ; cnt++){
            int aux;
            int random = random_number(pcount-1, 0);
            aux = order[cnt];
            order[cnt] = order[random];
            order[random] = aux;
        };
        for(int cnt=0 ; cnt<pcount ; cnt++){
            printf("%i - %s\n", cnt+1, players[order[cnt]].nombre);
        };
        printf("----------\n");
        int turn = 0;
        int dice1, dice2;

    } while(player_non_bankrupt>1);

    return 0;
}

int main(){
    srand(time(NULL));
    int cnt;
    char ans;
    FILE *file;
    reload:
    file = fopen("card.bin", "rb");
    if (file == NULL) {
        card_file_gen(0);
    };
    struct Card card[40];
    size_t num_read = fread(&card, sizeof(struct Card), 40, file);
    if (num_read != 40) {
        perror("Error reading file, file is probably corrupted");
        fclose(file);
        return 1;
    };
    for(cnt=0 ; cnt<40 ; cnt++){
        printf("%i - %s %s loaded\n", cnt, cardTypeToString(card[cnt].type), card[cnt].nombre);
    }
    do {
        printf("\nWelcome to Monopoly\n----------\nd - Debug menu\ns - Start the game\nq - quit\n--> ");
        clearInputBuffer();
        scanf("%c", &ans);
        if (ans == 'd') {
            printf("Debug menu\n----------\ne - Edit card.bin\nr - Reload card.bin\nq - Exit debug menu\n--> ");
            clearInputBuffer();
            scanf("%c", &ans);
            if (ans == 'e') {
                card_file_gen(1);
            } else if (ans == 'r') {
                printf("Reloading card.bin\n");
                fclose(file);
                goto reload;
            } else if (ans == '*') {
                printf("Exiting\n");
                fclose(file);
                break;
            } else {
                printf("Unknown command\n");
            }
        } else if (ans == 'q') {
            printf("Exiting\n");
            fclose(file);
            break;
        } else if (ans == 's') {
            monocpoly(card);
        }else {
            printf("Unknown command\n");
        }
    } while(ans!='q');
}