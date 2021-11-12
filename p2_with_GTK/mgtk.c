//
// Created by Ziming on 2021/11/5.
//
#include <stdlib.h>
#include "basic.h"
#include "animation.h"


#ifdef GTK
#include "mgtk.h"
void disp_start(GtkWidget* widget, gpointer pt)
{
    // printf equivalent in GTK+

    if(widget==NULL){}
    if(pt==NULL){}


    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),"Start");
    gtk_window_set_default_size(GTK_WINDOW(window),1920,1080);
    setting_data *data1=(setting_data*) pt;

    fetch_setting(pt);
    static animation_data data2;
    data2.num=data1->num;
    data2.c=data1->c;
    data2.r=data1->r;
    data2.d=data1->d;
    data2.windows=window;
    data2.log=NULL;
    data2.cards=NULL;
    data2.fixed=NULL;
    data2.result=0;
    game(&data2);
    gtk_widget_show_all(window);
}

void destroy(GtkWidget* widget, gpointer data)
{
    if(widget==NULL){}
    if(data==NULL){}
    gtk_main_quit();
}
void wquit(GtkWidget* widget, gpointer window)
{
    if(widget==NULL){}

    gtk_widget_destroy((GtkWidget*)window);
}

void disp_score(GtkWidget* widget, gpointer data)
{
    if(widget==NULL){}
    if(data==NULL){}

    FILE *log;
    char line[30];

    GtkWidget *fixed=gtk_fixed_new();
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *quit;
    gtk_window_set_title(GTK_WINDOW(window),"Start");
    gtk_window_set_default_size(GTK_WINDOW(window),1920,1080);


    log= fopen("score.log","r");
    if(log==NULL){
		fprintf(stderr,"Error#001 Cannot Open File. The Game will quit.\n"); 
		exit(-1);
	}
    int counter=1;
    while(fgets(line,30,log)!=NULL){
        GtkWidget *label= gtk_label_new(line);
        gtk_fixed_put(GTK_FIXED(fixed), label, 600 , counter*50);
        gtk_widget_set_size_request(label, 300, 100);
        counter++;
    }

    quit = gtk_button_new_with_label("Quit");
    gtk_fixed_put(GTK_FIXED(fixed), quit, 600, 720);
    gtk_widget_set_size_request(quit, 180,35);
    g_signal_connect(G_OBJECT(quit),"clicked",G_CALLBACK(wquit),window);
    /// always put at the very end!
    gtk_container_add(GTK_CONTAINER(window), fixed);
    gtk_widget_show_all(window);
}
void plus_info(GtkWidget* widget, gpointer data){
    if(widget==NULL){}

    setting_data *data1=(setting_data*) data;
    char v[10];
    int i;
    for(i =0;i<4;i++)
        if(widget==data1->right[i])
            break;
    switch(i)
    {
        case 0: data1->num++; itoa(data1->num,v,10); break;
        case 1: data1->c++; itoa(data1->c,v,10); break;
        case 2: data1->r++; itoa(data1->r,v,10); break;
        case 3: data1->d++; itoa(data1->d,v,10); break;
        default:{}
    }
    GtkWidget* value = data1->value[i];
    gtk_label_set_text(GTK_LABEL(value),v);
}
void minus_info(GtkWidget* widget, gpointer data){
    setting_data *data1=(setting_data*)data;
    char v[10];
    int i;
    for(i =0;i<4;i++)
        if(widget==data1->left[i])
            break;
    switch(i)
    {
        case 0: data1->num--; itoa(data1->num,v,10); break;
        case 1: data1->c--; itoa(data1->c,v,10); break;
        case 2: data1->r--; itoa(data1->r,v,10); break;
        case 3: data1->d--; itoa(data1->d,v,10); break;
        default:{}
    }
    GtkWidget* value = data1->value[i];
    gtk_label_set_text(GTK_LABEL(value),v);
}
void disp_setting(GtkWidget* widget, gpointer pt)
{
    if(widget==NULL){}
        /***
         * basic properties
         * gtk_disp on/off
         *
         * card_type
         *
         */
        ///setting state


    //    int num=4;
    //    int c=5;
    //    int r=2;
    //    int d=1;


        //int gtk_disp;
        setting_data *data1=(setting_data*) pt;

        GtkWidget *fixed=gtk_fixed_new();
        GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        GtkWidget *quit;
        GtkWidget *save;
        GtkWidget *label;
        GtkWidget *value;
        data1->path="setting.log";
        gtk_window_set_title(GTK_WINDOW(window),"Start");
        gtk_window_set_default_size(GTK_WINDOW(window),1920,1080);


        fetch_setting(pt);

        /*
         * label
         */

        label= gtk_label_new("Players:");
        gtk_fixed_put(GTK_FIXED(fixed), label, 400, 150);
        gtk_widget_set_size_request(label, 180,35);
        label= gtk_label_new("Initial Cards:");
        gtk_fixed_put(GTK_FIXED(fixed), label, 400, 250);
        gtk_widget_set_size_request(label, 180,35);
        label= gtk_label_new("Rounds:");
        gtk_fixed_put(GTK_FIXED(fixed), label, 400, 350);
        gtk_widget_set_size_request(label, 180,35);
        label= gtk_label_new("Decks:");
        gtk_fixed_put(GTK_FIXED(fixed), label, 400, 450);
        gtk_widget_set_size_request(label, 180,35);

        /**
         * value
         */
         char v[10];
         int dv;
         for(int i = 0;i<4;i++)
         {
             data1->i=i;
             switch(i)
             {
                 case 0: dv = data1->num; break;
                 case 1: dv = data1->c; break;
                 case 2: dv = data1->r; break;
                 case 3: dv = data1->d; break;
                 default:{}
             }
             itoa(dv,v,10);
             value= gtk_label_new(v);
             gtk_fixed_put(GTK_FIXED(fixed), value, 450, 150+100*i);
             gtk_widget_set_size_request(value, 180,35);

             data1->value[i]=value;
             data1->left[i]= gtk_button_new_with_label("-");
             gtk_fixed_put(GTK_FIXED(fixed), data1->left[i], 600, 150+100*i);
             gtk_widget_set_size_request(data1->left[i], 70,70);
             //gtk_container_add(GTK_CONTAINER(left),value);

             g_signal_connect(G_OBJECT(data1->left[i]),"clicked",G_CALLBACK(minus_info),pt);
             data1->right[i]= gtk_button_new_with_label("+");
             gtk_fixed_put(GTK_FIXED(fixed), data1->right[i], 700, 150+100*i);
             gtk_widget_set_size_request(data1->right[i], 70,70);
             //gtk_container_add(GTK_CONTAINER(right),value);
             g_signal_connect(G_OBJECT(data1->right[i]),"clicked",G_CALLBACK(plus_info),pt);
         }
        /*
         * +/-
         */

        save = gtk_button_new_with_label("Save");
        gtk_fixed_put(GTK_FIXED(fixed), save, 600, 620);
        gtk_widget_set_size_request(save, 180,35);

        g_signal_connect(G_OBJECT(save),"clicked",G_CALLBACK(save_setting),pt);

        quit = gtk_button_new_with_label("Quit");
        gtk_fixed_put(GTK_FIXED(fixed), quit, 600, 720);
        gtk_widget_set_size_request(quit, 180,35);
        g_signal_connect(G_OBJECT(quit),"clicked",G_CALLBACK(wquit),window);

        ///always put at the very end
        gtk_container_add(GTK_CONTAINER(window), fixed);

        gtk_widget_show_all(window);
}

