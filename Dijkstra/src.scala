 //  ____            _     _                
 // |  _ \ _ __ ___ | |__ | | ___ _ __ ___  
 // | |_) | '__/ _ \| '_ \| |/ _ \ '_ ` _ \ 
 // |  __/| | | (_) | |_) | |  __/ | | | | |
 // |_|   |_|  \___/|_.__/|_|\___|_| |_| |_|
 // ========================================

// You have three batteries, with capacities of 40, 25, and 16 Ah (Amp-hours), respectively.
// The 25 and 16-Ah batteries are fully charged (containing 25 Ah and 16 Ah, respectively),
// while the 40-Ah battery is empty, with 0 Ah. You have a battery transfer device which has
// a “source” battery position and a “target” battery position. When you place two batteries
// in the device, it instantaneously transfers as many Ah from the source battery to the target
// battery as possible. Thus, this device stops the transfer either when the source battery has
// no Ah remaining or when the destination battery is fully charged (whichever comes first).

// But battery transfers aren’t free! The battery device is also hooked up to your phone by
// bluetooth, and automat- ically charges you a number of dollars equal to however many Ah
// it just transfered.

// The goal in this problem is to determine whether there exists a sequence of transfers that
// leaves exactly 10 Ah either in the 25-Ah battery or the 16-Ah battery, and if so,
// how little money you can spend to get this result.

 //  ____        _       _   _             
 // / ___|  ___ | |_   _| |_(_) ___  _ __  
 // \___ \ / _ \| | | | | __| |/ _ \| '_ \ 
 //  ___) | (_) | | |_| | |_| | (_) | | | |
 // |____/ \___/|_|\__,_|\__|_|\___/|_| |_|
 // =======================================

import scala.collection.mutable.PriorityQueue

class Battery(val charge: Int, val cap: Int) {
  override def toString(): String = s"($charge:$cap)"
  def drain(x: Int): Battery = {
    require(charge >= x, "Tried to drain more than the charge")
    new Battery(charge - x, cap)
  }

  def fill(x: Int): Battery = {
    require(charge + x <= cap, "Not enough capacity to fill that much")
    new Battery(charge + x, cap)
  }

  def equiv(other: Battery): Boolean = {
    charge == other.charge && cap == other.cap
  }
}

// class State(val b1: Int, val b2: Int, val b3: Int, val cost: Int, val parent: Option[State]) {
class State(val b1:Battery, val b2:Battery, val b3:Battery, val cost:Int, val parent:Option[State]) {
  override def toString(): String = s"{b1:$b1 b2:$b2 b3:$b3 cost:$cost}"

  def getPath: List[State] = {
    def _getPath(s: Option[State], acc: List[State]): List[State] = s match {
      case Some(s) => _getPath(s.parent, s::acc)
      case None => acc
    }
    _getPath(Some(this), Nil)
  }

  def equiv(other: State): Boolean = {
    b1.equiv(other.b1) &&
    b2.equiv(other.b2) &&
    b3.equiv(other.b3)
  }

  def neighbors: List[State] = {
    var res: List[State] = List()

    if (b1.charge > 0 && b2.charge < b2.cap) {
      res = (transfer("b1", "b2")::res)
    }

    if (b1.charge > 0 && b3.charge < b3.cap) {
      res = (transfer("b1", "b3")::res)
    }

    if (b2.charge > 0 && b1.charge < b1.cap) {
      res = (transfer("b2", "b1")::res)
    }

    if (b2.charge > 0 && b3.charge < b3.cap) {
      res = (transfer("b2", "b3")::res)
    }   

    if (b3.charge > 0 && b1.charge < b1.cap) {
      res = (transfer("b3", "b1")::res)
    }

    if (b3.charge > 0 && b2.charge < b2.cap) {
      res = (transfer("b3", "b2")::res)
    }

    res
  }

