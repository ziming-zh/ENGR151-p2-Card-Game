//
// Created by Ziming on 2021/11/5.
//

#ifndef P2_1_MGTK_H
#define P2_1_MGTK_H
#include "GTK_ENABLED.h"

#include "card_pile.h"
#include "basic.h"

#ifdef GTK
#include <gtk/gtk.h>
typedef struct _setting_data{
    int i;
    int num;
    int c;
    int r;
    int d;
    GtkWidget *value[4];
    GtkWidget *left[4];
    GtkWidget *right[4];
    char *path;
}setting_data;

typedef struct _animation_data{

    int num;
    int c;
    int r;
    int d;
    GtkWidget *windows;
    GtkWidget *fixed;
    GtkWidget *cardpt[20];
    FILE *log;
    pile *dock_pile;
    pile *disc_pile;
    table *t;
    int **cards;
    game_status *st;
    int *score;
    int card_counter[CardTotal];
    int pl;
    int cur;
    int nxt;
    int add;
    int result;
    int j;// for position


}animation_data;

void disp_card(int num,int x,int y,GtkWidget *fixed,int id,animation_data* data);
void disp_start(GtkWidget* widget, gpointer data);
void destroy(GtkWidget* widget, gpointer data);
void wquit(GtkWidget* widget, gpointer window);
void disp_score(GtkWidget* widget, gpointer data);
void disp_setting(GtkWidget* widget, gpointer data);
void init_ggame(GtkWidget *window);
void save_setting(GtkWidget* widget, gpointer data);
void fetch_setting(gpointer data);
void game(animation_data *data);
void select_card(GtkWidget *widget,gpointer pt);
void deal_cards(GtkWidget* widget, gpointer pt);
void confirm_card(GtkWidget *widget,gpointer pt);
void act_status(GtkWidget *window,gpointer pt);
void prepare_for_next(GtkWidget *window,gpointer pt);
#endif
#endif //P2_1_MGTK_H
