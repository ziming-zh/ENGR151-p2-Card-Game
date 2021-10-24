//
// Created by Ziming on 2021/10/12.
//
#include "basic.h"
#include <stdio.h>
#include <stdlib.h>

int cmpf (const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}
void print_help(void)
{
    int o;
    printf("-h|--help print this help message\n"
           "--log filename write the logs in filename (default: onecard.log)\n"
           "-n n|--player-number=n n players, n must be larger than 2 (default: 4)\n"
           "-c c|--initial-cards=c deal c cards per player, c must be at least 2 (default: 5)\n"
           "-d d|--decks=d use d decks 52 cards each, d must be at least 2 (default: 2)\n"
           "-r r|--rounds=r play r rounds, r must be at least 1 (default: 1)\n"
           "-a|--auto run in demo mode\n");
    printf("Now Start the game or not <y/n>:");
    scanf("%c",&o);
    if(o!='y')  exit(EXIT_SUCCESS);
}

void init_table(table *t)
{
    t->pre=t->next=t->head=NULL;
}

void read_table(table *t,int dir,FILE *log,int sec,int *score)
{

    player *temp=t->next;
    int pl=t->win-1;
    int n=t->n;
    int rem,j;
    do{

        printf("Player %d:",pl+1);
        if(log!=NULL)
            fprintf(log,"Player %d:",pl+1);
        // sec=1 for cards
        if(sec==1)
        {
            for(j=0;j<t->next->card_num;j++)
            {
                //printf("%d\n",t->next->card[j]);
                tell_card(t->next->card[j],log);
            }
            printf("\n");
            if(log!=NULL)
                fprintf(log,"\n");
        }
        // sec=2 for score
        if(sec==2)
        {
            rem=t->next->card_num;
            score[pl]+=-rem;
            printf("%d, ",-rem);
            printf("total: %d\n",score[pl]);
            if(log!=NULL)
            {
                fprintf(log,"%d, ",-rem);
                fprintf(log,"total: %d\n",score[pl]);
            }
        }

        move(t,dir);
        pl++;
        if(pl<=0)
            pl+=n;
        if(pl>=n)
            pl-=n;
    }while(t->next!=temp);
    printf("\n");
    if(log!=NULL)
        fprintf(log,"\n");
}
void clear_table(table *t)
{
    do{
        delete_player(t,1);
    }while(!table_empty(t)); //careful
    return;
}
int table_empty(table *t)
{
    return t->next==NULL && t->pre==NULL && t->head==NULL;
}
int move(table *t,int dir)
{
    if(table_empty(t))
    {
        fprintf(stderr,"Error#111: No Participants Involved. Cannot switch the player.\n");
        return -1; //move unsuccessfully
    }
    if(dir==1) //Ori: counter-clockwise -> turn right
        {
        player *cur=t->next;
        t->next=t->next->nt;
        t->pre=cur;
        }
    else if(dir==-1) //Ori: clockwise -> turn left
        {
        player *cur=t->pre;
        t->pre=t->pre->pr;
        t->next=cur;
        }
    return 0;
}
table init_order(table *t,pile* disc_pile,FILE *log)
{
    int suit=3,rank=12;
    int cur;
    table seed;
    int pl=1;
    int first_player=0;
    player *temp=t->next;
    do{
        cur=delete_card(t->next,0);
        push_card(disc_pile,cur);
        printf("Player %d:",pl);
        if(log!=NULL)
            fprintf(log,"Player %d:",pl);
        tell_card(cur,log);
        printf("\n");
        if(log!=NULL)
            fprintf(log,"\n");
        if(rank> get_rank(cur) || (rank==get_rank(cur) && suit>= get_suits(cur)))
        {
            seed=*t;
            rank= get_rank(cur);
            suit=get_suits(cur);//common mistake: always forget to replace the old reference item with the new one
            first_player=pl;
        }
        move(t,1);
        pl++;
    }while(t->next!=temp);

    printf("The game will start with player %d\n",first_player);
    if(log!=NULL)
        fprintf(log,"The game will start with player %d\n",first_player);
    seed.pl=first_player;
    return seed;
}
player_info init_player(int c,int* ori_card,int score)
{
    player_info pl;
    pl.score=score;
    pl.move=MOVE;
    pl.card_num=c+1;
    pl.card=ori_card;

    return pl;
}
void add_player(table *t,player_info pinf,int dir)
{
    player *pl;
    pl=(player*)malloc(sizeof(player));
    pl->card=(pinf).card;
    pl->card_num=(pinf).card_num;
    pl->move=(pinf).move;
    pl->score=(pinf).score;
    pl->nt=pl->pr=NULL;
    if(table_empty(t)){
        t->pre=t->next=t->head=pl;
        pl->nt=pl->pr=pl;
    }
    else{

        if(dir==1)
        {

            pl->nt=t->next;
            pl->pr=t->pre;
            t->next->pr=pl;
            t->pre->nt=pl;

        }
        else if(dir==-1)
        {
            pl->pr=t->pre;
            pl->nt=t->next;
            t->pre->nt=pl;
            t->next->pr=pl;
        }
        move(t,dir);
    }
}
player_info delete_player(table *t,int dir)
{
    player_info pinf;
    player *pl;
    if(table_empty(t))
    {
        fprintf(stderr,"Error#112: No Participants Involved. Cannot delete the player.\n");
        exit(EXIT_FAILURE);
    }
    else{
        //always cope with the special case first
        if(t->pre==t->next)
        {
            pl=t->next;
            pinf.card=pl->card;
            pinf.card_num=pl->card_num;
            pinf.move=pl->move;
            pinf.score=pl->score;
            t->pre=t->next=t->head=NULL;
        }

        else if(dir==1)
        {
            pl=t->next;
            pinf.card=pl->card;
            pinf.card_num=pl->card_num;
            pinf.move=pl->move;
            pinf.score=pl->score;
            pl->nt->pr=pl->pr;
            t->next=pl->nt;

        }
        else if(dir==-1)
        {
            pl=t->pre;
            pinf.card=pl->card;
            pinf.card_num=pl->card_num;
            pinf.move=pl->move;
            pinf.score=pl->score;
            pl->pr->nt=pl->nt;
            t->pre=pl->pr;

        }
        free(pl);
        return pinf;

    }
}

/**
 *  card_setting
 */
void add_card(player *pl,int card)
{

    pl->card= realloc(pl->card,sizeof(int)*(pl->card_num+1));
    pl->card[pl->card_num]=card;

    pl->card_num+=1;
    qsort(pl->card,pl->card_num,sizeof(int),cmpf);
//    for(i=0;i<pl->card_num;i++)//insert
//    {
//        printf("%d ",pl->card[i]);
//    }
    return;
}
int delete_card(player *pl,int pos)
{
    int j,value;
    //int index,value;
    //qsort(pos,num,sizeof(int),cmpf);
    //for(i=0;i<num;i++){
        //index=pos[i]-i;
//        value=pl->card[index];
//        printf("<delete>%d\n",value);
//        for(j=index+1;j<num;j++)
//        {
//            pl->card[j-1]=pl->card[j];
//            pl->card_num--;//
//        }
//        pl->card[num-1]=NULL;
    //}
    value=pl->card[pos];
    //printf("<delete>%d\n",value);
    for(j=pos+1;j<pl->card_num;j++){
        pl->card[j-1]=pl->card[j];
    }
    pl->card_num--;
    pl->card[pl->card_num]=0;
    return value;
}
int read_card(player *pl,int pos)
{
    int value;
    value=pl->card[pos];
    return value;
}

void ask_command(FILE *log)
{
    getchar();
   // printf("%c",8);
}