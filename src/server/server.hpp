#ifndef  SCALABLE_SERVER_SERVER_HPP
#define  SCALABLE_SERVER_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <string>
#include <atomic>
#include <set>
#include <memory>
#include <mutex>
#include "config/config.hpp"
#include "logger/logger.hpp"
#include "session/session.hpp"

namespace mysql = boost::mysql;
using boost::asio::ip::tcp;

namespace scalable {
namespace server {

class Server
{
public: 

explicit Server(boost::asio::io_context& io, 
        Config& cnf, std::shared_ptr<Logger> log, mysql::connection_pool& pool);

void start_workers();
void start_accept();
void stop();
void remove_session(std::shared_ptr<Session> session);

private:

void do_accept();
void do_await_stop();
bool add_connection();
void remove_connection();

boost::asio::io_context& io_context_;
tcp::acceptor acceptor;
std::set<std::shared_ptr<Session>> sessions_;
std::shared_ptr<Logger> logger_;
Config& config_;
mysql::connection_pool& connection_pool_;
std::atomic<size_t> active_connections{0};
size_t max_sessions{0};
std::mutex session_mutex_;

};

}
}
#endif