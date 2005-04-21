# -*- Python -*-
# Twisted, the Framework of Your Internet
# Copyright (C) 2001 Matthew W. Lefkowitz
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

__version__ = '$Revision$'[11:-2]

from StringIO import StringIO
from twisted.trial import unittest
from twisted.protocols import pcp

# Goal:

# Take a Protocol instance.  Own all outgoing data - anything that
# would go to p.transport.write.  Own all incoming data - anything
# that comes to p.dataReceived.

# I need:
# Something with the AbstractFileDescriptor interface.
# That is:
#  - acts as a Transport
#    - has a method write()
#    - which buffers
#  - acts as a Consumer
#    - has a registerProducer, unRegisterProducer
#    - tells the Producer to back off (pauseProducing) when its buffer is full.
#    - tells the Producer to resumeProducing when its buffer is not so full.
#  - acts as a Producer
#    - calls registerProducer
#    - calls write() on consumers
#    - honors requests to pause/resume producing
#    - honors stopProducing, and passes it along to upstream Producers


class DummyTransport:
    """A dumb transport to wrap around."""

    def __init__(self):
        self._writes = []

    def write(self, data):
        self._writes.append(data)

    def getvalue(self):
        return ''.join(self._writes)

class DummyProducer:
    resumed = False
    stopped = False
    paused = False

    def __init__(self, consumer):
        self.consumer = consumer

    def resumeProducing(self):
        self.resumed = True
        self.paused = False

    def pauseProducing(self):
        self.paused = True

    def stopProducing(self):
        self.stopped = True


class DummyConsumer(DummyTransport):
    producer = None
    finished = False
    unregistered = True

    def registerProducer(self, producer, streaming):
        self.producer = (producer, streaming)

    def unregisterProducer(self):
        self.unregistered = True

    def finish(self):
        self.finished = True

class TransportInterfaceTest(unittest.TestCase):
    proxyClass = pcp.BasicProducerConsumerProxy

    def setUp(self):
        self.underlying = DummyConsumer()
        self.transport = self.proxyClass(self.underlying)

    def testWrite(self):
        self.transport.write("some bytes")

class ConsumerInterfaceTest:
    """Test ProducerConsumerProxy as a Consumer.

    Normally we have ProducingServer -> ConsumingTransport.

    If I am to go between (Server -> Shaper -> Transport), I have to
    play the role of Consumer convincingly for the ProducingServer.
    """

    def setUp(self):
        self.underlying = DummyConsumer()
        self.consumer = self.proxyClass(self.underlying)
        self.producer = DummyProducer(self.consumer)

    def testRegisterPush(self):
        self.consumer.registerProducer(self.producer, True)
        ## Consumer should NOT have called PushProducer.resumeProducing
        self.failIf(self.producer.resumed)

    ## I'm I'm just a proxy, should I only do resumeProducing when
    ## I get poked myself?
    #def testRegisterPull(self):
    #    self.consumer.registerProducer(self.producer, False)
    #    ## Consumer SHOULD have called PushProducer.resumeProducing
    #    self.failUnless(self.producer.resumed)

    def testUnregister(self):
        self.consumer.registerProducer(self.producer, False)
        self.consumer.unregisterProducer()
        # Now when the consumer would ordinarily want more data, it
        # shouldn't ask producer for it.
        # The most succinct way to trigger "want more data" is to proxy for
        # a PullProducer and have someone ask me for data.
        self.producer.resumed = False
        self.consumer.resumeProducing()
        self.failIf(self.producer.resumed)

    def testFinish(self):
        self.consumer.registerProducer(self.producer, False)
        self.consumer.finish()
        # I guess finish should behave like unregister?
        self.producer.resumed = False
        self.consumer.resumeProducing()
        self.failIf(self.producer.resumed)


