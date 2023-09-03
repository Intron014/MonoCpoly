//
// Created by intron014 on 4/09/23.
//

#ifndef MONOCPOLY_BASICS_H
#define MONOCPOLY_BASICS_H

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

#endif //MONOCPOLY_BASICS_H
