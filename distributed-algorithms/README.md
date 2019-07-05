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

Implementation of [reliable broadcast] protocol. Here, p1 sends message to p2, then
crash-stops. Since p2 sends to p3, all correct processes eventually deliver the message:

```
$ sbt "runMain com.github.dlorch.ReliableBroadcast.Main"
[p1] crashed
[p2] message delivered: Hello, reliable world!
[p3] message delivered: Hello, reliable world!
```

[best-effort broadcast]: http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf
[reliable broadcast]: http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf