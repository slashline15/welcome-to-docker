# Use an official Python runtime as a parent image
FROM python:3.12-slim

# Set a working directory in the container
WORKDIR /the/workdir/path

# Copy the current directory contents into the container at /app
COPY . /app

# Install any needed packages specified in requirements.txt

RUN pip install --no-cache-dir -r requirements.txt

# Make port 80 available to the world outside this container
EXPOSE 80

# Define environment variables
ENV NAME world

# Run app.py when container launches
CMD ["python", "app.py"]


# build the docker image
# docker buildx build -t my-python-app
# docker buildx run -p 4000:80 my-python-app