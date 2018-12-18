#
# File: animal_farm.py
#
# The main entry point for the program. To start playing the
# game run `python animal_farm.py`
#
#

import farm
from animal import Animal
from stats import Stats
import animal
from printer import printer
import threading
from multiprocessing import Process
import time

INIT_BOARD_SIZE = 8
NUM_ANIMALS = 2
REDRAW_TICK_TIME = 0.09166666666
MAX_COMMUNISM_METER = 100.0


def draw_board(farmyard):
    while True:
        time.sleep(REDRAW_TICK_TIME)
        printer.draw_farm(farmyard.as_rows())

def stats_loop(stats):
    while True:
        time.sleep(REDRAW_TICK_TIME)
        stats.update()
        printer.display_stats(stats.to_print())

def new_animals(farmyard, animals, new_as):
    """ Should only be called by prompt thread. """
    new_threads = [threading.Thread(target=a.run) for a in new_as]
    for t in new_threads:
        t.daemon = True
    animals.extend(zip(new_as, new_threads))
    map(lambda t: t.start(), new_threads)
    return animals

def sell_animals(animals, comp, num, stats):
    n = 0
    for (a, t) in animals:
        with a.mutex:
            if comp(a) and not a.dead:
                m = a.appraise()
                a.die("Sold")
                stats.update(money = m)
                printer.display("Sold for {}".format(m))
                n += 1
                if n == num:
                    return n

    printer.display("Asked to sell {} but only {} alive".format(num, n))
    return n

def prompt_loop(farmyard, animals, stats):
    """ Subscribe to the user input and transition to a revolutionary state if
        the animals become communist enough.
    """
    for message in printer.prompt_inputs():
        message = message.upper()
        c = message.split()
        handle_message(c, farmyard, animals, stats)
        if stats.communism > MAX_COMMUNISM_METER:
            return pinko_prompt_loop()

def pinko_prompt_loop():
    """ Subscribe to the user input, but don't act on it. Capitalists have no
        power here.
    """
    stats.update(money = -stats.money)
    for _ in printer.prompt_inputs():
        printer.display("The people's revolution has arrived!")
        printer.display("You have no power here, capitalist pig-dog!")

def handle_message(msg, farmyard, animals, stats):

    # emtpy line
    if len(msg) == 0:
        return

    # quit command
    if msg[0] == "Q":
        return animals

    if msg[0] == "MONEY":
        stats.update(money = 100000)
        return

    if msg[0] == "CLEAR":
        printer.clear_display()
        return

    # invalid command
    if len(msg) != 3:
        printer.display("Usage: [BUY | SELL] [ANIMAL | LAND] [AMOUNT | TYPE]")
        return

    if msg[0] == 'BUY':
        handle_buy(msg, farmyard, animals, stats)
    elif msg[0] == 'SELL':
        handle_sell(msg, farmyard, animals, stats)
    else:
        printer.display("USAGE: BUY ANIMAL_NAME NUM_ANIMALS")


def handle_buy(msg, farmyard, animals, stats):

    product = msg[1]

    if product == "LAND":
        dimension = msg[2]

        if not dimension_check(dimension, farmyard):
            printer.display("UNABLE PURCHASE LAND")
            return

        if dimension == 'COL':
            cost = farmyard.appraise_col()
            if cost < stats.money:
                farmyard.new_column()
                stats.update(money = -farmyard.appraise_col())
                printer.display("You bought a new column of land!")
            else:
                printer.display("Not enough money!")
        elif dimension == 'ROW':
            cost = farmyard.appraise_row()
            if cost < stats.money:
                farmyard.new_row()
                stats.update(money = -farmyard.appraise_row())
                printer.display("You bought a new row of land!")
            else:
                printer.display("Not enough money!")
        else:
            printer.display("USAGE: BUY LAND [COL/ROW]")
    else:
        quantity = msg[2]

        if not product_check(product, int(quantity), farmyard):
            printer.display("UNABLE TO PURCHASE: " + str(quantity) + " " + str(product))
            return

        if product == 'SHEEP':
            if quantity.isdigit():
                quantity = int(quantity)
                if quantity > (stats.money / animal.Sheep.base_price):
                    quantity = int(stats.money / animal.Sheep.base_price)
                    printer.display("You only have enough money for {} new sheep".format(quantity))
                printer.display("You bought {} new sheep!".format(quantity))
                new_sheep = [ animal.Sheep(farmyard) for _ in range(quantity)]
                animals = new_animals(farmyard, animals,new_sheep)
                stats.update(money = -(quantity * animal.Sheep.base_price))
            else:
                printer.display("USAGE: BUY ANIMAL_NAME NUM_ANIMALS")
        elif product == 'HORSE':
            if quantity.isdigit():
                quantity = int(quantity)
                if quantity > (stats.money / animal.Horse.base_price):
                    quantity = int(stats.money / animal.Horse.base_price)
                    printer.display("You only have enough money for {} new horses".format(quantity))
                printer.display("You bought {} new horses!".format(quantity))
                new_horses = [ animal.Horse(farmyard) for _ in range(quantity)]
                animals = new_animals(farmyard, animals, new_horses)
                stats.update(money = -(quantity * animal.Horse.base_price))
            else:
                printer.display("USAGE: BUY ANIMAL_NAME NUM_ANIMALS")
        elif product == 'PIG':
            if quantity.isdigit():
                quantity = int(quantity)
                if quantity > (stats.money / animal.Pig.base_price):
                    quantity = int(stats.money / animal.Pig.base_price)
                    printer.display("You only have enough money for {} new pigs".format(quantity))
                printer.display("You bought {} new pigs!".format(quantity))
                new_pigs = [ animal.Pig(farmyard) for _ in range(quantity)]
                animals = new_animals(farmyard, animals, new_pigs)
                stats.update(money = -(quantity * animal.Pig.base_price))
            else:
                printer.display("USAGE: BUY ANIMAL_NAME NUM_ANIMALS")


