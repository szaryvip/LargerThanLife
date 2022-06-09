from typing import Any, Dict, List
import pygame
import myrandoms


class Game:
    _screen = None
    _size = None
    _width = None
    _height = None
    _tile_size = 10
    _dead_color = (115, 140, 165)
    _alive_color = (165, 230, 130)
    _states_number = None  # nunmber of states that can cell be

    def __init__(self, screen: pygame.Surface, params: Dict) -> Any:
        """Creates object of class Game that is responsible
        for displaying actual state of game

        Args:
            screen (pygame.Surface): surface to display
            params (Dict): parameters of the game

        """
        self._screen = screen
        self._size = screen.get_size()
        self._width = self._size[0]//self._tile_size
        self._height = self._size[1]//self._tile_size
        for y in range(self._height):
            for x in range(self._width):
                pygame.draw.rect(self._screen, self._dead_color,
                                 pygame.Rect(x*(self._tile_size)+1,
                                             y*(self._tile_size)+1,
                                             self._tile_size-2,
                                             self._tile_size-2))
        if params == {}:
            states = myrandoms.random_states_number()
        else:
            states = params["Cc"]
        self._states_number = states

    def update(self, new_values: List[List[int]]) -> None:
        """Updates rects on pygame.Screen to new states

        Args:
            new_values (List[List[int]]): new states of cells
        """
        for y in range(self._height):
            for x in range(self._width):
                color = self.pick_color(new_values[x][y])
                pygame.draw.rect(self._screen, color,
                                 pygame.Rect(x*(self._tile_size)+1,
                                             y*(self._tile_size)+1,
                                             self._tile_size-2,
                                             self._tile_size-2))

    def pick_color(self, value: int) -> tuple[int]:
        """Generates new color depends on cell value

        Args:
            value (int): age of cell, where 0 is dead

        Returns:
            tuple[int]: calculated color in RGB
        """
        if value == 0:
            return self._dead_color
        elif value == 1:
            return self._alive_color
        else:
            new_rgb = 255 - (value - 1) * (255 // self._states_number)
            return new_rgb, new_rgb, new_rgb
