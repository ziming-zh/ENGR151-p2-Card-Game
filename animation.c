//
// Created by Ziming on 2021/10/13.
//
#include <stdlib.h>
#include "animation.h"



void init_status(game_status *st){
    st->invalid=0;
    st->effect=0;
    st->draw=0;
    st->redirect=0;
    st->skip=0;
    st->under_attack=0;
    st->forced=0;
    st->skip=0;
}
void change_status(game_status *st,int cur,int nxt){
    //define car type
    /***
     * Cards split into four main categories:
        • Attack:
        – Cards with rank 2: the next player draws two cards from the stock pile;
        – Cards with rank 3: the next player draws three cards from the stock pile;
        • Defense:
        – Cards with rank 7: cancel an attack, i.e. do not draw any card if a 2 or a 3 was played before;
        • Action:
        – Queen cards: reverse the playing order from counter-clockwise to clockwise or clockwise to counterclockwise;
        – Jack cards: skip the next player;
     */
     int c_suit,c_rank;
     int n_suit,n_rank;
     st->invalid=0;
     c_suit= get_suits(cur);
     c_rank= get_rank(cur);
     n_suit= get_suits(nxt);
     n_rank= get_rank(nxt);
     //invalid
     if(c_suit!=n_suit && c_rank!=n_rank){
         fprintf(stderr,"Player Error #001: Your card selection doesn't conform with the rule. Please try again!\n");
         st->invalid=1;
         return;
     }
     /*
      * attack
      */
     if(st->under_attack){
         st->draw=1;
     }
     if(n_rank==1 || n_rank==2)//rank 2 / 3
     {
         if(!st->under_attack){
             st->under_attack=1;
             st->effect=n_rank+1;
         }
         else{
             st->effect+=(n_rank+1);
             st->draw=0;
         }
     }
     /*
      * defense
      */
     else if(n_rank==6)//rank 7
     {
         init_status(st);
     }
     else if(n_rank==11 || n_rank==10) // Q/K
     {
         if(n_rank==11)
         {
             st->redirect=1;
             st->dir=-st->dir; //reverse the direction
         }
         else
             st->skip=1;
         if(c_suit==n_suit && st->under_attack){
             st->draw=0;
             st->forced=1;//force the next player to draw card no matter what card he is playing.
         }
     }
}

