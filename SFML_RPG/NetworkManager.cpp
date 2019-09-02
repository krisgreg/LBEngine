#include "NetworkManager.h"
#include "Consts.h"
#include "SFML\Network.hpp"
#include "LevelManager.h"
#include "SceneObjectFactory.h"
#include "GameState.h"

lbe::NetworkState lbe::NetworkManager::ActiveNetworkState = lbe::NetworkState::Host;
sf::TcpSocket lbe::NetworkManager::HostSocket;
sf::TcpListener lbe::NetworkManager::Listener;
std::list<sf::TcpSocket*> lbe::NetworkManager::ClientSockets;
sf::UdpSocket lbe::NetworkManager::UdpSocket;

lbe::NetworkManager::NetworkManager()
{
}

lbe::NetworkManager::~NetworkManager()
{
}

void lbe::NetworkManager::Init()
{
	Listener.setBlocking(false);
}

void lbe::NetworkManager::BindUdpPort()
{
	if (ActiveNetworkState == NetworkState::Host)
	{
		if (UdpSocket.bind(lbe::HOST_PORT) != sf::Socket::Done)
		{
			LBE_WARN("NetworkManager::Init() Could not bind UDP socket to port: {0}", lbe::HOST_PORT);
		}
	}
	else
	{
		if (UdpSocket.bind(lbe::CLIENT_PORT) != sf::Socket::Done)
		{
			LBE_WARN("NetworkManager::Init() Could not bind UDP socket to port: {0}", lbe::CLIENT_PORT);
		}
	}
	UdpSocket.setBlocking(false);
}

void lbe::NetworkManager::SetActiveNetworkState(NetworkState NewNetworkState)
{
	ActiveNetworkState = NewNetworkState;
}

lbe::NetworkState lbe::NetworkManager::GetActiveNetworkState()
{
	return ActiveNetworkState;
}

bool lbe::NetworkManager::ConnectToHost()
{
	HostSocket.setBlocking(true);
	sf::Socket::Status Status = HostSocket.connect(HOST_IP, lbe::HOST_PORT);
	if (Status != sf::Socket::Done)
	{
		return false;
	}
	else
	{
		sf::Packet packet;
		HostSocket.receive(packet);
		//LBE_TRACE("Packet received! size={0}", packet.getDataSize());
		sf::Uint32 NewPlayerID;
		sf::Vector2f PlayerSpawnPos;
		packet >> NewPlayerID >> PlayerSpawnPos.x >> PlayerSpawnPos.y;
		lbe::GameState::CreateLocalPlayer(PlayerSpawnPos, NewPlayerID);
		std::string Name;
		while (packet >> Name)
		{
			sf::Vector2f Pos;
			int UniqueID;
			packet >> UniqueID >> Pos.x >> Pos.y;
			SceneObject* NewSceneObject = SceneObjectFactory::CreateInstance(Name, Pos);
			NewSceneObject->SetUniqueID(UniqueID);
			LevelManager::AddSceneObject(NewSceneObject, Pos);
			//LBE_TRACE("Object read from packet! name={0} ID={1} Pos.x={2} Pos.y={3}", Name, UniqueID, Pos.x, Pos.y);
		}
		HostSocket.setBlocking(false);
		return true;
	}
}

void lbe::NetworkManager::StartListening()
{
	if (Listener.listen(lbe::HOST_PORT) != sf::Socket::Done)
	{
		LBE_WARN("NetworkManager::ListenForConnections() .listen error");
	}
}

void lbe::NetworkManager::StopListening()
{
	Listener.close();
}

void lbe::NetworkManager::ListenForConnections()
{
	sf::TcpSocket* Client = new sf::TcpSocket();
	sf::Socket::Status SocketStatus = Listener.accept(*Client);
	if (SocketStatus == sf::Socket::NotReady)
	{
		delete Client;
		return;
	}
	if (SocketStatus != sf::Socket::Done)
	{
		LBE_WARN("NetworkManager::ListenForConnections() .accept error");
		delete Client;
		return;
	}


	sf::Packet packet;
	sf::Vector2f PlayerSpawnPos(400, 400);
	lbe::Player* NewPlayer = new Player(PlayerSpawnPos);
	sf::Uint32 NewPlayerID = NewPlayer->GetUniqueID();
	packet << NewPlayerID << PlayerSpawnPos.x << PlayerSpawnPos.y;
	for (auto &object : LevelManager::GetSceneObjectsFromLevel(sf::Vector2f(400, 400)))
	{
		packet << object->GetName() << object->GetUniqueID() << object->GetPosition().x << object->GetPosition().y;
		//LBE_TRACE("Object added to packet!");
		
	}
	Client->send(packet);
	lbe::LevelManager::AddSceneObject(NewPlayer, NewPlayer->GetPosition());
	
	Client->setBlocking(false);
	ClientSockets.emplace_back(Client);
}

void lbe::NetworkManager::SendUpdate()
{
	if (ActiveNetworkState == NetworkState::Host)
	{
		std::vector<SceneObject*> Objects = LevelManager::GetSceneObjectsFromLevel(sf::Vector2f(400, 400));
		sf::Packet packet;
		packet << lbe::PACKET_TYPE_TICK_UPDATE;
		for (auto &object : Objects)
		{
			packet << object->GetUniqueID();
			object->WriteUpdatePacket(packet);
		}
		for (auto &socket : ClientSockets)
		{
			UdpSocket.send(packet, socket->getRemoteAddress(), lbe::CLIENT_PORT);
		}
	}
	else if (ActiveNetworkState == NetworkState::Client)
	{
		sf::Packet packet;
		packet << lbe::PACKET_TYPE_TICK_UPDATE;
		packet << GameState::LocalPlayer->GetUniqueID();
		GameState::LocalPlayer->WriteUpdatePacket(packet);
		UdpSocket.send(packet, HostSocket.getRemoteAddress(), lbe::HOST_PORT);
	}
}

