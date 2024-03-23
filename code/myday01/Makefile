server:
	g++ src/util.cpp client.cpp -o client -g && \
	g++ src/util.cpp server.cpp src/Epoll1.cpp src/InetAddress.cpp src/Socket.cpp src/Channel.cpp src/EventLoop.cpp src/Server.cpp src/Acceptor.cpp src/Connection.cpp -o server -g
clean:
	rm server && rm client