class ProducerInterfaceTest:
    """Test ProducerConsumerProxy as a Producer.

    Normally we have ProducingServer -> ConsumingTransport.

    If I am to go between (Server -> Shaper -> Transport), I have to
    play the role of Producer convincingly for the ConsumingTransport.
    """

    def setUp(self):
        self.consumer = DummyConsumer()
        self.producer = self.proxyClass(self.consumer)

    def testRegistersProducer(self):
        self.failUnlessEqual(self.consumer.producer[0], self.producer)

    def testPause(self):
        self.producer.pauseProducing()
        self.producer.write("yakkity yak")
        self.failIf(self.consumer.getvalue(),
                    "Paused producer should not have sent data.")

    def testResume(self):
        self.producer.pauseProducing()
        self.producer.resumeProducing()
        self.producer.write("yakkity yak")
        self.failUnlessEqual(self.consumer.getvalue(), "yakkity yak")

    def testResumeNoEmptyWrite(self):
        self.producer.pauseProducing()
        self.producer.resumeProducing()
        self.failUnlessEqual(len(self.consumer._writes), 0,
                             "Resume triggered an empty write.")

    def testResumeBuffer(self):
        self.producer.pauseProducing()
        self.producer.write("buffer this")
        self.producer.resumeProducing()
        self.failUnlessEqual(self.consumer.getvalue(), "buffer this")

    def testStop(self):
        self.producer.stopProducing()
        self.producer.write("yakkity yak")
        self.failIf(self.consumer.getvalue(),
                    "Stopped producer should not have sent data.")


class PCP_ConsumerInterfaceTest(ConsumerInterfaceTest, unittest.TestCase):
    proxyClass = pcp.BasicProducerConsumerProxy

class PCPII_ConsumerInterfaceTest(ConsumerInterfaceTest, unittest.TestCase):
    proxyClass = pcp.ProducerConsumerProxy

class PCP_ProducerInterfaceTest(ProducerInterfaceTest, unittest.TestCase):
    proxyClass = pcp.BasicProducerConsumerProxy

class PCPII_ProducerInterfaceTest(ProducerInterfaceTest, unittest.TestCase):
    proxyClass = pcp.ProducerConsumerProxy

class ProducerProxyTest(unittest.TestCase):
    """Producer methods on me should be relayed to the Producer I proxy.
    """
    proxyClass = pcp.BasicProducerConsumerProxy

    def setUp(self):
        self.proxy = self.proxyClass(None)
        self.parentProducer = DummyProducer(self.proxy)
        self.proxy.registerProducer(self.parentProducer, True)

    def testStop(self):
        self.proxy.stopProducing()
        self.failUnless(self.parentProducer.stopped)


class ConsumerProxyTest(unittest.TestCase):
    """Consumer methods on me should be relayed to the Consumer I proxy.
    """
    proxyClass = pcp.BasicProducerConsumerProxy

    def setUp(self):
        self.underlying = DummyConsumer()
        self.consumer = self.proxyClass(self.underlying)

    def testWrite(self):
        # NOTE: This test only valid for streaming (Push) systems.
        self.consumer.write("some bytes")
        self.failUnlessEqual(self.underlying.getvalue(), "some bytes")

    def testFinish(self):
        self.consumer.finish()
        self.failUnless(self.underlying.finished)

    def testUnregister(self):
        self.consumer.unregisterProducer()
        self.failUnless(self.underlying.unregistered)


class PullProducerTest:
    def setUp(self):
        self.underlying = DummyConsumer()
        self.proxy = self.proxyClass(self.underlying)
        self.parentProducer = DummyProducer(self.proxy)
        self.proxy.registerProducer(self.parentProducer, True)

    def testHoldWrites(self):
        self.proxy.write("hello")
        # Consumer should get no data before it says resumeProducing.
        self.failIf(self.underlying.getvalue(),
                    "Pulling Consumer got data before it pulled.")

    def testPull(self):
        self.proxy.write("hello")
        self.proxy.resumeProducing()
        self.failUnlessEqual(self.underlying.getvalue(), "hello")

    def testMergeWrites(self):
        self.proxy.write("hello ")
        self.proxy.write("sunshine")
        self.proxy.resumeProducing()
        nwrites = len(self.underlying._writes)
        self.failUnlessEqual(nwrites, 1, "Pull resulted in %d writes instead "
                             "of 1." % (nwrites,))
        self.failUnlessEqual(self.underlying.getvalue(), "hello sunshine")


    def testLateWrite(self):
        # consumer sends its initial pull before we have data
        self.proxy.resumeProducing()
        self.proxy.write("data")
        # This data should answer that pull request.
        self.failUnlessEqual(self.underlying.getvalue(), "data")

