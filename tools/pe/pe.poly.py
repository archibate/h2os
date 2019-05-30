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

    def reflect(a, n, k=2):
        return a - k * a.dot(n) * n

    def reflectize(a, n, k=2):
        a -= k * a.dot(n) * n
        return a

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
class Polygon:

    __slots__ = ['points', 'displaces']

    def __init__(p, **kwargs):
        p.points = [Vec(coor) for coor in kwargs['points']]
        pos = kwargs.get('pos')
        if pos is not None:
            pos = Vec(pos)
            p.points = [coor + pos for coor in p.points]
        p.displaces = [p.points[(i+1)%len(p.points)] - p.points[i] for i in range(len(p.points))]

    def move(p, dt):
        pass

    def get_normal(p, pos):
        dis = pos - p.points[0]
        if len(p.points) < 3:
            return None
        elif dis.cross(p.displaces[0]) > 0:
            return None
        print('!!')
        elif dis.cross(p.displaces[-1]) > 0:
            return None

        i, j = 2, len(p.points) - 1
        line = None
        while i <= j:
            mid = (i + j) / 2
            if dis.cross(p.points[mid] - p.points[0]) > 0:
                line = mid
                j = mid - 1
            else:
                i = mid + 1
        n = p.displaces[line-1].rotccw
        return n if n.dot(pos - p.points[line-1]) > 0 else None

    def render(p, g):
        points = [pos.int for pos in p.points]
        pygame.draw.polygon(g.screen, LIGHT, points, 2)

##############################################
class Point:

    __slots__ = ['pos', 'vel', 'acc']

    def __init__(p, **kwargs):
        p.pos = Vec(kwargs['pos'])
        p.vel = Vec(kwargs.get('vel', (0, 0)))
        p.acc = Vec()

    def get_normal(p, pos):
        return None

    def render(p, g):
        pygame.draw.circle(g.screen, LIGHT, p.pos.int, 2, 2)

    def move(p, dt):
        p.pos += p.vel * dt
        p.vel += p.acc * dt

##############################################
class World:

    __slots__ = ['bodies', 'static_bodies', 'gravity']

    def __init__(w):
        w.bodies = []
        w.static_bodies = []
        w.gravity = Vec(0, 100)

    def move(w, dt):
        w.do_gravity()
        w.do_collide()

        for o in w.bodies:
            o.move(dt)

    def render(w, g):
        for o in w:
            o.render(g)

    def do_gravity(w):
        for o in w.bodies:
            o.acc = w.gravity

    def do_collide(w):
        for o in w.bodies:
            for s in w.static_bodies:
                w.static_collide(s, o)

    def static_collide(w, s, o):
        n = s.get_normal(o.pos)
        if n is not None:
            print(n)

    def add(w, o, is_static=False):
        if not is_static:
            w.bodies.append(o)
        else:
            w.static_bodies.append(o)

    def __iter__(w):
        return iter(w.bodies + w.static_bodies)

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
game.add(Point(pos=(game.hwidth, game.hheight)))
game.add(Polygon(pos=(game.hwidth - 50, game.hheight * 1.5), points=[(0, 0), (0, 100), (100, 100)]), is_static=True)
game.mainloop()
