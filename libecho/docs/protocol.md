# Protocol (EchoProtocol) {#Protocol}

The main objective for the protocol is to provide reliable audio-based transmission between nodes by checking the correctness and running retransmission when needed. Below there are main characteristics of it:

- stream-oriented transmission like in TCP
- client-server like, where the connection between nodes is being created one node listens for other initiating the conversation
- P2P as due to the hardware limitations more nodes would interfere with each other
- there is no checking who is transmitting and everyone can listen so it is not data safe solution, identity confirmation could be implemented on top of the protocol
- transmission split into packets which arrive in the sending order or not at all

Worth saying is that we have UDP & broadcast like transmission implemented as well. It is realized by EchoRaw Connection and protocol bases on this solution. Main characteristics of it are:

- data sending without correctness checking, confirmations and retransmissions
- as in the main protocol, there is no sender and recipient specified

<br>

### Under the hood

- - -


### Notes

- - -


<br>

### Implementation

- - -

Current protocol declaration and definition is available to read in [EchoProtocol.h](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/include/EchoProtocol.h) and [EchoProtocol.cc](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/src/EchoProtocol.cc) files in [EchoConnect](https://github.com/Lorak-mmk/EchoConnect) project repository.
