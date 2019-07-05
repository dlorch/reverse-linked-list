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
        val othersExceptMe = Π.filter(actor => actor.path.name != self.path.name)
        othersExceptMe.foreach(q => q ! m)
        deliver(m)
        delivered += m.uuid
    }
    case m: Message => {
        if(!delivered.contains(m.uuid)) {
            val othersExceptSenderAndMe = Π.filter(actor => actor != m.sender || actor.path.name != self.path.name)
            othersExceptSenderAndMe.foreach(q => q ! m)
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
          Π.find(actor => actor.path.name == "p2") match {
              case Some(p2) => p2 ! m
              case None =>
          }         
          println(s"[${self.path.name}] crashed")
      }
      case m: Message => // do nothing, we are crashed
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
