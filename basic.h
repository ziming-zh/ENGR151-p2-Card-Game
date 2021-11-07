//
// Created by Ziming on 2021/10/11.
//


#ifndef P2_1_BASIC_PROPERTY_H
#define P2_1_BASIC_PROPERTY_H

#include <stdio.h>
#include <math.h>
#include <getopt.h>

#include "GTK_ENABLED.h"
#ifdef GTK
#include "gtk/gtk.h"
#endif

/*
 *  detect environment
 */

/*
 * #define section
 */

// rank property

#define MOVE    1
#define CardTotal   52
#define MAX_LINE_NUM    1000
#define get_suits(x)    (int)floor((x)/13)
#define get_rank(x)     (int)((x)-13*get_suits(x))


int cmpf (const void * a, const void * b);
/**
 * intro setting
 */


void print_help(void);

/***
 * Player Setting
 */
typedef struct Player_Info{
    int* card;
    int card_num; //
    int move; //chances to play cards
    //0 means not on a state to play cards n>0 means the remaining chances to play (originally set to 1)
    int score;
}player_info;

typedef struct Player{
    int* card;
    int card_num; //
    int move; //chances to play cards
    //0 means not on a state to play cards n>0 means the remaining chances to play (originally set to 1)
    int score;
    struct Player *nt;
    struct Player *pr;
}player;

typedef struct{
    player *head;
    player *pre;
    player *next;
    int pl;
    int n;
    int win;
}table;

typedef struct{
    int invalid;
    int under_attack; //record the attack status ->1 when launching attack and ->0 when launching defense
    int effect; //the accumulative card
    int redirect; //0 for none -1 for Q->the previous 1 for J->the next
    int skip; //value 1 means skip the next player and turn to the player after the next
    int draw; //0 for not pulling cards and 1 for pulling cards immediately and end the move
    int dir;
    int forced;
}game_status;

void init_table(table *t);
#ifndef GTK
void read_table(table *t,int dir,FILE *log,int sec,int *score);
#endif
#ifdef GTK

void read_table(GtkWidget *windows,GtkWidget *fixed,table *t,int dir,FILE *log,int sec,int *score);
#endif
void clear_table(table *t);
int table_empty(table *t);
int move(table *t,int dir);




player_info init_player(int c,int* ori_card,int score);
void add_player(table *t,player_info pinf,int dir);
player_info delete_player(table *t,int dir);

void add_card(player *pl,int card);
int read_card(player *pl,int pos);
int delete_card(player *pl,int pos);
/***
 *
 */
void create_score(char* file_name,int score[],int pl);

void ask_command(FILE *log);
#endif //P2_1_BASIC_PROPERTY_H
