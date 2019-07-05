Distributed Algorithms
======================

This repository contains my solutions for challenges on distributed algorithms.

Build and run programs using:

```
$ sbt run
```

Best-effort broadcast
---------------------

Implementation of a [best-effort broadcast] distributed algorithm.

```
$ sbt "runMain com.github.dlorch.BestEffortBroadcast.Main"
[p3] message delivered: Hello, distributed world!
[p1] message delivered: Hello, distributed world!
[p2] message delivered: Hello, distributed world!

[best-effort broadcast]: http://disi.unitn.it/~montreso/ds/handouts/04-rb.pdf
