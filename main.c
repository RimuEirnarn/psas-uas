#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#ifdef _WIN32
#include <windows.h>
#else
#define _POSIX_C_SOURCE 199309L
#include <time.h>
#endif

void delay_ms(unsigned int milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds);
#else
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#endif
}

#define ENEMY_ACTION_SLEEP 750
#define PLAYER_ACTION_SLEEP 500

typedef struct Entity Entity;
typedef int (*PlayerSkill)();
typedef int (*Ability)(Entity *self, Entity *target);
typedef enum { AT_NORMAL, AT_FOLLOW_UP, AT_ULTIMATE, AT_EXTRA_ACTION } ActionType;

typedef struct Attributes { float HP; int ATK; int DEF; } Attributes;

struct Entity { char name[20]; Attributes attr; int level; Ability abilities[10]; }; // h. Struct, e. Array, g. String

void clear()
{
#ifdef _WIN32
    system("cls");
#endif

#ifdef __linux__
    system("clear");
#endif
}

int attack(Entity *self, Entity *target) { // f. Fungsi
    float damage = self->attr.ATK - target->attr.DEF; // a. Deklarasi, b. inisialisasi
    target->attr.HP -= damage; // a. Deklarasi
    printf("\n%s deals %.0f damage!\n", self->name, damage);
    delay_ms(ENEMY_ACTION_SLEEP);
    return target->attr.HP <= 0;
}

int player_basic(Entity *self, Entity *target) { // f. Fungsi
    float damage = (self->attr.ATK * 1) - target->attr.DEF; // a. Deklarasi, b. Inisialisasi
    target->attr.HP -= damage; // a. Deklarasi
    printf("%s deals %.0f damage!\n", self->name, damage);
    delay_ms(PLAYER_ACTION_SLEEP);
    return target->attr.HP <= 0;
}

int player_altba(Entity *self, Entity *target) { // f. Fungsi
    float damage = (self->attr.ATK * 1.5) - target->attr.DEF; // a
    target->attr.HP -= damage; // a
    printf("%s deals %.0f damage!\n", self->name, damage);
    delay_ms(PLAYER_ACTION_SLEEP);
    return target->attr.HP <= 0;
}

Ability action_prompt(Entity *player) { // f. Fungsi
    while (true) { // d. Perulangan
        int index = -1; // a
        printf("1. Basic | 2. Alt Skill | > ");
        if (scanf("%d", &index) != 1) { while (getchar() != '\n'); continue; }; // c. Percabangan
        if ((index == 1) || (index == 2)) { // c
            return player->abilities[index - 1];
        }
    }
}

void update(Entity *turn_order[], int turn_index) { // f. Fungsi
    Entity *attacker = turn_order[turn_index];
    Entity *defender = turn_order[(turn_index == 0) ? 1 : 0];
    Ability ability; // a

    if (strcmp(attacker->name, turn_order[1]->name) == 0) ability = attack; // b, c
    else ability = action_prompt(attacker); // b, c

    ability(attacker, defender);
}

int main() { // f. Fungsi
    Entity player = {"Rimu Aerisya", {250, 20, 8}, 1, {player_basic, player_altba}}; // a, b
    Entity bat = {"Bat", {120, 10, 0}, 1, {attack}}; // a, b
    Entity *turn_order[2] = {&player, &bat}; // a, b
    unsigned int cycle = 1; // a, b
    while (true) { // d
        for (size_t i = 0; i < 2; i++) { // d 
            clear();
            printf("Cycle #%d\n", cycle);
            printf("%-20s [%.0f HP] [%d ATK] [%d DEF]\n", player.name, player.attr.HP, player.attr.ATK, player.attr.DEF);
            printf("%-20s [%.0f HP] [%d ATK] [%d DEF]\n", bat.name, bat.attr.HP, bat.attr.ATK, bat.attr.DEF);
            update(turn_order, i);
            if (i == 0) cycle++; // c
            if (!((player.attr.HP <= 0) || (bat.attr.HP <= 0))) { continue; } // c
            Entity e = (player.attr.HP <= 0) ? bat : player; // b, c
            char *name = e.name;
            printf("%s win!\n", name);
            return 0;
        }
    }
    return 0;   
}