def handle_sell(msg, farmyard, animals, stats):
    product = msg[1]

    if product == 'LAND':
        dimension = msg[2]

        if dimension == 'COL':
            farmyard.delete_column()
        elif dimension == 'ROW':
            farmyard.delete_row()
        else:
            printer.display("USAGE: SELL LAND [COL/ROW]")

    else:
        quantity = msg[2]

        if product == 'SHEEP':
            if quantity.isdigit():
                quantity = sell_animals(animals, lambda x: x.ascii_rep() == 'S', int(quantity), stats)
                printer.display("You sold {} sheep!".format(quantity))
            else:
                printer.display("USAGE: SELL ANIMAL_NAME NUM_ANIMALS")
        elif product == 'HORSE':
            if quantity.isdigit():
                quantity = sell_animals(animals, lambda x: x.ascii_rep() == 'H', int(quantity), stats)
                printer.display("You sold {} horses!".format(quantity))
            else:
                printer.display("USAGE: SELL ANIMAL_NAME NUM_ANIMALS")
        elif product == 'PIG':
            if quantity.isdigit():
                quantity = sell_animals(animals, lambda x: x.ascii_rep() == 'P', int(quantity), stats)
                printer.display("You sold {} pigs!".format(quantity))
            else:
                printer.display("USAGE: SELL ANIMAL_NAME NUM_ANIMALS")

def dimension_check(dim, farmyard):

    maxy, maxx = printer.get_max_farm_size()

    max_col = (maxx - 5) // 3 - 1 # dont' ask idk
    max_row = (maxy - 5) // 3

    if dim == "COL":
        return farmyard.dim_col + 1 <= max_col
    elif dim == "ROW":
        return farmyard.dim_row + 1 <= max_row

def product_check(product, quantity, farmyard):
    total = farmyard.dim_col * farmyard.dim_row
    capacity = total * 0.75

    num_occupied = total - farmyard.num_available()

    if num_occupied + quantity <= capacity:
        return True
    else:
        return False

# If run as a script, run the unit tests for this module.
# otherwise, don't
if __name__ == "__main__":
    printer.start()

    farmyard = farm.Farm(INIT_BOARD_SIZE)

    # Set up the page refresh metronome
    draw_thread = threading.Thread(target=draw_board, args=(farmyard,))
    draw_thread.daemon = True
    draw_thread.start()

    # Make the Animals
    animals = [(a, threading.Thread(target=a.run)) for 
                 a in [Animal.make_random_animal(farmyard) for _ in range(NUM_ANIMALS)]]
    for (a, t) in animals:
        t.daemon = True
    map(lambda (_,t): t.start(), animals)

    # Set up the stats window
    stats = Stats(animals, farmyard)
    stats_thread = threading.Thread(target=stats_loop, args=(stats,))
    stats_thread.daemon = True
    stats_thread.start()

    # We shouldn't need this, but curses is not happy otherwise
    time.sleep(0.05)
    # consume the prompts
    prompt_loop(farmyard, animals, stats)
    # for _ in printer.prompt_inputs():
    #    pass

    printer.join()
