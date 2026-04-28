#pragma once
#include <enet/enet.h>
#include <string>

// 网络模块类：封装所有 ENet 功能
class Network {
public:
    Network();
    ~Network();

    // ============= 主机初始化 =============
    bool InitHost(int port = 8888);

    // ============= 客户端初始化 =============
    bool InitClient(const std::string& ip, int port = 8888);

    // ============= 网络更新（必须每帧调用） =============
    void Update();

    // ============= 发送数据 =============
    void SendData(const void* data, size_t size);

    // ============= 接收数据（回调） =============
    void SetOnReceive(void (*callback)(const void*, size_t));

    // ============= 连接状态 =============
    bool IsConnected() const;
    void Disconnect();

private:
    ENetHost* host;       // 主机
    ENetPeer* peer;       // 连接对象
    bool isConnected;     // 是否已连接
    void (*onReceive)(const void*, size_t); // 接收回调
};