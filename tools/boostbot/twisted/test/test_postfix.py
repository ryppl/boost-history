# Twisted, the Framework of Your Internet
# Copyright (C) 2001-2003 Matthew W. Lefkowitz
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of version 2.1 of the GNU Lesser General Public
# License as published by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

"""
Test cases for twisted.protocols.postfix module.
"""

from twisted.trial import unittest
from twisted import protocols
from twisted import internet
from twisted.protocols import loopback
from twisted.protocols import postfix
from twisted.internet import defer, protocol
from twisted.test.test_protocols import StringIOWithoutClosing
from twisted.python import components

class PostfixTCPMapQuoteTestCase(unittest.TestCase):
    data = [
        # (raw, quoted, [aliasQuotedForms]),
        ('foo', 'foo'),
        ('foo bar', 'foo%20bar'),
        ('foo\tbar', 'foo%09bar'),
        ('foo\nbar', 'foo%0Abar', 'foo%0abar'),
        ('foo\r\nbar', 'foo%0D%0Abar', 'foo%0D%0abar', 'foo%0d%0Abar', 'foo%0d%0abar'),
        ('foo ', 'foo%20'),
        (' foo', '%20foo'),
        ]

    def testData(self):
        for entry in self.data:
            raw = entry[0]
            quoted = entry[1:]

            self.assertEquals(postfix.quote(raw), quoted[0])
            for q in quoted:
                self.assertEquals(postfix.unquote(q), raw)

class PostfixTCPMapServerTestCase:
    data = {
        # 'key': 'value',
        }

    chat = [
        # (input, expected_output),
        ]

    def testChat(self):
        factory = postfix.PostfixTCPMapDictServerFactory(self.data)
        output = StringIOWithoutClosing()
        transport = internet.protocol.FileWrapper(output)

        protocol = postfix.PostfixTCPMapServer()
        protocol.service = factory
        protocol.factory = factory
        protocol.makeConnection(transport)

        for input, expected_output in self.chat:
            protocol.lineReceived(input)
            self.runReactor(1)
            self.assertEquals(output.getvalue(), expected_output,
                              'For %r, expected %r but got %r' % (
                input, expected_output, output.getvalue()
                ))
            output.truncate(0)
        protocol.setTimeout(None)

    def testDeferredChat(self):
        factory = postfix.PostfixTCPMapDeferringDictServerFactory(self.data)
        output = StringIOWithoutClosing()
        transport = internet.protocol.FileWrapper(output)

        protocol = postfix.PostfixTCPMapServer()
        protocol.service = factory
        protocol.factory = factory
        protocol.makeConnection(transport)

        for input, expected_output in self.chat:
            protocol.lineReceived(input)
            self.runReactor(1)
            self.assertEquals(output.getvalue(), expected_output,
                              'For %r, expected %r but got %r' % (
                input, expected_output, output.getvalue()
                ))
            output.truncate(0)
        protocol.setTimeout(None)

class Valid(PostfixTCPMapServerTestCase, unittest.TestCase):
    data = {
        'foo': 'ThisIs Foo',
        'bar': ' bar really is found\r\n',
        }
    chat = [
        ('get', "400 Command 'get' takes 1 parameters.\n"),
        ('get foo bar', "500 \n"),
        ('put', "400 Command 'put' takes 2 parameters.\n"),
        ('put foo', "400 Command 'put' takes 2 parameters.\n"),
        ('put foo bar baz', "500 put is not implemented yet.\n"),
        ('put foo bar', '500 put is not implemented yet.\n'),
        ('get foo', '200 ThisIs%20Foo\n'),
        ('get bar', '200 %20bar%20really%20is%20found%0D%0A\n'),
        ('get baz', '500 \n'),
        ('foo', '400 unknown command\n'),
        ]
