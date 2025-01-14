#pragma once

#include <json/json.h>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace config {
struct ModelConfig {
  std::string name;
  std::string model;
  std::string version;
  std::vector<std::string> stop = {};
  float top_p = std::numeric_limits<float>::quiet_NaN();
  float temperature = std::numeric_limits<float>::quiet_NaN();
  float frequency_penalty = std::numeric_limits<float>::quiet_NaN();
  float presence_penalty = std::numeric_limits<float>::quiet_NaN();
  int max_tokens = std::numeric_limits<int>::quiet_NaN();
  bool stream = std::numeric_limits<bool>::quiet_NaN();
  int ngl = std::numeric_limits<int>::quiet_NaN();
  int ctx_len = std::numeric_limits<int>::quiet_NaN();
  std::string engine;
  std::string prompt_template;
  std::string system_template;
  std::string user_template;
  std::string ai_template;

  std::string os;
  std::string gpu_arch;
  std::string quantization_method;
  std::string precision;
  int tp = std::numeric_limits<int>::quiet_NaN();
  std::string trtllm_version;
  bool text_model = std::numeric_limits<bool>::quiet_NaN();
  std::string id;
  std::vector<std::string> files;
  std::size_t created;
  std::string object;
  std::string owned_by = "";

  int seed = -1;
  float dynatemp_range = 0.0f;
  float dynatemp_exponent = 1.0f;
  int top_k = 40;
  float min_p = 0.05f;
  float tfs_z = 1.0f;
  float typ_p = 1.0f;
  int repeat_last_n = 64;
  float repeat_penalty = 1.0f;
  bool mirostat = false;
  float mirostat_tau = 5.0f;
  float mirostat_eta = 0.1f;
  bool penalize_nl = false;
  bool ignore_eos = false;
  int n_probs = 0;
  int min_keep = 0;
  std::string grammar;

