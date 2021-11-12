//
// Created by Ziming on 2021/10/11.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "card_pile.h"

#include "log.h"
#include "GTK_ENABLED.h"
#ifdef GTK
#include "mgtk.h"
#endif
//initialize

void init_pile(pile* q)
{
    q->first=q->last=NULL;
}

//check empty
int pile_empty(pile* q)
{
    return q->first==NULL && q->last==NULL;
}
//read the top card of the pile without picking it up
#ifndef GTK
void read_pile(pile *q,FILE* log,int n)
{
    int i=0;
    node *temp=q->first;
    if(pile_empty(q))
    {
        fprintf(stderr,"Error#101: Card Pile Empty. Cannot read the pile.\n");
        close_log(log);
        exit(EXIT_FAILURE);
    }
    while(temp->next!=NULL){
        tell_card(temp->card_num,log,0);
        i++;
        temp=temp->next;
        if(i==n){
            printf("... # more results skipped here\n");
            if(log!=NULL)
                fprintf(log,"... # more results skipped here\n");
            break;
        }
    }
    printf("\n");
    if(log!=NULL)
        fprintf(log,"\n");
}
#endif
#ifdef GTK
void read_pile(GtkWidget *window,GtkWidget *fixed,pile *q,FILE* log,int n)
{
    if(window==NULL){}
    int i=0;
    node *temp=q->first;
    GtkWidget *label;
    if(pile_empty(q))
    {
        fprintf(stderr,"Error#101: Card Pile Empty. Cannot read the pile.\n");
        close_log(log);
        exit(EXIT_FAILURE);
    }
    while(temp->next!=NULL){
        tell_card(temp->card_num,log);
        disp_card(temp->card_num,i%7*200, (int)(100+floor(i/7)*200),fixed,-1,NULL);
        //gtk_widget_show_all(window);
        i++;
        temp=temp->next;
        if(i==n){
            printf("... # more results skipped here\n");
            if(log!=NULL)
                fprintf(log,"... # more results skipped here\n");
            break;
        }
    }
    label= gtk_label_new("... # more results skipped here");
    gtk_fixed_put(GTK_FIXED(fixed), label,0 , (int)(250+ floor(i/7)*200));
    gtk_widget_set_size_request(label, 300, 35);
    printf("\n");
    if(log!=NULL)
        fprintf(log,"\n");

}
#endif
void push_card(pile *q,int new_card)
{
    node *s;
    /***
     * COMMENT: PAY SPECIAL ATTENTION TO SEGMENT FAULT  caused by not distributing space
     */
    s=(node*)malloc(sizeof(node));
    if(s==NULL){
        fprintf(stderr,"Cannot allocate memory\n.");
        exit(1);
    }
    s->card_num=new_card;
    s->next=NULL;
    if(pile_empty(q))
    {
        q->first=q->last=s;
    }
    else
    {
        //THINK just care about the last one
        q->last->next=s;
        q->last=s;
    }
}
int pull_card(pile *q,pile *disc,int counter[],int state,FILE *log)
{
    int i;
    int temp;
    node *s;
    if(pile_empty(q) && state!=2)
    {
        //the current card pile is empty resort to the discard pile
        if(pile_empty(disc) && state)
        {
            fprintf(stderr,"Error#101: ALL Cards missing. Please Checkout the error!\n");
            close_log(log);
            exit(EXIT_FAILURE);
        }
        else{
            printf("Stock pile exhausted. Shuffling the discard pile and restore the stock pile\n");
            if(log!=NULL)
                fprintf(log,"Stock pile exhausted. Shuffling the discard pile and restore the stock pile\n");
            do{
                temp=pull_card(disc,NULL,NULL,2,log);
                //printf("%d\n",temp);
                counter[temp]++;
            }while(!pile_empty(disc));
            shuffle_card(counter,q);
        }
    }
    s=q->first;
    i=s->card_num;
    //be discreet
    if(q->first==q->last)
        q->first=q->last=NULL;
    else
        q->first=q->first->next;
    free(s);
    return i;
}
void clear_pile(pile *q)
{
    while(!pile_empty(q)){
        pull_card(q,q,NULL,1,NULL);
    }
}
void tell_card(int x,FILE *log,int state)
{

    char *suit[5]={"Spades","Hearts","Diamonds","Clubs"};
    /**
     * COMMENT: REMEMBER in C a string array could be directly defined by char*
     */
    char *rank[14]={"1","2","3","4","5","6","7","8","9","10","J","Q","K"};
    if(state)
        printf("%s %s  \t",suit[get_suits(x)],rank[get_rank(x)]);
    if(log!=NULL)
        fprintf(log,"%s %s  \t",suit[get_suits(x)],rank[get_rank(x)]);
}

void shuffle_card(int card_counter[],pile* P)
{
    //initialize the counter;
    int i,j,eff=0;
    int s,ct;
    for(j=0;j<CardTotal;j++)
    {
        if(card_counter[j])
            eff++;
    }

    //randomly choose element from the counter and push it to the queue
    i=0;
    srand((unsigned int)time(NULL));
    while(eff)
    {
        ct=0;
        s=rand()%(eff);
        for(j=0;j<CardTotal;j++)
            if(card_counter[j])
            {
                if(ct==s)
                {
                    card_counter[j]--;

                    push_card(P,j);
                    if(card_counter[j]==0)
                        eff--;
                    break;
                }
                ct++;
            }
        i++;
    }
}
