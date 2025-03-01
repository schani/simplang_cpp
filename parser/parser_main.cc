#undef GOOGLE_STRIP_LOG
#define GOOGLE_STRIP_LOG 1
#include <gflags/gflags.h>
#include <glog/logging.h>

#include "parser.h"

DEFINE_bool(verbose, true, "Enable verbose output");
DEFINE_string(file, "", "File to parse");

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
  simp::Parser parser(FLAGS_file);
  bool success = parser.parse();
  if (!success) {
    LOG(ERROR) << "Failed to parse file";
    return 1;
  }
  parser.print_expressions();

  return 0;
}