  def transfer(src: String, dst: String): State = {
    require(src != dst)
    require(List("b1","b2","b3").contains(src))
    require(List("b1","b2","b3").contains(dst))

    if (src == "b1") {
      if (dst == "b2") {
        require(b1.charge > 0, "Can't transfer from b1: b1 is empty")
        require(b2.charge < b2.cap, "Can't transfer to b2: b2 is full")
        // either b2 cap will be reached, or b1 will lose charge first
        val transferred = (if (b1.charge > (b2.cap - b2.charge)) {
          b2.cap - b2.charge
        } else {
          b1.charge
        })

        new State(b1.drain(transferred), b2.fill(transferred), b3, cost + transferred, Some(this))
      } else { // dst == b3
        require(b1.charge > 0, "Can't transfer from b1: b1 is empty")
        require(b3.charge < b3.cap, "Can't transfer to b3: b3 is full")

        val transferred = (if (b1.charge > (b3.cap - b3.charge)) {
          b3.cap - b3.charge
        } else {
          b1.charge
        })

        new State(b1.drain(transferred), b2, b3.fill(transferred), cost + transferred, Some(this))
      }
    } else if (src == "b2") {
      if (dst == "b1") {
        require(b2.charge > 0, "Can't transfer from b2: b2 is empty")
        require(b1.charge < b1.cap, "Can't transfer to b1: b1 is full")
        val transferred = (if (b2.charge > (b1.cap - b1.charge)) {
          b1.cap - b1.charge
        } else {
          b2.charge
        })

        new State(b1.fill(transferred), b2.drain(transferred), b3, cost + transferred, Some(this))
      } else { // dst == b3
        require(b2.charge > 0, "Can't transfer from b2: b2 is empty")
        require(b3.charge < b3.cap, "Can't transfer to b3: b3 is full")

        val transferred = (if (b2.charge > (b3.cap - b3.charge)) {
          b3.cap - b3.charge
        } else {
          b2.charge
        })

        new State(b1, b2.drain(transferred), b3.fill(transferred), cost + transferred, Some(this))
      }
    } else { // src == b3
      if (dst == "b1") {
        require(b3.charge > 0, "Can't transfer from b3: b3 is empty")
        require(b1.charge < b1.cap, "Can't transfer to b1: b1 is full")

        val transferred = (if (b3.charge > (b1.cap - b1.charge)) {
          b1.cap - b1.charge
        } else {
          b3.charge
        })

        new State(b1.fill(transferred), b2, b3.drain(transferred), cost + transferred, Some(this))
      } else { // dst == b2
        require(b3.charge > 0, "Can't transfer from b3: b3 is empty")
        require(b2.charge < b2.cap, "Can't transfer to b2: b2 is full")

        val transferred = (if (b3.charge > (b2.cap - b2.charge)) {
          b2.cap - b2.charge
        } else {
          b3.charge
        })

        new State(b1, b2.fill(transferred), b3.drain(transferred), cost + transferred, Some(this))
      }
    }
  }  
}

def printPath(s: Option[State]): Unit = s match {
  case Some(s) => {
    println(s)
    printPath(s.parent)
  }
  case None => return
}

object MinOrder extends Ordering[Int] {
  def compare(x:Int, y:Int) = y compare x
}

object OrderState extends Ordering[State] {
  def compare(x:State, y:State) = y.cost compare x.cost
}

 //  ____             _         ____  _  _ _        _             
 // | __ )  ___  __ _(_)_ __   |  _ \(_)(_) | _____| |_ _ __ __ _ 
 // |  _ \ / _ \/ _` | | '_ \  | | | | || | |/ / __| __| '__/ _` |
 // | |_) |  __/ (_| | | | | | | |_| | || |   <\__ \ |_| | | (_| |
 // |____/ \___|\__, |_|_| |_| |____/|_|/ |_|\_\___/\__|_|  \__,_|
 //             |___/                 |__/                        

val s0 = new State(
  new Battery(0,40),
  new Battery(25,25),
  new Battery(16,16),
  0,
  None
)
val pq = new PriorityQueue[State]()(OrderState)
pq.enqueue(s0)

var result: Option[State] = None
var visiting: State = s0
var visited: List[State] = List()
var j = 0

while (!pq.isEmpty && result == None) {
  visiting = pq.dequeue
  visited = visiting::visited
  j += 1

  if (visiting.b2.charge == 10 || visiting.b3.charge == 10) {
    if (result == None) {
      result = Some(visiting)      
    } else if (result.get.cost > visiting.cost) {
      result = Some(visiting)
    }
  }

  for (neighbor <- visiting.neighbors) {
    var hasVisited = false
    var i = 0
    var break = false
    while (i < visited.length && !break) {
      if (visited(i).equiv(neighbor)) {
        hasVisited = true
        break = true
      }
      i += 1
    }

    if (!hasVisited) {
      pq.enqueue(neighbor)
    }
  }
}
