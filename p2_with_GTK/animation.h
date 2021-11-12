//
// Created by Ziming on 2021/10/13.
//

#ifndef P2_1_ANIMATION_H
#define P2_1_ANIMATION_H

#include "basic.h"
#include "card_pile.h"
void init_status(game_status *st);
#ifndef GTK
void animation(table *t, game_status *st,pile* dock_pile,pile* disc_pile,int* counter,FILE *log,int pl);
#endif
#ifdef GTK
#include "mgtk.h"
void set_up_animation(GtkWidget *widget,gpointer pt);
void animation(GtkWidget *widget,gpointer pt);
#endif
void change_status(game_status *st,int cur,int nxt);
int card_available(int cur,player *pl,game_status* st);

#endif //P2_1_ANIMATION_H
