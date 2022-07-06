#include "hello.hpp"

#include <fmt/format.h>

#include <userver/clients/dns/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

namespace service_template {

namespace {

class Hello final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-hello";

  Hello(const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        pg_cluster_(
            component_context
                .FindComponent<userver::components::Postgres>("users-db")
                .GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    const auto& name = request.GetArg("name");

    if (!name.empty()) {
      pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                          "INSERT INTO hello_postgres.users(name, count) VALUES($1) "
                          "ON CONFLICT (name) "
                          "DO UPDATE SET count = users.count + 1 "
                          "RETURNING users.count",
                          name);
    }

    return service_template::SayHelloTo(name);
  }

  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

std::string SayHelloTo(std::string_view name) {
  if (name.empty()) {
    name = "unknown user";
  }

  return fmt::format("Hello, {}!\n", name);
}

void AppendHello(userver::components::ComponentList& component_list) {
  component_list.Append<Hello>();
  component_list.Append<userver::components::Postgres>("users-db");
  component_list.Append<userver::clients::dns::Component>();
}

}  // namespace service_template
