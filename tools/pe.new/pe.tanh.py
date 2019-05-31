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

    def distanceSquared(a, b):
        return (a - b).lengthSquared

    def distance(a, b):
        return (a - b).length

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
class Point:

    __slots__ = ['invmass', 'mass', 'pos', 'vel', 'acc']

    def __init__(p, **kwargs):
        p.mass = kwargs.get('mass', 1)
        p.invmass = 1 / p.mass
        p.pos = Vec(kwargs['pos'])
        p.vel = Vec(kwargs.get('vel', (0, 0)))
        p.acc = Vec()

    @property
    def momenta(p):
        return p.mass * p.vel

    def detect(p, pos):
        return None, None

    def degetnorm(p, s):
        n, sdf = s.detect(p.pos)
        return n if sdf <= 0 else None

    def render(p, g):
        pygame.draw.circle(g.screen, LIGHT, p.pos.int, 2, 2)

    def move(p, dt):
        p.pos += p.vel * dt
        p.vel += p.acc * dt

##############################################
class Circle(Point):

    __slots__ = ['radius']

    def __init__(p, **kwargs):
        super().__init__(**kwargs)
        p.radius = kwargs['radius']

    def detect(p, pos):
        disp = pos - p.pos
        distance = disp.length
        return disp / distance, distance - p.radius

    def degetnorm(p, s):
        n, sdf = s.detect(p.pos)
        sdf -= p.radius
        return n if sdf <= 0 else None

    def render(p, g):
        pygame.draw.circle(g.screen, LIGHT, p.pos.int, int(p.radius), 2)

##############################################
class Contraint:

    __slots__ = ['ps']

    def __init__(c, a, b, distance=None):
        c.a, c.b = a, b
        c.distance = distance if distance is not None else a.pos.distance(b.pos)

    def apply(c):
        a, b = c.ps
        tm = a.momenta + b.momenta
        a.vel -= tm * a.invmass
        b.vel -= tm * b.invmass
        n = (a.pos - b.pos).norm
        a.vel -= n * n.dot(a.vel)
        b.vel -= n * n.dot(b.vel)
        a.vel += tm * a.invmass
        b.vel += tm * b.invmass

##############################################
class World:

    __slots__ = ['bodies', 'static_bodies', 'contraints', 'gravity']

    def __init__(w):
        w.bodies = []
        w.static_bodies = []
        w.contraints = []
        w.gravity = Vec(0, 200)

    def move(w, dt):
        w.do_gravity()
        w.do_collide()
        w.do_contraint()

        for o in w.bodies:
            o.move(dt)

    def render(w, g):
        for o in w:
            o.render(g)

    def do_gravity(w):
        for o in w.bodies:
            o.acc = w.gravity

    def do_contraint(w):
        for c in w.contraints:
            c.apply()

    def do_collide(w):
        for o in w.bodies:
            for s in w.static_bodies:
                w.static_collide(s, o)

    def static_collide(w, s, o):
        n = o.degetnorm(s)
        if n is not None:
            nv = n.dot(o.vel)
            #if nv >= 0 and nv < 5:
            #    na = n.dot(o.acc)
            #    o.vel -= nv * n
            #    o.acc -= na * n
            if nv <= 0:
                if nv < -8:
                    o.vel -= nv * n * (1 + 0.6)
                else:
                    o.vel -= nv * n
                    o.acc -= n * n.dot(o.acc)
            #elif nv > 0 and nv < 5:
                #o.vel -= nv * n
            #elif nv == 0:
            #    na = n.dot(o.acc)
            #    o.acc -= na * n
            #    o.vel -= nv * n

    def add(w, o):
        w.bodies.append(o)

    def add_static(w, o):
        w.static_bodies.append(o)

    def contraint(w, *args, **kwargs):
        w.contraints.append(Contraint(*args, **kwargs))

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

    def mainloop(g):
        while True:
            for event in pygame.event.get():
                if event.type in (QUIT, KEYDOWN):
                    pygame.quit()
                    return

            g.screen.fill(BLACK)
            g.world.render(g)
            pygame.display.update()
            dt = g.clock.tick(60) / 1000
            g.world.move(dt / 2)
            g.world.move(dt / 2)

##############################################
game = Game()
#for k in range(-30, 30):
#    game.world.add(Point(pos=(game.hwidth * (1 + 0.001 * k), game.hheight * 0.1)))
last_pt = None
for k in range(-4, 5):
    pt = Point(pos=(game.hwidth * (1 + 0.1 * k), game.hheight * 0.06))
    if last_pt is not None:
        game.world.contraint(last_pt, pt)
    game.world.add(pt)
    last_pt = pt
game.world.add(Circle(pos=(game.hwidth * 0.99, game.hheight * 0.1), radius=game.hheight * 0.04))
game.world.add_static(Circle(pos=(game.hwidth, game.hheight), radius=game.hheight * 0.3))
game.mainloop()
