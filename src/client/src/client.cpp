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

#define MYPORT 8080
#define BUF_SIZE 1024

const char * SERVER_IP = "127.0.0.1";

using namespace std;

class TcpClientNode : public rclcpp::Node
{
public:
  TcpClientNode() : Node("tcp_client")
  {
    // 创建套接字
    socket_cli = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_cli < 0)
    {
      RCLCPP_ERROR(this->get_logger(), "socket() error");
      exit(EXIT_FAILURE);
    }

    // 初始化服务器地址信息
    memset(&sev_addr, 0, sizeof(sev_addr));
    sev_addr.sin_family = AF_INET;
    sev_addr.sin_port = htons(MYPORT);
    sev_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 连接服务器
    RCLCPP_INFO(this->get_logger(), "Connecting...");
    if (connect(socket_cli, (struct sockaddr *)&sev_addr, sizeof(sev_addr)) < 0)
    {
      RCLCPP_ERROR(this->get_logger(), "Connect error");
      exit(EXIT_FAILURE);
    }
    else
    {
      RCLCPP_INFO(this->get_logger(), "Connected successfully!");
    }
  }

  void sendMessages()
  {
    char msgbuf[1024] = "你好，我是客户端!";
    while (rclcpp::ok())
    {
      send(socket_cli, msgbuf, sizeof(msgbuf), 0);
      this_thread::sleep_for(chrono::seconds(1));  // 每秒发送一次消息
    }
  }

  void receiveMessages()
  {
    char recvbuf[BUF_SIZE];
    while (rclcpp::ok())
    {
      ssize_t n = recv(socket_cli, recvbuf, sizeof(recvbuf), 0);
      if (n <= 0)
      {
        break;
      }
      // 对收到的消息进行处理
      string receivedMessage(recvbuf);
      string processedMessage = receivedMessage + "0000000";

      // 发送处理后的消息回服务端
      // send(socket_cli, processedMessage.c_str(), processedMessage.length(), 0);
      // this_thread::sleep_for(chrono::seconds(1));
      RCLCPP_INFO(this->get_logger(), "Server message: %s", recvbuf);
    }
  }

  void start()
  {
    // 创建发送和接收消息的线程
    thread sendThread(&TcpClientNode::sendMessages, this);
    thread receiveThread(&TcpClientNode::receiveMessages, this);

    // 分离线程，让它们在后台运行
    sendThread.detach();
    receiveThread.detach();
  }

private:
  int socket_cli;
  struct sockaddr_in sev_addr;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TcpClientNode>();

  // 启动发送和接收线程
  node->start();

  // 保持主线程运行，让 ROS2 节点保持活跃
  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}
