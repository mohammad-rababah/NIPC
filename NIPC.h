#ifndef NIPC_H
#define NIPC_H

#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <map>
#include "configs.h"

#define FIFO_NAME "/tmp/my_fifo"

// Define the structure for the message

struct nipc_message {
    int msg_type;
    char messageData[MESSAGE_SIZE];
};

class NIPC {
public:
    NIPC(key_t key);

    ~NIPC();

    key_t getKey();

    int send(struct nipc_message msg, int reciver_id);

    int closeNIPC();

    int openNIPC();

    void receive_msg();

    bool isOpen();


    int msgid;
private:
    key_t key;
    std::vector<key_t> topics;

    bool is_open = false;

};

#endif // NIPC_H
