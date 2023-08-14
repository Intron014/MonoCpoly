#include <stdio.h>
#DEFINE HAS_OWNER_DEFAULT -1
#DEFINE IS_MORTGAGED_DEFAULT 0
#DEFINE HOUSE_DEFAULT 0

enum CardType {
    PROPERY,
    TRAIN_STATION,
    SPECIAL
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
    };
    struct Train_station{ 
    /* The prices will be determined by the 
    train station flow, looking at the flags
    found on the players */
        int has_owner;
    };
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
    };
};

struct Card {
    enum CardType type;
    enum CardProperties properties;
    char nombre[50];
};

struct Ccandl {
    int type; // 0 - Suerte // 1 - Caja de la Comunidad //
    char description[250];
};

struct { // Players
    int money;
    int pos;
    // Flags
    bool in_jail;
    bool has_water;
    bool has_electric;
};

void card_file_gen(int why) {
    // why 0 - file not found // why 1 - edit struct // 
    struct Card card[40];
    int cnt, ans, aux;
    printf("Master card.bin Editor\n");
    for(cnt = 0; cnt<40 ; cnt++){
        strcpy(card[cnt].nombre, "NULL");
    }
    for(cnt = 0; cnt<40 ; cnt++){
         printf("Editando la casilla %i\n
         Nombre (if any): %s\n
         Introduzca el tipo de casilla\n
         1 - Propiedad\n
         2 - Estacion de tren\n
         3 - Especial\n
         --> ", cnt, card[cnt].nombre);
         scanf("%i", &ans);
         fflish(stdin);
         if(ans==1){ 
            card[cnt].type = PROPERTY;
            card[cnt].properties.Property.has_owner = HAS_OWNER_DEFAULT;
            card[cnt].properties.Property.is_mortgaged = IS_MORTGAGED_DEFAULT;
            card[cnt].properties.Property.houses = HOUSE_DEFAULT;
            printf("Nombre: ");
            gets(card[cnt].nombre);
            printf("Precio: ");
            scanf("%i", &card[cnt].properties.Property.precio);
            printf("Mortgage: ");
            scanf("%i", &card[cnt].properties.Property.mortgage);
            printf("Color: ");
            scanf("%i", &card[cnt].properties.Property.color);
            printf("Precio por casa: ");
            scanf("%i", &card[cnt].properties.Property.precioXcasa);
            for(aux = 0; aux < 6 ; aux++){
                printf("Alquiler %i: ", aux);
                scanf("%i", &card[cnt].properties.Property.rent[aux]);
        };
        else if(ans==2){
            card[cnt].type = TRAIN_STATION;
            card[cnt].properties.Train_station.has_owner = HAS_OWNER_DEFAULT;
            printf("Nombre: ");
            gets(card[cnt].nombre);
        };
        else if(ans==3){
            card[cnt].type = SPECIAL;
            printf("Editando la casilla especial %i\n
            Introduzca el tipo de casilla\n
            0 - Salida\n
            1 - Carcel (Visita)\n
            2 - Suerte\n
            3 - Caja de la comunidad\n
            4 - Impuesto pre-salida\n
            5 - Impuesto pos-salida\n
            6 - Go to jail\n
            7 - Parking gratuito\n
            --> ", cnt);
            scanf("%i", &card[cnt].properties.Special.type);
            fflush(stdin);
            // Switch here para poner nombre (el mismo que el tipo de casilla)
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