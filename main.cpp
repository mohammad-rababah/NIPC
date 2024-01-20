#include "NIPC.h"
#include "NIPCManeger.h"
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
# include <iostream>

int main() {
    NIPCManeger nipcManeger = NIPCManeger();
    while (true) {
        sleep(2);
        std::cout << "\n\n\nMenu:\n";
        std::cout << "1. Create a new NIPC\n";
        std::cout << "2. open NIPC\n";
        std::cout << "3. print all NIPC\n";
        std::cout << "4. send broadcast message\n";
        std::cout << "5. send unicast message\n";
        std::cout << "6. subscribe NIPC to topic\n";
        std::cout << "7. send  message from NIPC to topic\n";
        std::cout << "8. close NIPC\n";
        std::cout << "9. remove NIPC\n";
        std::cout << "10. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                // Create a new NIPC instance
                std::cout << "Enter the key: ";
                key_t key;
                std::cin >> key;
                nipcManeger.nipc_create(key);
                break;
            }
            case 2: {
                std::cout << "Enter the key: ";
                key_t key;
                std::cin >> key;
                int id = nipcManeger.nipc_get(key);
                if (id == -1) {
                    std::cout << "NIPC with key " << key << " does not exist.\n";
                    break;
                }

                nipcManeger.nipc_open(id);
                break;
            }
            case 3: {
                nipcManeger.print_all_nipc();
                break;
            }

            case 4: {
                std::cout << "Enter the sender key: ";
                key_t key;
                std::cin >> key;
                int id = nipcManeger.nipc_get(key);
                if (id == -1) {
                    std::cout << "NIPC with key " << key << " does not exist.\n";
                    break;
                }

                std::cout << "Enter the message: ";
                std::string message;
                std::cin >> message;
                struct nipc_message msg;
                strcpy(msg.messageData, message.c_str());
                nipcManeger.nipc_send(id, 0, msg);
                break;
            }
            case 5: {
                std::cout << "Enter the sender key: ";
                key_t key;
                std::cin >> key;
                int sender_id = nipcManeger.nipc_get(key);
                if (sender_id == -1) {
                    std::cout << "NIPC with key " << key << " does not exist.\n";
                    break;
                }
                std::cout << "Enter the receiver key: ";
                std::cin >> key;
                int receiver_id = nipcManeger.nipc_get(key);
                if (receiver_id == -1) {
                    std::cout << "NIPC with key " << key << " does not exist.\n";
                    break;
                }
                std::cout << "Enter the message: ";
                std::string message;
                std::cin >> message;
                struct nipc_message msg;
                strcpy(msg.messageData, message.c_str());
                nipcManeger.nipc_send(sender_id, receiver_id, msg);
                break;
            }
            case 6: {
                std::cout << "Enter the subscriber key: ";
                key_t key;
                std::cin >> key;
                int subscriber_id = nipcManeger.nipc_get(key);
                if (subscriber_id == -1) {
                    std::cout << "NIPC with key " << key << " does not exist.\n";
                    break;
                }
                std::cout << "Enter the topic: ";
                long topic;
                std::cin >> topic;
                nipcManeger.nipc_subscribe(subscriber_id, topic, [](struct nipc_message *msg) {
                    printf("Received a message:\t%s\n", msg->messageData);
                });
                break;
            }
            case 7: {
                std::cout << "Enter the sender key: ";
                key_t key;
                std::cin >> key;
                int sender_id = nipcManeger.nipc_get(key);
                if (sender_id == -1) {
                    std::cout << "NIPC with key " << key << " does not exist.\n";
                    break;
                }
                std::cout << "Enter the topic: ";
                long topic;
                std::cin >> topic;
                std::cout << "Enter the message: ";
                std::string message;
                std::cin >> message;
                struct nipc_message msg;
                strcpy(msg.messageData, message.c_str());
                nipcManeger.nipc_send(sender_id, topic, msg);
                break;
            }
            case 8: {
                std::cout << "Enter the key: ";
                key_t key;
                std::cin >> key;

                nipcManeger.nipc_close(key);
                break;
            }
            case 9: {
                std::cout << "Enter the key: ";
                key_t key;
                std::cin >> key;
                nipcManeger.nipc_remove(key);
                break;
            }
            case 10:

                return 0;
            default: {
                std::cout << "Invalid choice. Please enter a valid option.\n";
                break;
            }

        }
    }

}
