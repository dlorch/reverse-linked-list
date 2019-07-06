package com.github.dlorch.FIFOBroadcast

import akka.actor.{Actor, ActorRef, ActorSystem, Props}
import scala.collection.mutable.{HashSet, HashMap}
import java.util.UUID

case class Message(uuid: String = UUID.randomUUID.toString, body: String, sender: ActorRef, seqn: Int)
case class FIFOBroadcast(m: Message)

object Environment {
  var Π = HashSet[ActorRef]()
}

trait ReliableBroadcastActor extends Actor {
  import Environment._

  var delivered = HashSet[String]()

  def R_broadcast(m: Message) {
    Π.filter(_ != self).foreach(q => q ! m)
    R_deliver(m)
    delivered += m.uuid
  }

  def R_receive(m: Message) {
    if(!delivered.contains(m.uuid)) {
      Π.filter(_ != m.sender).filter(_ != self).foreach(q => q ! m)
      R_deliver(m)
      delivered += m.uuid
    }
  }

  def R_deliver(m: Message)
}

class FIFOBroadcastActor extends ReliableBroadcastActor {
  import Environment._

  var buffer = HashSet[Message]()
  var next = HashMap[ActorRef, Int]().withDefaultValue(1)

  def receive = {
    case FIFOBroadcast(m) => super.R_broadcast(m)
    case m: Message => super.R_receive(m)
  }

  override def R_deliver(m: Message) {
    buffer += m

    var m_prime = nextMessageInBuffer(m)
    while(m_prime.isDefined) {
      F_deliver(m_prime.get)
      next(m.sender) += 1
      buffer -= m_prime.get

      m_prime = nextMessageInBuffer(m)
    }
  }

  def nextMessageInBuffer(m: Message) = buffer.filter(_.sender == m.sender).filter(_.seqn == next(m.sender)).headOption

  def F_deliver(m: Message) {
    println(s"[${self.path.name}] [seqn: ${m.seqn}] message delivered: ${m.body} ")
  }
}

object Main extends App {
  import Environment._

  val system = ActorSystem("FIFOBroadcastSystem")
  val p1 = system.actorOf(Props[FIFOBroadcastActor], name = "p1")
  val p2 = system.actorOf(Props[FIFOBroadcastActor], name = "p2")
  val p3 = system.actorOf(Props[FIFOBroadcastActor], name = "p3")

  Π += p1
  Π += p2
  Π += p3

  p1 ! FIFOBroadcast(Message(body = "1st message", sender = p1, seqn = 1))
  p1 ! FIFOBroadcast(Message(body = "3rd message", sender = p1, seqn = 3))
  p1 ! FIFOBroadcast(Message(body = "2nd message", sender = p1, seqn = 2))
}