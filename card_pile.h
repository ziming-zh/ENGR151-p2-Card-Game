//
// Created by Ziming on 2021/10/11.
//

#ifndef P2_1_CARD_PILE_H
#define P2_1_CARD_PILE_H


//setting up structure card_pile (as a queue mode)
/**
 * COMMENT:REMEMBER TO PUT YOUR SHARED VARIABLE TYPES INTO THE .h FILE
 */
typedef struct card_node{
    int card_num;
    struct card_node *next;
}node;
typedef struct card_pile{
    node *first;
    node *last;
    node *next;
}pile;

void init_pile(pile* q);
int pile_empty(pile* q);
void read_pile(pile *q,FILE* log,int n);
void push_card(pile *q,int new_card);
int pull_card(pile *q,pile *disc,int* counter,int state,FILE *log);//a very good strategy is to embed the pile_empty detection into the pull_card function since every time we pull a card, we need to check the validity
void clear_pile(pile *q);
void tell_card(int x,FILE *log); //tell the suit and rank of the card
void shuffle_card(int card_counter[],pile* P);


#endif //P2_1_CARD_PILE_H
