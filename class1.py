#!/usr/bin/python3
#-*- coding: utf-8 -*-

class Class:
    
    a = 10
    b = 20
    c = 30
    d = 0
    
    def __init__(self, a = 30, b = 40):
        self.a = a
        self.b = b

    
# model.set_params(iterations=500, thread_count=2)
    def set_params(self, a , b = 0, c=0,d=0):
        print("set_params(a={}, b={}, c={}, d={})".format(a,b,c,d))

        self.a = a
        self.b = b
        self.c = c
        self.d = d


    def get_value(self, x):
        return x
    
    def get_bool(self, x):
        if x :
            return True
        else :
            return False

    def add(self, x, y=1):
        print("x={}, y={}".format(x,y))
        return x + y
