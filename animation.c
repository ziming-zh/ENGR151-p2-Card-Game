//
// Created by Ziming on 2021/10/13.
//

#include "animation.h"
#include <stdlib.h>

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
     else if(n_rank==11 || n_rank==12) // Q/K
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
    int i=0;
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
    system("clear");
    system("cls");
    printf("---- Game start ----\n");
    if(log!=NULL)
        fprintf(log,"---- Game start ----\n");
    cur=pull_card(dock_pile,disc_pile,counter,0,log);
    printf("First card:");
    if(log!=NULL)
        fprintf(log,"First card:");
    tell_card(cur,log);
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
                        tell_card(t->next->card[j],NULL);
                    }
                    printf("\nNO. of cards(count from left):");
                    scanf("%d",&j);
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
                tell_card(nxt,log);
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
                    tell_card(add,log);
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
            printf("Player %d cards:",pl);
            if(log!=NULL)
                fprintf(log,"Player %d cards:",pl);
            for(j=0;j<t->next->card_num;j++)
            {
                tell_card(t->next->card[j],log);
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


            //clear screen
            system("clear");
            system("cls");
            printf("the previously played card (");
            if(log!=NULL)
                fprintf(log,"# clear screen here for a real game and show the previously played card (");
            tell_card(nxt,log);
            printf(")\n");
            if(log!=NULL)
                fprintf(log,")\n");


            //prepare for the next player
            move(t,st->dir);
            cur=nxt;
        }
        pl+=st->dir;
        if(pl<=0)
            pl+=n;
        if(pl>n)
            pl-=n;


    }while(!result);


}