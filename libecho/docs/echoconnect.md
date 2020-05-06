# EchoConnect

EchoConnect is library enabling data transmissions between computers using audio. It consists of three parts: Echo, EchoRawConnection and EchoProtocol.


<br>

### Echo

- - -


Echo is a class for general use, for things not subject to EchoRawConnection and EchoProtocol. Right now there is only one method in there which initializes hidden under the hood QT and should be called only if user had not used QT in project yet.



<br>

### EchoRawConnection

- - -

EchoRawConnection is an abstraction for low-level data transmissions using audio. User may choose one from some data encoding implementations (which are explained in Sender and Receiver documents) by calling rigth metohod to obtain an object. Then data transfer is nearly as simple as calling send method on one machine and receive on the other. Unfortunately, it may happen that transmission was incorrect because of many possible factors and it cannot be detected by our library. In such case there is no way to retrieve valid message other than sending it again.



<br>

### EchoProtocol

- - -

EchoProtocol is a higer-level way to send data using sound than in EchoRawConnection (which it actually uses). It is based on well known network protocols. To send data user must first connect computers by using appropriate methods such as listen and connect. Then data transmission is as simple as calling send and recive. Transmission correctness is checked by our library and if any retransmission is needed it happens automatically. At the end there is only need to finish connection. Usage is analogous to ex. socket programming in C/C++, though much more slower.



<br>

### Notes

- - -

- This is a group project for the university course called Software Engineering.
- Because of hardware limitations sending data by sound is really slow and unreliable, We wouldn't advise anyone to use this library in sitations when time is crucial. It is more for fun, to try out how it works.



<br>

### Implementation

- - -

External headers of library are [Echo.h](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/include/Echo.h), [EchoRawConnection.h](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/include/EchoRawConnection.h) and [EchoProtocol.h](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/include/EchoProtocol.h). They are available to read (as well as the whole implementation) in [EchoConnect](https://github.com/Lorak-mmk/EchoConnect) project repository.
