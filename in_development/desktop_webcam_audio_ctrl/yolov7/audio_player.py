import threading
import time

from playsound import playsound  # For playing audio.

thread_executing = False


class AudioPlayThread(threading.Thread):
    def __init__(self, name):
        global thread_executing
        threading.Thread.__init__(self)
        self.name = name
        thread_executing = True

    def run(self):
        global thread_executing
        print("Starting thread: " + self.name)
        # Get lock to synchronize threads
        # print_time(self.name, self.counter, 3)
        playsound('./assets/audio/no_drinks.wav')
        time.sleep(3)
        thread_executing = False
        # Free lock to release next thread


def thread_running() -> bool:
    global thread_executing
    return thread_executing
