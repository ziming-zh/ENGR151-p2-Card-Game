//
// Created by Ziming on 2021/10/13.
//

#ifndef P2_1_ANIMATION_H
#define P2_1_ANIMATION_H

#include "basic.h"
void init_status(game_status *st);
void animation(table *t, game_status *st,pile* dock_pile,pile* disc_pile,int* counter,FILE *log,int pl);
void change_status(game_status *st,int cur,int nxt);
int card_available(int cur,player *pl,game_status* st);

#endif //P2_1_ANIMATION_H
