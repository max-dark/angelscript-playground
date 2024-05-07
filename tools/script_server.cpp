#include <iostream>

#include <string_view>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include <jsonrpccpp/server.h>
#include <jsonrpccpp/server/connectors/httpserver.h>

namespace lsp
{
using namespace std::literals;
using method_id = const std::string_view;
using request_t = Json::Value;
using response_t = Json::Value;

namespace methods
{
#define lsp_method(ID) ::lsp::method_id ID = #ID
#define lsp_method_sys(ID) ::lsp::method_id sys_##ID = "$/" #ID
#define lsp_method_ns(NS, ID) ::lsp::method_id NS##_##ID = (#NS "/" #ID)

// initialize connection
lsp_method(initialize);
lsp_method(initialized);

lsp_method_ns(client, registerCapability);
lsp_method_ns(client, unregisterCapability);

lsp_method_sys(setTrace);
lsp_method_sys(logTrace);
lsp_method(shutdown);
lsp_method(exit);


#undef lsp_method_ns
#undef lsp_method_sys
#undef lsp_method
}

class Server: public jsonrpc::AbstractServer<Server>
{
    using method_ptr = void(Server::*)(const request_t&, response_t&);
    using notify_ptr = void(Server::*)(const request_t&);
public:
    explicit Server(jsonrpc::HttpServer& server)
        : jsonrpc::AbstractServer<Server>(server)
    {}

    static auto new_method(const method_id& id)
    {
        auto method = jsonrpc::Procedure{{id.data(), id.size()}, jsonrpc::PARAMS_BY_NAME, jsonrpc::JSON_OBJECT, nullptr};
        return method;
    }

    static auto new_notify(const method_id& id)
    {
        auto notify = jsonrpc::Procedure{{id.data(), id.size()}, jsonrpc::PARAMS_BY_NAME, nullptr};
        notify.SetProcedureType(jsonrpc::RPC_NOTIFICATION);
        return notify;
    }

    void bindMethods()
    {
        bindAndAddMethod(new_method(methods::initialize), &Server::initialize);
        bindAndAddMethod(new_method(methods::initialized), &Server::initialized);
        bindAndAddMethod(new_method(methods::shutdown), &Server::shutdown);
        bindAndAddMethod(new_method(methods::exit), &Server::exit);
    }

    void startAndWait()
    {
        is_running = StartListening();
        while (is_running)
        {
            std::unique_lock lock{mtx_running};
            cv_running.wait(lock);
        }
        std::this_thread::sleep_for(100ms); // wait for response was sent
        StopListening();
    }

    void setRunning(bool value)
    {
        {
            std::unique_lock lock{mtx_running};
            is_running = value;
        }
        cv_running.notify_all();
    }

protected:
    void initialize(const request_t& inp, response_t& result)
    {}
    void initialized(const request_t& inp, response_t& result)
    {}


    void shutdown(const request_t& inp, response_t& result)
    {}

    void exit(const request_t& inp, response_t& result)
    {
        setRunning(false);
    }

private:
    std::atomic_bool is_running = false;
    std::mutex mtx_running;
    std::condition_variable cv_running;
};
}

int main(int argc, char** argv)
{
    auto srv_thread = std::thread{[] {

        jsonrpc::HttpServer listener{8383};
        lsp::Server server{listener};

        server.bindMethods();
        server.startAndWait();
    }};

    if (srv_thread.joinable())
    {
        srv_thread.join();
    }
    return EXIT_SUCCESS;
}