class PCP_PullProducerTest(PullProducerTest, unittest.TestCase):
    class proxyClass(pcp.BasicProducerConsumerProxy):
        iAmStreaming = False

class PCPII_PullProducerTest(PullProducerTest, unittest.TestCase):
    class proxyClass(pcp.ProducerConsumerProxy):
        iAmStreaming = False

# Buffering!

class BufferedConsumerTest(unittest.TestCase):
    """As a consumer, ask the producer to pause after too much data."""

    proxyClass = pcp.ProducerConsumerProxy

    def setUp(self):
        self.underlying = DummyConsumer()
        self.proxy = self.proxyClass(self.underlying)
        self.proxy.bufferSize = 100

        self.parentProducer = DummyProducer(self.proxy)
        self.proxy.registerProducer(self.parentProducer, True)

    def testRegisterPull(self):
        self.proxy.registerProducer(self.parentProducer, False)
        ## Consumer SHOULD have called PushProducer.resumeProducing
        self.failUnless(self.parentProducer.resumed)

    def testPauseIntercept(self):
        self.proxy.pauseProducing()
        self.failIf(self.parentProducer.paused)

    def testResumeIntercept(self):
        self.proxy.pauseProducing()
        self.proxy.resumeProducing()
        # With a streaming producer, just because the proxy was resumed is
        # not necessarily a reason to resume the parent producer.  The state
        # of the buffer should decide that.
        self.failIf(self.parentProducer.resumed)

    def testTriggerPause(self):
        """Make sure I say \"when.\""""

        # Pause the proxy so data sent to it builds up in its buffer.
        self.proxy.pauseProducing()
        self.failIf(self.parentProducer.paused, "don't pause yet")
        self.proxy.write("x" * 51)
        self.failIf(self.parentProducer.paused, "don't pause yet")
        self.proxy.write("x" * 51)
        self.failUnless(self.parentProducer.paused)

    def testTriggerResume(self):
        """Make sure I resumeProducing when my buffer empties."""
        self.proxy.pauseProducing()
        self.proxy.write("x" * 102)
        self.failUnless(self.parentProducer.paused, "should be paused")
        self.proxy.resumeProducing()
        # Resuming should have emptied my buffer, so I should tell my
        # parent to resume too.
        self.failIf(self.parentProducer.paused,
                    "Producer should have resumed.")
        self.failIf(self.proxy.producerPaused)

class BufferedPullTests(unittest.TestCase):
    class proxyClass(pcp.ProducerConsumerProxy):
        iAmStreaming = False

        def _writeSomeData(self, data):
            pcp.ProducerConsumerProxy._writeSomeData(self, data[:100])
            return min(len(data), 100)

    def setUp(self):
        self.underlying = DummyConsumer()
        self.proxy = self.proxyClass(self.underlying)
        self.proxy.bufferSize = 100

        self.parentProducer = DummyProducer(self.proxy)
        self.proxy.registerProducer(self.parentProducer, False)

    def testResumePull(self):
        # If proxy has no data to send on resumeProducing, it had better pull
        # some from its PullProducer.
        self.parentProducer.resumed = False
        self.proxy.resumeProducing()
        self.failUnless(self.parentProducer.resumed)

    def testLateWriteBuffering(self):
        # consumer sends its initial pull before we have data
        self.proxy.resumeProducing()
        self.proxy.write("datum" * 21)
        # This data should answer that pull request.
        self.failUnlessEqual(self.underlying.getvalue(), "datum" * 20)
        # but there should be some left over
        self.failUnlessEqual(self.proxy._buffer, ["datum"])


# TODO:
#  test that web request finishing bug (when we weren't proxying
#    unregisterProducer but were proxying finish, web file transfers
#    would hang on the last block.)
#  test what happens if writeSomeBytes decided to write zero bytes.
