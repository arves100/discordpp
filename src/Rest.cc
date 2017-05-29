//
// Created by aidan on 5/28/17.
//

#include "discordpp/Rest.hh"

#include <beast/core.hpp>
#include <beast/http.hpp>

using json = nlohmann::json;
using namespace discordpp;
using snowflake = uint64_t;

Rest::Rest(boost::asio::io_service *ios, std::string token)
		: host_("discordapp.com")
		, ios_(ios)
		, sock_(*ios_)
		, ctx_(boost::asio::ssl::context::sslv23)
		, stream_(sock_, ctx_)
		, token_(token){
	boost::asio::ip::tcp::resolver r{*ios_};
	boost::asio::connect(sock_,
	                     r.resolve(boost::asio::ip::tcp::resolver::query{host_, "https"}));

	stream_.set_verify_mode(boost::asio::ssl::verify_none);
	stream_.handshake(boost::asio::ssl::stream_base::client);
}

Rest::~Rest(){
	boost::system::error_code ec;
	stream_.shutdown(ec);
	if(ec && ec != boost::asio::error::eof)
		std::cout << "error: " << ec.message();
}

json Rest::call(std::string requestType, std::string path, json arguments){
	beast::http::request<beast::http::string_body> req;

	req.version = 11;   // HTTP/1.1
	req.fields.replace("Host", host_ + ":" +
	                           boost::lexical_cast<std::string>(sock_.remote_endpoint().port()));
	req.method(requestType);
	req.target("/api" + path);
	req.fields.insert("Content-Type", "application/json");
	req.fields.insert("Authorization", token_);
	//req.fields.insert("Connection", "keep-alive");
	req.fields.insert("User-Agent", "DiscordBot (https://github.com/Aidoboy/discordpp, v0.1)");

	static_assert(std::is_same<decltype(req.body), std::string>::value);
	req.body = arguments.dump();

	beast::http::prepare(req);
	beast::http::write(stream_, req);//, std::bind(&handle_write, std::placeholders::_1));

	/*beast::multi_buffer sb;
	beast::http::response<beast::http::string_body> resp;
	static_assert(std::is_same<decltype(resp.body), std::string>::value);
	beast::http::read(sock_, sb, resp);*/

	beast::multi_buffer b;
	beast::http::response<beast::http::string_body> resp;
	static_assert(std::is_same<decltype(resp.body), std::string>::value);
	beast::http::read(stream_, b, resp);

	return json::parse(resp.body);
}

/*void Rest::handle_write(boost::system::error_code){

}*/