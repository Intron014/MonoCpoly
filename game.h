//
// Created by intron014 on 4/09/23.
//

#ifndef MONOCPOLY_GAME_H
#define MONOCPOLY_GAME_H
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

void display_player_position(struct Players player, struct Card card) {
    printf("%s has landed on %s\n", player.nombre, card.nombre);
    if (card.type == PROPERTY) {
        printf("Type: Property\nOwner: %s\nPrice: %d\nColor: %s\n",
               card.properties.Property.has_owner == -1 ? "None" : player.nombre,
               card.properties.Property.precio,
               colorToString(card.properties.Property.color));
    } else if (card.type == TRAIN_STATION) {
        printf("Type: Train Station\nOwner: %s\n",
               card.properties.Train_station.has_owner == -1 ? "None" : player.nombre);
    } else if (card.type == SPECIAL) {
        printf("Type: Special\n");
    }
}

void trade_cards(struct Players *players, int pcount, struct Card card[40]) {
    char trader_name[50], tradee_name[50];
    int trader_index = -1, tradee_index = -1;
    printf("Enter your name: ");
    fflush(stdin);
    scanf(" %49[^\n]", trader_name);
    for (int i = 0; i < pcount; i++) {
        if (strcmp(players[i].nombre, trader_name) == 0) {
            trader_index = i;
            break;
        }
    }
    if (trader_index == -1) {
        printf("Player not found.\n");
        return;
    }

    printf("Enter the name of the player you want to trade with: ");
    fflush(stdin);
    scanf(" %49[^\n]", tradee_name);
    for (int i = 0; i < pcount; i++) {
        if (strcmp(players[i].nombre, tradee_name) == 0) {
            tradee_index = i;
            break;
        }
    }
    if (tradee_index == -1) {
        printf("Player not found.\n");
        return;
    }

    int trader_property_index, tradee_property_index;
    printf("Enter the index of your property (0-39): ");
    fflush(stdin);
    scanf("%d", &trader_property_index);
    if (trader_property_index < 0 || trader_property_index >= 40 || card[trader_property_index].properties.Property.has_owner != trader_index) {
        printf("Invalid property index.\n");
        return;
    }

    printf("Enter the index of the property you want to trade for (0-39): ");
    fflush(stdin);
    scanf("%d", &tradee_property_index);
    if (tradee_property_index < 0 || tradee_property_index >= 40 || card[tradee_property_index].properties.Property.has_owner != tradee_index) {
        printf("Invalid property index.\n");
        return;
    }

    card[trader_property_index].properties.Property.has_owner = tradee_index;
    card[tradee_property_index].properties.Property.has_owner = trader_index;
    printf("Trade completed successfully.\n");
}

int monocpoly(struct Card card[40]) {
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
        random_number(6, 1);

    } while(player_non_bankrupt>1);

    return 0;
}

#endif //MONOCPOLY_GAME_H
