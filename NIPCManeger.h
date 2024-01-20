
#ifndef NIPC_NIPCMANEGER_H
#define NIPC_NIPCMANEGER_H


#include <locale>
#include "NIPC.h"
#include <vector>


# define MAX_NIPC 10

class NIPCManeger {
public:
    NIPCManeger();

    ~NIPCManeger();

    int nipc_create(key_t _key);

    int nipc_get(key_t _key);

    int nipc_subscribe(int id, long type);

    int nipc_send(int id, long type, struct nipc_message msg);

    int nipc_close(int key);

    int nipc_remove(key_t _key);

    void print_all_nipc();

    int nipc_open(int id);

private:
    std::vector<NIPC*> nipc_array;
    std::map<long,std::vector<NIPC*>> subscribers; // subscribers[-100] ---> array NIPC
    int count = 0;

};


#endif //NIPC_NIPCMANEGER_H
