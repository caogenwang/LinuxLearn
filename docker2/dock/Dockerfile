FROM prod_centos_hk:latest
RUN mkdir /data
ADD hello.c /data
RUN cd /data && \
    gcc -o hello hello.c && \
    rm hello.c
WORKDIR /data
CMD ["./hello"]