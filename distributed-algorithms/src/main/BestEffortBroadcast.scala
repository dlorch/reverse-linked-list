package com.github.dlorch.BestEffortBroadcast

import akka.actor.{Actor, ActorRef, ActorSystem, Props}
import scala.collection.mutable.HashSet
import java.util.UUID

case class Message(uuid: String = UUID.randomUUID.toString, body: String)
case class BestEffortBroadcast(m: Message)

object Environment {
  var Π = HashSet[ActorRef]()
}

class BestEffortBroadcastActor extends Actor {
  import Environment._

  def receive = {
     // the actor library provides you facilities for broadcasting that you might prefer
     // to use in a *real* actor system, e.g. system.actorSelection("/user/*") ! m
     // or alternatively context.system.eventStream.subscribe() and .publish()
     // the approach here was chosen to keep in line with the documentation
    case BestEffortBroadcast(m) => Π.foreach(q => q ! m)
    case m: Message => deliver(m)
  }

  def deliver(m: Message) {
    println(s"[${self.path.name}] message delivered: ${m.body}")
  }
}

object Main extends App {
  import Environment._

  val system = ActorSystem("BestEffortBroadcastSystem")
  val p1 = system.actorOf(Props[BestEffortBroadcastActor], name = "p1")
  val p2 = system.actorOf(Props[BestEffortBroadcastActor], name = "p2")
  val p3 = system.actorOf(Props[BestEffortBroadcastActor], name = "p3")

  Π += p1
  Π += p2
  Π += p3

  p1 ! BestEffortBroadcast(Message(body = "Hello, distributed world!"))
}
