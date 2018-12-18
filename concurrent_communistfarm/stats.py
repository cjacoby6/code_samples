

class Stats:
    def __init__(self, animals, farmyard):
        self.animals = animals
        self.farmyard = farmyard
        self.occupied = 0
        self.thirst = 0
        self.hunger = 0 
        self.communism = 0
        self.money = 100
        self.sheep = 0
        self.horse = 0
        self.pig = 0

        self.update()

    def update(self, money = 0):
        alive = self.alive_animals()

        self.calc_percentage(alive)
        self.calc_animal_status(alive)
        self.count_animals(alive)

        self.money += money

        return

    def alive_animals(self):
        return filter(lambda (a, _): not a.dead, self.animals)

    def count_animals(self, animals):
        s = 0
        h = 0
        p = 0

        for (a, _) in animals:
            if a.ascii_rep() == 'S':
                s += 1
            elif a.ascii_rep() == 'H':
                h += 1
            elif a.ascii_rep() == 'P':
                p += 1

        self.sheep = s
        self.horse = h
        self.pig = p

        return

    def calc_percentage(self, animals):
        self.occupied = float(len(animals)) /  \
                        float((self.farmyard.dim_row * self.farmyard.dim_col))

        return

    def calc_animal_status(self, animals):
        thirst = 0
        hunger = 0
        rage = 0
        num_animals = len(animals)

        for (a, _) in animals:
            thirst += a.thirst
            hunger += a.hunger
            rage += a.proletariat_rage

        self.thirst = float(thirst) / float(num_animals) if num_animals > 0 else 0
        self.hunger = float(hunger) / float(num_animals) if num_animals > 0 else 0
        self.communism = float(rage) / float(num_animals) if num_animals > 0 else 0

        return

    def to_print(self):
        return ["MONEY: " + str(self.money),
                "SHEEP: " + str(self.sheep),
                "PIGS: " + str(self.pig),
                "HORSE: " + str(self.horse),
                "PERCENTAGE OCCUPIED: " + str(self.occupied),
                "AVERAGE THIRST: " + str(self.thirst),
                "AVERAGE HUNGER: " + str(self.hunger),
                "COMMUNISM METER: " + str(self.communism)]
