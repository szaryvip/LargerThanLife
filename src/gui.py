from typing import Any, List, Dict
import pygame
from src.game import Game
import json
import tkinter
import tkinter.filedialog
import pygame_menu
import myrandoms
from tkinter import messagebox


class GUI:
    _screen = None
    _size = [600, 600]
    _close_gui = False
    _menu = None
    _manual = None
    _params = {}
    game = None

    def __init__(self) -> Any:
        self._close_gui = False
        pygame.display.set_caption('Larger Than Life')
        self._screen = pygame.display.set_mode(self._size)

    def get_params(self) -> Dict:
        return self._params

    def if_close(self) -> bool:
        """Checks if program can close gui

        Returns:
            bool: close gui or not
        """
        return self._close_gui

    def set_gui_close(self) -> None:
        """Sets close_gui to true"""
        self._close_gui = True

    def create_game(self) -> None:
        """Ask how to initialize alive cells
        """
        self._init_board = pygame_menu.Menu("How to initialize alive cells",
                                            600, 600)
        self._init_board.add.button("Manual (click Enter to start)",
                                    self.create_game_manual)
        self._init_board.add.button("Random", self.create_game_random)
        self._init_board.mainloop(self._screen)

    def clear_screen(self) -> None:
        """Clears screen from menus
        """
        self._menu.disable()
        self._init_board.disable()
        if self._manual is not None:
            self._manual.disable()
        self._screen.fill((0, 0, 0))

    def create_game_manual(self) -> None:
        """Creates game and provide opportunity
        to provide manualy which cells are alive.
        """
        self.clear_screen()
        self.game = Game(self._screen, self._params)
        pygame.display.flip()
        start = False
        self._params["board"] = []
        for _ in range(self._size[0]//self.game._tile_size):
            columns = self._size[1]//self.game._tile_size
            self._params["board"].append([0 for x in range(columns)])
        while not start:
            for event in pygame.event.get():
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_RETURN:
                        start = True
                if event.type == pygame.MOUSEBUTTONUP:
                    x, y = pygame.mouse.get_pos()
                    tile_size = self.game._tile_size
                    x, y = x//tile_size, y//tile_size
                    if self._params["board"][x][y] == 0:
                        pygame.draw.rect(self._screen, self.game._alive_color,
                                         pygame.Rect(x*(tile_size)+1,
                                                     y*(tile_size)+1,
                                                     tile_size-2,
                                                     tile_size-2))
                        self._params["board"][x][y] = 1
                    else:
                        pygame.draw.rect(self._screen, self.game._dead_color,
                                         pygame.Rect(x*(tile_size)+1,
                                                     y*(tile_size)+1,
                                                     tile_size-2,
                                                     tile_size-2))
                        self._params["board"][x][y] = 0
                    pygame.display.flip()

    def create_game_random(self) -> None:
        """Creates game without manualy provided alive cells.
        """
        self.clear_screen()
        self.game = Game(self._screen, self._params)
        pygame.display.flip()

    def prepare_rules(self) -> None:
        """Prepares rules from widgets from manual menu
        """
        widgets = self._manual.get_widgets()
        self._params["Rr"] = int(widgets[1].get_value())
        self._params["Cc"] = int(widgets[2].get_value())
        self._params["Mm"] = bool(widgets[3].get_value()[0][1])
        self._params["Nn"] = widgets[4].get_value()[0][1]
        Bb_cond = widgets[5].get_value()
        self._params["Bb"] = self.check_condition(Bb_cond)
        Ss_cond = widgets[6].get_value()
        self._params["Ss"] = self.check_condition(Ss_cond)

    def get_params_from_json(self, path: str) -> None:
        """Loads parameters from json file

        Args:
            path (str): path to json file with params
        """
        with open(path, "r") as file:
            json_object = json.load(file)
            self._params["Rr"] = json_object["Rr"]
            self._params["Cc"] = json_object["Cc"]
            self._params["Mm"] = json_object["Mm"]
            self._params["Nn"] = json_object["Nn"]
            self._params["Bb"] = self.check_condition(json_object["Bb"])
            self._params["Ss"] = self.check_condition(json_object["Ss"])

    def export_rules(self) -> None:
        """Export manually provided rules to json file
        """
        self.prepare_rules()
        self._params["Bb"] = self._manual.get_widgets()[5].get_value()
        self._params["Ss"] = self._manual.get_widgets()[6].get_value()
        file_name = self._manual.get_widgets()[0].get_value()
        json_object = json.dumps(self._params, indent=4)
        with open(f"{file_name}.json", "w") as file:
            file.write(json_object)

    def prompt_file(self) -> None:
        """Create a Tk file dialog to choose json file
        with rules for the game and cleanup when finished"""
        top = tkinter.Tk()
        top.withdraw()
        file_name = tkinter.filedialog.askopenfilename(parent=top)
        top.destroy()
        self.get_params_from_json(file_name)
        self.create_game()

    def check_condition(self, text_cond: str) -> List[int]:
        """Gets conditions from string
        Example: from '2-5,7' gets [2,3,4,5,7]

        Args:
            text_cond (str): string with rules in format '2-3,5'

        Raises:
            ValueError: when format of string is not correct

        Returns:
            List[int]: list with conditions
        """
        conditions = text_cond.split(',')
        conds = []
        for cond in conditions:
            if '-' in cond:
                from_number, to_number = cond.split('-')
                if from_number.isdigit() and to_number.isdigit():
                    for i in range(int(from_number), int(to_number)+1):
                        conds.append(i)
                else:
                    raise ValueError("Incorrect input in birth conditions")
            elif cond.isdigit():
                conds.append(int(cond))
            else:
                raise ValueError("Incorrect input in birth conditions")
        return conds

    def start_with_manual_rules(self) -> None:
        """Prepare rules provided and start main loop of the game
        """
        self.prepare_rules()
        self.create_game()

    def start_with_random_rules(self) -> None:
        """Generates random rules and export them to 'actual_rules.json'"""
        self._params["Cc"] = myrandoms.random_states_number()
        self._params["Rr"] = myrandoms.random_range()
        birth_params = myrandoms.random_birth()
        Bb = ""
        for x in birth_params:
            Bb += str(x)
            Bb += ","
        self._params["Bb"] = Bb[:-1]
        Ss = ""
        survive_params = myrandoms.random_survive()
        for x in survive_params:
            Ss += str(x)
            Ss += ","
        self._params["Ss"] = Ss[:-1]
        self._params["Nn"] = myrandoms.random_neighbourhood_type()
        self._params["Mm"] = myrandoms.random_middle_include()
        json_object = json.dumps(self._params, indent=4)
        with open("actual_rules.json", "w") as file:
            file.write(json_object)
        self._params["Bb"] = birth_params
        self._params["Ss"] = survive_params
        self.create_game()

    def choosing_menu(self) -> None:
        """Shows main menu. Here it is possible to choose source
        of rules for the game"""
        self._menu = pygame_menu.Menu("How to import parameters",
                                      self._size[0], self._size[1])
        self._menu.add.button('Import from json', self.prompt_file)
        self._menu.add.button('Manual', self.manual_menu)
        self._menu.add.button('Random', self.start_with_random_rules)
        self._menu.add.button('Quit', pygame_menu.events.EXIT)
        try:
            self._menu.mainloop(self._screen)
        except ValueError:
            top = tkinter.Tk()
            top.withdraw()
            messagebox.showerror("Bad file", "Try again with other file")
            top.destroy()
            self.choosing_menu()

    def manual_menu(self) -> None:
        """Shows menuto provide rules manually.
        Provide option to save rules in json file.
        """
        self._menu.close()
        self._manual = pygame_menu.Menu("Provide parameters",
                                        self._size[0], self._size[1])
        self._manual.add.text_input("Rules name: ", default="export_rules")
        self._manual.add.range_slider('Range: ', 1, (1, 10), 1,
                                      value_format=lambda x: str(round(x)))
        self._manual.add.range_slider('States number: ', 2, (2, 255), 1,
                                      value_format=lambda x: str(round(x)))
        self._manual.add.selector('Middle included: ', [('Yes', 1), ('No', 0)])
        self._manual.add.selector('Neighborhood: ',
                                  [('Moore', 'm'), ('Neumann', 'n')])
        self._manual.add.text_input('Birth condition: ', default='2-3')
        self._manual.add.text_input('Survival condition: ', default='2')
        self._manual.add.button('Accept', self.start_with_manual_rules)
        self._manual.add.button('Export rules', self.export_rules)
        self._manual.add.button('Quit', pygame_menu.events.EXIT)
        try:
            self._manual.mainloop(self._screen)
        except ValueError:
            top = tkinter.Tk()
            top.withdraw()
            messagebox.showerror("Incorrect rules!", "Try again!")
            top.destroy()
            self.manual_menu()


if __name__ == "__main__":
    pygame.init()
    game = GUI()
    game.choosing_menu()
    pygame.quit()
