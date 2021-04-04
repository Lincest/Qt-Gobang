import socket
import select


# 创建一个类
class GameServer:
    # 初始化 传入监听端口即可
    def __init__(self, port):
        self.clients_num = 0
        self.the_first = True
        self.port = port
        # 配置参数
        self.srvsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.srvsock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.srvsock.bind(("", port))
        self.srvsock.listen(10)  # 设置监听数量
        # 添加在线服务器
        self.descripors = [self.srvsock]
        print("Server started!")

    # 运行监听方法
    def run(self):
        while True:
            # io复用, select机制
            (sread, swrite, sexc) = select.select(self.descripors, [], [])
            # 读已获取到的申请
            for sock in sread:
                # 判断是否有新的连接
                if sock == self.srvsock:
                    self.accept_new_connection()
                else:
                    # 读接收到的消息
                    str_send = sock.recv(1024).decode("utf-8")  # 读缓冲区大小1024
                    # 检测是否断开连接
                    host, port = sock.getpeername()
                    if str_send == "end" or str_send == "":
                        self.descripors.remove(sock)
                        self.clients_num -= 1
                        sock.close()
                        print("客户端退出")
                        print("当前socket数: " + str(len(self.descripors)))
                        # 只剩下监听进程了
                        if len(self.descripors) == 1:
                            self.the_first = True  # 重置
                            print("客户端已全部断开")
                    else:
                        newstr = str_send
                        self.broadcast_str(newstr, sock)

    # 建立一个新的连接客户机
    def accept_new_connection(self):
        # 接收请求
        newsock, (remhost, remport) = self.srvsock.accept()
        # 添加连接
        self.descripors.append(newsock)
        self.clients_num += 1
        if self.the_first:
            newsock.send("start".encode('utf8'))
            self.the_first = False
            print("已连接一个客户端")
        if self.clients_num == 2:  # 广播游戏开始
            for sock in self.descripors:
                if (sock != self.srvsock):
                    sock.send("ready".encode('utf8'))
                    print("已连接两个客户端, 游戏开始")

    # 广播函数
    def broadcast_str(self, str_send, my_sock):
        # 遍历已有连接
        for sock in self.descripors:
            if sock != self.srvsock and sock != my_sock:
                sock.send(str_send.encode('utf8'))
        print("转发内容:" + str_send)

# 实例化监听服务器
GameServer(6667).run()