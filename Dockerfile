FROM ubuntu:latest

# Install necessary build tools
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    libx11-dev \
    libxtst-dev

# Copy your source code into the container
COPY mouse_trace.c /app/mouse_trace.c

# Set the working directory
WORKDIR /app

# Compile the code
RUN gcc -o mouse_trace mouse_trace.c -lX11 -lXtst

# Run the compiled program
CMD ["./mouse_trace"]