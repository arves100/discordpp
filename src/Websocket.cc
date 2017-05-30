//
// Created by aidan on 5/29/17.
//

#include "discordpp/Websocket.hh"

using namespace discordpp;

Websocket::Websocket(boost::asio::io_service *ios, std::string token, std::string gateway, ws_callback_handler handler)
		: host_("discordapp.com")
		, ios_(ios)
		, token_(token){
	std::string::size_type protocolPos = gateway.find("wss://");
	if (protocolPos != std::string::npos) {
		gateway.erase(protocolPos, 6);
	}
	std::cout << "Connecting to gateway at " << gateway << "\n";
	boost::asio::ip::tcp::resolver r{*ios_};
	boost::asio::ip::tcp::socket sock{*ios_};
	boost::asio::connect(sock, r.resolve(boost::asio::ip::tcp::resolver::query{gateway, "https"}));

	// Perform SSL handshaking
	boost::asio::ssl::context ctx{boost::asio::ssl::context::sslv23};
	boost::asio::ssl::stream<boost::asio::ip::tcp::socket&> stream{sock, ctx};
	stream.set_verify_mode(boost::asio::ssl::verify_none);
	stream.handshake(boost::asio::ssl::stream_base::client);

	// Secure WebSocket connect and send message using Beast
	ws_ = std::make_shared<beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> >>(stream);
	//ws_ = &ws;
	std::cout << "handshaking at " << gateway << "\n";
	ws_->handshake(gateway, "/");


	std::cout << "Connection established.\n";

	json connect = {
			{"op", 2},
			{"d", {
					       {"token", token_},
					       {"v", 4},
					       {"properties", {
							                      {"$os", "linux"},
							                      {"$browser", "discordpp"},
							                      {"$device", "discordpp"},
							                      {"$referrer",""}, {"$referring_domain",""}
					                      }
					       },
					       {"compress", false},
					       {"large_threshold", 250}
			       }
			}
	};
	std::cout << "Client Handshake:\n" << connect.dump(1) << "\n";

	ws_->write(boost::asio::buffer(connect.dump()));

	std::shared_ptr<beast::multi_buffer> b;
	std::shared_ptr<beast::websocket::opcode> op;
	ws_->async_read(*op, *b, std::bind(handler, this, std::placeholders::_1, op, b));
}

Websocket::~Websocket(){
	//ws_->close(beast::websocket::close_code::normal);
}