  Json::Value ToJson() const {
    Json::Value obj;

    obj["id"] = id;
    obj["name"] = name;
    obj["model"] = model;
    obj["version"] = version;

    Json::Value stop_array(Json::arrayValue);
    for (const auto& s : stop) {
      stop_array.append(s);
    }
    obj["stop"] = stop_array;

    obj["stream"] = stream;
    obj["top_p"] = top_p;
    obj["temperature"] = temperature;
    obj["frequency_penalty"] = frequency_penalty;
    obj["presence_penalty"] = presence_penalty;
    obj["max_tokens"] = max_tokens;
    obj["seed"] = seed;
    obj["dynatemp_range"] = dynatemp_range;
    obj["dynatemp_exponent"] = dynatemp_exponent;
    obj["top_k"] = top_k;
    obj["min_p"] = min_p;
    obj["tfs_z"] = tfs_z;
    obj["typ_p"] = typ_p;
    obj["repeat_last_n"] = repeat_last_n;
    obj["repeat_penalty"] = repeat_penalty;
    obj["mirostat"] = mirostat;
    obj["mirostat_tau"] = mirostat_tau;
    obj["mirostat_eta"] = mirostat_eta;
    obj["penalize_nl"] = penalize_nl;
    obj["ignore_eos"] = ignore_eos;
    obj["n_probs"] = n_probs;
    obj["min_keep"] = min_keep;
    obj["ngl"] = ngl;
    obj["ctx_len"] = ctx_len;
    obj["engine"] = engine;
    obj["prompt_template"] = prompt_template;
    obj["system_template"] = system_template;
    obj["user_template"] = user_template;
    obj["ai_template"] = ai_template;
    obj["os"] = os;
    obj["gpu_arch"] = gpu_arch;
    obj["quantization_method"] = quantization_method;
    obj["precision"] = precision;

    Json::Value files_array(Json::arrayValue);
    for (const auto& file : files) {
      files_array.append(file);
    }
    obj["files"] = files_array;

    obj["created"] = static_cast<Json::UInt64>(created);
    obj["object"] = object;
    obj["owned_by"] = owned_by;
    obj["text_model"] = text_model;

    if (engine == "cortex.tensorrt-llm") {
      obj["trtllm_version"] = trtllm_version;
      obj["tp"] = tp;
    }

    return obj;
  }
  std::string ToString() const {
    std::ostringstream oss;

    // Color codes
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";
    const std::string GREEN = "\033[1;32m";
    const std::string YELLOW = "\033[0;33m";
    const std::string BLUE = "\033[0;34m";
    const std::string MAGENTA = "\033[0;35m";
    const std::string GRAY = "\033[1;90m";

    // Helper function to print comments
    auto print_comment = [&oss, &GRAY, &RESET](const std::string& comment) {
      oss << GRAY << "# " << comment << RESET << "\n";
    };

    // Helper function to print key-value pairs
    auto print_kv = [&oss, &GREEN, &RESET](
                        const std::string& key, const auto& value,
                        const std::string& color = "\033[0m") {
      oss << GREEN << key << ":" << RESET << " " << color << value << RESET
          << "\n";
    };

    // Helper function to print boolean values
    auto print_bool = [&print_kv, &MAGENTA](const std::string& key,
                                            bool value) {
      print_kv(key, value ? "true" : "false", MAGENTA);
    };

    // Helper function to print float values with fixed precision
    auto print_float = [&print_kv, &BLUE](const std::string& key, float value) {
      if (!std::isnan(value)) {
        std::ostringstream float_oss;
        float_oss << std::fixed << std::setprecision(9) << value;
        print_kv(key, float_oss.str(), BLUE);
      }
    };

    print_comment("BEGIN GENERAL GGUF METADATA");
    if (!id.empty())
      print_kv("id", id, YELLOW);
    if (!name.empty())
      print_kv("name", name, YELLOW);
    if (!model.empty())
      print_kv("model", model, YELLOW);
    if (!version.empty())
      print_kv("version", version, YELLOW);
    if (!files.empty()) {
      oss << GREEN << "files:" << RESET << "\n";
      for (const auto& file : files) {
        oss << "  - " << YELLOW << file << RESET << "\n";
      }
    }
    print_comment("END GENERAL GGUF METADATA");

    print_comment("BEGIN INFERENCE PARAMETERS");
    print_comment("BEGIN REQUIRED");
    if (!stop.empty()) {
      oss << GREEN << "stop:" << RESET << "\n";
      for (const auto& s : stop) {
        oss << "  - " << YELLOW << s << RESET << "\n";
      }
    }
    print_comment("END REQUIRED");
    print_comment("BEGIN OPTIONAL");

    print_bool("stream", stream);
    print_float("top_p", top_p);
    print_float("temperature", temperature);
    print_float("frequency_penalty", frequency_penalty);
    print_float("presence_penalty", presence_penalty);
    if (max_tokens != std::numeric_limits<int>::quiet_NaN())
      print_kv("max_tokens", max_tokens, MAGENTA);
    if (seed != -1)
      print_kv("seed", seed, MAGENTA);
    print_float("dynatemp_range", dynatemp_range);
    print_float("dynatemp_exponent", dynatemp_exponent);
    print_kv("top_k", top_k, MAGENTA);
    print_float("min_p", min_p);
    print_kv("tfs_z", tfs_z, MAGENTA);
    print_float("typ_p", typ_p);
    print_kv("repeat_last_n", repeat_last_n, MAGENTA);
    print_float("repeat_penalty", repeat_penalty);
    print_bool("mirostat", mirostat);
    print_float("mirostat_tau", mirostat_tau);
    print_float("mirostat_eta", mirostat_eta);
    print_bool("penalize_nl", penalize_nl);
    print_bool("ignore_eos", ignore_eos);
    print_kv("n_probs", n_probs, MAGENTA);
    print_kv("min_keep", min_keep, MAGENTA);

    print_comment("END OPTIONAL");
    print_comment("END INFERENCE PARAMETERS");
    print_comment("BEGIN MODEL LOAD PARAMETERS");
    print_comment("BEGIN REQUIRED");

    if (!engine.empty())
      print_kv("engine", engine, YELLOW);
    if (!prompt_template.empty())
      print_kv("prompt_template", prompt_template, YELLOW);

    print_comment("END REQUIRED");
    print_comment("BEGIN OPTIONAL");

    if (ctx_len != std::numeric_limits<int>::quiet_NaN())
      print_kv("ctx_len", ctx_len, MAGENTA);
    if (ngl != std::numeric_limits<int>::quiet_NaN())
      print_kv("ngl", ngl, MAGENTA);

    print_comment("END OPTIONAL");
    print_comment("END MODEL LOAD PARAMETERS");

    return oss.str();
  }
};

}  // namespace config
