# vi: syntax=python:et:ts=4
from SCons.Script import AddOption

def exists(env):
    return True

def generate(env):
    AddOption('--test', dest='test', action="store_true")