bool lbe::NetworkManager::ReceiveUpdate()
{
	bool bPacketReceived = false;
	if (ActiveNetworkState == NetworkState::Host)
	{
		sf::Packet packet;
		sf::IpAddress SenderIP = sf::IpAddress::Any;
		unsigned short SenderPort = lbe::HOST_PORT;
		if (UdpSocket.receive(packet, SenderIP, SenderPort) == sf::Socket::Done)
		{
			sf::String PacketType;
			packet >> PacketType;
			if (PacketType == lbe::PACKET_TYPE_TICK_UPDATE)
			{
				LevelManager::ApplyUpdatePacketToLevel(packet, sf::Vector2f(400, 400));
			}

			bPacketReceived = true;
		}

		for (auto &socket : ClientSockets)
		{
			if (socket->receive(packet) == sf::Socket::Done)
			{
				sf::String PacketType;
				packet >> PacketType;
				if (PacketType == lbe::PACKET_TYPE_REMOVE_OBJECT)
				{
					sf::Uint32 ObjectToDelete;
					packet >> ObjectToDelete;
					if (LevelManager::SceneObjectWithIDExist(ObjectToDelete))
					{
						LevelManager::GetSceneObjectByID(ObjectToDelete)->MarkForDeletion();
					}
				}
				else if (PacketType == lbe::PACKET_TYPE_SPAWN_OBJECT)
				{
					std::string NewObjectName;
					sf::Vector2f NewObjectPos;
					packet >> NewObjectName >> NewObjectPos.x >> NewObjectPos.y;
					SceneObject* NewObject = lbe::SceneObjectFactory::CreateInstance(NewObjectName, NewObjectPos);
					NewObject->ApplyUpdatePacket(packet);
					LevelManager::AddSceneObject(NewObject, NewObject->GetPosition());
				}
				bPacketReceived = true;
			}
		}
	}
	else if (ActiveNetworkState == NetworkState::Client)
	{
		sf::Packet packet;
		sf::IpAddress SenderIP = lbe::HOST_IP;
		unsigned short SenderPort  = lbe::CLIENT_PORT;
		if (UdpSocket.receive(packet, SenderIP, SenderPort) == sf::Socket::Done)
		{
			sf::String PacketType;
			packet >> PacketType;
			if (PacketType == lbe::PACKET_TYPE_TICK_UPDATE)
			{
				//LBE_TRACE("update packet recieved from host!");
				LevelManager::ApplyUpdatePacketToLevel(packet, sf::Vector2f(400, 400));
			}
			bPacketReceived = true;
		}
		else if (HostSocket.receive(packet) == sf::Socket::Done)
		{
			sf::String PacketType;
			packet >> PacketType;
			if (PacketType == lbe::PACKET_TYPE_REMOVE_OBJECT)
			{
				sf::Uint32 ObjectToDelete;
				packet >> ObjectToDelete;
				if (LevelManager::SceneObjectWithIDExist(ObjectToDelete))
				{
					LevelManager::GetSceneObjectByID(ObjectToDelete)->MarkForDeletion();
				}
			}
			bPacketReceived = true;
		}
		else
		{
			//LBE_INFO("NO update packet recieved from host!");
		}
	}
	return bPacketReceived;
}

void lbe::NetworkManager::RemoveObjectByID(sf::Uint32 ID)
{
	sf::Packet packet;
	packet << lbe::PACKET_TYPE_REMOVE_OBJECT;
	packet << ID;

	if (ActiveNetworkState == NetworkState::Host)
	{
		for (auto &socket : ClientSockets)
		{
			socket->send(packet);
		}
	}
	else if (ActiveNetworkState == NetworkState::Client)
	{
		HostSocket.send(packet);
	}
}

void lbe::NetworkManager::SpawnObject(std::string ObjectName, sf::Vector2f Pos)
{
	sf::Packet packet;
	packet << lbe::PACKET_TYPE_SPAWN_OBJECT;
	packet << ObjectName << Pos.x << Pos.y;

	if (ActiveNetworkState == NetworkState::Host)
	{
		for (auto &socket : ClientSockets)
		{
			socket->send(packet);
		}
	}
	else if (ActiveNetworkState == NetworkState::Client)
	{
		HostSocket.send(packet);
	}
}

void lbe::NetworkManager::SpawnObject(lbe::SceneObject* NewObject)
{
	sf::Packet packet;
	packet << lbe::PACKET_TYPE_SPAWN_OBJECT;
	packet << NewObject->GetName() << NewObject->GetPosition().x << NewObject->GetPosition().y;
	NewObject->WriteUpdatePacket(packet);

	if (ActiveNetworkState == NetworkState::Host)
	{
		for (auto &socket : ClientSockets)
		{
			socket->send(packet);
		}
	}
	else if (ActiveNetworkState == NetworkState::Client)
	{
		HostSocket.send(packet);
	}
}
