//
// Created by aidan on 5/29/17.
//

#ifndef DISCORDPP_WEBSOCKET_HH
#define DISCORDPP_WEBSOCKET_HH

#include <beast/websocket.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <json.hpp>

namespace discordpp{
	using json = nlohmann::json;

	typedef std::function<void(beast::error_code const&, std::shared_ptr<beast::multi_buffer>, std::shared_ptr<beast::websocket::opcode>)> ws_callback_handler;

	class Websocket{
	public:
		Websocket(boost::asio::io_service *ios, std::string token, std::string gateway, ws_callback_handler handler);

		~Websocket();

	private:
		std::string const host_;
		boost::asio::io_service *ios_;
		std::string token_;
		std::shared_ptr<beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> > > ws_;
	};
}

#endif //DISCORDPP_WEBSOCKET_HH