int card_available(int cur,player *pl,game_status* st)
{
    int i;
    int states=0;
    int r;
    int c_rank= get_rank(cur);
    int c_suit= get_suits(cur);
    if(st->under_attack){
        for(i=0;i<pl->card_num;i++)
        {
            r = get_rank(pl->card[i]);
            if(r==c_rank || c_suit== get_suits(pl->card[i]))
                if(r==1 || r==2 || r==6 || r==11 || r==12) //under attack -> make sure that the following player has the following card to continue
                {
                    states=1;
                    st->draw=1;
                    break;
                }
        }
        if(~states)
            return 0;
    }
    for(i=0;i<pl->card_num;i++)
    {
        if(c_rank== get_rank(pl->card[i]) || c_suit== get_suits(pl->card[i]))
            return 1;
    }
    st->draw=1;
    st->effect=1;
    return 0;
}
#ifndef GTK
void animation(table *t,game_status *st,pile *dock_pile,pile *disc_pile,int* counter,FILE *log,int pl)
{
    int i;
    ///get a card from the top of the pile
    int cur;
    int nxt;
    int add;
    int n=t->n;
    int j = 0;
    int result=0;
    ask_command(log);
#ifdef WIN32
    if(system("clear")){}
#endif
#ifdef WIN64
    if(system("clear")){}
#endif
#ifdef linux
    if(system("cls")){}
#endif
    printf("---- Game start ----\n");
    if(log!=NULL)
        fprintf(log,"---- Game start ----\n");
    cur=pull_card(dock_pile,disc_pile,counter,0,log);
    printf("First card:");
    if(log!=NULL)
        fprintf(log,"First card:");
    tell_card(cur,log,1);
    printf("\n");
    if(log!=NULL)
        fprintf(log,"\n");

    do{

        printf("---------------player %d--------------\n",pl);
        for(i=0;i<t->next->move;i++)
        {

            if(!card_available(cur,t->next,st) || st->forced)
            {
                printf("Sorry, but it seems that you cannot play any cards now. Now draw cards from the pile\n");

                st->draw=1;
                st->forced=0;
                nxt=cur;
            }
            else{
                //let the player choose which car to play
                do{
                    printf("Chose your card from your card pile below:\n");
                    for(j=0;j<t->next->card_num;j++)
                    {
                        tell_card(t->next->card[j],NULL,1);
                    }
                    printf("\nNO. of cards(count from left):");
                    if(scanf("%d",&j)){}
                    if(j>t->next->card_num){
                        printf("Invalid card number. Please input again!\n");
                        st->invalid=1;
                        continue;
                    }
                    nxt=read_card(t->next,j-1);


                    change_status(st,cur,nxt);
                }while(st->invalid);
                printf("Player %d plays:",pl);
                if(log!=NULL)
                    fprintf(log,"Player %d plays:",pl);
                tell_card(nxt,log,1);
                printf("\n");
                if(log!=NULL)
                    fprintf(log,"\n");

                push_card(disc_pile,delete_card(t->next,j-1));
                ///check whether the game ends
                if(t->next->card_num==0)
                {
                    result=1;
                    printf("Player %d wins!\n",pl);
                    if(log!=NULL)
                        fprintf(log,"Player %d wins!\n",pl);
                    t->win=pl;
                    break;
                }
            }
            ///display the attribute effect of the player if there is one
            if(st->skip)
            {
                move(t,st->dir);
                st->skip=0;
                pl+=st->dir;
            }
            if(st->draw)
            {
                printf("#Panelty Attention! %d cards will be added!\n",st->effect);
                printf("Player %d draws:",pl);
                if(log!=NULL)
                    fprintf(log,"Player %d draws:",pl);
                for(j=0;j<st->effect;j++){
                    add = pull_card(dock_pile, disc_pile, counter, 0,log);
                    tell_card(add,log,1);
                    add_card(t->next,add);
                }
                printf("\n");
                if(log!=NULL)
                    fprintf(log,"\n");
                st->draw=0;
                st->effect=0;
                st->under_attack=0;
            }
            ///display the remaining card of the player
            printf("\n\nPlayer %d cards:",pl);
            if(log!=NULL)
                fprintf(log,"Player %d cards:",pl);
            for(j=0;j<t->next->card_num;j++)
            {
                tell_card(t->next->card[j],log,1);
            }
            printf("\n");
            if(log!=NULL)
                fprintf(log,"\n");

            if(st->redirect==1){
                move(t,st->dir);
                pl+=st->dir;
                st->redirect=0;
            }

            //display the attribute effect of the player if there is one

            getchar();
            getchar();
            //wait for display
            //clear screen
#ifdef WIN32
            if(system("clear")){}
#endif
#ifdef WIN64
            if(system("clear")){}
#endif
#ifdef linux
            if(system("cls")){}
#endif
            printf("the previously played card (");
            if(log!=NULL)
                fprintf(log,"# clear screen here for a real game and show the previously played card (");
            tell_card(nxt,log,1);
            printf(")\n");
            if(log!=NULL)
                fprintf(log,")\n");


            //prepare for the next player

            cur=nxt;
        }
        move(t,st->dir);
        pl+=st->dir;
        if(pl<=0)
            pl+=n;
        if(pl>n)
            pl-=n;


    }while(!result);


}
#endif
#ifdef GTK
void set_up_animation(GtkWidget *widget,gpointer pt){
    if(widget==NULL){}

    animation_data *data1=(animation_data*)pt;
    GtkWidget *window=data1->windows;
    GtkWidget *button;
    GtkWidget *fixed =data1->fixed;
    gtk_widget_destroy(fixed);
    fixed=gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window),fixed);
    data1->fixed=fixed;
    GtkWidget *label;
    label= gtk_label_new("Game Start");
    gtk_fixed_put(GTK_FIXED(fixed), label,0 , 0);
    gtk_widget_set_size_request(label, 180, 35);

    label= gtk_label_new("First card:");
    gtk_fixed_put(GTK_FIXED(fixed), label,0 , 50);
    gtk_widget_set_size_request(label, 300, 35);

    //ask_command(data1->log);
    FILE *log=data1->log;
    printf("---- Game start ----\n");
    if(log!=NULL)
        fprintf(log,"---- Game start ----\n");
    data1->cur=pull_card(data1->dock_pile,data1->disc_pile,data1->card_counter,0,log);

    printf("First card:");
    if(log!=NULL)
        fprintf(log,"First card:");
    tell_card(data1->cur,log,1);
    disp_card(data1->cur,400,300,fixed,-1,NULL);
    printf("\n");
    if(log!=NULL)
        fprintf(log,"\n");

    button= gtk_button_new_with_label("Continue");
    gtk_fixed_put(GTK_FIXED(fixed), button,600 , 800);
    gtk_widget_set_size_request(button, 100, 35);
    gtk_widget_show_all(fixed);
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(animation),data1);

}
void animation(GtkWidget *widget,gpointer pt)
{
    if(widget==NULL){}

    animation_data *data1=(animation_data*)pt;
    ///get a card from the top of the pile
    GtkWidget *window=data1->windows;
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *fixed =data1->fixed;
    gtk_widget_destroy(fixed);
    fixed=gtk_fixed_new();
    data1->fixed=fixed;
    gtk_container_add(GTK_CONTAINER(window),fixed);

    table *t=data1->t;
    game_status *st=data1->st;
    char words[30];
    int j;

    sprintf(words,"player %d",data1->pl);
    label= gtk_label_new("Game Start");
    gtk_fixed_put(GTK_FIXED(fixed), label,0 , 0);
    gtk_widget_set_size_request(label, 180, 35);

    label= gtk_label_new(words);
    gtk_fixed_put(GTK_FIXED(fixed), label,0 , 50);
    gtk_widget_set_size_request(label, 300, 35);

    printf("---------------player %d--------------\n",data1->pl);
    if(!card_available(data1->cur,t->next,st) || st->forced)
    {
        printf("Sorry, but it seems that you cannot play any cards now. Now draw cards from the pile\n");
        st->draw=1;
        st->forced=0;
        data1->nxt=data1->cur;
        label= gtk_label_new("Sorry, but it seems that you cannot play any cards now.");
        gtk_fixed_put(GTK_FIXED(fixed), label,200 , 100);
        gtk_widget_set_size_request(label, 300, 50);
        button= gtk_button_new_with_label("Continue");
        gtk_fixed_put(GTK_FIXED(fixed), button,600 , 800);
        gtk_widget_set_size_request(button, 100, 35);

        gtk_widget_show_all(fixed);


        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(act_status),data1);
    }
    else{
        //let the player choose which car to play
        printf("Chose your card from your card pile below:\n");
        for(j=0;j<t->next->card_num;j++)
        {
            tell_card(t->next->card[j],NULL,1);
            disp_card(t->next->card[j],j%7*200+200, (int)(100+floor(j/7)*200),fixed,j,data1);
        }
        printf("\nNO. of cards(count from left):");
        /***
         * get card id by selecting
         */

    }
    gtk_widget_show_all(fixed);


}
#endif
#ifdef CHECK
void check_valid(){



            ///check whether the game ends
            if(t->next->card_num==0)
            {
                result=1;
                printf("Player %d wins!\n",data1->pl);
                if(log!=NULL)
                    fprintf(log,"Player %d wins!\n",data1->pl);
                t->win=data1->pl;
                break;
            }
        }
        ///display the attribute effect of the player if there is one
        if(st->skip)
        {
            move(t,st->dir);
            st->skip=0;
            data1->pl+=st->dir;
        }
        if(st->draw)
        {
            printf("#Panelty Attention! %d cards will be added!\n",st->effect);
            printf("Player %d draws:",data1->pl);
            if(log!=NULL)
                fprintf(log,"Player %d draws:",data1->pl);
            int add;
            for(j=0;j<st->effect;j++){
                add = pull_card(dock_pile, disc_pile, data1->card_counter, 0,log);
                tell_card(add,log,1);
                add_card(t->next,add);
            }
            printf("\n");
            if(log!=NULL)
                fprintf(log,"\n");
            st->draw=0;
            st->effect=0;
            st->under_attack=0;
        }
        ///display the remaining card of the player
        printf("Player %d cards:",data1->pl);
        if(log!=NULL)
            fprintf(log,"Player %d cards:",data1->pl);
        for(j=0;j<t->next->card_num;j++)
        {
            tell_card(t->next->card[j],log,1);
        }
        printf("\n");
        if(log!=NULL)
            fprintf(log,"\n");

        if(st->redirect==1){
            move(t,st->dir);
            data1->pl+=st->dir;
            st->redirect=0;
        }

        //display the attribute effect of the player if there is one



        printf("the previously played card (");
        if(log!=NULL)
            fprintf(log,"# clear screen here for a real game and show the previously played card (");
        tell_card(data1->nxt,log,1);
        printf(")\n");
        if(log!=NULL)
            fprintf(log,")\n");


        //prepare for the next player
        move(t,st->dir);
        data1->cur=data1->nxt;
    }
    data1->pl+=st->dir;
    if(data1->pl<=0)
        data1->pl+=n;
    if(data1->pl>n)
        data1->pl-=n;


    if(result){

    }
    else{
        button= gtk_button_new_with_label("Continue");
        gtk_fixed_put(GTK_FIXED(fixed), button,600 , 800);
        gtk_widget_set_size_request(button, 100, 35);
        gtk_widget_show_all(fixed);
        data1->pl=0;
        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(animation),data1);
    }


}
#endif
