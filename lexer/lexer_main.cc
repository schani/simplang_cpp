#undef GOOGLE_STRIP_LOG
#define GOOGLE_STRIP_LOG 1
#include <gflags/gflags.h>
#include <glog/logging.h>

#include "lexer.h"

DEFINE_bool(verbose, true, "Enable verbose output");
DEFINE_string(file, "", "File to scan");

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (FLAGS_verbose) {
    LOG(INFO) << "Verbose output enabled.";
#undef GOOGLE_STRIP_LOG
#define GOOGLE_STRIP_LOG 0
  }
  if (FLAGS_file.empty()) {
    LOG(ERROR) << "No file provided";
    return 1;
  }
  simp::Lexer lexer{FLAGS_file};
  bool success = lexer.scan();
  if (!success) {
    LOG(ERROR) << "Failed to scan file";
    return 1;
  }
  lexer.print_tokens();

  return 0;
}
