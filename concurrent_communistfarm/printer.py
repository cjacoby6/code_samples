#
# file: printer.py
#
# The printer has sole ownership of the terminal. All
# other threads should talk to it for display and logging.
# Printer is basically an actor.
#

import Queue
import threading
import curses
import collections
import time

def init_windows():
    wins = collections.OrderedDict()

    screen = curses.initscr()
    screen.border(0)
    wins["screen"] = screen

    y, x = screen.getmaxyx()

    farm_win = curses.newwin(y, x//2, 0, 0)
    farm_win.border(0)
    farm_win.addstr(5, 8, "farm")
    wins["farm"] = farm_win

    stat_win = curses.newwin(y//2, x//2, 0, x//2)
    stat_win.border(0)
    stat_win.addstr(5, 8, "stats")
    wins["stats"] = stat_win

    # the console
    input_win = curses.newwin(y//2 + 1, x//2, y//2, x//2)
    input_win.border(0)
    wins["input"] = input_win

    return (screen, wins)


class Printer:
    """ This class should never be accessed directly.
        There must be exactly one printer object for the whole
        program, so other modules should instead access the
        printer object exported by this module. If only modern
        programming languages had advanced to the point where you
        could mark some module members as public and others as private.
    """

    def __init__(self):
        self.q = Queue.Queue()

        # Invariant: If (not self.open) all computation by the prompt and printer threads
        # must terminate in a finite ammount of time.
        self.open = True
        (self.screen, self.wins) = init_windows()

        maxy, maxx = self.screen.getmaxyx()
        self.console_log = collections.deque(maxlen=((maxy//2) - 3))
        self.farm_model = []
        self.stats_model = []
        self.print_thread = threading.Thread(target=self.run, args=())

        # Without this semaphore attempts to join the printer thread can result in deadlock
        self.stopped = threading.Semaphore(0)

        self.die_cb = None

    def start(self):
        self.print_thread.start()

    def join(self):
        self.stopped.acquire() # allows the printer to continue to make progress
        self.open = False
        self.print_thread.join()
        curses.endwin()

    def on_die(self, die_cb):
        """ die_cb should be function of arity zero """
        self.die_cb = die_cb

    def stop(self):
        if self.die_cb != None:
            self.die_cb()
        self.stopped.release()

    def prompt_inputs(self):
        """ Read user input from the prompt (in the input window), and yield each
            new input string as the next thing in an infinite iterator. This provides
            a nice way for the calling thread to block until something interesting happens.
        """
        y, x = self.screen.getmaxyx()
        y -= 2
        x = (x//2) + 4

        self.__clear_prompt()
        s = self.screen.getstr(y, x)
        while s != 'q' and self.open:
            self.display(" > " + s)
            self.__clear_prompt()
            yield s
            s = self.screen.getstr(y,x)

        if s == 'q':
            self.stop()
            raise StopIteration

    def __clear_prompt(self):
        if not self.open:
            return
        self.q.put(("clear_prompt", ()))

    def run(self):
        y, x = self.screen.getmaxyx()

        empty_line = " > " + ( " " * (x//2 - 5) )

        while self.open or (not self.q.empty()):
            msg = None
            tag = None
            try:
                (tag, msg) = self.q.get(timeout=1)
            except Queue.Empty:
                continue

            if "display" == tag:
                self.console_log.append(msg)
            elif "draw_farm" == tag:
                self.farm_model = msg
            elif "clear_prompt" == tag:
                self.wins["input"].addstr(y//2 - 2, 1, empty_line)
                self.wins["input"].addstr(y//2 - 1, 1, empty_line)
            elif "display_stats" == tag:
                self.stats_model = msg
            elif "clear" == tag:
                self._clear_board()
            # only draw when we get a message
            self.draw_tick()

    def _clear_board(self):
        y, x = self.screen.getmaxyx()
        hash_line = "#" * ((x//2) - 2)
        blank_line = " " * ((x//2) - 2)

        input_window = self.wins["input"]
        farm_window = self.wins["farm"]
        stats_window = self.wins["stats"]

        for offset in range((y//2) - 3):
            input_window.addstr(1 + offset, 1, hash_line)
            stats_window.addstr(1 + offset, 1, hash_line)

            input_window.addstr(1 + offset, 1, blank_line)
            stats_window.addstr(1 + offset, 1, blank_line)
        for offset in range(y - 2):
            farm_window.addstr(1 + offset, 1, hash_line)
            farm_window.addstr(1 + offset, 1, blank_line)

    def draw_tick(self):
        y, x = self.screen.getmaxyx()

        blank_line = " " * ((x//2) - 2)

        input_window = self.wins["input"]
        farm_window = self.wins["farm"]
        stats_window = self.wins["stats"]

        for offset, line in enumerate(self.console_log):
            input_window.addstr(1 + offset, 1, blank_line)
            input_window.addstr(1 + offset, 2, line)

        for offset, line in enumerate(self.farm_model):
            farm_window.addstr(1 + offset, 1, blank_line)
            farm_window.addstr(1 + offset, 2, line)

        for offset, line in enumerate(self.stats_model):
            stats_window.addstr(1 + offset, 1, blank_line)
            stats_window.addstr(1 + offset, 2, line)

        self.refresh_all_windows()

    def refresh_all_windows(self):
        for (_, win) in self.wins.iteritems():
            win.refresh()

    def draw_farm(self, board):
        if not self.open:
            return
        self.q.put(("draw_farm", board))

    def display_stats(self, stats):
        if not self.open:
            return
        self.q.put(("display_stats", stats))

    def clear_display(self):
        if not self.open:
            return
        self.q.put(("clear", ()))

    def display(self, message):
        """ Print a message to console """
        if not self.open:
            return
        self.q.put(("display", message))

    def get_max_farm_size(self):
        return self.wins["farm"].getmaxyx()


printer = Printer()


if __name__ == "__main__":

    printer.start()

    for p in printer.prompt_inputs():
        printer.display(p)

    printer.join()

    printer.display("You should not see this message")
