import scala.collection.mutable.PriorityQueue

// Define a tree type for the huffman encoding. Leaf nodes are Chr (characters), else Frq (frequencies)
sealed trait Tree {
  def to_latex: String = {
    val cs = collection.mutable.ArrayDeque('a')
    cs.removeHead() // to create an empty ArrayDeque

    def _to_latex(tree:Tree): String = tree match {
      case Frq(f, (Some(a:Frq), Some(b:Frq))) => {
        f"[.$f%2.3f\n" + 
        "  \\edge node[auto=right] {0}; " + _to_latex(a) +
        "  \\edge node[auto=left] {1}; " + _to_latex(b) + " ]"
      }
        // case Frq(f, (Some(Chr(c:Char)), None)) => f"[.$f%2.2f $c%c ]"
      case Frq(f, (Some(Chr(c:Char)), None)) => {
        val C:Char = c.toUpper
        cs.append(c)
        f"[.\\node ($C%c) {$f%2.4f}; ]"
      }
    }

    def _paths(cs: List[Char]): String = {
      (for (c <- cs) yield {
        "\\path (" + c.toUpper + ".east) node[right] {" + c + "};\n"
      }).mkString
    }

    """\begin{center}
\begin{tikzpicture}[every tree node/.style={draw,circle,align=center},
    level distance=1.8cm,sibling distance=0.4cm,
    edge from parent path={(\tikzparentnode) -- (\tikzchildnode)}]
""" + "\\Tree" + _to_latex(this) + _paths(cs.toList) + 
    """
\end{tikzpicture}
\end{center}"""
  }
}
case class Chr(c: Char) extends Tree
case class Frq(f: Double, children: (Option[Tree], Option[Tree])) extends Tree {
  def format_pp: String = {
    def collect_leaf_chars(tree:Frq): String = tree match {
      case Frq(f, (Some(a:Frq), Some(b:Frq))) => collect_leaf_chars(a) + collect_leaf_chars(b)
      case Frq(f, (Some(Chr(c:Char)), None)) => c.toString
    }

    val Frq(f, _) = this
    val leaf_chars = collect_leaf_chars(this)    
    f"($leaf_chars%s, $f%2.3f)"
  }
}

// This is necessary for Scala's std. lib PriorityQueue type,
// Without this, our Priority Queue would be a max heap
// We define our Ordering to make it be a min heap
object OrderFrq extends Ordering[Frq] {
  def compare(x:Frq, y:Frq) = y.f compare x.f
}

// method to traverse the huffman tree and return the encoding as key-value pairs
def collect_huffman_encoding(tree: Frq, acc: String = ""): Map[Char, String] = tree match {
  case Frq(f, (Some(a:Frq), Some(b:Frq))) => collect_huffman_encoding(a, acc + "0") ++ collect_huffman_encoding(b, acc + "1")
  case Frq(f, (Some(Chr(c:Char)), None)) => Map[Char, String](c -> acc)
}

def huffman_tree(s: String): Frq = {
  val pq = new PriorityQueue[Frq]()(OrderFrq)

  val chars_freq_assoc = s.toSet                     // Get distinct characters.
    .map(c => {                                   // Transform the set of chars
      (c, s.count(_ == c).toDouble / s.length.toDouble) // into a set of tuples, e.g. ("a", 0.06)
    })

  // Enqueue each member of the char_freq_assoc into the priority queue as Tree's.
  // e.g. pq: {(0.06, a), (0.12, h), ...}
  for (x <- chars_freq_assoc) {
    pq.enqueue(Frq(x._2, (Some(Chr(x._1)), None)))
  }

  while (pq.size > 1) {
    val a:Frq = pq.dequeue
    val b:Frq = pq.dequeue
    // merge a and b into a subtree with frq of a + b, and enqueue it.
    pq.enqueue(Frq(a.f + b.f, (Some(a), Some(b))))
  }

  return pq.dequeue
}

def huffman_encode(s: String): Map[Char, String] = collect_huffman_encoding(huffman_tree(s))


def format_pp_pq(pq: PriorityQueue[Frq]): String = {
  val pq_clone = pq.clone
  var res: List[String] = List()
  while (pq_clone.size != 0) {
    res = res :+ pq_clone.dequeue.format_pp
  }
  res.mkString("pq: $\\left(",", ","\\right)$")
}
