#include <boost/asio.hpp>
#include <memory>
#include <cstdint>
#include "writer.hpp"
#include "session/session.hpp"
#include "config/config.hpp"

using boost::asio::ip::tcp;
using boost::asio::steady_timer;

namespace scalable {
namespace server {


    Writer::Writer(tcp::socket& socket,
        std::weak_ptr<Session> session, Config& config) :
    socket_(socket),
    session_(session),
    write_timer_(socket_.get_executor())
    {
        seconds_write = config.get_int(
            "timeouts.write_timeout_seconds", seconds_write);
        
        write_timer_.expires_at(steady_timer::time_point::max());

        check_write_deadline();
    }

    void Writer::add_packet(std::vector<uint8_t> packet)
    {
        auto self=shared_from_this();
        boost::asio::post(
            socket_.get_executor(),
            [self, packet_copy=std::move(packet)]() mutable {
                bool writing = !self->queue_buffer_.empty();
                self->queue_buffer_.push_back(std::move(packet_copy));
                if(!writing)
                {
                    self->write();
                }
            }
        );
    }

    void Writer::write()
    {       
        auto self = shared_from_this();

        write_timer_.expires_after(std::chrono::seconds(seconds_write));

        boost::asio::async_write(socket_,
            boost::asio::buffer(queue_buffer_.front(), 
            queue_buffer_.front().size()),
            [self](boost::system::error_code ec, size_t)
            {
                if(!self->socket_.is_open())
                    return;
                    
                if(!ec)
                {
                    self->queue_buffer_.pop_front();
                    if(!self->queue_buffer_.empty())
                    {    
                        self->write();
                    }
                    else
                    {
                        self->write_timer_.expires_at(steady_timer::time_point::max());
                    }
                }
                else
                {
                    if(ec!=boost::asio::error::operation_aborted)
                    {
                        if(auto session=self->session_.lock())
                        {
                            session->stop();
                        }
                    }
                }
            });
    }


    void Writer::check_write_deadline()
    {
        auto self = shared_from_this();
        write_timer_.async_wait(
            [self](boost::system::error_code ec)
            {
                // operation_aborted means the pending async_wait was cancelled by
                // expires_after(), expires_at(), or cancel().
                // not because the timer expired.
                if(ec == boost::asio::error::operation_aborted)
                {
                    self->check_write_deadline();
                    return;
                }

                if(auto session = self->session_.lock())
                {
                    session->stop();
                }
            
            });
    }

}
}