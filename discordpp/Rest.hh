//
// Created by aidan on 5/28/17.
//

#ifndef DISCORDPP_REST_HH
#define DISCORDPP_REST_HH

#include <json.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace discordpp{
	using json = nlohmann::json;
	//namespace asio = boost::asio;
	//using boost::system::error_code;
	using snowflake = uint64_t;

	class Rest{
	public:
		Rest(boost::asio::io_service *ios, std::string token);

		~Rest();

		json call(std::string requestType, std::string path, json arguments = {});

	private:
		//void handle_write(boost::system::error_code);

		boost::asio::io_service *ios_;
		boost::asio::ip::tcp::socket sock_;
		boost::asio::ssl::context ctx_;
		boost::asio::ssl::stream < boost::asio::ip::tcp::socket &> stream_;
		std::string const host_;
		std::string token_;
	};
}


#endif //DISCORDPP_REST_HH
