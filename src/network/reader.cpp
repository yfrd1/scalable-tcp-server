#include <boost/asio.hpp>
#include <chrono>
#include "reader.hpp"
#include "session/session.hpp"
#include "common/packet.hpp"
#include "config/config.hpp"

using boost::asio::ip::tcp;
using boost::asio::steady_timer;

namespace scalable {
namespace server {

    Reader::Reader(tcp::socket& socket,
        std::weak_ptr<Session> session, Config& config) :
    socket_(socket),
    session_(session),
    read_timer(socket_.get_executor())
    {
        read_seconds = config.get_int("timeouts.read_timeout_seconds", 10);
        
        read_timer.expires_at(steady_timer::time_point::max());
        
        check_read_deadline();
    }

    void Reader::read_length()
    {
        auto self = shared_from_this();

        read_timer.expires_after(std::chrono::seconds(read_seconds));
    
        boost::asio::async_read(socket_,
            boost::asio::buffer(&packet_length_, sizeof(packet_length_)),
            [self](boost::system::error_code ec, size_t)
            {
                if(!self->socket_.is_open())
                    return;

                if(!ec)
                {
                    self->packet_length_=
                        ntohl(self->packet_length_);

                    if(self->packet_length_ >
                        Packet::MAX_PACKET_SIZE)
                    {
                        if(auto session=
                            self->session_.lock())
                        {
                            session->stop();
                        }
                        return;
                    }

                    self->read_body();
                }
                else
                {
                 
                    if(ec!=boost::asio::error::operation_aborted)
                    {
                        if(auto session=
                            self->session_.lock())
                        {
                            session->stop();
                            return;
                        }   
                    }
                }
            });
    }

    void Reader::read_body()
    {
        auto self = shared_from_this();

        read_timer.expires_after(std::chrono::seconds(read_seconds));
    
        // Add packet length to buffer 
        packet_buffer_.resize(packet_length_ + Packet::PACKET_LENGTH);

        // Copy packet length to buffer
        // Convert packet length back to network byte order before storing it in the buffer.
        uint32_t length=htonl(packet_length_); 
        std::memcpy(packet_buffer_.data(), &length, sizeof(length));
        
        boost::asio::async_read(
            socket_,
            boost::asio::buffer(
                packet_buffer_.data()+Packet::PACKET_LENGTH, 
                packet_length_),
            [self](boost::system::error_code ec, size_t)
            {
                if(!self->socket_.is_open())
                    return;

                auto session=self->session_.lock();

                if(!session)
                {
                    //Session destroyed
                    return;
                }
                else if(!ec)
                {
                    session->on_packet(
                        std::move(self->packet_buffer_));
                    
                    self->packet_buffer_.clear();

                    self->read_length();
                }
                else
                {
                    if(ec!=boost::asio::error::operation_aborted)
                    {
                        session->stop();
                    }
                }
            }
        );
    }

    void Reader::check_read_deadline()
    { 
        auto self = shared_from_this();
        read_timer.async_wait(
            [self](boost::system::error_code ec)
        {
            // operation_aborted means the pending async_wait was cancelled by
            // expires_after(), expires_at(), or cancel().
            // not because the timer expired.
            if(ec==boost::asio::error::operation_aborted)
            {
                self->check_read_deadline();
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