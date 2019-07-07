Distributed Algorithms
======================

This repository contains my solutions for challenges on distributed algorithms.

Build and run programs using:

```
$ sbt run
```

Broadcast protocols
-------------------

Implementation of a [best-effort broadcast] protocol:

```
$ sbt "runMain com.github.dlorch.BestEffortBroadcast.Main"
[p3] message delivered: Hello, distributed world!
[p1] message delivered: Hello, distributed world!
[p2] message delivered: Hello, distributed world!
```

[![Best-effort broadcast algorithm](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-10.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf)
[![Best-effort broadcast example](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-12.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf)

Implementation of [reliable broadcast] protocol. Here, p1 sends message to p2, then
crash-stops. Since p2 sends to p3, all correct processes eventually deliver the message:

```
$ sbt "runMain com.github.dlorch.ReliableBroadcast.Main"
[p1] crashed
[p2] message delivered: Hello, reliable world!
[p3] message delivered: Hello, reliable world!
```

[![Reliable broadcast algorithm](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-22.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf)
[![Reliable broadcast example](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-23.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf)

Implementation of [FIFO broadcast] protocol, which is a "FIFO Order transformation applied to a reliable
broadcast". Note that p1 sends the first, then third, finally the second message, but due to sequencing,
the messages are delivered in correct FIFO order. In reality, p1 would be sending those messages in the
*correct order*. The messages might arrive in the wrong order at the destination due to forwarding or
networking delays.

```
$ sbt "runMain com.github.dlorch.FIFOBroadcast.Main"
[p1] [seqn: 1] message delivered: 1st message
[p2] [seqn: 1] message delivered: 1st message
[p3] [seqn: 1] message delivered: 1st message
[p1] [seqn: 2] message delivered: 2nd message
[p3] [seqn: 2] message delivered: 2nd message
[p2] [seqn: 2] message delivered: 2nd message
[p1] [seqn: 3] message delivered: 3rd message
[p3] [seqn: 3] message delivered: 3rd message
[p2] [seqn: 3] message delivered: 3rd message
```

The key component of the FIFO Order transformation is the `next` table, kept *at each node*. This
variable keeps track of the expected next sequence number from its peer processes. At the *beginning*
of the execution, each node's next array looks like this:

| Peer Process | Next Sequence Number
| :----------: | :------------------:
| p1           | 1 
| p2           | 1
| p3           | 1

At the *end* of the execution, the `next` table at each process looks like this:

| Peer Process | Next Sequence Number
| :----------: | :------------------:
| p1           | 4 
| p2           | 1
| p3           | 1

In the example above, a message with sequence number 2 arrives out-of-order. Each processe
`receive()`s and stores it in the `buffer`, until the next message with sequence number 1
arrives, then it `deliver()`s messages 1 and 2 in the correct sequence to the process.

[![FIFO Order](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-31.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf)
[![A modular approach to broadcast](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-39.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf)
[![FIFO Order algorithm](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-44.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf)

[best-effort broadcast]: http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf
[reliable broadcast]: http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf
[FIFO broadcast]: http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf
