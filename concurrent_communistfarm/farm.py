#
# File: farm.py
#
# The farm is responsable for holding all the board state
#

import sys
import time
import random

from printer import printer
from datetime import datetime

from threading import Semaphore
import threading

class Square:
    def __init__(self, row, col):
        self.spawn_time = None # should be set by subclass
        self.resource_type = None # should be set by subclass
        self.last_used = datetime.now()
        self.occupant = None
        self.mutex = Semaphore(1)
        self.row = row
        self.col = col

    def available(self):
        return self.occupant == None

    def ascii_rep(self):
        """ returns a tuple of (square_letter, occupant_letter)
                occupant_letter might be the empty string
        """
        return (self.square_letter(),
                " " if self.available() else self.occupant.ascii_rep())

    def try_enter(self, animal, old_square = None):
        """ Try to enter the square. If `old_square` is provided, also
            leave `old_square`
        """
        assert(self != old_square, "trying to enter your current square causes deadlock.")
        with self.mutex:
            if self.available():
                self.occupant = animal
                animal.location = (self.row, self.col)
                if old_square != None:
                    old_square._leave(animal)
                return True
            else:
                return False

    def try_eat(self, animal):
        """ If `animal` can eat from `self`, reset `animal.hunger`,
            set `self.resource` to `None`, and return True. Otherwise,
            return false
        """
        return False
    def try_drink(self, animal):
        """ See `self.try_eat`. Works on `animal.thirst` instead.
        """
        return False

    def _leave(self, animal):
        with self.mutex:
            assert(animal == self.occupant)
            self.occupant = None

    def square_letter(self):
        """ returns the letter representing the type of square. """
        raise NotImplementedError("do it yoself")

class Grass(Square):
    def __init__(self, row, col):
        Square.__init__(self, row, col)
        self.spawn_time = 2.0
        self.resource = "grass"

    def try_eat(self, animal):
        with self.mutex:
            assert(self.occupant == animal)
            now = datetime.now()
            if (now - self.last_used).total_seconds() > self.spawn_time:
                animal.hunger -= 20.0
                if animal.hunger < 0.0:
                    animal.hunger = 0.0
                self.last_used = now
                return True
            return False

    def square_letter(self):
        return "G"

class Rock(Square):
    def __init__(self, row, col):
        Square.__init__(self, row, col)
        self.resource = "rock"

    def square_letter(self):
        return "R"

class Water(Square):
    def __init__(self, row, col):
        Square.__init__(self, row, col)
        self.spawn_time = 0.75 
        self.resource = "water"

    def try_drink(self, animal):
        with self.mutex:
            assert(self.occupant == animal)
            now = datetime.now()
            if (now - self.last_used).total_seconds() > self.spawn_time:
                animal.thirst -= 20.0
                if animal.thirst < 0:
                    animal.thirst = 0.0
                self.last_used = now
                return True
            return False

    def square_letter(self):
        return "W"


class Farm:
    def __init__(self, n):
        self.dim_row = n # height
        self.dim_col = n # width
        self.grass_cutoff = 0.4
        self.water_cutoff = 0.6
        self.rock_cutoff = 1.0
        # grid is a row-major representation
        self.grid = self._generate_grid()

    def appraise_square(self):
        """ Return the price of a square. Squares get more expensive the bigger
            your farm is.
        """
        return float(self.dim_row * self.dim_col) / 8.0

    def appraise_row(self):
        return self.appraise_square() * self.dim_col
    def appraise_col(self):
        return self.appraise_square() * self.dim_row

    def at(self, (row, col)):
        return self.grid[row][col]

    def num_available(self):
        num_available = 0
        for col in self.grid:
            for sq in col:
                if sq.available():
                    num_available += 1
        return num_available

    def as_rows(self):
        """ Returns an iterator over an ascii representation of the grid """
        yield (' ' + '_' * (self.dim_col * 3))
        divider = ' ' + '-' * (self.dim_col * 3)

        for r in xrange(self.dim_row):
            top_row = '|'
            bottom_row = '|'

            for c in xrange(self.dim_col):
                # uncomment to list row/col
                # square = str(self.grid[r][c].row)
                # occupant = str(self.grid[r][c].col)
                (square, occupant) = self.grid[r][c].ascii_rep()
                top_row += square + "/|" 
                bottom_row += "/" + occupant + "|"

            yield top_row
            yield bottom_row
            yield divider

    def make_square(self, row, col):
            rand = random.random()
            if rand < self.grass_cutoff:
                return Grass(row, col)
            elif rand < self.water_cutoff:
                return Water(row, col)
            elif rand < self.rock_cutoff:
                return Rock(row, col)
            assert(False)

    def _generate_grid(self):
        """ procedurally generates a grid of different squares based
            on `self.dim`.
            @returns the grid of squairs
        """
        return [[self.make_square(row, col) for col in range(self.dim_col)]
                        for row in range(self.dim_row)]

    def delete_row(self):
        self.dim_row -= 1

        for c in range(self.dim_col):
            with self.grid[self.dim_row][c].mutex:
                if self.grid[self.dim_row][c].occupant:
                    self.grid[self.dim_row][c].occupant.force_move()
                    self.grid[self.dim_row][c].occupant = None

    def delete_column(self):
        self.dim_col -= 1

        for r in range(self.dim_row):
            with self.grid[r][self.dim_col].mutex:
                if self.grid[r][self.dim_col].occupant:
                    self.grid[r][self.dim_col].occupant.force_move()
                    self.grid[r][self.dim_col].occupant = None

    def new_column(self):
        """ Mutably adds a new column to the grid. """
        for r in range(self.dim_row):
            self.grid[r].append(self.make_square(r, self.dim_col))

        self.dim_col += 1

    def new_row(self):
        """ Mutably adds a new row to the grid """
        self.grid.append([self.make_square(self.dim_row, col) for \
            col in range(self.dim_col)])

        self.dim_row += 1

#
# For testing
#
#

def draw_board(farmyard):
    while not draw_board.done:
        time.sleep(0.25)
        printer.display("trying to re-draw board")
        printer.draw_farm(farmyard.as_rows())
draw_board.done = False

# If run as a script, run the unit tests for this module.
# otherwise, don't
if __name__ == "__main__":
    printer.start()

    f = Farm(8)
    # printer.draw_farm(f.as_rows())

    draw_thread = threading.Thread(target=draw_board, args=(f,))
    draw_thread.daemon = True
    draw_thread.start()

    time.sleep(0.05)
    # consume the prompts
    for _ in printer.prompt_inputs():
        pass

    printer.join()
    # draw_board.done = True
    draw_thread.join()

