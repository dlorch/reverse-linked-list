package com.github.dlorch.ReliableBroadcast

import akka.actor.{Actor, ActorRef, ActorSystem, Props}
import scala.collection.mutable.HashSet
import java.util.UUID

case class Message(uuid: String = UUID.randomUUID.toString, body: String, sender: ActorRef)
case class ReliableBroadcast(m: Message)

object Environment {
  var Π = HashSet[ActorRef]()
}

class ReliableBroadcastActor extends Actor {
  import Environment._

  var delivered = HashSet[String]()

  def receive = {
    case ReliableBroadcast(m) => {
        Π.filter(_ != self).foreach(q => q ! m)
        deliver(m)
        delivered += m.uuid
    }
    case m: Message => {
        if(!delivered.contains(m.uuid)) {
            Π.filter(_ != m.sender).filter(_ != self).foreach(q => q ! m)
            deliver(m)
            delivered += m.uuid
        }
    }
  }

  def deliver(m: Message) {
    println(s"[${self.path.name}] message delivered: ${m.body}")
  }
}

class CrashStopReliableBroadcastActor extends Actor {
    import Environment._
  
    def receive = {
      case ReliableBroadcast(m) => {
          Π.filter(_ != self).head ! m // randomly pick another peer ("random" because HashSet does not guarantee ordering)
          context.stop(self)
          println(s"[${self.path.name}] crashed")
      }
    }
}

object Main extends App {
  import Environment._

  val system = ActorSystem("ReliableBroadcastSystem")
  val p1 = system.actorOf(Props[CrashStopReliableBroadcastActor], name = "p1")
  val p2 = system.actorOf(Props[ReliableBroadcastActor], name = "p2")
  val p3 = system.actorOf(Props[ReliableBroadcastActor], name = "p3")

  Π += p1
  Π += p2
  Π += p3

  p1 ! ReliableBroadcast(Message(body = "Hello, reliable world!", sender = p1))
}
