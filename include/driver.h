#ifndef DRIVER_H
#define DRIVER_H

#include <iostream> 
#include <filesystem>
#include <fstream>
#include <sys/wait.h>
#include <vector>

namespace fs = std::filesystem; 

static const int SYS_SUC = 0; 
static const int SYS_ERR = 1; 

static const int MIN_ARGS = 2; 
static const int MAX_ARGS = 3; 

struct DriverOptions {
  std::string file; 
  bool lex = false; 
  bool parse = false;
  bool codegen = false; 
}; 

class Driver {
  public: 
    Driver(int &argc, char **argv) : argc(argc), argv(argv) {}

    int run(void); 

  private:  
    void parseArgs(void); 
    bool runPreprocessor(const char* input, const char* output); 
    bool runCPPCC(void); 
    
    int argc; 
    char **argv; 
    DriverOptions opts; 
};

#endif
