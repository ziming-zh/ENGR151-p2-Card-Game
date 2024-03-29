# ENGR151-p2 Card Game

### Contributor(s) of this project:

*This project is completed solely by Zhou Ziming 521370910142. That means all of the files and codes have been scripted by the author individually.*

Name: Zhou Ziming

Student ID: 521370910142

identity: the ONLY contributor of this project

### The tasks finished in p2 with details:

##### Mandatory Part: 

- Milestone 1
  - Define the card structure: struct card_pile in "card_pile.h"
  - Define the player structure: struct player in "basic.h"
  - Accept command line arguments: in "main.c"
  - Write a function to play a card: separate it into card-related functions "add_card","read_card"and"delete_card" in "basic.c"
  - Write a function to draw (take) a card from the card pile: separate it into pile-related functions "push_card","pull_card" in "card_pile.c"
- Milestone 2
  - Write a function to shuffle decks of cards: function "shuffle_card" in "card_pile.c"
  - Write a function to sort deck of cards: now using the prototype qsort in "stdlib.h" and write a function cmp to activate it.
  - Write a function to deal the cards to the players: in function "game" part "initializing personal card"
  - Write a function to decide the first player: function "init_order" in "basic.c"
  - Write functions allowing several players to play together: function "animation" in "animation.c"
  - Write a function to calculate the round and game result": game_winning detection (embedded in function "game" in "main.c")
  - Define the deck data structure using an array (This part revised in milestone 3)
  - Arrange all the players in an array (This part revised in milestone 3)
- Milestone 3
  - Use a circular double linked list for the players; (struct "table" together with struct "player" in file "basic.h")
  - Use a dynamic array to handle the stock and discard piles; (struct "pile" together with struct "node" in file "card_pile.h")
  - Write a function to restore the stock pile; (to make the whole thing easier, I embedded the auto-detect and restore function in function "pull_card" so that every time I pull a card it will check whether the pile is empty and auto-fill the pile if it is empty)
  - Write a function to dump the game details into a log file; (log related function all stored in file "log.c")
  - Complete both the real game and the demo mode; (demo mode is not written in GTK version since the whole translation is too complex, the command-line demo showing function is "read_log" in file "log.c")
  - Proof-read you code and ensure it is fully complying with the C standard, C11 revision; 

*Special Notation:* NONE

##### Bonus Part: <1>

bonus <1> is for allowing players to play more than one card on their turn;

​	To enact this function go to file MUL_ENABLED.h and delete the comment mark at the beginning of line 7 or define a macro MUL in the command line;

​	Then add -m m or "--move=m" parameter to adjust the number of cards a player can play in one turn

*Special Notation:* The Bonus part 1 is not compatible with Bonus part 4. That means in the GTK mode you could only play one card at a time.

##### Bonus Part: <4>

bonus <4> is for ''Using a toolkit to implement a GUI"

​	for this bonus I'm using the gtk+3.0 package in mingw64

For further explanation please go to section "About the advanced-mode project (with GTK support)"


### Special things learnt in P2

1. create a log file: write in a file

2. create a demo mode: read file in carefully-designed way, cope  '\n' with getchar()

3. create a -h/--help (an instruction file './onecard.h') : function getopt together with getopt_long , properties of their argument optarg

4. redraw the screen after turn which requires detection of OS to complete differently: understanding of macro the usage of #ifdef / #ifndef in pre-compilation process

5. GTK  for graphic drawing and socket for remote connection:

   the usage of call_back function: a totally different strategy 

   the properties of widget/ container / fixed/ window

​	6. layer strategy

## About the simple-mode project (without GTK support)

### Introduction:

The onecard project is a very simple card-game. Very similar to the popular UNO game setting, players are given a certain amount of cards at the beginning and follow the rules to play their cards. If they cannot play cards according to the rule, they need to draw another one. And there are also special effects accompanying with special cards so the players might get punished with extra cards. The player who play out all of his cards the earliest will be the winner of the game and other players will get their score deducted according to the number of cards they remain.

### Instruction about the game procedure:

#### Game Start

For simple mode without GTK support, you could activate the program by calling in the command line WITH ARGUMENT ACCOMPANIED. The argument designed for the project is the same as listed in "p2.pdf file"

```
-h|--help print this help message
--log filename write the logs in filename (default: onecard.log)
-n n|--player-number=n n players, n must be larger than 2 (default: 4)
-c c|--initial-cards=c deal c cards per player, c must be at least 2 (default: 5)
-d d|--decks=d use d decks 52 cards each, d must be at least 2 (default: 2)
-r r|--rounds=r play r rounds, r must be at least 1 (default: 1)
-a|--auto run in demo mode
```

*Special Notation*:

1. for "--log" command you MUST input the file name to ensure the functioning of the program
2. if the basic game argument has not been set in the command line, it will be the default value as listed in help
3. for the auto mode please DO put the demo.log in the same repo with the execution file

#### Game mode:

there are three modes designed in the project:

1. regular mode: without log-writting and without demo
2. log mode: regular mode +log-writting. You will get a "xx.log" file after you finish the game (with all rounds end)
3. demo mode: a demo-reading mode, the user cannot manipulate the game process, only to see what had happened. *(DO put the demo.log in the same repo with the execution file)*

#### Game setting build-up:

After the game starts, the program will automatically setup the basic environment required for following animation. This process includes:

1. initializing dock_pile and disc_pile (fill the cards into them and successfully shuffle them)
2. initializing the table: put all the persons on the table
3. initializing the player: set up the personal cards, their number of cards as well as their score
4. determining the playing order: find the **minimum value card** of each player and find out the player whose **minimum value card has the minimum value**.

#### Game in-process:

- After all these procedures mentioned above, "getchar" from the user to let the user recognize those procedures.
- Then clear up the screen, present the current user the last played card and their current personal cards
- If they still have more than one available cards to play, then let them choose the card they want to play by inputting a number. The number represents the nth card we see from left to right. Then print out their personal cards after the play and "getchar" to ask for their recognition.
- If they have no available card to play, the program will give them no chance to play the cards and direcctly present their penalty and their personal cards after receiving the penalty. Of course there would be "getchar" to ask for their recognition.
- Then, if any player has successfully play all his cards out the program will automatically detect it and announce the game ends.
- After that the score of every gamer will be calculated by subtracting the number of cards remained in their hands. Then the round ends. If all rounds of the game end successfully, the game will end successfully.

## About the advanced-mode project (with GTK support)

### Instruction for proper compilation and execution:

I build up the advanced-mode project in IDE Clion with compiling configuration "mingw-64". 

To successfully make it work, install the package gtk+3.0 in msys2.

Add the mingw bin into the system path

Edit the CmakeList.txt file so that the flag gtk+ could be successfully added to the project (the CmakeList.txt file in the repo has been written in that way)

Then use Cmake to compile the whole project.

***Special Notification*：**

The GTK_based version contains display of pictures (as cards). When execute the project, do remember to put the folder "pukeImage" at the same path with the final execution file. Or else the card will not display

### Special Features in the GTK mode

- I've tried to make the GUI mode more close to a real game by adding "setting" and "score" to give the user more free space to handle the game.
- However, do make sure that there exists a setting.log and score.log file at the same path with the final execution file or else the game will output the error and exit with -1.
- The principle of GTK realization is based on call_back function (which means if a button is clicked then the according call_back function works.) The conventional methods designing the flow of function from the upper to the bottom of the file does not work. Therefore, huge revision has been made towards the original code especially the animation function.
- In this project I pay super attention to the memory usage. But in GTK mode, the handling of memory seems too opaque for me to figure it out thoroughly. I have debugged for a lot of time without encountering memory issue. But according to some claim on the Internet, the memory leak issue in GTK mode is very tricky therefore I hope I can be safe if any memory leak occurs because of the use of GTK. Hope TA could be careful towards that issue. THANKS A LOT!

### Instruction for the game process

#### Game Process:

The basic game process in this advanced mode is the same as the previous simple-mode. And with the help of GUI anything has been made easier. You can just click the card you want to play instead of inputting a number. And the getchar process is replaced by clicking the continue button. Other process are basically unchanged.

#### Game Startup:

For this advanced version. There is an opening GUI continuously waiting for you (if you start a new game, it will only open a new window without destroying the opening one), that means you can open as many as window as you want. You can play two games simultaneously, or see the score page at the same time of manipulating the setting, which makes the game extremely fun.

There are four initial button for the startup of the game:

1. start: obviously to start the game

2. score: display the score from "score.log" (remember to have the file and everytime a game end, the new score will rewrite the former one)

3. setting: display the score from "setting.log" (remember to have the file, and press save before quit if you want to put your own setting into action

   ​	More about the setting mode: there are basic properties (number of cards, rounds ... ) listed. Use the '-' and '+' button at their right to manipulate the value ('-' stands for 'minus one' while '+' stands for 'plus one')

4. quit: to formally quit the game.

## Project Structures

### Summary of each file mainly features:

##### main.c

get argument in the command line initialize the setting of the game, prepare for animation

##### basic.h

set up Player Related operation

structure Player/Player_Info/table/game_status

##### basic.c

function regarding table/player manipulation

##### card_pile.h

set up Pile Related operation

structure card_pile

##### card.c

function regarding card_pile manipulation

##### mgtk.c/mgtk.h

any gtk_related operation

##### log.c/log.h

any operation related with the log file

##### GTK_ENABLED.h

an independant file documenting whether GTK is defined or not

##### MUL_ENABLED.h

an independant file documenting whether MUL is defined or not

(a switch to turn on/off the multiple card playing mode)

### Bug Section:

No bugs detected in the project

## Future Goals:
- Think about revision of the project from the suggestions from instructors and the TA team. Learn to improve the efficiency of coding and enhance the ability for debugging (always contributing huge amount of time and effort trying to fix a bug)