void fetch_setting(gpointer data){
    FILE *set;
    setting_data *data1=(setting_data*) data;
    set=fopen(data1->path,"r");
    if(set==NULL){
		fprintf(stderr,"Error#001 Cannot Open File. The Game will quit.\n"); 
		exit(-1);
	}
    fscanf(set,"%d",&data1->num);
    fscanf(set,"%d",&data1->c);
    fscanf(set,"%d",&data1->r);
    fscanf(set,"%d",&data1->d);
    fclose(set);
}
void save_setting(GtkWidget* widget, gpointer data){
    if(widget==NULL){}
    setting_data *data1=(setting_data*) data;
    FILE *set;
    set=fopen(data1->path,"w");
    if(set==NULL){
		fprintf(stderr,"Error#001 Cannot Open File. The Game will quit.\n"); 
		exit(-1);
	}
    fprintf(set,"%d\n%d\n%d\n%d\n",data1->num,data1->c,data1->r,data1->d);
    fclose(set);
}

/**
 * animation part
 * ready to output the card
 * @param window
 */

void disp_card(int num,int x,int y,GtkWidget *fixed,int id,animation_data* data1){
    gchar filename[30];
    GtkWidget *act_card;

    sprintf(filename,"./pukeImage/%d.jpg",num+1);
    GtkWidget *pic_card = gtk_image_new_from_file (filename);


    if(id!=-1){

        data1->cardpt[id]=gtk_button_new();
        act_card=data1->cardpt[id];
        g_signal_connect(G_OBJECT(act_card),"clicked",G_CALLBACK(select_card),data1);
    }
    else
        act_card=gtk_button_new();
    gtk_container_add(GTK_CONTAINER(act_card),pic_card);


    gtk_fixed_put(GTK_FIXED(fixed), act_card, x , y);
    gtk_widget_set_size_request(act_card, 125, 175);

    //gtk_container_add(GTK_CONTAINER(window), card);
}
void select_card(GtkWidget *widget,gpointer pt){
    animation_data *data1=(animation_data*)pt;

    GtkWidget *label;
    GtkWidget *fixed=data1->fixed;
    GtkWidget *button;
    int j;
    for(j=0;j<data1->t->next->card_num;j++)
        if(data1->cardpt[j]==widget)
            break;
    j++;
    data1->j=j;
    table *t=data1->t;
    game_status *st=data1->st;

    if(j>t->next->card_num){
        printf("Invalid card number. Please input again!\n");
        st->invalid=1;
    }
    else{
        data1->nxt=read_card(t->next,j-1);
        change_status(st,data1->cur,data1->nxt);
    }
    if(st->invalid) animation(widget,data1);
    else{
        char words[30];
        printf("%d\n",j);
        sprintf(words,"You have choose No. %d card from the left",j);
        label= gtk_label_new(words);
        gtk_fixed_put(GTK_FIXED(fixed), label,600 , 650);
        gtk_widget_set_size_request(label, 100, 35);
        button= gtk_button_new_with_label("Continue");
        gtk_fixed_put(GTK_FIXED(fixed), button,600 , 800);
        gtk_widget_set_size_request(button, 100, 35);

        gtk_widget_show_all(fixed);
        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(confirm_card),data1);
    }
}
void confirm_card(GtkWidget *widget,gpointer pt){
    if(widget==NULL){}
    animation_data *data1=(animation_data*)pt;
    GtkWidget *fixed =data1->fixed;
    GtkWidget *window=data1->windows;
    gtk_widget_destroy(fixed);
    fixed=gtk_fixed_new();

    gtk_container_add(GTK_CONTAINER(window),fixed);
    GtkWidget *button;
    GtkWidget *label;
    char words[30];
    FILE *log=data1->log;
    sprintf(words,"Player %d plays:",data1->pl);
    label= gtk_label_new(words);
    gtk_fixed_put(GTK_FIXED(fixed), label,50 , 0);
    gtk_widget_set_size_request(label, 300, 35);
    printf("Player %d plays:",data1->pl);
    if(log!=NULL)
        fprintf(log,"Player %d plays:",data1->pl);
    tell_card(data1->nxt,log,1);
    disp_card(data1->nxt,400,300,fixed,-1,NULL);

    printf("\n");
    if(log!=NULL)
        fprintf(log,"\n");

    push_card(data1->disc_pile,delete_card(data1->t->next,data1->j-1));
    if(data1->t->next->card_num==0)
    {
        data1->result=1;
        printf("Player %d wins!\n",data1->pl);
        if(log!=NULL)
            fprintf(log,"Player %d wins!\n",data1->pl);
        data1->t->win=data1->pl;
        //prepare_for_next(NULL,pt);

        gtk_widget_destroy(fixed);
        fixed=gtk_fixed_new();
        data1->fixed=fixed;
        int **card=data1->cards;
        int num=data1->num;

        int *score=data1->score;
        pile* dock_pile=data1->dock_pile;
        pile* disc_pile=data1->disc_pile;
        table *t=data1->t;

        /**
         * Game Ended!
         */

        /***
         * displaying the scores of every player
         */
        printf("---- Stats ----\n"
               "Round result:\n");
        if(log!=NULL)
            fprintf(log,"---- Stats ----\n"
                        "Round result:\n");
        read_table(window,fixed,t,1,log,2,score,1);
        create_score("score.log",score,num);
        printf("Round ends.\n\n");
        if(log!=NULL)
            fprintf(log,"Round ends.\n\n");
        label= gtk_label_new("Game Over");
        gtk_fixed_put(GTK_FIXED(fixed), label,600 , 200);
        gtk_widget_set_size_request(label, 100, 35);

        button= gtk_button_new_with_label("Confirm");
        gtk_fixed_put(GTK_FIXED(fixed), button,600 , 800);
        gtk_widget_set_size_request(button, 100, 35);

        gtk_container_add(GTK_CONTAINER(window),fixed);
        gtk_widget_show_all(fixed);
        ///clear all the memory
        free(card);
        clear_table(t);
        clear_pile(dock_pile);
        clear_pile(disc_pile);

    }
    else{

        gtk_widget_destroy(fixed);
        fixed=gtk_fixed_new();
        data1->fixed=fixed;

        label= gtk_label_new(words);
        gtk_fixed_put(GTK_FIXED(fixed), label,50 , 0);
        gtk_widget_set_size_request(label, 300, 35);

        disp_card(data1->nxt,400,300,fixed,-1,NULL);
        button= gtk_button_new_with_label("Continue");
        gtk_fixed_put(GTK_FIXED(fixed), button,600 , 800);
        gtk_widget_set_size_request(button, 100, 35);
        gtk_container_add(GTK_CONTAINER(window),fixed);
        gtk_widget_show_all(fixed);

        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(act_status),data1);
    }

}
void act_status(GtkWidget *widget,gpointer pt)
{
    if(widget==NULL){}
    animation_data *data1=(animation_data*)pt;
    gtk_widget_destroy(data1->fixed);
    GtkWidget *fixed=gtk_fixed_new();
    GtkWidget *window=data1->windows;
    gtk_container_add(GTK_CONTAINER(window),fixed);
    GtkWidget *button;
    GtkWidget *label;
    FILE *log=data1->log;
    data1->fixed=fixed;
    game_status *st=data1->st;
    table* t=data1->t;
    char words[30];
    int j;
    ///display the attribute effect of the player if there is one
    if(st->skip)
    {
        move(t,st->dir);
        st->skip=0;
        data1->pl+=st->dir;
    }
    if(st->draw)
    {
        sprintf(words,"#Panelty Attention! %d cards will be added!\n",st->effect);
        label= gtk_label_new(words);
        gtk_fixed_put(GTK_FIXED(fixed), label,50 , 0);
        gtk_widget_set_size_request(label, 300, 35);
        printf("#Panelty Attention! %d cards will be added!\n",st->effect);
        printf("Player %d draws:",data1->pl);

        if(log!=NULL)
            fprintf(log,"Player %d draws:",data1->pl);
        int add;
        for(j=0;j<st->effect;j++){
            add = pull_card(data1->dock_pile, data1->disc_pile, data1->card_counter, 0,log);
            tell_card(add,log,1);
            disp_card(add,j%7*200+200, (int)(100+floor(j/7)*200),fixed,-1,NULL);
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
    sprintf(words,"player %d cards:",data1->pl);
    label= gtk_label_new(words);
    gtk_fixed_put(GTK_FIXED(fixed), label,50 , 400);
    gtk_widget_set_size_request(label, 300, 35);
    printf("Player %d cards:",data1->pl);
    if(log!=NULL)
        fprintf(log,"Player %d cards:",data1->pl);
    for(j=0;j<t->next->card_num;j++)
    {
        tell_card(t->next->card[j],log,1);
        disp_card(t->next->card[j],j%7*200+200, (int)(500+floor(j/7)*200),fixed,-1,NULL);
    }
    printf("\n");
    if(log!=NULL)
        fprintf(log,"\n");

    if(st->redirect==1){
        move(t,st->dir);
        data1->pl+=st->dir;
        st->redirect=0;
    }
    button= gtk_button_new_with_label("Continue");
    gtk_fixed_put(GTK_FIXED(fixed), button,600 , 800);
    gtk_widget_set_size_request(button, 100, 35);

    gtk_widget_show_all(fixed);
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(prepare_for_next),data1);

}

void prepare_for_next(GtkWidget *widget,gpointer pt){
    if(widget==NULL){}

    animation_data *data1=(animation_data*) pt;
    gtk_widget_destroy(data1->fixed);
    GtkWidget *fixed=gtk_fixed_new();
    GtkWidget *window=data1->windows;
    gtk_container_add(GTK_CONTAINER(window),fixed);
    GtkWidget *button;
    GtkWidget *label;
    FILE *log=data1->log;
    data1->fixed=fixed;
    game_status *st=data1->st;
    table* t=data1->t;
    int n = data1->num;

    label= gtk_label_new("the previously played card:");
    gtk_fixed_put(GTK_FIXED(fixed), label,50 , 0);
    gtk_widget_set_size_request(label, 300, 35);
    printf("the previously played card (");
    if(log!=NULL)
        fprintf(log,"# clear screen here for a real game and show the previously played card (");
    tell_card(data1->nxt,log,1);
    disp_card(data1->nxt,400,300,fixed,-1,NULL);
    gtk_widget_show_all(fixed);
    printf(")\n");
    if(log!=NULL)
        fprintf(log,")\n");


    //prepare for the next player
    move(t,st->dir);
    data1->cur=data1->nxt;

    data1->pl+=st->dir;
    if(data1->pl<=0)
        data1->pl+=n;
    if(data1->pl>n)
        data1->pl-=n;


    if(data1->result){

    }
    else{
        button= gtk_button_new_with_label("Continue");
        gtk_fixed_put(GTK_FIXED(fixed), button,600 , 800);
        gtk_widget_set_size_request(button, 100, 35);
        gtk_widget_show_all(fixed);

        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(animation),data1);
    }
}
void init_ggame(GtkWidget *window){
    GtkWidget  *start;
    GtkWidget  *score;
    GtkWidget  *setting;
    GtkWidget  *quit;
    GtkWidget  *Game_Name;
    GtkWidget  *fixed=gtk_fixed_new();

    static setting_data data1={0,4,5,2,1,{NULL,NULL,NULL,NULL},.path="setting.log"};
    gpointer pt=&data1;
    gtk_container_add(GTK_CONTAINER(window), fixed);
    Game_Name=gtk_label_new("Onecard");
    gtk_fixed_put(GTK_FIXED(fixed), Game_Name,500 , 100);
    gtk_widget_set_size_request(Game_Name, 380, 95);
    start = gtk_button_new_with_label("Start");
    gtk_fixed_put(GTK_FIXED(fixed), start,600 , 450);
    gtk_widget_set_size_request(start, 180, 35);
    score = gtk_button_new_with_label("Score");
    gtk_fixed_put(GTK_FIXED(fixed), score, 600, 540);
    gtk_widget_set_size_request(score, 180, 35);
    setting = gtk_button_new_with_label("Setting");
    gtk_fixed_put(GTK_FIXED(fixed), setting, 600, 630);
    gtk_widget_set_size_request(setting, 180, 35);
    quit = gtk_button_new_with_label("Quit");
    gtk_fixed_put(GTK_FIXED(fixed), quit, 600, 720);
    gtk_widget_set_size_request(quit, 180,35);
    g_signal_connect(G_OBJECT(start),"clicked",G_CALLBACK(disp_start),pt);
    g_signal_connect(G_OBJECT(score),"clicked",G_CALLBACK(disp_score),NULL);
    g_signal_connect(G_OBJECT(setting),"clicked",G_CALLBACK(disp_setting),pt);
    g_signal_connect(G_OBJECT(quit),"clicked",G_CALLBACK(destroy),NULL);



}

