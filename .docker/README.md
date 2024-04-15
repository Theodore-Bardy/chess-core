# CHESS-CORE build environment

The Dockerfile defined in this directory is used to generate a docker image with the required environment to build the [chess-core](https://github.com/Theodore-Bardy/chess-core) project.

This build environment is used by the Github CI. For more information check the following link: [CI with Docker](https://docs.docker.com/build/ci/).

## Build docker image

To build a new docker image and make it available on the DockerHub for the CI follow the below steps:

 - Build the docker image:
```Shell
docker build --tag theodorebardy/chess-core-build-env:latest .
```

 - Run in local the docker image:
```Shell
docker run -it --volume [LOCAL_PATH_TO_PROJECT]:/data theodorebardy/chess-core-build-env:latest /bin/bash
# Here a shell which runs the docker image should open
# Move to /data directory and test the dokcer image
root@6ca3f17d31d0:/app# cd /data/
root@6ca3f17d31d0:/data# .github/build.sh 
###################################
--- Build Chess-core repository ---
###################################
Executable name: chess_core.elf
-- Configuring done
-- Generating done
-- Build files have been written to: /data/build-ci
…
[100%] Linking CXX executable chess_core.elf
[100%] Built target chess_core.elf

```

 - Push the docker image to DockerHub:
```Shell
docker login (enter your username)
docker push theodorebardy/chess-core-build-env:latest
docker logout
```
