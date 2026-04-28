#include "Network.h"
#include <iostream>

Network::Network() 
    : host(nullptr), peer(nullptr), isConnected(false), onReceive(nullptr) {}

Network::~Network() {
    Disconnect();
    if (host) enet_host_destroy(host);
    enet_deinitialize();
}

// ==================== 【主机初始化】 ====================
bool Network::InitHost(int port) {
    // 1. 初始化 ENet
    if (enet_initialize() != 0) {
        std::cout << "ENet 初始化失败！" << std::endl;
        return false;
    }

    // 2. 创建主机，绑定端口
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;

    host = enet_host_create(
        &address,   // 地址
        1,         // 最大连接数（双人联机=1）
        1,         // 通道数
        0, 0       // 不限速
    );

    if (!host) {
        std::cout << "主机创建失败！" << std::endl;
        return false;
    }

    std::cout << "主机已启动，等待客户端连接... 端口：" << port << std::endl;
    return true;
}

// ==================== 【客户端初始化】 ====================
bool Network::InitClient(const std::string& ip, int port) {
    // 1. 初始化 ENet
    if (enet_initialize() != 0) {
        std::cout << "ENet 初始化失败！" << std::endl;
        return false;
    }

    // 2. 创建客户端主机
    host = enet_host_create(nullptr, 1, 1, 0, 0);
    if (!host) {
        std::cout << "客户端创建失败！" << std::endl;
        return false;
    }

    // 3. 解析服务器地址并连接
    ENetAddress address;
    enet_address_set_host(&address, ip.c_str());
    address.port = port;

    peer = enet_host_connect(host, &address, 1, 0);
    if (!peer) {
        std::cout << "连接请求失败！" << std::endl;
        return false;
    }

    std::cout << "正在连接主机：" << ip << ":" << port << " ..." << std::endl;
    return true;
}

// ==================== 【每帧更新：处理连接、收包、断开】 ====================
void Network::Update() {
    if (!host) return;

    ENetEvent event;
    while (enet_host_service(host, &event, 0) > 0) {
        switch (event.type) {
            // 连接成功
            case ENET_EVENT_TYPE_CONNECT:
                isConnected = true;
                peer = event.peer;
                std::cout << "✅ 连接成功！" << std::endl;
                break;

            // 收到数据
            case ENET_EVENT_TYPE_RECEIVE:
                if (onReceive) {
                    onReceive(event.packet->data, event.packet->dataLength);
                }
                enet_packet_destroy(event.packet);
                break;

            // 断开连接
            case ENET_EVENT_TYPE_DISCONNECT:
                isConnected = false;
                std::cout << "❌ 连接断开" << std::endl;
                peer = nullptr;
                break;

            default: break;
        }
    }
}

// ==================== 【发送数据包】 ====================
void Network::SendData(const void* data, size_t size) {
    if (!isConnected || !peer) return;

    ENetPacket* packet = enet_packet_create(
        data, size, 
        ENET_PACKET_FLAG_RELIABLE // 可靠发送
    );
    enet_peer_send(peer, 0, packet);
}

// ==================== 设置接收回调 ====================
void Network::SetOnReceive(void (*callback)(const void*, size_t)) {
    onReceive = callback;
}

bool Network::IsConnected() const { return isConnected; }

void Network::Disconnect() {
    if (peer && isConnected) {
        enet_peer_disconnect(peer, 0);
        enet_host_flush(host);
    }
    isConnected = false;
}