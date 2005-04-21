#! /usr/bin/python

commandRegistry = {}

def registerSlaveCommand(name, factory, version):
    assert not commandRegistry.has_key(name)
    commandRegistry[name] = (factory, version)
