#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <rclcpp/rclcpp.hpp>
#include <thread>

#define PORT 8080
#define LOG 10

using namespace std;

class TcpServerNode : public rclcpp::Node
{
public:
  TcpServerNode() : Node("tcp_server")
  {
    // 创建套接字
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
      RCLCPP_ERROR(this->get_logger(), "socket() error");
      exit(EXIT_FAILURE);
    }

    // 初始化服务器套接字地址信息
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // 绑定套接字
    if (bind(listenfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
      RCLCPP_ERROR(this->get_logger(), "bind() error");
      exit(EXIT_FAILURE);
    }

    // 监听套接字
    if (listen(listenfd, LOG) < 0)
    {
      RCLCPP_ERROR(this->get_logger(), "listen() error");
      exit(EXIT_FAILURE);
    }

    RCLCPP_INFO(this->get_logger(), "TCP Server started on port %d", PORT);
  }

  void start()
  {
    // 在后台线程中运行 accept 循环
    thread acceptThread(
      [this]()
      {
        while (rclcpp::ok())
        {
          socklen_t clientlen = sizeof(client);
          connectfd = accept(listenfd, (struct sockaddr *)&client, &clientlen);
          if (connectfd < 0)
          {
            RCLCPP_ERROR(this->get_logger(), "accept() error");
            continue;
          }
          char client_ip[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, &client.sin_addr, client_ip, INET_ADDRSTRLEN);
          RCLCPP_INFO(
            this->get_logger(), "Client connected from IP: %s, port: %d", client_ip,
            ntohs(client.sin_port));

          // 创建新线程处理客户端连接
          thread clientThread(&TcpServerNode::handleClient, this, connectfd);
          clientThread.detach();  // 分离线程，让它在后台运行
        }
      });
    acceptThread.detach();
  }

  void handleClient(int connectfd)
  {
    char msgbuf[1024] = "你好，我是服务端!";
    send(connectfd, msgbuf, sizeof(msgbuf), 0);

    // 接收客户端消息
    char recvbuf[1024];
    while (rclcpp::ok())
    {
      ssize_t n = recv(connectfd, recvbuf, sizeof(recvbuf), 0);
      if (n <= 0)
      {
        break;
      }
      RCLCPP_INFO(this->get_logger(), "Received message from client: %s", recvbuf);
      send(connectfd, msgbuf, sizeof(msgbuf), 0);
    }

    close(connectfd);
  }

private:
  int listenfd, connectfd;
  struct sockaddr_in server;
  struct sockaddr_in client;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TcpServerNode>();

  // 启动服务器接受连接的线程
  node->start();

  // 保持主线程运行，让 ROS2 节点保持活跃
  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}
