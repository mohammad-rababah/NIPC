#include <sys/stat.h>
#include <unistd.h>
#include <sys/msg.h>
#include "NIPCManeger.h"
# include "NIPC.h"

int NIPCManeger::nipc_create(key_t _key) {
    // check if the key is already exist
    if (this->nipc_array.size() == MAX_NIPC) {
        printf("unable to create more than %d NIPC\n", MAX_NIPC);
        return -1;
    }

    if (nipc_get(_key) != -1) {
        printf("NIPC with key %d already exist\n", _key);
        return -1;
    }
    // add object to the array and keep it in memory
    NIPC *nipc = new NIPC(_key);

    // Store the pointer in the vector
    nipc_array.push_back(nipc);


    return 0;
}


int NIPCManeger::nipc_get(key_t _key) {
    for (int i = 0; i < this->nipc_array.size(); i++) {
        if (nipc_array[i]->getKey() == _key) {
            return i;
        }
    }
    return -1;
}

int NIPCManeger::nipc_remove(key_t _key) {
    int id = nipc_get(_key);
    if (id == -1) {
        printf("NIPC with key %d does not exist\n", _key);
        return -1;
    }
    if (this->nipc_array[id]->isOpen()) {
        this->nipc_array[id]->closeNIPC();
    }
    this->nipc_array.erase(this->nipc_array.begin() + id);
    printf("NIPC with key %d is removed\n", _key);
    return 0;
}


int NIPCManeger::nipc_subscribe(int id, long type) { // key , id(index) --> nipc
    // check if type exist in the map
    if (type > 0) {
        printf("type must be negative\n");
        return -1;
    }
    if (subscribers.count(type) == 0) { // topic is not exist
        this->subscribers[type] = std::vector<NIPC *>();
    }
    this->subscribers[type].push_back(this->nipc_array[id]);
    return 0;
}

int NIPCManeger::nipc_send(int id, long type, struct nipc_message msg) { // 100 -->0 , 200 --> 1 , 300 -->2

    if (type > 0) {
        int receiver_id = nipc_get(type); // ---> 300 -->2
        if (receiver_id == -1) {
            printf("receiver NIPC with id %d does not exist\n", type);
            return -1;
        }
        printf("send unicast message\n");
        this->nipc_array[id]->send(msg, nipc_array[receiver_id]->msgid); // queue id
    } else if (type < 0) {
        if (subscribers.count(type) == 0) {
            printf("NIPC with type %ld does not exist\n", type);
            return -1;
        }

        printf("send multicast message\n");

        for (int i = 0; i < subscribers[type].size(); i++) {
            // map ---> (key,value)
            // key ---> long [-1...-inf]
            // value ---> array[NIPC]
            this->nipc_array[id]->send(msg, subscribers[type][i]->msgid);
        }
    } else {
        printf("send broadcast message\n");
        for (int i = 0; i < this->nipc_array.size(); i++) {
            if (i == id) {
                continue;
            }
            this->nipc_array[id]->send(msg, nipc_array[i]->msgid);
        }
    }

}

int NIPCManeger::nipc_close(int key) {
    int id = nipc_get(key);
    if (id == -1) {
        printf("NIPC with key %d does not exist\n", key);
        return -1;
    }
    if (!this->nipc_array[id]->isOpen()) {
        printf("NIPC with key %d is already closed\n", key);
        return -1;
    }
    this->nipc_array[id]->closeNIPC();
    return 0;
}

NIPCManeger::NIPCManeger() {
    this->subscribers = std::map<long, std::vector<NIPC *>>();
    this->nipc_array = std::vector<NIPC *>();
}

NIPCManeger::~NIPCManeger() {
    for (int i = 0; i < this->nipc_array.size(); i++) {
        this->nipc_array[i]->closeNIPC();
    }

}

void NIPCManeger::print_all_nipc() {
    for (int i = 0; i < this->nipc_array.size(); i++) {
        printf("NIPC %d: key = %d\tstatus = %s\n", i, this->nipc_array[i]->getKey(),
               this->nipc_array[i]->isOpen() ? "open" : "close");
    }

}

int NIPCManeger::nipc_open(int id) { // open ---> recive
    if (id == -1) {
        printf("NIPC with key %d does not exist\n", id);
        return -1;
    }
    if (this->nipc_array[id]->isOpen()) {
        printf("NIPC with key %d is already open\n", id);
        return -1;
    }
    this->nipc_array[id]->openNIPC();
    pid_t pid = fork(); // fork
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        this->nipc_array[id]->receive_msg();
    }
    return 0;
}
