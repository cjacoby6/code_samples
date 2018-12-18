# Jones Farming Simulator   

=== Team Members ===

Ethan Pailes
Cori Jacoby
Kabir Singh

=== Summary ===

Text-based farmyard simulator with subtle communist themes.

=== Supported platforms ===

  Ethan's Laptop (No, seriously)

=== Code Overview ===

    animal.py - implementation of the animal class, contains subclasses 
                Sheep, Horse, and Pig. An animal is a thread that lives 
                in the farmyard and has attributes like hunger, thirst, 
                and proletariat rage. As the simulation runs, its member
                variables change as a product of the animal’s interactions
                with the environment and other farmyard animals.

    animal_farm.py - the main “entry point” of the game and the de facto 
                     “game manager.” The animal farm python file handles 
                     initializing the printer singleton, initializing the
                     farmyard and its occupants, running the prompt loop, 
                     and kickstarting the drawing and stat-keeping threads. 
                     Game-state functions, such as handling user input, 
                     are in this file.

    farm.py - collection of gridsquares, can have a resource type, and 
              hold an animal. 

    printer.py - the wrapper class for curses and handler of all 
                 visual-related items. It consists of a set of curses 
                 windows, a queue of messages (a la gen_server), a 
                 queue of console messages for output, a visual model
                 of the farm and stats to display in curses, and a
                 printing thread. It exists as a thread-safe singleton
                 shared between all other aspects of the program.

    stats.py - a class to hold statistics about the farm simulator, 
               such as the number of animals present, the average hunger 
               or thirst of the animals, and the “amount” of communism
               present. The contents of the stats object are displayed 
               in a curses window.

=== Instructions to run ===

To run the program, simply type the following while in the same directory:

	python animal_farm.py

(Please ensure that you are running with Python2.7, and have the curses 
library installed). 

=== User Interaction ===

Once the program has been launched, the simulation will begin. Commands
can be typed in the following format:

	BUY|SELL LAND ROW|COL
    BUY|SELL SHEEP|PIG|HORSE [QUANTITY]

Some example commands include:

    BUY HORSE 12
    SELL LAND ROW
 
At any point the quit command can be entered, which will immediately 
shut the game down:

    q

A user can also refresh the prompt screen simply using:

    clear

If you want more money quickly, you can type:

    money

All commands are case insensitive.
