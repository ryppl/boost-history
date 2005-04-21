# -*- makefile -*-

# This is a simple makefile which lives in a buildmaster/buildslave
# directory (next to the buildbot.tap file). It allows you to start/stop the
# master ot slave by doing 'make start' or 'make stop'.

# The 'reconfig' target will tell a buildmaster to reload its config file.

start:
	twistd --no_save -f buildbot.tap

stop:
	kill `cat twistd.pid`

reconfig:
	kill -HUP `cat twistd.pid`
