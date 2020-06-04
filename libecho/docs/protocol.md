# Protocol (EchoProtocol) {#Protocol}

The main objective for the protocol is to provide reliable audio-based transmission between nodes by checking the correctness and running retransmission when needed. Below there are main characteristics of it:

- **stream-oriented** transmission like in TCP
- **client-server like**, where the connection between nodes is being created one node listens for other initiating the conversation
- **P2P** as due to the hardware limitations more nodes would interfere with each other
- there is **no checking who is transmitting and everyone can listen** so it is not data safe solution, identity confirmation could be implemented on top of the protocol
- **data split into packets** which arrive in the sending order or not at all

Worth saying is that we have UDP & broadcast like transmission implemented as well. It is realized by EchoRaw Connection and protocol bases on this solution. Main characteristics of it are:

- data sending without correctness checking, confirmations and retransmissions
- as in the main protocol, there is no sender and recipient specified

<br>

### Under the hood

- - -

**To not interfere with each other only one node can transmit audio at the moment.**

To ensure that, we set up constant length time windows alternately assigned to nodes during which only the assigned one can speak. Length of such time windows is being calculated from a few variables (it's explained below). Nodes supervise themselves to not to transmit during the other node's window by making calculations based on the system clock.
In the beginning, the node needs to synchronize these windows with the other one. When initiating node connects (by sending a packet with SYN flag set) and the listening one confirms the connection (by sending a packet with ACK1 flag set) they are calculating when the first window had begun and start to exchange messages according to that.


**Even when there is no data to broadcast there must be something showing that the other side still lives. The connection needs to be kept alive.**

Because of that, when a node has nothing to confirm or send it sends a packet with no data included just to mark its presence.

**How is the length of sending time window decided?**

Each node sends no more than some hardcoded value B of bytes in one packet (and sends as much as it can depending on how big is "to send" queue when it is time to transmit). The length of the sending window is safely bigger than the exact time needed to send one full packet (with B bytes of data encapsulated).

<br>

### Implementation

- - -

Current protocol declaration and definition are available to read in [EchoProtocol.h](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/include/EchoProtocol.h) and [EchoProtocol.cc](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/src/EchoProtocol.cc) files in [EchoConnect](https://github.com/Lorak-mmk/EchoConnect) project repository.
