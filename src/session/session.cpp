#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <memory>
#include <array>
#include <cstdint>
#include <utility>
#include "session.hpp"
#include "logger/logger.hpp"
#include "config/config.hpp"
#include "common/packet.hpp"

namespace mysql = boost::mysql;
using boost::asio::ip::tcp;
using LogLevel = scalable::server::Logger::LogLevel;

namespace scalable {
namespace server {

    Session::Session(tcp::socket socket, 
        Config& config, 
        std::shared_ptr<Logger> logger,
        mysql::connection_pool& pool,
        std::function<void(std::shared_ptr<Session>)> on_close) :
        socket_(std::move(socket)),
        config_(config),
        logger_(logger),
        connection_pool_(pool),
        on_close_(std::move(on_close)),
        idle_timer_(socket_.get_executor())
    {
        
        seconds_idle_ = config_.get_int(
            "timeouts.idle_timeout_seconds", seconds_idle_);
        idle_timer_.expires_at(steady_timer::time_point::max());

        check_idle_timer();
    }

    void Session::start()
    {
        reader_=std::make_shared<Reader>(socket_,
            weak_from_this(), config_);

        writer_=std::make_shared<Writer>(socket_, 
            weak_from_this(), config_);

        read_packet();
    }


    void Session::read_packet()
    {
        idle_timer_.expires_after(std::chrono::seconds(seconds_idle_));
        reader_->read_length();
    }

    void Session::on_packet(std::vector<uint8_t> packet)
    {
        Packet packetObj(std::move(packet));
        router_.route(*this, packetObj, connection_pool_);
    }   

    void Session::write_packet(std::vector<uint8_t> packet)
    {
        idle_timer_.expires_after(std::chrono::seconds(seconds_idle_));

        writer_->add_packet(std::move(packet));
    }

    void Session::stop()
    {
        if(stopped_)
            return;

        stopped_=true;

        boost::system::error_code ec;
        socket_.close(ec);
        idle_timer_.cancel();

        if(on_close_)
            on_close_(shared_from_this());
    }
    
    void Session::check_idle_timer()
    {
        auto self = shared_from_this();
        idle_timer_.async_wait(
            [self](boost::system::error_code ec)
            {
                if(ec==boost::asio::error::operation_aborted)
                {
                    self->check_idle_timer();
                    return;
                }

                self->stop();
            }
        );
    }
}
}