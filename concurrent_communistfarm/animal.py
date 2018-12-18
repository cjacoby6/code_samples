#
# File: animal.py
#
# The animal object hiarchy
#
import random
from printer import printer
from time import sleep
from farm import Farm
from threading import Thread
from threading import Semaphore

ANIMAL_SLEEP_TIME = 0.5
MAX_HUNGER = 100.0
MAX_THIRST = 100.0
MAX_SICKNESS = 100.0
MAX_PROLETARIAT_RAGE = 100.0
COMPLAINT_RATIO = 0.1 # The amount of other's proletariat rage
                      # that an animal gets on a collision

# Hunger increases at `DIGESTION_RATE * metabolism`
DIGESTION_RATE = 0.1

# Thirst increases at `THIRST_RATE * metabolism`
THIRST_RATE = 0.2

from farm import Farm

class Animal:
    def __init__(self, farmyard):
        self.hunger = MAX_HUNGER / 2.0
        self.thirst = MAX_THIRST / 2.0
        self.sickness = 0.0
        self.proletariat_rage_m = Semaphore(1)
        self.proletariat_rage = 1.0 # new animals start off at 1/100
        self.farmyard = farmyard
        self.metabolism = None
        self.mutex = Semaphore(1)

        self.dead = False

        self.location = (0,0)
        self._choose_starting_position()

    @staticmethod
    def make_random_animal(farmyard):
        """ Returns a randomly generated animal. """
        return random.choice([Sheep, Horse, Pig])(farmyard)

    def _choose_starting_position(self):
        """ Causes the animal to choose and attempt to enter a
        starting position on the board.
        """
        row = random.randrange(self.farmyard.dim_row)
        col = random.randrange(self.farmyard.dim_col)

        success = self.farmyard.grid[row][col].try_enter(self)
        if not success:
            self._choose_starting_position()

    def force_move(self):
        self._choose_starting_position()
        return

    def die(self, reason):
        self.dead = True
        printer.display("{} in {} died with reason: {}"
                        .format(self.ascii_rep(), self.location, reason))

    def run(self):
        """ The loop that each animal follows """
        while not self.dead:
            self.move()
            current_square = self.farmyard.at(self.location)

            current_square.try_eat(self)
            self.hunger += DIGESTION_RATE * self.metabolism

            current_square.try_drink(self)
            self.thirst += THIRST_RATE * self.metabolism

            sleep(ANIMAL_SLEEP_TIME)

            if self.thirst > MAX_THIRST:
                with self.mutex:
                    self.die("Thirst")
            if self.hunger > MAX_HUNGER:
                with self.mutex:
                    self.die("Hunger")

        # The animal died
        current_square = self.farmyard.at(self.location)
        current_square._leave(self)

    def in_bounds(self, (target_r, target_c)):
        return(target_r < self.farmyard.dim_row and target_r >=0 \
           and target_c < self.farmyard.dim_col and target_c >= 0)

    def move(self):
        """ generate random movement and go there if not occupied
        """
        direction = random.randint(1, 5)

        current_square = self.farmyard.at(self.location)

        target = self.location
        if direction == 1:
            target = (target[0] + 1, target[1])
        elif direction == 2:
            target = (target[0] - 1, target[1])
        elif direction == 3:
            target = (target[0], target[1] + 1)
        elif direction == 4:
            target = (target[0], target[1] - 1)

        if self.in_bounds(target):
            if not self.farmyard.at(target).try_enter(self, old_square=current_square):
                # collision
                self.agitate(target)

    def agitate(self, target):
        """ Talk to the animal in the target square, sharing complaints about the
            capitalist oppressors
        """
        other = self.farmyard.at(target).occupant
        if other != None and other != self:
            own_rage = self.proletariat_rage
            other_rage = None
            with other.proletariat_rage_m:
                other_rage = other.proletariat_rage
                other.proletariat_rage += own_rage * COMPLAINT_RATIO
            with self.proletariat_rage_m:
                self.proletariat_rage += other_rage * COMPLAINT_RATIO

    def ascii_rep(self):
        raise NotImplementedError("do it yoself")

    def appraise(self):
        ratio_value =  float((MAX_HUNGER - self.hunger) + (MAX_THIRST - self.thirst)) / \
                       (float(MAX_HUNGER + MAX_THIRST) / 2.0)
        return self.base_price * ratio_value

class Sheep(Animal):
    base_price =  100
    def __init__(self, farmyard):
        Animal.__init__(self, farmyard)
        self.metabolism = 1

    def ascii_rep(self):
        return 'S'

class Horse(Animal):
    base_price = 200
    def __init__(self, farmyard):
        Animal.__init__(self, farmyard)
        self.metabolism = 1

    def ascii_rep(self):
        return 'H'

class Pig(Animal):
    base_price = 60
    def __init__(self, farmyard):
        Animal.__init__(self, farmyard)
        self.metabolism = 1

    def ascii_rep(self):
        return 'P'


#
# Tests
#

def start_animal(animal):
    """ Returns a (running) animal thread handle """
    def start_it(a):
        a.run()
    t = Thread(target=start_it, args=(animal,))
    t.start()
    return t


if __name__ == "__main__":
    printer.start()
    f = Farm(8)
    s = Sheep(f)
    s_t = start_animal(s)
    printer.draw_farm(f.as_rows())
    s_t.join()
    printer.stop()
    printer.join()
