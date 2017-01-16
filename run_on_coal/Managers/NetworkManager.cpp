#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LuaManager.h"
#include "Managers/NetworkManager.h"
#include "Lua/LuaArguments.h"

unsigned char GetPacketIdentifier(RakNet::Packet *f_packet)
{
    if(f_packet == 0) return 255;
    if((unsigned char)f_packet->data[0] == ID_TIMESTAMP)
    {
        RakAssert(f_packet->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
        return (unsigned char)f_packet->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
    }
    return (unsigned char)f_packet->data[0];
}

namespace ROC
{

const std::string g_networkStateTable[] = { "connected", "disconnected" };

}

ROC::NetworkManager::NetworkManager(Core *f_core)
{
    m_core = f_core;
    m_networkInterface = NULL;
    m_networkState = NetworkState::Disconnected;
    m_argument = new LuaArguments();
}
ROC::NetworkManager::~NetworkManager()
{
    if(m_networkInterface)
    {
        m_networkInterface->Shutdown(ROC_NETWORK_SHUTDOWN_DURATION);
        RakNet::RakPeerInterface::DestroyInstance(m_networkInterface);
    }
    delete m_argument;
}

bool ROC::NetworkManager::Connect(std::string &f_ip, unsigned short f_port)
{
    if(m_networkState == NetworkState::Disconnected)
    {
        m_networkInterface = RakNet::RakPeerInterface::GetInstance();
        if(m_networkInterface->Startup(ROC_NETWORK_MAX_CONNECTIONS, &m_socketDescriptor, 1) == RakNet::StartupResult::RAKNET_STARTED)
        {
            if(m_networkInterface->Connect(f_ip.c_str(), f_port, NULL, 0, NULL, 0, ROC_NETWORK_CONNECTION_TRIES, ROC_NETWORK_CONNECTION_PER_TRY_TIME) == RakNet::CONNECTION_ATTEMPT_STARTED)
            {
                m_networkState = NetworkState::Connecting;
            }
            else
            {
                RakNet::RakPeerInterface::DestroyInstance(m_networkInterface);
                m_networkInterface = NULL;
            }
        }
        else
        {
            RakNet::RakPeerInterface::DestroyInstance(m_networkInterface);
            m_networkInterface = NULL;
        }
    }
    return (m_networkState == NetworkState::Connecting);
}
bool ROC::NetworkManager::Disconnect()
{
    if(m_networkState == NetworkState::Connected)
    {
        m_networkInterface->CloseConnection(m_serverAddress, true);
        m_networkState = NetworkState::Disconnecting;
    }
    return (m_networkState == NetworkState::Disconnecting);
}
bool ROC::NetworkManager::SendData(std::string &f_data)
{
    if(m_networkState == NetworkState::Connected)
    {
        RakNet::BitStream l_data;
        l_data.Write(static_cast<unsigned char>(ID_ROC_DATA_PACKET));
        l_data.Write(static_cast<unsigned int>(f_data.size()));
        l_data.Write(f_data.data(),f_data.size());
        m_networkInterface->Send(&l_data, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, m_serverAddress, false);
    }
    return (m_networkState == NetworkState::Connected);
}

void ROC::NetworkManager::DoPulse()
{
    if(m_networkInterface)
    {
        RakNet::Packet *l_packet;
        for(l_packet = m_networkInterface->Receive(); l_packet; m_networkInterface->DeallocatePacket(l_packet), l_packet = m_networkInterface->Receive())
        {
            switch(GetPacketIdentifier(l_packet))
            {
                case ID_DISCONNECTION_NOTIFICATION: case ID_INCOMPATIBLE_PROTOCOL_VERSION: case ID_CONNECTION_BANNED: case ID_CONNECTION_ATTEMPT_FAILED: case ID_NO_FREE_INCOMING_CONNECTIONS: case ID_CONNECTION_LOST:
                {
                    m_networkState = NetworkState::Disconnected;
                    m_argument->PushArgument(const_cast<std::string*>(&g_networkStateTable[1]));
                    m_core->GetLuaManager()->GetEventManager()->CallEvent(EventType::NetworkStateChange, m_argument);
                    m_argument->Clear();
                } break;
                case ID_CONNECTION_REQUEST_ACCEPTED:
                {
                    m_serverAddress = l_packet->systemAddress;
                    m_networkState = NetworkState::Connected;
                    m_argument->PushArgument(const_cast<std::string*>(&g_networkStateTable[0]));
                    m_core->GetLuaManager()->GetEventManager()->CallEvent(EventType::NetworkStateChange, m_argument);
                    m_argument->Clear();
                } break;
                case ID_ROC_DATA_PACKET:
                {
                    RakNet::BitStream l_dataIn(l_packet->data, l_packet->length, false);
                    unsigned int l_textSize;
                    std::string l_stringData;
                    l_dataIn.IgnoreBytes(sizeof(unsigned char));
                    l_dataIn.Read(l_textSize);
                    l_stringData.resize(l_textSize);
                    l_dataIn.Read(const_cast<char*>(l_stringData.data()), l_textSize);

                    m_argument->PushArgument(const_cast<std::string*>(&l_stringData));
                    m_core->GetLuaManager()->GetEventManager()->CallEvent(EventType::NetworkDataRecieve, m_argument);
                    m_argument->Clear();
                } break;
            }
        }
        if(m_networkState == NetworkState::Disconnected)
        {
            m_networkInterface->Shutdown(ROC_NETWORK_SHUTDOWN_DURATION);
            RakNet::RakPeerInterface::DestroyInstance(m_networkInterface);
            m_networkInterface = NULL;
        }
    }
}