import pygame, sys
from game import Game

pygame.init()
clock = pygame.time.Clock()

X, Y = 1000, 800
FPS = 30

screen = pygame.display.set_mode([X,Y])

radius = 40 

obj = Game(radius)
obj.init_points(6, 280, 200)

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    screen.fill((112,112,112))

    obj.run_game(screen)

    pygame.display.flip()
    clock.tick(FPS)
    pygame.display.update()

pygame.quit()
