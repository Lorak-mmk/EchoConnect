# Packet

We designed our own packet format to send data as sound in EchoConnect project.
Packets are essential when implementing the protocol, allow to encapsulate data into rather small parts, each with its own checksum. Thanks to that when incorrect transmission happens, it is easier to detect that and ask for retransmission. No less important is that by setting particular flags we are able to indicate particular events to the receiver.


<br>

### Structure

- - -

Packets in serialized form are structured as below:

| element | size in bytes |
| :---: | :---: |
| **header ⬎** | 6 |
| flags | 2 |
| packet size | 2 |
| sequential number | 2 |
| **transmitted data** | up to \f$2^{16}-11\f$ |
| **frame check sequence ⬎** | 4 |
| control sum | 4 |


#### Flags

As for now there is **7** flags defined, but we have space for at most **16** flags (assuming we keep current implementation) and new ones can be easily added.
Existing flags:
- **SYN** : synchronization, used in connection initialization between hosts, only first packet sent out by host should have that flag set.
- **ACK1** : acknowledgement 1, used to confirm something, ex. creating a connection or that the packet was received coffectly.
- **ACK2**: acknowledgement 2, used to confirm receiving of the packet with the flag ACK1.
- **LPC**: last packet, informs that this is the last packet in a group of consecutive packets.
- **DMD**: demand resend, used to inform that some received packets were invalid or lost and there is a need to resend them.
- **FIN**: finish, used to inform that party is ending connection.
- **RST**: reset, used to break the connection brutally when something is not right.


#### Packet size

Size of a packet (header + content) in bytes. As it is a 16 bit number, the maximum valid packet size is \f$2^{16}-1\f$ bytes.


#### Sequential number

Identifier of packet.


#### Data

Data encapsulated in packet. Its size is limited to \f$2^{16}-11\f$ bytes because of existing packet size and need to reserve 10 bytes for packet frame.


#### Control sum

Packet checksum, to be more specific **CRC32** of other other packet components. Whenever transmitted packet is deserialized, if the control sum is invalid, the exception is raised. We are using [CRCpp](https://github.com/d-bahr/CRCpp/tree/51fbc35ef892e98abe91a51f7320749c929d72bd) library to calculate checksum.


<br>

### Notes

- - -

- We are supporting packet from object to bytes serialization as well as packet partial deserialization to object (in 3 phases - first header, then data and checksum at the end).
- We are using network endianness when sending serialized bytes. Although it doesn't change anything for content data, as it is being kept as a series of single bytes. So that fact is important only when someone is trying to build or parse a packet on their own. In such case it is crucial to remember about converting header fields and checksum to right endianness (_actually, author had mass of debugging while implementing and writing tests for packets cause she forgot about that converting :P_).


<br>

### Implementation

- - -

Current packet declaration and definition is available to read in [Packet.h](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/src/Packet.h) and [Packet.cc](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/src/Packet.cc) files in [EchoConnect](https://github.com/Lorak-mmk/EchoConnect) project repository.
