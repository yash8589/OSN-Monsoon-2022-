#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

////////////////
#include <bits/stdc++.h>
using namespace std;
// Regular bold text
#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define ANSI_RESET "\x1b[0m"

typedef long long LL;

#define pb push_back
#define debug(x) cout << #x << " : " << x << endl
#define part cout << "-----------------------------------" << endl;

///////////////////////////////
#define MAX_CLIENTS 4
#define PORT_ARG 8001

const int initial_msg_len = 256;
int n;
int m;

////////////////////////////////////

const LL buff_sz = 1048576;
/////////////////////////////////////////////////////////////////////////////////////////////////

struct routing_table_entry
{
    int dest;
    int hop;
    int cost;
};

struct message
{
    int type;
    int dest;
    int source;
    int prev_delay;
    string s;
    vector<struct routing_table_entry> routing_table;
};

vector<vector<struct routing_table_entry>> routing_table_vector;
vector<vector<pair<int, int>>> adj_list;
vector<queue<message>> message_queue;
vector<sem_t> queue_sem;
vector<pthread_mutex_t> queue_mutex;

void *node_func(void *arg)
{
    int node = *(int *)arg;

    // cout << "Node " << node << " started" << endl;
    for (int i = 0; i < adj_list[node].size(); i++)
    {
        struct message msg = { 0, adj_list[node][i].first, node, adj_list[node][i].second , "", routing_table_vector[node] };

        vector<struct routing_table_entry> routing_table = routing_table_vector[node];

        pthread_mutex_lock(&queue_mutex[msg.dest]);
        message_queue[msg.dest].push(msg);
        sem_post(&queue_sem[msg.dest]);
        pthread_mutex_unlock(&queue_mutex[msg.dest]);

        while (1)
        {
            sem_wait(&queue_sem[node]);
            pthread_mutex_lock(&queue_mutex[node]);
            struct message msg = message_queue[node].front();
            message_queue[node].pop();
            pthread_mutex_unlock(&queue_mutex[node]);

            if (msg.type == 0)
            {
                int changed = 0;
                for (int i = 0; i < n; i++)
                {
                    if (routing_table_vector[node][i].cost > msg.routing_table[i].cost + msg.prev_delay)
                    {
                        routing_table_vector[node][i].cost = msg.routing_table[i].cost + msg.prev_delay;
                        routing_table_vector[node][i].hop = msg.source;
                        changed = 1;
                    }
                }

                if (changed)
                {
                    for (auto it : adj_list[node])
                    {
                        struct message msg = { 0, it.first, node, it.second, "", routing_table_vector[node] };

                        pthread_mutex_lock(&queue_mutex[msg.dest]);
                        message_queue[msg.dest].push(msg);
                        sem_post(&queue_sem[msg.dest]);
                        pthread_mutex_unlock(&queue_mutex[msg.dest]);
                    }
                }
            }
        }
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

pair<string, int> read_string_from_socket(const int &fd, int bytes)
{
    std::string output;
    output.resize(bytes);

    int bytes_received = read(fd, &output[0], bytes - 1);
    debug(bytes_received);
    if (bytes_received <= 0)
    {
        cerr << "Failed to read data from socket. \n";
    }

    output[bytes_received] = 0;
    output.resize(bytes_received);
    // debug(output);
    return {output, bytes_received};
}

int send_string_on_socket(int fd, const string &s)
{
    // debug(s.length());
    int bytes_sent = write(fd, s.c_str(), s.length());
    if (bytes_sent < 0)
    {
        cerr << "Failed to SEND DATA via socket.\n";
    }

    return bytes_sent;
}

///////////////////////////////

void handle_connection(int client_socket_fd)
{
    // int client_socket_fd = *((int *)client_socket_fd_ptr);
    //####################################################

    int received_num, sent_num;

    /* read message from client */
    int ret_val = 1;

    while (true)
    {

        string cmd;
        string out = "";
        tie(cmd, received_num) = read_string_from_socket(client_socket_fd, buff_sz);
        ret_val = received_num;
        // debug(ret_val);
        // printf("Read something\n");
        if (ret_val <= 0)
        {
            // perror("Error read()");
            printf("Server could not read msg sent from client\n");
            goto close_client_socket_ceremony;
        }
        cout << "Client sent : " << cmd << endl;
        if (cmd == "exit")
        {
            cout << "Exit pressed by client" << endl;
            goto close_client_socket_ceremony;
        }
        else if (cmd == "pt")
        {
            out = "\ndest\tforw\tdelay\n";

            for (int i = 1; i < n; i++)
            {
                out += to_string(i) + "\t" + to_string(routing_table_vector[0][i].hop) + "\t" + to_string(routing_table_vector[0][i].cost) + "\n";
            }
        }
        string msg_to_send_back = "Ack: " + cmd + out;

        ////////////////////////////////////////
        // "If the server write a message on the socket and then close it before the client's read. Will the client be able to read the message?"
        // Yes. The client will get the data that was sent before the FIN packet that closes the socket.

        int sent_to_client = send_string_on_socket(client_socket_fd, msg_to_send_back);
        // debug(sent_to_client);
        if (sent_to_client == -1)
        {
            perror("Error while writing to client. Seems socket has been closed");
            goto close_client_socket_ceremony;
        }
    }

close_client_socket_ceremony:
    close(client_socket_fd);
    printf(BRED "Disconnected from client" ANSI_RESET "\n");
    // return NULL;
}

int main(int argc, char *argv[])
{

    int i, j, k, t;
    cin >> n;
    cin >> m;
    message_queue.resize(n);
    queue_sem.resize(n);
    queue_mutex.resize(n);
    adj_list.resize(n);

    routing_table_vector.resize(n);
    for (i = 0; i < n; i++)
    {
        sem_init(&queue_sem[i], 0, 0);
        pthread_mutex_init(&queue_mutex[i], NULL);
        routing_table_vector[i].resize(n);
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            routing_table_vector[i][j].cost = 1e9;

            routing_table_vector[i][j].hop = j;

            routing_table_vector[i][j].dest = j;
        }
    }

    for (int i = 0; i < m; i++)
    {
        int a, b, c;
        cin >> a;
        cin >> b;
        cin >> c;
        routing_table_vector[b][a].cost = c;
        routing_table_vector[b][a].hop = a;
        routing_table_vector[b][a].dest = a;

        routing_table_vector[a][b].cost = c;
        routing_table_vector[a][b].hop = b;
        routing_table_vector[a][b].dest = b;

        adj_list[a].pb({b, c});
        adj_list[b].pb({a, c});
    }

    int node_ids[n];
    pthread_t nodes[n];

    for (int i = 0; i < n; i++)
    {
        node_ids[i] = i;
        pthread_create(&nodes[i], NULL, node_func, (void *)&node_ids[i]);
    }

    int wel_socket_fd, client_socket_fd, port_number;
    socklen_t clilen;

    struct sockaddr_in serv_addr_obj, client_addr_obj;
    /////////////////////////////////////////////////////////////////////////
    /* create socket */
    /*
    The server program must have a special door—more precisely,
    a special socket—that welcomes some initial contact
    from a client process running on an arbitrary host
    */
    // get welcoming socket
    // get ip,port
    /////////////////////////
    wel_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (wel_socket_fd < 0)
    {
        perror("ERROR creating welcoming socket");
        exit(-1);
    }

    //////////////////////////////////////////////////////////////////////
    /* IP address can be anything (INADDR_ANY) */
    bzero((char *)&serv_addr_obj, sizeof(serv_addr_obj));
    port_number = PORT_ARG;
    serv_addr_obj.sin_family = AF_INET;
    // On the server side I understand that INADDR_ANY will bind the port to all available interfaces,
    serv_addr_obj.sin_addr.s_addr = INADDR_ANY;
    serv_addr_obj.sin_port = htons(port_number); // process specifies port

    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    /* bind socket to this port number on this machine */
    /*When a socket is created with socket(2), it exists in a name space
       (address family) but has no address assigned to it.  bind() assigns
       the address specified by addr to the socket referred to by the file
       descriptor wel_sock_fd.  addrlen specifies the size, in bytes, of the
       address structure pointed to by addr.  */

    // CHECK WHY THE CASTING IS REQUIRED
    if (bind(wel_socket_fd, (struct sockaddr *)&serv_addr_obj, sizeof(serv_addr_obj)) < 0)
    {
        perror("Error on bind on welcome socket: ");
        exit(-1);
    }
    //////////////////////////////////////////////////////////////////////////////////////

    /* listen for incoming connection requests */

    listen(wel_socket_fd, MAX_CLIENTS);
    cout << "Server has started listening on the LISTEN PORT" << endl;
    clilen = sizeof(client_addr_obj);

    while (1)
    {
        /* accept a new request, create a client_socket_fd */
        /*
        During the three-way handshake, the client process knocks on the welcoming door
of the server process. When the server “hears” the knocking, it creates a new door—
more precisely, a new socket that is dedicated to that particular client.
        */
        // accept is a blocking call
        printf("Waiting for a new client to request for a connection\n");
        client_socket_fd = accept(wel_socket_fd, (struct sockaddr *)&client_addr_obj, &clilen);
        if (client_socket_fd < 0)
        {
            perror("ERROR while accept() system call occurred in SERVER");
            exit(-1);
        }

        printf(BGRN "New client connected from port number %d and IP %s \n" ANSI_RESET, ntohs(client_addr_obj.sin_port), inet_ntoa(client_addr_obj.sin_addr));

        handle_connection(client_socket_fd);
    }

    close(wel_socket_fd);
    return 0;
}