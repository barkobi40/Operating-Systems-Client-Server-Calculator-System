### Operating Systems - Assignment: Client-Server Calculator System
This project implements a client-server system for performing basic arithmetic operations (addition, subtraction, multiplication, and division) using signal communication and file transfer.

### Overview
The system consists of two main components:

-Server (ex2_srv.c): Acts as a calculator that receives requests from clients, performs the requested arithmetic operation, and sends the result back to the client.

-Client (ex2_client.c): Sends requests to the server with two numbers and an operation code, receives the calculated result from the server, and prints it to the console.

### Communication Protocol
The communication between the client and server is facilitated through signals and a fixed file named srv_to (or to_srv in the instructions). The protocol works as follows:

-The server is started first, and it waits for signals from clients.
-The client creates the srv_to (or to_srv) file, writes its PID, the two numbers, and the operation code into the file, and sends a signal to the server.
-Upon receiving the signal, the server creates a child process to handle the client's request.
-The child process reads the parameters from the srv_to (or to_srv) file, performs the requested calculation, and writes the result to a temporary file named xxxxxx_client_to (or to_client_xxxxxx), where xxxxxx is the client's PID.
-The child process sends a signal to the client, indicating that the calculation is complete.
-The client reads the result from the temporary file, prints it to the console, and terminates.
-The server continues to listen for new client requests until no requests have been received for 60 seconds, at which point it terminates.
Features
-The server can handle multiple client requests concurrently by creating a separate child process for each client.
-The client has a timeout mechanism that terminates the client if no response is received from the server within 30 seconds.
Error handling and resource cleanup are implemented to handle various error conditions and ensure proper termination of processes.
-The assignment instructions include specific requirements for file naming, error handling, and output formatting, which are followed in the implementation.
Usage
-Compile the server and client programs using a C compiler.

Run the server program (ex2_srv.out) in the background and note its Process ID (PID).

Run the client program (ex2_client.out) with the following parameters:

P1: The PID of the server process
P2: The first numeric value for the arithmetic operation
P3: The operation code (-1 for addition, -2 for subtraction, -3 for multiplication, -4 for division)
P4: The second numeric value for the arithmetic operation
Example: ex2_client.out 1234 70 2 30 (Subtracts 30 from 70 using the server with PID 1234)

The client will print the result of the calculation to the console and terminate.

The server will continue running and listening for new client requests until no requests have been received for 60 seconds, at which point it will terminate.

### Files
ex2_srv.c: Source code for the server program
ex2_client.c: Source code for the client program
ex2_srv.out: Compiled server program
ex2_client.out: Compiled client program