void game(animation_data *data1)
{
    GtkWidget *window = data1->windows;
    int num=data1->num;
    int d=data1->d;
    FILE *log=data1->log;
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget  *fixed=gtk_fixed_new();
    data1->fixed=fixed;
    //initial setup


    //card_counter is a container that will automatically fill every time there is s reshuffle

    int i;//temp

    static int *score;

    //int array[5]={0,4,5,8};
    //int *test;
    //test=&array[0];
    static pile dock_pile;
    data1->dock_pile=&dock_pile;
    static pile disc_pile;
    data1->disc_pile=&disc_pile;
    static table t;
    data1->t=&t;
    static game_status st;
    data1->st=&st;
    gtk_container_add(GTK_CONTAINER(window),fixed);

    t.n=num;
    t.win=1;
    score=malloc((size_t)num*sizeof(int));
    if(score==NULL){
        fprintf(stderr,"Cannot allocate memory\n.");
        exit(1);
    }
    data1->score=score;
    for(i=0;i<num;i++)
        score[i]=0;

    /**
     * Game Settings Initialized
     */
    init_table(&t);
    init_pile(&dock_pile);
    init_pile(&disc_pile);
    for (i=0;i<CardTotal;i++)
    {
        data1->card_counter[i]=d;
    }
    printf("Shuffling cards...\n");
    printf("Shuffle result: # shuffle result only displayed in log and demo mode\n");
    if(log!=NULL)
    {
        fprintf(log,"Shuffling cards...\n");
        fprintf(log,"Shuffle result: # shuffle result only displayed in log and demo mode\n");
    }
    label= gtk_label_new("Shuffling cards...");
    gtk_fixed_put(GTK_FIXED(fixed), label,0 , 0);
    gtk_widget_set_size_request(label, 180, 35);

    label= gtk_label_new("Shuffle result:");
    gtk_fixed_put(GTK_FIXED(fixed), label,0 , 50);
    gtk_widget_set_size_request(label, 300, 35);

    shuffle_card(data1->card_counter,&dock_pile);//initializing dock_pile
    read_pile(window,fixed,&dock_pile,log,28);
    button= gtk_button_new_with_label("Continue");
    gtk_fixed_put(GTK_FIXED(fixed), button,600 , 800);
    gtk_widget_set_size_request(button, 100, 35);
    gtk_widget_show_all(fixed);
    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(deal_cards),data1);

}

