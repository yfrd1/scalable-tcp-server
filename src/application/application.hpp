#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include "config/config.hpp"
#include "logger/logger.hpp"
#include "server/server.hpp"
#include "router/router.hpp"
#include "pools/buffer_pool.hpp"
#include "pools/thread_pool.hpp"

namespace mysql = boost::mysql;

namespace scalable {
namespace server {

class Application
{
public:
    explicit Application();

    void run();
    void stop();
    void do_await_stop();;

private:
    boost::asio::io_context io_context_;
    boost::asio::executor_work_guard
        <boost::asio::io_context::executor_type> work_guard_;
    boost::asio::signal_set signals_;

    Config config_;
    std::shared_ptr<Logger> logger_;
    
    ThreadPool thread_pool_;
    BufferPool buffer_pool_;
    //mysql::connection_pool connection_pool_;

    // Router is shared across all sessions because routing depends only
    // on packet type and does not store any session-specific state.
    Router router_;

    std::unique_ptr<Server> server_;
};

}
}