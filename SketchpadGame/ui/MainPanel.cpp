#include <iostream>
#include "core/Window.h"
#include "MainPanel.h"
#include "network/Peer.h"

#include <imgui/imgui.h>


 MainPanel::MainPanel(Window* win): window(win){
	
}

void MainPanel::render() {
	ImGui::Begin("Skeychpad Panel");
	if (ImGui::CollapsingHeader("Sketchpad Networking"))
	{
		//--------------------------------------------------
		//					Peer creation
		//--------------------------------------------------
		ImGui::NewLine();
		static char portBuffer[256] = "";
		static char idBuffer[256] = "";
		static ImVec4 color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // Initial color (red)
		if (buttonPCEnabled) {
			ImGui::Text("Setup Peer");

			ImGui::ColorEdit3("Color", (float*)&color); // 3-component color picker
			ImGui::InputText("Port", portBuffer, sizeof(portBuffer));
			ImGui::InputText("ID", idBuffer, sizeof(idBuffer));

			if (ImGui::Button("Connect")) {
				int port = 0;
				try {
					port = std::stoi(portBuffer);
				}
				catch (const std::exception& e) {
					std::cout << "Failed to convert to integer: " << e.what() << std::endl;
					return;
				}
				window->getScene()->setupPeer(std::string(idBuffer), port,(float*)&color);
				buttonPCEnabled = false;
			}
		}
		else {			
			ImGui::Text("Peer connected");
		}
		
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
		

		//--------------------------------------------------
		//					Connections setup
		//--------------------------------------------------
		static char port_Buffer[256] = "";
		static char ip_Buffer[256] = "";
		static char id_Buffer[256] = "";
		int port2 = 0;

		ImGui::Text("Create Connection");
		ImGui::InputText("peer IP", ip_Buffer, sizeof(ip_Buffer));
		ImGui::InputText("peer PORT", port_Buffer, sizeof(port_Buffer));
		ImGui::InputText("peer ID", id_Buffer, sizeof(id_Buffer));

		if (ImGui::Button("Create")) {
			try {
				port2 = std::stoi(port_Buffer);
			}
			catch (const std::exception& e) {
				std::cout << "Failed to convert to integer: " << e.what() << std::endl;
				return;
			}
			window->getScene()->createConnection(id_Buffer, ip_Buffer, port2, -1);
		}
		//--------------------------------------------------
		//					broadCast Messsage. debug
		//--------------------------------------------------
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
		static char broadCastMessage[256] = "";
		ImGui::InputText("Message", broadCastMessage, sizeof(broadCastMessage));

		if (ImGui::Button("BroadCast")) {		

			window->getScene()->broadcast(broadCastMessage);
		}
		//--------------------------------------------------
		//					Networking Debug
		//--------------------------------------------------
		ImGui::NewLine();
		ImGui::Separator();
		ImGui::NewLine();
		if (!buttonPCEnabled) {
			std::string temp = std::string("Peer id: ") + std::string(idBuffer);
			ImGui::Text(temp.c_str());
			std::string temp2 = std::string("Peer Port: ") + std::string(portBuffer);
			ImGui::Text(temp2.c_str());

			int num_cnx = window->getScene()->getPeer()->get_num_connections();
			int num_msg = window->getScene()->getPeer()->get_num_messages();
			std::string temp3 = std::string("Num connections : ") + std::to_string(num_cnx);
			ImGui::Text(temp3.c_str());
			std::string temp4 = std::string("Num messages : ") + std::to_string(num_msg);
			ImGui::Text(temp4.c_str());

			//std::string log = window->getScene()->getPeer()->get_message().body;
			//std::string temp5 = std::string("Message LOG: ") + log;
			//ImGui::Text(temp5.c_str());
		}
		




	}
	ImGui::End();
}