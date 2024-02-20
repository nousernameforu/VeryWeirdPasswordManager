# This is a Dockerfile

FROM alpine:latest

RUN apk update && apk --no-cache add g++ make sqlite-dev

# Set the working directory
WORKDIR /app

# Copy the C++ code into the container
COPY server.cpp .

# Compile the C++ code
RUN g++ -o server server.cpp -lsqlite3

# Expose the port
EXPOSE 8080

# Command to run the server
CMD ["./server"]

