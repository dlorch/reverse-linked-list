package com.github.dlorch.FIFOBroadcast

import akka.actor.{Actor, ActorRef, ActorSystem, Props}
import scala.collection.mutable.{HashSet, HashMap}

case class Message(body: String, sender: ActorRef, seqn: Int)
case class FIFOBroadcast(m: Message)

object Environment {
  var Π = HashSet[ActorRef]()
}

trait ReliableBroadcastActor extends Actor {
  import Environment._

  var delivered = HashSet[Message]()

  def R_broadcast(m: Message) {
    Π.filter(_ != self).foreach(q => q ! m)
    R_deliver(m)
    delivered += m
  }

  def R_receive(m: Message) {
    if(!delivered.contains(m)) {
      Π.filter(_ != m.sender).filter(_ != self).foreach(q => q ! m)
      R_deliver(m)
      delivered += m
    }
  }

  def R_deliver(m: Message)
}

class FIFOBroadcastActor extends ReliableBroadcastActor {
  import Environment._

  var buffer = HashSet[Message]()
  var next = HashMap[ActorRef, Int]().withDefaultValue(1)

  def receive = {
    case FIFOBroadcast(m) => {
      println(s"[${self.path.name}] [seqn: ${m.seqn}] initiating reliable broadcast: ${m.body}")
      super.R_broadcast(m)
    }
    case m: Message => super.R_receive(m)
  }

  override def R_deliver(m: Message) {
    buffer += m

    var m_prime = nextMessageForPeer(m.sender)
    while(m_prime.isDefined) {
      F_deliver(m_prime.get)
      next(m.sender) += 1
      buffer -= m_prime.get

      m_prime = nextMessageForPeer(m.sender)
    }
  }

  private def nextMessageForPeer(p: ActorRef) = {
    buffer.filter(_.sender == p)
          .filter(_.seqn == next(p))
          .headOption
  }

  def F_deliver(m: Message) {
    println(s"[${self.path.name}] [seqn: ${m.seqn}] message delivered: ${m.body}")
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
  Thread.sleep(200)
  p1 ! FIFOBroadcast(Message(body = "3rd message", sender = p1, seqn = 3))
  Thread.sleep(200)
  p1 ! FIFOBroadcast(Message(body = "2nd message", sender = p1, seqn = 2))
}
