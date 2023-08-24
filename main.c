#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define HAS_OWNER_DEFAULT -1
#define IS_MORTGAGED_DEFAULT 0
#define HOUSE_DEFAULT 0

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
        case BROWN: return "Brown"; break;
        case LBLUE: return "Light Blue"; break;
        case PINK: return "Pink"; break;
        case ORANGE: return "Orange"; break;
        case RED: return "Red"; break;
        case YELLOW: return "Yellow"; break;
        case GREEN: return "Green"; break;
        case DBLUE: return "Dark Blue"; break;
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

struct { // Players
    int money;
    int pos;
    // Flags
    bool in_jail;
    bool has_water;
    bool has_electric;
} Players;

int card_file_gen(int why) {
    // why 0 - file not found // why 1 - edit struct //
    struct Card card[40];
    int cnt, ans, aux;
    printf("Master card.bin Editor\n");
    if(why==0){
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
    for(cnt = 0; cnt<40 ; cnt++){
        do {
            printf("Editando la casilla %i\nNombre: %s\nTipo: %s\nIntroduzca el tipo de casilla\n1 - Propiedad\n2 - Estacion de tren\n3 - Especial\n0 - Skip\n--> ",
                   cnt, card[cnt].nombre, cardTypeToString(card[cnt].type));
            if(card[cnt].type == PROPERTY){
                printf("Precio: %i\nMortgage: %i\nColor: %s\nPrecio por casa: %i\nAlquileres: %i %i %i %i %i %i\n",
                       card[cnt].properties.Property.precio, card[cnt].properties.Property.mortgage, colorToString(card[cnt].properties.Property.color),
                       card[cnt].properties.Property.precioXcasa, card[cnt].properties.Property.rent[0], card[cnt].properties.Property.rent[1],
                       card[cnt].properties.Property.rent[2], card[cnt].properties.Property.rent[3], card[cnt].properties.Property.rent[4],
                       card[cnt].properties.Property.rent[5]);
            }
            scanf("%i", &ans);
            fflush(stdin);
        }while(ans<0 || ans>3);
        if(ans==0) {
            printf("Skipping\n");
            continue;
        }
        else if(ans==1) {
             card[cnt].type = PROPERTY;
             card[cnt].properties.Property.has_owner = HAS_OWNER_DEFAULT;
             card[cnt].properties.Property.is_mortgaged = IS_MORTGAGED_DEFAULT;
             card[cnt].properties.Property.houses = HOUSE_DEFAULT;
             printf("Nombre: ");
             fflush(stdin);
             scanf("%s", card[cnt].nombre);
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
         }
         else if(ans==2){
            card[cnt].type = TRAIN_STATION;
            card[cnt].properties.Train_station.has_owner = HAS_OWNER_DEFAULT;
            printf("Nombre: ");
            fflush(stdin);
            scanf("%s", card[cnt].nombre);
         }
         else if(ans==3){
            card[cnt].type = SPECIAL;
            printf("Editando la casilla especial %i\nIntroduzca el tipo de casilla\n0 - Salida\n1 - Carcel (Visita)\n2 - Suerte\n3 - Caja de la comunidad\n4 - Impuesto pre-salida\n5 - Impuesto pos-salida\n6 - Go to jail\n7 - Parking gratuito\n--> ", cnt);
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
             };
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

int main(){
    card_file_gen(0);
}