# This is a Dockerfile

FROM alpine:latest

RUN apk update && apk --no-cache add g++ sqlite-dev libressl-dev

# Copy source code to the app directory
COPY server.cpp UserAuthentication.h UserAuthentication.cpp FileHandeling.h FileHandeling.cpp /app/

# Set the working directory
WORKDIR /app

# Running all compilations needed 
RUN \
# Compile the UserAuthentication lib
g++ -c UserAuthentication.cpp -o UserAuthentication.o -lsqlite3 -lssl -lcrypto && ar rcs libUserAuthentication.a UserAuthentication.o && \
# Compile the FileHandeling lib
g++ -c FileHandeling.cpp -o FileHandeling.o && ar rcs libFileHandeling.a FileHandeling.o && \
# Compile the server
g++ server.cpp -o server -L/app/ -lUserAuthentication -lFileHandeling -lsqlite3 -lssl -lcrypto 

# Expose the port
EXPOSE 8080

# Command to run the server
CMD ["./server"]
