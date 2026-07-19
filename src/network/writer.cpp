#include <boost/asio.hpp>
#include <memory>
#include <cstdint>
#include "writer.hpp"
#include "session/session.hpp"

using boost::asio::ip::tcp;

namespace scalable {
namespace server {


    Writer::Writer(tcp::socket& socket,
        std::weak_ptr<Session> session) :
    socket_(socket),
    session_(session)
    {

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
        std::cout<<"Reader::read_body "<<queue_buffer_.front().data()<<'\n';
                    
        auto self = shared_from_this();
        boost::asio::async_write(socket_,
            boost::asio::buffer(queue_buffer_.front(), 
            queue_buffer_.front().size()),
            [self](boost::system::error_code ec, size_t)
            {
                if(!ec)
                {
                    std::cout<<"Writed\n";
                    self->queue_buffer_.pop_front();
                    if(!self->queue_buffer_.empty())
                        self->write();
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
}
}