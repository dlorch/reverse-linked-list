package com.github.dlorch.CausalOrderNonBlockingBroadcast

import akka.actor.{Actor, ActorRef, ActorSystem, Props}
import scala.collection.mutable.{HashSet, HashMap}

case class Message(body: String, sender: ActorRef, seqn: Int)
case class CausalOrderNonBlockingBroadcast(m: Message)

object Environment {
  var Π = HashSet[ActorRef]()
}

trait ReliableBroadcastActor extends Actor {
  import Environment._

  var R_delivered = HashSet[Message]()

  def R_broadcast(m: Message, recent: List[Message]) {
    Π.filter(_ != self).foreach(q => q ! m :: recent) // broadcast the message including the recent history
    R_deliver(m)
    R_delivered += m
  }

  def R_receive(m: Message, recent: List[Message]) {
    if(!R_delivered.contains(m)) {
      Π.filter(_ != m.sender).filter(_ != self).foreach(q => q ! m :: recent)

      (m :: recent).reverse.foreach { r => // recent holds the history in reverse order (last element is oldest)
        if(!R_delivered.contains(r)) {
          R_deliver(r)
          R_delivered += r
        }
      }
    }
  }

  def R_deliver(m: Message)
}

trait FIFOBroadcastActor extends ReliableBroadcastActor {
  import Environment._

  var buffer = HashSet[Message]()
  var next = HashMap[ActorRef, Int]().withDefaultValue(1)

  override def R_deliver(m: Message) {
    buffer += m

    var messages = List[Message]()

    var m_prime = nextMessageForPeer(m.sender)
    while(m_prime.isDefined) {
      messages = messages :+ m_prime.get
      next(m.sender) += 1
      buffer -= m_prime.get

      m_prime = nextMessageForPeer(m.sender)
    }
    F_deliver(messages)
  }

  private def nextMessageForPeer(p: ActorRef) = {
    buffer.filter(_.sender == p)
          .filter(_.seqn == next(p))
          .headOption
  }

  def F_broadcast(m: Message, recent: List[Message]) = super.R_broadcast(m, recent)

  def F_receive(m: Message, recent: List[Message]) = super.R_receive(m , recent)

  def F_deliver(ms: List[Message])
}

class CausalOrderNonBlockingBroadcastActor extends FIFOBroadcastActor {
  import Environment._

  var delivered = HashSet[Message]()
  var recent = List[Message]()

  def receive = {
    case CausalOrderNonBlockingBroadcast(m) => {
        super.F_broadcast(m, recent)
        recent = List[Message]()
    }
    case m :: recent => super.F_receive(m.asInstanceOf[Message], recent.asInstanceOf[List[Message]])
  }

  override def F_deliver(ms: List[Message]) {
    ms.foreach(m => 
      if(!delivered.contains(m)) {
        delivered += m
        recent = m :: recent
        C_deliver(m)
      }
    )
  }

  def C_deliver(m: Message) {
    println(s"[${self.path.name}] [sender: ${m.sender.path.name}, seqn: ${m.seqn}] message delivered: ${m.body}")
  }
} 

object Main extends App {
  import Environment._

  val system = ActorSystem("CausalOrderNonBlockingBroadcastSystem")
  val p1 = system.actorOf(Props[CausalOrderNonBlockingBroadcastActor], name = "p1")
  val p2 = system.actorOf(Props[CausalOrderNonBlockingBroadcastActor], name = "p2")
  val p3 = system.actorOf(Props[CausalOrderNonBlockingBroadcastActor], name = "p3")
  val m1 = Message(body = "Buy milk", sender = p1, seqn = 1)
  val m2 = Message(body = "Sell milk", sender = p2, seqn = 1)

  Π += p1
  Π += p2

  p1 ! CausalOrderNonBlockingBroadcast(m1)
  Thread.sleep(100)

  Π += p3
  println(s"[${p3.path.name}] joined the party late")

  p2 ! CausalOrderNonBlockingBroadcast(m2)
}
