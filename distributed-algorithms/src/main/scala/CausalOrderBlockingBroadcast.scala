package com.github.dlorch.CausalOrderBlockingBroadcast

import akka.actor.{Actor, ActorRef, ActorSystem, Props}
import scala.collection.mutable.{HashSet, HashMap}

case class Message(body: String, sender: ActorRef, seqn: Int)
case class CausalOrderBlockingBroadcast(m: Message)

object Environment {
  var Π = HashSet[ActorRef]()
}

trait ReliableBroadcastActor extends Actor {
  import Environment._

  var R_delivered = HashSet[Message]()

  def R_broadcast(m: Message, VC: HashMap[ActorRef, Int]) {
    Π.filter(_ != self).foreach(q => q ! (m, VC))
    R_deliver(m, VC)
    R_delivered += m
  }

  def R_receive(m: Message, VC: HashMap[ActorRef, Int]) {
    if(!R_delivered.contains(m)) {
      Π.filter(_ != m.sender).filter(_ != self).foreach(q => q ! (m, VC))
      R_deliver(m, VC)
      R_delivered += m
    }
  }

  def R_deliver(m: Message, VC: HashMap[ActorRef, Int])
}

trait FIFOBroadcastActor extends ReliableBroadcastActor {
  import Environment._

  var F_buffer =  HashSet[Tuple2[Message, HashMap[ActorRef, Int]]]()
  var next = HashMap[ActorRef, Int]().withDefaultValue(1)

  override def R_deliver(m: Message, VC: HashMap[ActorRef, Int]) {
    F_buffer += Tuple2(m, VC)

    var m_prime = nextMessageForPeer(m.sender)
    while(m_prime.isDefined) {
      F_deliver(m_prime.get._1, VC)
      next(m_prime.get._1.sender) += 1
      F_buffer -= m_prime.get

      m_prime = nextMessageForPeer(m.sender)
    }
  }

  private def nextMessageForPeer(p: ActorRef) = {
    F_buffer.find(m_prime => m_prime._1.sender == p && m_prime._1.seqn == next(p))
  }

  def F_broadcast(m: Message, VC: HashMap[ActorRef, Int]) = super.R_broadcast(m, VC)

  def F_receive(m: Message, VC: HashMap[ActorRef, Int]) = super.R_receive(m , VC)

  def F_deliver(m: Message, TS: HashMap[ActorRef, Int])
}

class CausalOrderBlockingBroadcastActor extends FIFOBroadcastActor {
  import Environment._

  var buffer = HashSet[Tuple2[Message, HashMap[ActorRef, Int]]]()
  var VC = HashMap[ActorRef, Int]()
  Π.foreach(p => VC(p) = 0)

  def receive = {
    case CausalOrderBlockingBroadcast(m) => {
      var TS = VC.clone()
      TS(self) += 1
      super.F_broadcast(m, TS)
    }
    case (m: Message, ts) => super.F_receive(m, ts.asInstanceOf[HashMap[ActorRef, Int]])
  }

  override def F_deliver(m: Message, TS: HashMap[ActorRef, Int]) {
    buffer += Tuple2(m, TS)

    var m_prime = nextMessage
    while(m_prime.isDefined) {
      C_deliver(m_prime.get._1)
      VC(m_prime.get._1.sender) = m_prime.get._2(m_prime.get._1.sender)
      buffer -= Tuple2(m_prime.get._1, m_prime.get._2)

      m_prime = nextMessage
    }
  }

  private def nextMessage = {
    buffer.find{ case (m_prime: Message, ts: HashMap[ActorRef, Int]) =>
                    VC(m_prime.sender) == ts(m_prime.sender) - 1 &&
                    ts.keys.forall(s => (s == m_prime.sender) || (s != m_prime.sender && VC(s) >= ts(s))) }
  }

  def C_deliver(m: Message) {
    println(s"[${self.path.name}] [sender: ${m.sender.path.name}, seqn: ${m.seqn}] message delivered: ${m.body}")
  }
} 

class ReplyingCausalOrderBlockingBroadcastActor extends CausalOrderBlockingBroadcastActor {
  override def C_deliver(m: Message) {
    if(m.body == "Buy milk") {
      self ! CausalOrderBlockingBroadcast(Message(body = "Sell cheese", sender = self, seqn = 1))
    }
    super.C_deliver(m)
  }
}

object Main extends App {
  import Environment._

  val system = ActorSystem("CausalOrderBlockingBroadcastSystem")
  val p1 = system.actorOf(Props[CausalOrderBlockingBroadcastActor], name = "p1")
  val p2 = system.actorOf(Props[ReplyingCausalOrderBlockingBroadcastActor], name = "p2")
  val p3 = system.actorOf(Props[CausalOrderBlockingBroadcastActor], name = "p3")

  Π += p1
  Π += p2
  Π += p3

  p1 ! CausalOrderBlockingBroadcast(Message(body = "Buy milk", sender = p1, seqn = 1))
}
