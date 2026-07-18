#include <boost/asio.hpp>
#include <memory>
#include <array>
#include <cstdint>
#include <utility>
#include "session.hpp"
#include "logger/logger.hpp"
#include "config/config.hpp"

using boost::asio::ip::tcp;
using LogLevel = scalable::server::Logger::LogLevel;

namespace scalable {
namespace server {

    Session::Session(tcp::socket socket, Config& config, 
        std::shared_ptr<Logger> logger) :
        socket_(std::move(socket)),
        config_(config),
        logger_(logger)
    {

    }

    void Session::start()
    {
        reader_=std::make_shared<Reader>(socket_,
            weak_from_this());

        writer_=std::make_shared<Writer>(socket_, 
            weak_from_this());

        read_packet();
    }


    void Session::read_packet()
    {
        reader_->read_length();
    }

    void Session::on_packet(std::vector<uint8_t> packet)
    {
        write_packet(std::move(packet));
    }   

    void Session::write_packet(std::vector<uint8_t> packet)
    {
        writer_->add_packet(std::move(packet));
    }

    void Session::stop()
    {
        boost::system::error_code ec;
        socket_.close(ec);
    }
    
}
}