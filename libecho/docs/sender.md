# Sender

**Senders** are objects which **take data and sent it to the ether as a sound**. And, as there are many possibilities to convert data to sound, consequently, many different senders may exist. What is also important, that **for every sender created by us exist matching receiver**, which is able to receive sound from the sender and convert it back to original form.



Below there are more details about how it works under the hood.


<br>

### ISender

- - -

ISender is an interface which represents all possible sender implementations. It offers methods which allow to start sending stream, to send out bytes, and to wait until sending finishes.


<br>

### Existing senders

- - -

EchoConnect provides two distinct sender implementations, both of which look at given data as a stream of single bits and encode every bit one after another. They take data, encode it to pcm/audio format and play this sound through speakers.


#### BitSender

To use BitSender we need to specify two distinct frequencies (they should be in the decent distance) - one would be used for encoding bits with value 1, the other one for bits with value 0. With that, bits are encoded one after another and corresponding sounds are played through speakers for the same length of time.


#### BitSenderv2

BitSenderv2 is nearly the same as its precursor. The difference is that it uses the existence of one specified frequency in sound to encode data. Although, because it uses silence to encoding, it needed some additions, which are explained below.

If we want to encode a message with an F-frequency and S-silence. Let's assume without loss of generality that we send 0 bit as S and 1 bit as F, message to send is 0000. In that case there is no sound, so the receiver doesn't even know that something is being transmitted. Because of that, there must be additional not silent header for every message to mark the beggining of transmission.

Now we know that some data is sent out and we receive and decode it, but apparently no receiver could distinguish between 100 and 10000 messages. It either needs to know the length of the message or receive a special terminating sequence marking the end of the transmission. As it is much easier to learn about message length than

So to summarize, BitSenderv2 is the same as BiTSender, but:
- it uses just one frequency and checks whether it is played or not
- every message has a special header added
- a receiver needs to know the length of the message to receive it correctly


<br>

### Notes

- - -

- Although it is possible to encode data more efficiently (so that one byte could take up less disk space when encoded into sound) by using more sound frequencies, we are pretty sure that for now, with current hardware possibilities it would not work at all.


<br>

### Implementation

- - -

Current headers and implementation of ISender, BitSender and BitSenderv2 are available to read in [EchoConnect](https://github.com/Lorak-mmk/EchoConnect) project repository. Links are listed below:

- [ISender.h](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/src/ISender.h)
- [BitSender.h](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/src/BitSender.h)
- [BitSender.cc](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/src/BitSender.cc)
- [BitSenderv2.h](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/src/BitSenderv2.h)
- [BitSenderv2.cc](https://github.com/Lorak-mmk/EchoConnect/blob/master/libecho/src/BitSenderv2.cc)

