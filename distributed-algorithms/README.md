Distributed Algorithms
======================

This repository contains my solutions for challenges on [distributed algorithms].

Build and run programs using:

```
$ sbt run
```

Broadcast Protocols
-------------------

### Best-Effort Broadcast

Implementation of a [best-effort broadcast] protocol:

```
$ sbt "runMain com.github.dlorch.BestEffortBroadcast.Main"
[p3] message delivered: Hello, distributed world!
[p1] message delivered: Hello, distributed world!
[p2] message delivered: Hello, distributed world!
```

[![Best-effort broadcast algorithm](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-10.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf#page=11)
[![Best-effort broadcast example](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-12.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf#page=13)

### Reliable Broadcast

Implementation of [reliable broadcast] protocol. Here, p1 sends message to p2, then
crash-stops. Since p2 sends to p3, all correct processes eventually deliver the message:

```
$ sbt "runMain com.github.dlorch.ReliableBroadcast.Main"
[p1] crashed
[p2] message delivered: Hello, reliable world!
[p3] message delivered: Hello, reliable world!
```

[![Reliable broadcast algorithm](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-22.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf#page=23)
[![Reliable broadcast example](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-23.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf#page=24)

### FIFO Broadcast

Implementation of [FIFO broadcast] protocol, which is a "FIFO Order transformation applied to a
reliable broadcast". In the example below, a message with sequence number 3 arrives out-of-order.
Each process `receive()`s and stores it in its `buffer`, because it is expecting sequence number
2 first. When the message with sequence number 2 finally arrives, all processes `deliver()`
messages 2 and 3 in the correct sequence to the process.

```
$ sbt "runMain com.github.dlorch.FIFOBroadcast.Main"
[p1] [seqn: 1] initiating reliable broadcast: 1st message
[p1] [seqn: 1] message delivered: 1st message
[p2] [seqn: 1] message delivered: 1st message
[p3] [seqn: 1] message delivered: 1st message
[p1] [seqn: 3] initiating reliable broadcast: 3rd message
[p1] [seqn: 2] initiating reliable broadcast: 2nd message
[p1] [seqn: 2] message delivered: 2nd message
[p2] [seqn: 2] message delivered: 2nd message
[p2] [seqn: 3] message delivered: 3rd message
[p1] [seqn: 3] message delivered: 3rd message
[p3] [seqn: 2] message delivered: 2nd message
[p3] [seqn: 3] message delivered: 3rd message
```

The key component of the FIFO Order transformation is the `next` table, kept at each node. This
variable keeps track of the expected next sequence number from its peer processes. The `next`
table *at each process* looks like this:

| Peer Process | Next Sequence Number
| :----------: | :------------------:
| p1           | 4 
| p2           | 1
| p3           | 1

At the *beginning of the execution*, the `next` tables in the entire system look like this:

| p1<br>Peer | <br>Next | p2<br>Peer | <br>Next | p3<br>Peer | <br>Next
| :--------: | :------: | :--------: | :------: | :--------: | :--------:
| p1 | 1 | p1 | 1 | p1 | 1
| p2 | 1 | p2 | 1 | p2 | 1
| p3 | 1 | p3 | 1 | p3 | 1

At the *end of the execution*, the `next` tables in the entire system look like this:

| p1<br>Peer | <br>Next | p2<br>Peer | <br>Next | p3<br>Peer | <br>Next
| :--------: | :------: | :--------: | :------: | :--------: | :--------:
| p1 | 4 | p1 | 4 | p1 | 4
| p2 | 1 | p2 | 1 | p2 | 1
| p3 | 1 | p3 | 1 | p3 | 1

[![FIFO Order](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-31.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf#page=32)
[![A modular approach to broadcast](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-39.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf#page=40)
[![FIFO Order algorithm](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-44.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf#page=45)

[distributed algorithms]: https://en.wikipedia.org/wiki/Distributed_algorithm
[best-effort broadcast]: http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf
[reliable broadcast]: http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf
[FIFO broadcast]: http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf
