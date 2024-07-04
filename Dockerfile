# Start your image with a node base image
FROM node:18-alpine

# The /app directory should act as the main application directory
WORKDIR /app
docker pull nvidia/cuda:11.2.2-devel-ubuntu20.04


# Copy the app package and package-lock.json file
COPY package*.json ./

# Copy local directories to the current local directory of our docker image (/app)
COPY ./src ./src
COPY ./public ./public

# Install node packages, install serve, build the app, and remove dependencies at the end
RUN npm install \
    && npm install -g serve \
    && npm run build \
    && rm -fr node_modules

FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/app

COPY mouse_trace.c .

CMD gcc -o mouse_trace mouse_trace.c && ./mouse_traceL
EXPOSE 3000
FROM python:3.9-slim

WORKDIR /app

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY touchpad_monitor.py .

CMD ["python", "touchpad_monitor.py"]
# Start the app using serve command
CMD [ "serve", "-s", "build" ]
