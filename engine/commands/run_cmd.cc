#include "run_cmd.h"
#include "chat_cmd.h"
#include "cmd_info.h"
#include "config/yaml_config.h"
#include "model_start_cmd.h"
#include "model_status_cmd.h"
#include "server_start_cmd.h"
#include "utils/file_manager_utils.h"

namespace commands {

void RunCmd::Exec() {
  auto address = host_ + ":" + std::to_string(port_);
  CmdInfo ci(model_id_);
  std::string model_file =
      ci.branch == "main" ? ci.model_name : ci.model_name + "-" + ci.branch;
  // TODO should we clean all resource if something fails?
  // Check if model existed. If not, download it
  {
    auto model_conf = model_service_.GetDownloadedModel(model_file + ".yaml");
    if (!model_conf.has_value()) {
      model_service_.DownloadModel(model_id_);
    }
  }

  // Check if engine existed. If not, download it
  {
    auto required_engine = engine_service_.GetEngineInfo(ci.engine_name);
    if (!required_engine.has_value()) {
      throw std::runtime_error("Engine not found: " + ci.engine_name);
    }
    if (required_engine.value().status == EngineService::kIncompatible) {
      throw std::runtime_error("Engine " + ci.engine_name + " is incompatible");
    }
    if (required_engine.value().status == EngineService::kNotInstalled) {
      engine_service_.InstallEngine(ci.engine_name);
    }
  }

  // Start server if it is not running
  {
    if (!commands::IsServerAlive(host_, port_)) {
      CLI_LOG("Starting server ...");
      commands::ServerStartCmd ssc;
      if (!ssc.Exec(host_, port_)) {
        return;
      }
    }
  }

  config::YamlHandler yaml_handler;
  yaml_handler.ModelConfigFromFile(
      file_manager_utils::GetModelsContainerPath().string() + "/" + model_file +
      ".yaml");
  auto mc = yaml_handler.GetModelConfig();

  // Always start model if not llamacpp
  // If it is llamacpp, then check model status first
  {
    if ((mc.engine.find("llamacpp") == std::string::npos) ||
        !commands::ModelStatusCmd().IsLoaded(host_, port_, mc)) {
      ModelStartCmd msc(host_, port_, mc);
      if (!msc.Exec()) {
        return;
      }
    }
  }

  // Chat
  {
    ChatCmd cc(host_, port_, mc);
    cc.Exec("");
  }
}
};  // namespace commands
