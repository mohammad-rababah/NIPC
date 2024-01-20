#include "NIPC.h"
#include <sys/msg.h>
#include <unistd.h>

NIPC::NIPC(key_t key) {
    this->key = key; //1,2,3
}

NIPC::~NIPC() {
    this->closeNIPC();
}

key_t NIPC::getKey() {
    return this->key;
}


int NIPC::send(struct nipc_message msg, int reciver_id) {
    // Send message to the message queue
    if (msgsnd(reciver_id, &msg, MESSAGE_SIZE, 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int NIPC::closeNIPC() {
    this->is_open = false;
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    printf("NIPC %d\tClosed", this->key);
    return 0;
}

void NIPC::receive_msg() {
    struct nipc_message receivedMsg{};


    while (this->isOpen()) {
        sleep(1);
        if (msgrcv(msgid, (void *) &receivedMsg, sizeof(receivedMsg.messageData), 0, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);

            // ENOMSG means no message available, which is expected if the queue is empty
            // You may want to handle this case differently, depending on your logic.
        } else {
            printf("NIPC %d\tReceived a message:\t%s\n", this->key, receivedMsg.messageData);
        }
    }
    printf("NIPC %d\tClosed\n", this->key);
}

int NIPC::openNIPC() {
    key_t keyT = ftok("/tmp", this->key);
    this->msgid = msgget(keyT, 0666 | IPC_CREAT); // queue id

    if (this->msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    is_open = true;

    printf("open NIPC %d: with Message queue id: %d\n", this->key, this->msgid);
    return 0;
}

bool NIPC::isOpen() {
    return this->is_open;
}