import multiprocessing as mp
from src.gui import GUI
import pygame
from time import sleep
from board import Board, BoardArgs


def calc(params, q, lock):
    boardArgs = BoardArgs()
    boardArgs.states = params["Cc"]
    boardArgs.neighborhoodRadius = params["Rr"]
    boardArgs.birthConds = set(params["Bb"])
    boardArgs.surviveConds = set(params["Ss"])
    boardArgs.isMooreType = params["Nn"] == "m"
    boardArgs.isIncludeCenter = params["Mm"]
    try:
        board = Board(boardArgs, params["board"])
    except Exception:
        board = Board(boardArgs)
    while True:
        cells = board.getCells()
        lock.acquire()
        q.put(cells)
        lock.release()
        sleep(0.1)
        board.update()


def update_loop(gui, lock, q, p):
    sleep(1)
    while not gui.if_close():
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                gui.set_gui_close()
                p.kill()
        lock.acquire()
        new_values = q.get()
        lock.release()
        gui.game.update(new_values)
        pygame.display.flip()
        sleep(1)


def main():
    lock = mp.Lock()
    pygame.init()
    q = mp.Queue()
    gui = GUI()
    gui.choosing_menu()
    sleep(0.1)
    p = mp.Process(target=calc, args=(gui.get_params(), q, lock,))
    p.start()
    update_loop(gui, lock, q, p)


if __name__ == '__main__':
    main()
    pygame.quit()
