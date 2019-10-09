FROM nvidia/opengl:1.0-glvnd-runtime-ubuntu18.04

RUN export DEBIAN_FRONTEND=noninteractive; \
    apt-get update && \
    apt-get install -y build-essential && \
    apt-get install -y pkg-config && \
    apt-get install -y git && \
    apt-get install -y wget && \
    apt-get install -y curl && \
    apt-get install -y sudo && \
    apt-get install -y ruby && \
    apt-get install -y ruby-dev && \
    apt-get install -y locales && \
    apt-get install -y tzdata && \
    apt-get install -y bash-completion && \
    curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | bash && \
    apt-get install -y git-lfs  && \
    apt-get clean && \
    rm -rf /tmp/* /var/tmp/*

RUN export DEBIAN_FRONTEND=noninteractive; \
    apt-get update && \
    apt-get -y install build-essential cmake pkg-config && \
    apt-get -y install libqt4-dev && \
    apt-get -y install libqt4-opengl-dev && \
    apt-get -y install python-dev python-numpy && \
    apt-get -y install python-scipy && \
    apt-get -y install libpng-dev libpng++-dev && \
    apt-get -y install libpnglite-dev && \
    apt-get -y install zlib1g zlib1g-dbg zlib1g-dev && \
    apt-get -y install pngtools && \
    apt-get clean && \
    rm -rf /tmp/* /var/tmp/*

RUN export DEBIAN_FRONTEND=noninteractive; \
    apt-get update && \
    apt-get -y install software-properties-common && \
    apt-get clean && \
    rm -rf /tmp/* /var/tmp/*

RUN export DEBIAN_FRONTEND=noninteractive; \
    add-apt-repository "deb http://security.ubuntu.com/ubuntu xenial-security main" && \
    apt-get update && \
    apt-get -y install libjasper1 libjasper-dev && \
    apt-get -y install libjpeg-dev libjpeg-progs && \
    apt-get -y install libtiff-dev libtiff-tools && \
    apt-get -y install openexr libopenexr-dev && \
    apt-get -y install libavcodec-dev libavformat-dev libavutil-dev && \
    apt-get -y install libswscale-dev && \
    apt-get -y install libatlas-base-dev libv4l-dev libx264-dev libxvidcore-dev && \
    apt-get -y install libunicap-dev && \
    apt-get -y install libdc1394-22 libdc1394-22-dev libdc1394-utils && \
    apt-get -y install libusb-dev freeglut3-dev && \
    apt-get clean && \
    rm -rf /tmp/* /var/tmp/*

RUN export DEBIAN_FRONTEND=noninteractive; \
    apt-get update && \
    apt-get install -y nano && \
    apt-get install -y vim && \
    apt-get install -y terminator && \
    apt-get install -y man-db && \
    apt-get install -y manpages && \
    curl https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > microsoft.gpg && \
    install -o root -g root -m 644 microsoft.gpg /etc/apt/trusted.gpg.d/ && \
    echo "deb [arch=amd64] https://packages.microsoft.com/repos/vscode stable main" | tee /etc/apt/sources.list.d/vscode.list && \
    apt-get update && \
    apt-get install -y code  && \
    apt-get clean && \
    rm -rf /tmp/* /var/tmp/* /microsoft.gpg

RUN export LANGUAGE=en_US.UTF-8; \
    export LANG=en_US.UTF-8; \
    export LC_ALL=en_US.UTF-8; \
    locale-gen en_US.UTF-8; \
    DEBIAN_FRONTEND=noninteractive dpkg-reconfigure locales

# Set locales
ENV LANGUAGE=en_US.UTF-8
ENV LANG=en_US.UTF-8
ENV LC_ALL=en_US.UTF-8
ENV QT_X11_NO_MITSHM=1

RUN useradd -m opencv && \
    echo "opencv:opencv" | chpasswd && \
    usermod --shell /bin/bash opencv && \
    usermod -aG sudo opencv && \
    echo "opencv ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers.d/opencv && \
    chmod 0440 /etc/sudoers.d/opencv

RUN usermod  --uid 1000 opencv && \
    groupmod --gid 1000 opencv; exit 0

RUN mkdir /source
RUN chown -R opencv /source

RUN mkdir /build
RUN chown -R opencv /build

USER opencv

RUN cd /source && \
    git clone --branch 4.0.0 https://github.com/opencv/opencv.git && \
    git clone --branch 4.0.0 https://github.com/opencv/opencv_contrib.git

USER root

RUN export DEBIAN_FRONTEND=noninteractive; \
    apt-get update && \
    apt-get install -y qt5-default  && \
    apt-get clean && \
    rm -rf /tmp/* /var/tmp/* /microsoft.gpg

USER opencv

RUN mkdir /build/opencv4.0

RUN cd /build/opencv4.0 && \
    cmake -D CMAKE_BUILD_TYPE=RELEASE \
	-D CMAKE_INSTALL_PREFIX=/opt/opencv \
	-D INSTALL_C_EXAMPLES=ON \
	-D INSTALL_PYTHON_EXAMPLES=ON \
	-D OPENCV_EXTRA_MODULES_PATH=/source/opencv_contrib/modules \
    -D BUILD_opencv_cvv=OFF \
	-D WITH_TBB=ON \
	-D WITH_V4L=ON \
	-D WITH_QT=ON \
	-D WITH_OPENGL=ON \
	-D WITH_GDAL=ON \
	-D WITH_XINE=ON \
	-D INSTALL_C_EXAMPLES=ON \
	-D BUILD_EXAMPLES=ON /source/opencv

RUN cd /build/opencv4.0 && \
    make -j$(nproc)

USER root
RUN usermod -a -G video opencv
RUN  cd /build/opencv4.0 && make install
RUN /bin/bash -c 'echo "/opt/opencv/lib" > /etc/ld.so.conf.d/opencv.conf' && \
	ldconfig


USER opencv

CMD ["/bin/bash"]