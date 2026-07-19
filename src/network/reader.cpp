#include <boost/asio.hpp>
#include "reader.hpp"
#include "session/session.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {


    Reader::Reader(tcp::socket& socket,
        std::weak_ptr<Session> session) :
    socket_(socket),
    session_(session)
    {

    }

    void Reader::read_length()
    {
        auto self = shared_from_this();

        boost::asio::async_read(socket_,
            boost::asio::buffer(&packet_length_, sizeof(packet_length_)),
            [self](boost::system::error_code ec, size_t)
            {
                if(!ec)
                {
                    self->packet_length_=
                        ntohl(self->packet_length_);
                    std::cout<<"self->packet_length_= "<<self->packet_length_<<'\n';

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
                        }   
                    }
                }
            });
    }

    void Reader::read_body()
    {
        auto self = shared_from_this();
        packet_buffer_.resize(packet_length_);
        std::cout<<"Reader::read_body "<<packet_length_<<'\n';

        boost::asio::async_read(
            socket_,
            boost::asio::buffer(packet_buffer_, packet_length_),
            [self](boost::system::error_code ec, size_t)
            {
                auto session=self->session_.lock();

                if(!session)
                {
                    std::cout<<"read body if(!session)\n";
                    //Session destroyed
                    return;
                }
                else if(!ec)
                {
                    std::cout<<"Reader::read_body "<<self->packet_buffer_.data()<<'\n';

                    session->on_packet(
                        std::move(self->packet_buffer_));

                    self->read_length();
                }
                else
                {
                    std::cout<<ec;
                    if(ec!=boost::asio::error::operation_aborted)
                    {
                        session->stop();
                    }
                }
            }
        );
    }

    /*
    void Reader::read_length()
    {
        auto self { shared_from_this() };
        deadline.expires_after(std::chrono::seconds(read_timeout));
        idle_deadline.expires_after(std::chrono::seconds(idle_timeout));
        
        boost::asio::async_read(
            socket,
            boost::asio::buffer(&packet_length, sizeof(packet_length)),
            [this, self](boost::system::error_code ec, size_t bytes)
            {
                if(!ec)
                {
                    read_body();
                }
                else
                {
                    logger_->log(LogLevel::Error, "Session", ec.message());
                    close();
                    return;
                }
            }
        );
    }

    void Reader::read_body()
    {
        auto self { shared_from_this() };
        deadline.expires_after(std::chrono::seconds(read_timeout));
        idle_deadline.expires_after(std::chrono::seconds(idle_timeout));

        packet_length=ntohl(packet_length);
        packet_body.reserve(packet_length);

        boost::asio::async_read(
            socket,
            boost::asio::buffer(packet_body, packet_length),
            [this, self](boost::system::error_code ec, size_t bytes)
            {
                if(!ec)
                {
                    
                }
                else
                {
                    logger_->log(LogLevel::Error, "Session", ec.message());
                    close();
                    return;
                }
            }
        );

    }
    */

}
}