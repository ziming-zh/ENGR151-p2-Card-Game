#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include "card_pile.h"
#include "basic.h"
#include "animation.h"
#include "log.h"



#ifdef GTK
#include "mgtk.h"
#endif

#ifndef GTK
void game(int r,int d,int num,int c,FILE* log)
{



    //initial setup

    int dir=1;//originally turning counter-clock wise
    int card_counter[CardTotal];//card_counter is a container that will automatically fill every time there is s reshuffle
    int cur_r;
    int i,pl;//temp
    int **card;
    int *score;
    //int array[5]={0,4,5,8};
    //int *test;
    //test=&array[0];
    pile dock_pile;
    pile disc_pile;
    table t;
    game_status st;


    t.n=num;
    t.win=1;
    score=malloc((size_t)num*sizeof(int));
    if(score==NULL){
        fprintf(stderr,"Cannot allocate memory\n.");
        exit(1);
    }
    for(i=0;i<num;i++)
        score[i]=0;
    for(cur_r=1;cur_r<=r;cur_r++)
    {
        /**
         * Game Settings Initialized
         */
        init_table(&t);
        init_pile(&dock_pile);
        init_pile(&disc_pile);
        for (i=0;i<CardTotal;i++)
        {
            card_counter[i]=d;
        }
        printf("Shuffling cards...\n");
        printf("Shuffle result: # shuffle result only displayed in log and demo mode\n");
        if(log!=NULL)
        {
            fprintf(log,"Shuffling cards...\n");
            fprintf(log,"Shuffle result: # shuffle result only displayed in log and demo mode\n");
        }
        shuffle_card(card_counter,&dock_pile);//initializing dock_pile
        read_pile(&dock_pile,log,150);

        printf("Dealing cards...\n");
        if(log!=NULL)
            fprintf(log,"Dealing cards...\n");

        /**
         * initializing personal card
         */
        card=(int**)malloc(sizeof(int*)*(size_t)num);
        if(card==NULL){
            fprintf(stderr,"Cannot allocate memory\n.");
            exit(1);
        }
        for(pl=0;pl<num;pl++)
        {
            card[pl]=(int*)malloc((size_t)c*sizeof(int));
            if(card[pl]==NULL){
                fprintf(stderr,"Cannot allocate memory\n.");
                exit(1);
            }
            //begin=temp;
            for(i=0;i<c+1;i++)
            {
                card[pl][i]=pull_card(&dock_pile,&disc_pile,card_counter,1,log);
                push_card(&disc_pile,card[pl][i]);
            }
            qsort(card[pl],(size_t)c+1,sizeof(int),cmpf);
            add_player(&t,init_player(c,card[pl],score[pl]),dir);
        }
        if(log!=NULL)
            fprintf(log,"\n# only display current user for a real game, server and demo mode show all players\n");
        move(&t,-dir);
        read_table(&t,dir,log,1,NULL,0);
        printf("Determining the playing order...\n");
        if(log!=NULL)
            fprintf(log,"Determining the playing order...\n");
        t=init_order(&t,&disc_pile,log);
        //add_card(t.next,test,4);
        //read_table(&t,dir,log,1);

        /**
         * Game Starts!
         */
         init_status(&st);
         st.dir=1;
         animation(&t,&st,&dock_pile,&disc_pile,card_counter,log,t.pl);


        /**
         * Game Ended!
         */

        /***
         * displaying the scores of every player
         */
        printf("---- Stats ----\n"
               "Round %d result:\n",cur_r);
        if(log!=NULL)
            fprintf(log,"---- Stats ----\n"
                        "Round %d result:\n",cur_r);
        read_table(&t,1,log,2,score,1);
        create_score("score.log",score,pl);
        printf("Round %d ends.\n\n",cur_r);
        if(log!=NULL)
            fprintf(log,"Round %d ends.\n\n",cur_r);
        ///clear all the memory
        free(card);
        clear_table(&t);
        clear_pile(&dock_pile);
        clear_pile(&disc_pile);

    }
    return;
}

#endif


void init_start(int argc, char *argv[])
{
    int option;
    char *arg;
    int r=1,d=2,num=4,c=5;
    FILE *log=NULL;
    static struct option long_opts[]=
    {
        {"help",no_argument,NULL,'h'},
        {"log",required_argument,NULL,'l'},
        {"player-number=",required_argument,NULL,'n'},
        {"initial-cards=",required_argument,NULL,'c'},
        {"decks",required_argument,NULL,'d'},
        {"rounds",required_argument,NULL,'r'},
        {"auto",no_argument,NULL,'a'},
        {NULL,0,NULL,0},
    };
    setvbuf(stdout, NULL, _IONBF, 0);

    while(1) {
        option= getopt_long(argc,argv,"hn:c:d:r:a",long_opts,NULL);
        if(option==-1)  break;
        //printf("%c\n",option);
        //printf("%s",optarg);
        switch(option)
        {
            case 'h': printf("help\n"); print_help(); break;

            case 'n': arg=optarg;   sscanf(arg,"%d",&num);  printf("<num> %d\n",num);   break;
            case 'c': arg=optarg;   sscanf(arg,"%d",&c);    printf("<card> %d\n",c);  break;
            case 'r': arg=optarg;   sscanf(arg,"%d",&r);    printf("<round> %d\n",r); break;
            case 'd': arg=optarg;   sscanf(arg,"%d",&d);    printf("<deck> %d\n",d);  break;
            case 'a': read_log("demo.log"); break;
            case 'l': {
                arg=optarg;

                printf("Creating Log\n");
                log=create_log(arg,log);
                break;
            }
            default:;
        }
    }
    printf("########################\n"
           "#                      #\n"
           "# Welcome to One Card! #\n"
           "#                      #\n"
           "########################\n");
    printf("---- Initial setup ----\n"
                "Number of rounds: %d\n"
                "Number of decks: %d\n"
                "Number of players: %d\n"
                "Initial number of cards offered to each player:%d\n",r,d,num,c);
    if(log!=NULL)
        fprintf(log,"---- Initial setup ----\n"
                    "Number of rounds: %d\n"
                    "Number of decks: %d\n"
                    "Number of players: %d\n"
                    "Initial number of cards offered to each player:%d\n",r,d,num,c);
#ifdef GTK
    GtkWidget* window;
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Onecard Game");
    gtk_window_set_default_size(GTK_WINDOW(window),1920,1080);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    g_signal_connect(window, "destroy",
                     G_CALLBACK(destroy), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);

    /**
     * init setting!
     */


    init_ggame(window);
    gtk_widget_show_all(window);

    gtk_main();
#endif

#ifndef GTK
    game(r, d, num, c, log);
#endif

    close_log(log);
}
int main(int argc,char *argv[])
{
    init_start(argc,argv);
    return 0; 
}
