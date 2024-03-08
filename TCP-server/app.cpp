#include <iostream>

//authentication library connection
#include "UserAuthentication.h"

//file handeling lib connection
#include "FileHandeling.h"

//tcp server library 
#include "TCPserver.h"

int main() {

    TCPserver server(8080);
    server.startAcceptingConnections();

    return 0;
}
