# Receiver

**Receivers** are objects which **receive data**. And, as there are many possibilities to convert data to sound, consequently, many different receivers may exist. What is also important, that **for every receiver created by us, there exists a matching sender**, which is able to convert data into waveforms and transfer them.



Below there are more details about how it works under the hood.


<br>

### IReceiver

- - -

IReceiver is an interface which represents all possible receiver implementations. It offers methods which allow to start receiving stream, and to receive bytes.


<br>

### Existing receivers

- - -

EchoConnect provides two distinct receiver implementations, both of which decode bits from waveforms captured by the microphone using discrete fourier transforms.


#### BitReceiver

To use BitReceiver we need to specify two distinct frequencies (they should be in the decent distance) - one would be used for encoding bits with value 1, the other one for bits with value 0. With that, bits are decoded from the microphone.


#### BitReceiverv2

BitReceiverv2 first listens for the header 10010100, which it uses to synchronize the receiving window. After that, it uses the specified threshold (lim) to distinguish between ones and zeroes. While it receives them, it tries to adjust the threshold, because speakers and microphones are inconsistent.

<br>

### Implementation

- - -

Current headers and implementation of IReceiver, BitReceiver and BitReceiverv2 are available to read in [EchoConnect](https://github.com/Lorak-mmk/EchoConnect) project repository. Links are listed below:

- [IReceiver.h](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/src/IReceiver.h)
- [BitReceiver.h](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/src/BitReceiver.h)
- [BitReceiver.cc](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/src/BitReceiver.cc)
- [BitReceiverv2.h](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/src/BitReceiverv2.h)
- [BitReceiverv2.cc](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/src/BitReceiverv2.cc)

