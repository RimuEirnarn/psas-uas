#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
    #include <windows.h>
#else
    #define _POSIX_C_SOURCE 199309L
    #include <time.h>
#endif

void delay_ms(unsigned int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#endif
}

typedef int (*PlayerSkill)();

typedef enum {
    AT_NORMAL,
    AT_FOLLOW_UP,
    AT_ULTIMATE,
    AT_EXTRA_ACTION
} ActionType;

typedef struct {
    float HP;
    int ATK;
    int DEF;
} Attributes;

typedef struct {
    char name[20];
    Attributes attr;
    int level;
} Entity;

Entity player = {"Rimu Aerisya", { 250, 20, 8 }, 1};
Entity bat = {"Bat", { 120, 10, 0 }, 1};

void clear() {
#ifdef _WIN32
    system("cls");
#endif

#ifdef __linux__
    system("clear");
#endif
}

int attack() {
    float damage = bat.attr.ATK - player.attr.DEF;
    player.attr.HP -= damage;
    printf("\n%s deals %.0f damage!\n", bat.name, damage);
    delay_ms(500);
    return player.attr.HP <= 0;
}

int player_basic() {
    float damage = (player.attr.ATK * 1) - bat.attr.DEF;
    bat.attr.HP -= damage;
    printf("%s deals %.0f damage!\n", player.name, damage);
    delay_ms(250);
    return bat.attr.HP <= 0;
}

int player_altba() {
    float damage = (player.attr.ATK * 1.5) - bat.attr.DEF;
    bat.attr.HP -= damage;
    printf("%s deals %.0f damage!\n", player.name, damage);
    delay_ms(250);
    return bat.attr.HP <= 0;
}

PlayerSkill action_prompt() {
    while (1) {
        int index = -1;
        printf("1. Basic | 2. Alt Skill | > ");
        scanf("%d", &index);
        if ((index == 1) || (index == 2)) {
            return (index == 1) ? player_basic : player_altba;
        }
    }
}

void update(Entity turn_order[], int turn_index) {
    Entity attacker = turn_order[turn_index];
    Entity defender = turn_order[(turn_index == 0) ? 1 : 0];
    PlayerSkill ability;

    if (strcmp(attacker.name, bat.name) == 0) ability = attack;
    else ability = action_prompt();

    ability();
}


int main() {
    Entity turn_order[2] = {player, bat};
    int turn_index = 0;
    while (1) {
        clear();
        printf("%-20s [%.0f HP] [%d ATK] [%d DEF]\n", player.name, player.attr.HP, player.attr.ATK, player.attr.DEF);
        printf("%-20s [%.0f HP] [%d ATK] [%d DEF]\n", bat.name, bat.attr.HP, bat.attr.ATK, bat.attr.DEF);
        update(turn_order, turn_index);
        turn_index = turn_index == 0 ? 1 : 0;
        if (!((player.attr.HP <= 0) || (bat.attr.HP <= 0))) {
            continue;
        }
        Entity e =  (player.attr.HP <= 0) ? bat : player;
        char* name = e.name;
        printf("%s win!\n", name);
        return 0;
    }
    return 0;
}
