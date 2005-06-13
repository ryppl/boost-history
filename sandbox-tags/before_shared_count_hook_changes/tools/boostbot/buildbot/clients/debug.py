#! /usr/bin/python

from twisted.internet import gtk2reactor
gtk2reactor.install()
from twisted.internet import reactor
from twisted.python import util
from twisted.spread import pb
from twisted.cred import credentials
import gtk, gtk.glade, gnome.ui
import os, sys, re

class DebugWidget:
    def __init__(self, master="localhost:8007", passwd="debugpw"):
        self.connected = 0
        try:
            host, port = re.search(r'(.+):(\d+)', master).groups()
        except:
            print "unparseable master location '%s'" % master
            print " expecting something more like localhost:8007"
            raise
        self.host = host
        self.port = int(port)
        self.passwd = passwd
        self.remote = None
        xml = self.xml = gtk.glade.XML(util.sibpath(__file__, "debug.glade"))
        g = xml.get_widget
        self.buildname = g('buildname')
        self.filename = g('filename')
        self.connectbutton = g('connectbutton')
        self.connectlabel = g('connectlabel')
        g('window1').connect('destroy', lambda win: gtk.mainquit())
        c = xml.signal_connect
        c('do_connect', self.do_connect)
        c('do_reload', self.do_reload)
        c('do_rebuild', self.do_rebuild)
        c('do_poke_irc', self.do_poke_irc)
        c('do_build', self.do_build)
        c('do_commit', self.do_commit)
        c('on_userevision_toggled', self.userevision_toggled)
        self.userevision_toggled(g('userevision'))
        c('do_current_offline', self.do_current, "offline")
        c('do_current_idle', self.do_current, "idle")
        c('do_current_waiting', self.do_current, "waiting")
        c('do_current_building', self.do_current, "building")

    def do_connect(self, widget):
        if self.connected:
            self.connectlabel.set_text("Disconnecting...")
            if self.remote:
                self.remote.broker.transport.loseConnection()
        else:
            self.connectlabel.set_text("Connecting...")
            f = pb.PBClientFactory()
            creds = credentials.UsernamePassword("debug", self.passwd)
            d = f.login(creds)
            reactor.connectTCP(self.host, int(self.port), f)
            d.addCallbacks(self.connect_complete, self.connect_failed)
    def connect_complete(self, ref):
        self.connectbutton.set_label("Disconnect")
        self.connectlabel.set_text("Connected")
        self.connected = 1
        self.remote = ref
        self.remote.callRemote("print", "hello cleveland")
        self.remote.notifyOnDisconnect(self.disconnected)
    def connect_failed(self, why):
        self.connectlabel.set_text("Failed")
        print why
    def disconnected(self, ref):
        self.connectbutton.set_label("Connect")
        self.connectlabel.set_text("Disconnected")
        self.connected = 0
        self.remote = None

    def do_reload(self, widget):
        if not self.remote:
            return
        d = self.remote.callRemote("reload")
        d.addErrback(self.err)
    def do_rebuild(self, widget):
        print "Not yet implemented"
        return
    def do_poke_irc(self, widget):
        if not self.remote:
            return
        d = self.remote.callRemote("pokeIRC")
        d.addErrback(self.err)

    def do_build(self, widget):
        if not self.remote:
            return
        name = self.buildname.get_text()
        d = self.remote.callRemote("forceBuild", name)
        d.addErrback(self.err)

    def userevision_toggled(self, widget):
        rev = self.xml.get_widget('revision')
        if widget.get_active():
            rev.set_sensitive(True)
        else:
            rev.set_sensitive(False)

    def do_commit(self, widget):
        if not self.remote:
            return
        filename = self.filename.get_text()
        who = self.xml.get_widget("who").get_text()
        revision = None
        if self.xml.get_widget("userevision").get_active():
            revision = self.xml.get_widget('revision').get_text()
            try:
                revision = int(revision)
            except ValueError:
                pass
        d = self.remote.callRemote("fakeChange", filename,
                                   revision=revision, who=who)
        d.addErrback(self.err)

    def do_current(self, widget, state):
        if not self.remote:
            return
        name = self.buildname.get_text()
        d = self.remote.callRemote("setCurrentState", name, state)
        d.addErrback(self.err)
    def err(self, failure):
        print "received error"
        failure.printTraceback()
        

    def run(self):
        reactor.run()

if __name__ == '__main__':
    master = "localhost:8007"
    if len(sys.argv) > 1:
        master = sys.argv[1]
    passwd = "debugpw"
    if len(sys.argv) > 2:
        passwd = sys.argv[2]
    d = DebugWidget(master, passwd)
    d.run()
