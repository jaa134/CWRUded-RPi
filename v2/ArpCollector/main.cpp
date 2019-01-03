#include "packetcollector.h"
#include "arpsender.h"
#include <chrono>
#include <thread>

using namespace std;

void start_collection(PacketCollector *pc) {
    pc->start();
}

void start_sender(ArpSender * as) {
    as->start();
}

int main()
{
    //start collecting packets
    PacketCollector *pc = new PacketCollector();
    thread collection_thread(start_collection, pc);

    //wait for 2 seconds before sending ARP requests
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    //start sending ARP requests
    ArpSender *as = new ArpSender();
    thread sender_thread(start_sender, as);

    //wait for ARP requests sending to finished
    sender_thread.join();

    //wait for 2 seconds for lagging packets
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    //request collection thread to stop
    pc->request_stop();

    //wait for packet collection to finish
    collection_thread.join();

    return 0;
}
