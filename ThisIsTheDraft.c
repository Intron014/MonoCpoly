#include <stdio.h>

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
        int precioXhotel;
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
    */
        int type;
    }
}