void deal_cards(GtkWidget* widget, gpointer pt)
{
    if(widget==NULL){}

    animation_data *data1=(animation_data*) pt;
    GtkWidget *window=data1->windows;
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *fixed0 =data1->fixed;
    gtk_widget_destroy(fixed0);
    GtkWidget *fixed=gtk_fixed_new();
    data1->fixed=fixed;
    gtk_container_add(GTK_CONTAINER(window),fixed); ///Don't forget!!!
    int **card;
    int num=data1->num;
    int c=data1->c;
    int *score=data1->score;
    pile* dock_pile=data1->dock_pile;
    pile* disc_pile=data1->disc_pile;
    table *t=data1->t;
    FILE *log=data1->log;
    game_status *st=data1->st;
    int i,pl;

    int dir=1;
    st->dir=dir;
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
    data1->cards= card;
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
            card[pl][i]=pull_card(dock_pile,disc_pile,data1->card_counter,1,log);
            push_card(disc_pile,card[pl][i]);
        }
        qsort(card[pl],(size_t)c+1,sizeof(int),cmpf);
        add_player(t,init_player(c,card[pl],(score[pl])),dir);
    }
    if(log!=NULL)
        fprintf(log,"\n# only display current user for a real game, server and demo mode show all players\n");
    move(t,-dir);
    read_table(window,fixed,t,dir,log,1,NULL,1);

    printf("Determining the playing order...\n");
    if(log!=NULL)
        fprintf(log,"Determining the playing order...\n");
    *t=init_order(t,disc_pile,log);
    data1->pl=t->pl;
    char words[30];
    sprintf(words,"The game will start with player %d",data1->pl);
    label= gtk_label_new(words);
    gtk_fixed_put(GTK_FIXED(fixed), label,600 , 0);
    gtk_widget_set_size_request(label, 100, 35);

    init_status(st);
    st->dir=1;
    //add_card(t.next,test,4);
    //read_table(&t,dir,log,1);

    button= gtk_button_new_with_label("Continue");
    gtk_fixed_put(GTK_FIXED(fixed), button,600 , 800);
    gtk_widget_set_size_request(button, 100, 35);

    gtk_widget_show_all(fixed);


    g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(set_up_animation),data1);
}


#endif
