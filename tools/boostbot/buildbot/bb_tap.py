#! /usr/bin/python

# a TAP construction module

import sys
from twisted.python import usage
from buildbot import master
from buildbot.slave import bot

class Options(usage.Options):
    synopsis = "Usage: mktap buildbot master|slave [options]"
    subCommands = [['master', None, master.Options,
                    "Create a buildmaster .tap file"],
                   ['slave', None, bot.Options,
                    "Create a buildslave (a 'bot')"]]
    
def NOTupdateApplication(app, config):
    if not hasattr(config, 'subCommand') or config.subCommand == None:
        print config # display usage
        sys.exit(2)
    if config.subCommand == 'slave':
        bot.updateApplication(app, config.subOptions)
    if config.subCommand == 'master':
        master.updateApplication(app, config.subOptions)

def makeService(config):
    if not hasattr(config, 'subCommand') or config.subCommand == None:
        print config # display usage
        sys.exit(2)
    if config.subCommand == 'slave':
        return bot.makeService(config.subOptions)
    if config.subCommand == 'master':
        return master.makeService(config.subOptions)
