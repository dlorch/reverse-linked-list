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
| p1           | 1
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

### Causal Order Broadcast

For any two events `a` and `b`, if there is any way that `a` could have influenced `b`, then
`a` [happened-before] `b` and the two events are causally related. It is also possible to
have two events where we cannot say which came first; when that happens, it means that they
could not have affected each other. If `a` and `b` cannot have any effect on each other,
then it does not matter which one came first (Source: [Lamport timestamps]).

[![Definition Causal Order](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-32.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf#page=33)
[![Definition Causal Order](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-33.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf#page=34)
[![Definition Causal Order](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-34.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf#page=35)

The following is an attempt to re-create the scenario from [Causal Order - Algorithm 1](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-49.png).
First `p1` broadcasts a message `m1` (suppose the message says "Buy milk") to all its peers, but due
to network delays can only reach itself and `p2`. Then, `p2` broadcasts a message `m2` (suppose it
is "Sell milk"). Note that the two messages are causally related. When `p2` broadcasts its message
`m2`, it also includes its recent history, a sequence or list `[m1]`, along with it. `p3` will
thus receive `m2` along with the history `[m1]` and correctly deliver the messages in causal order.

Implementation of a *non-blocking* [causal order] protocol:

```
$ sbt "runMain com.github.dlorch.CausalOrderNonBlockingBroadcast.Main"
[p1] [sender: p1, seqn: 1] message delivered: Buy milk
[p2] [sender: p1, seqn: 1] message delivered: Buy milk
[p3] joined the party late
[p2] [sender: p2, seqn: 1] message delivered: Sell cheese
[p3] [sender: p1, seqn: 1] message delivered: Buy milk
[p1] [sender: p2, seqn: 1] message delivered: Sell cheese
[p3] [sender: p2, seqn: 1] message delivered: Sell cheese
```

[![Causal Order Non-Blocking Algorithm](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-48.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf#page=49)
[![Causal Order Non-Blocking Algorithm](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-49.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf#page=50)

Implementation of a *blocking* [causal order] broadcast protocol based on vector clocks,
where "blocking" means that the process may delay the delivery of a message for a later
time.

```
[p2] [sender: p1, seqn: 1] message delivered: Buy milk
[p1] [sender: p1, seqn: 1] message delivered: Buy milk
[p3] [sender: p1, seqn: 1] message delivered: Buy milk
[p2] [sender: p2, seqn: 1] message delivered: Sell cheese
[p3] [sender: p2, seqn: 1] message delivered: Sell cheese
[p1] [sender: p2, seqn: 1] message delivered: Sell cheese
```

[![Causal Order Blocking Algorithm](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-51.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf#page=52)
[![Causal Order Blocking Algorithm](https://raw.githubusercontent.com/dlorch/reverse-linked-list/master/distributed-algorithms/images/04-rb-52.png)](http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf#page=53)

[distributed algorithms]: https://en.wikipedia.org/wiki/Distributed_algorithm
[best-effort broadcast]: http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf
[reliable broadcast]: http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf
[FIFO broadcast]: http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf
[causal order]: http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf
[Lamport timestamps]: https://en.wikipedia.org/wiki/Lamport_timestamps
[happened-before]: https://en.wikipedia.org/wiki/Happened-before