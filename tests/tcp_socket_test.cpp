#define CATCH_CONFIG_MAIN

#include "../src/catch2/catch.hpp"
#include "../src/tcp_client.h"
#include "../src/tcp_server.h"

const int PORT = 51000;

TcpServer *server;
TcpClient *client;
TcpClient *serverClient;

LogSettings LOG_SETTINGS;

//  THIS TEST CASE MUST BE FIRST
TEST_CASE("Initialise Logger", "[single-file]")
{
    //  TODO: Consider changing this for production
    LOG_SETTINGS.ls_print_to_file = false;
    LOG_SETTINGS.ls_selected_level = LT_INFO;
    TestLogInit;
    clog << "Logger initialised";
}

TEST_CASE("Initialising server object", "[single-file]")
{
    try { 
        server = new TcpServer();

        REQUIRE(server->GetSocketFd() > -1);

        REQUIRE(server->StartListening(PORT));
    } 
    catch (std::runtime_error &err){
        REQUIRE(false);
    }
}

TEST_CASE("Initialising socket object", "[single-file]")
{
    try {
        client = new TcpClient();
        REQUIRE(client->GetSocketFd() > -1);
    }
    catch (std::runtime_error &err){
        REQUIRE(false);
    }
}

TEST_CASE("Connect client to server", "[single-file]")
{
    REQUIRE(client->Connect(PORT));

    REQUIRE(client->GetServerFd() > -1);

    serverClient = new TcpClient(server->NextConnection());

    REQUIRE(serverClient->GetSocketFd() > -1);
}

TEST_CASE("Send data across a server and socket object", "[single=file]")
{
    std::string client_msg = "Message coming from client side";
    int client_msg_size = strlen(client_msg.c_str());
    std::string server_msg = "A message was received from the client and this is the reply";
    int server_msg_size = strlen(server_msg.c_str());

    REQUIRE(client->Send(client_msg.c_str()) == client_msg_size);

    usleep(1000000);

    char output_buffer[1000];

    REQUIRE(serverClient->Read(output_buffer, 1000) == client_msg_size);

    REQUIRE(strcmp(output_buffer, client_msg.c_str()) == 0);

    ////////////////////////////////////////////////////////////////////////////
    
    REQUIRE(serverClient->Send(server_msg.c_str()) == server_msg_size);

    usleep(1000000);

    memset(output_buffer, '\0', 1000);

    REQUIRE(client->Read(output_buffer, 1000) == server_msg_size);

    REQUIRE(strcmp(output_buffer, server_msg.c_str()) == 0);
}
