#include "Packet.h"
#include <cstring>
#include <cstdio>

int main(int argc, char *argv[]) {
    std::vector<uint8_t> bytes1 = PacketBuilder().setFlag(Flag::SYN).getPacket().toBytes();
    std::vector<uint8_t> bytes2 = PacketBuilder().getPacket().toBytes();
    for (size_t i =0; i<bytes1.size(); ++i) printf("%d ", bytes1[i]);
    puts("");
    for (size_t i =0; i<bytes2.size(); ++i) printf("%d ", bytes2[i]);
}
