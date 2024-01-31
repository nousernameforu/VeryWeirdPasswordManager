# Dockerfile
FROM ubuntu:latest

# Update package list and install necessary packages
RUN apt-get update && \
    apt-get install -y g++ && \
    rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Copy the C++ code into the container
COPY server.cpp .

# Compile the C++ code
RUN g++ -o server server.cpp

# Expose the port
EXPOSE 8080

# Command to run the server
CMD ["./server"]

