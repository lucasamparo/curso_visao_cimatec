docker run --device=/dev/video0:/dev/video0 -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$DISPLAY -p 5000:5000 -p 8888:8888 --name opencv_class -v ${HOME}/workspace/opencv:/home/opencv -it opencv_class
