#include <stdio.h>
#DEFINE HAS_OWNER_DEFAULT -1
#DEFINE IS_MORTGAGED_DEFAULT 0
#DEFINE HOUSE_DEFAULT 0

enum CardType {
    PROPERY,
    TRAIN_STATION,
    SPECIAL
}

enum PropertyColor {
    BROWN,
    LBLUE,
    PINK,
    ORANGE,
    RED,
    YELLOW,
    GREEN,
    DBLUE
} 

union CardProperties {
    struct { // Propiedad
        int has_owner;
        char nombre[50];
        int precio;
        enum PropertyColor color;
        int mortgage;
        int is_mortgaged;
        int houses; // 0 - No house // 1 to 4 - House // 5 - Hotel // 
        int precioXcasa;
        int rent[6]
    } property;
    struct { // Train Station
    /* The prices will be determined by the 
    train station flow, looking at the flags
    found on the players */
        int has_owner;
        char nombre[50];
    } train_station;
    struct { // Special
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
    } special;
};

struct Card {
    enum CardType type;
    enum CardProperties properties;
};

struct { // Players
    int money;
    int pos;
    // Flags
    bool in_jail;
    bool has_water;
    bool has_electric;
};

void init_board (struct Card card[]) {
    card[0].type = SPECIAL;
    card[0].properties.special.type = 0;
    
    card[1].type = PROPERTY;
    card[1].properties.property.has_owner = HAS_OWNER_DEFAULT;
    strcpy(card[1].properties.propery.nombre, "Ronda de Valencia");
    card[1].properties.property.precio = 60;
    card[1].properties.property.color = BROWN;
    card[1].properties.property.mortgage = 30;
    card[1].properties.property.is_mortgaged = IS_MORTGAGED_DEFAULT;
    card[1].properties.property.houses = HOUSE_DEFAULT;
    card[1].properties.property.precioXcasa = 50;
    card[1].properties.property.rent[0] = 2;
    card[1].properties.property.rent[1] = 10;
    card[1].properties.property.rent[2] = 30;
    card[1].properties.property.rent[3] = 90;
    card[1].properties.property.rent[4] = 160;
    card[1].properties.property.rent[5] = 250;
};