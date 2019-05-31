#!/usr/bin/env python

from math import *

##############################################
class Vec:

    __slots__ = ['x', 'y']

    def __init__(a, x_or_pair=(0, 0), y=None):
        if y is None:
            a.x, a.y = x_or_pair
        else:
            a.x, a.y = x_or_pair, y

    def __len__(a):
        return 2

    @property
    def int(a):
        return Vec(int(a.x), int(a.y))

    def __str__(a):
        return '(%s, %s)' % (a.x, a.y)

    def __repr__(a):
        return 'Vec(%s, %s)' % (a.x, a.y)

    def __getitem__(a, i):
        return [a.x, a.y][i]

    def __setitem__(a, i, v):
        if i == 0:
            a.x = v
        elif i == 1:
            a.y = v
        else:
            raise IndexError

    @property
    def lengthSquared(a):
        return a.x * a.x + a.y * a.y

    @property
    def length(a):
        return sqrt(a.x * a.x + a.y * a.y)

    __abs__ = length

    @property
    def angle(a):
        return atan2(a.y, a.x)

    @property
    def norm(a):
        return a / a.length

    def normalize(a):
        a /= a.length
        return a

    def dot(a, b):
        return a.x * b.x + a.y * b.y

    def cross(a, b):
        return a.x * b.y - a.y * b.x

    @property
    def vec(a):
        return Vec(a)

    @property
    def rotccw(a):
        return Vec(-a.y, a.x)

    @property
    def rotcw(a):
        return Vec(a.y, -a.x)

    __pos__ = vec

    def __neg__(a):
        return Vec(-a.x, -a.y)

    def __add__(a, b):
        return Vec(a.x + b.x, a.y + b.y)

    def __sub__(a, b):
        return Vec(a.x - b.x, a.y - b.y)

    __radd__ = __add__

    def __rsub__(a, b):
        return Vec(b.x - a.x, b.y - a.y)

    def __mul__(a, k):
        return Vec(a.x * k, a.y * k)

    def __truediv__(a, k):
        return a * (1 / k)

    def __rmul__(a, k):
        return a * k

    def __iadd__(a, b):
        a.x += b.x
        a.y += b.y
        return a

    def __isub__(a, b):
        a.x -= b.x
        a.y -= b.y
        return a

    def __imul__(a, k):
        a.x *= k
        a.y *= k
        return a

    def __itruediv__(a, k):
        a *= 1 / k
        return a

##############################################
class Cs:
    pass

##############################################
class Plane(Cs):

    __slots__ = ['center', 'normal']

    def __init__(p, **kwargs):
        p.center = Vec(kwargs['center'])
        p.normal = Vec(kwargs.get('normal', (0, 1))).norm

    def render(p, g):
        direction = max(g.width, g.height) * p.normal.rotcw
        pygame.draw.line(g.screen, LIGHT,
                p.center + direction, p.center - direction, 2)

##############################################
class Pt:

    __slots__ = ['pos', 'vel', 'acc']

    def __init__(p, **kwargs):
        p.pos = Vec(kwargs['pos'])
        p.vel = Vec(kwargs.get('vel', (0, 0)))
        p.acc = Vec(kwargs.get('acc', (0, 100)))

    def render(p, g):
        pygame.draw.circle(g.screen, LIGHT, p.pos.int, 1, 2)

    def move(p, dt):
        p.pos += p.vel * dt
        p.vel += p.acc * dt

##############################################
class Ball(Pt):

    __slots__ = ['radius']

    def __init__(p, **kwargs):
        super().__init__(**kwargs)
        p.radius = kwargs['radius']

    def render(p, g):
        pygame.draw.circle(g.screen, LIGHT, p.pos.int, int(p.radius), 2)

##############################################
class World:

    __slots__ = ['objects', 'statics', 'collide_layers']

    def __init__(w):
        w.objects = []
        w.statics = []
        w.collide_layers = {}

    def move(w, dt):
        for o in w.objects:
            o.move(dt)

    def render(w, g):
        for o in w:
            o.render(g)

    def do_collides(w):
        for layer in w.collide_layers:
            for i in range(len(layer)):
                for j in range(i, len(layer)):
                    o, p = layer[i], layer[j]

    def add(w, o, cas='default'):
        if isinstance(o, Pt):
            w.objects.append(o)
        elif isinstance(o, Cs):
            w.statics.append(o)
        else:
            raise TypeError

        try:
            cas = iter(cas)
        except:
            cas = iter([cas])

        for ca in cas:
            if ca in w.collide_layers:
                w.collide_layers[ca].append(o)
            else:
                w.collide_layers[ca] = [o]

    def __iter__(w):
        return iter(w.objects + w.statics)

##############################################
import pygame
from pygame.locals import *

BLACK = 0, 0, 0
RED   = 255, 0, 0
GREEN = 0, 255, 0
BLUE  = 0, 0, 255
LIGHT = 200, 200, 200
WHITE = 255, 255, 255

class Game:

    __slots__ = ['screen', 'world', 'clock']

    def __init__(g):
        pygame.init()
        g.screen = pygame.display.set_mode((600, 500))
        pygame.display.set_caption('2D Physics Engine')
        g.world = World()
        g.clock = pygame.time.Clock()

    @property
    def width(g):
        return g.screen.get_width()

    @property
    def height(g):
        return g.screen.get_height()

    @property
    def hwidth(g):
        return g.width / 2

    @property
    def hheight(g):
        return g.height / 2

    def add(g, *args, **kwargs):
        g.world.add(*args, **kwargs)

    def mainloop(g):
        while True:
            for event in pygame.event.get():
                if event.type in (QUIT, KEYDOWN):
                    pygame.quit()
                    return

            g.screen.fill(BLACK)
            g.world.render(g)
            pygame.display.update()
            dt = g.clock.tick(30) / 1000
            g.world.move(dt)

##############################################
game = Game()
game.add(Ball(pos=(game.hwidth, game.hheight), radius=50))
game.add(Plane(center=(0, game.height * 0.8)))
game.mainloop()
