import pygame

class Game:
    def __init__(self, radius):
        self.dots = []
        self.erased_dots = []

        self.human = []
        self.gpt = []

        self.radius = radius

    def init_points(self, rows, xo, yo):
        cols = rows
        for j in range(rows):
            for i in range(cols):
                x = xo + (2 * self.radius + 20) * i;
                y = yo + (2 * self.radius + 10) * j;

                v = pygame.math.Vector2(x, y)
                self.dots.append(v)

                self.erased_dots.append(False)
            cols -= 1

    def draw_points(self, screen):
        for dot in self.dots:
            pygame.draw.circle(screen, (255,255,0,30), dot, self.radius)

    def run_game(self, screen):
        self.draw_points(screen)
