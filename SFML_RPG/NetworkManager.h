#pragma once
#include "Enums.h"
#include "SFML\Network.hpp"
#include <list>
#include "SceneObject.h"


namespace lbe
{
	class NetworkManager
	{
	private:
		static NetworkState ActiveNetworkState;
		static sf::TcpSocket HostSocket;
		static sf::TcpListener Listener;
		static std::list<sf::TcpSocket*> ClientSockets;
		static sf::UdpSocket UdpSocket;
	public:
		NetworkManager();
		~NetworkManager();

		static void Init();
		static void BindUdpPort();
		static void SetActiveNetworkState(NetworkState NewNetworkState);
		static lbe::NetworkState GetActiveNetworkState();
		static bool ConnectToHost();
		static void StartListening();
		static void StopListening();
		static void ListenForConnections();
		static void SendUpdate();
		static bool ReceiveUpdate();
		static void RemoveObjectByID(sf::Uint32 ID);
		static void SpawnObject(std::string ObjectName, sf::Vector2f Pos);
		static void SpawnObject(lbe::SceneObject* NewObject);
	};
}
