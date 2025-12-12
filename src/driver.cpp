#include "driver.h"

void Driver::parseArgs(void) {
  if (this->argc < MIN_ARGS || this->argc > MAX_ARGS) 
    throw std::runtime_error("Usage: cppcc <file.c> [--lex|--parse|--codegen]");
   
  this->opts.file = std::string(this->argv[1]);

  if (this->argc > MIN_ARGS) {
    std::string flag = this->argv[2]; 
    if (flag == "--lex") this->opts.lex = true; 
    else if (flag == "--parse") this->opts.parse = true; 
    else if (flag == "--codegen") this->opts.codegen = true; 
    else
      throw std::runtime_error("Unknown option: " + flag); 
  } 
}

bool Driver::runPreprocessor(const char* input, const char* output) {
  pid_t pid = fork();   //create child for preprocessor pass 

  if (pid < 0) 
    throw std::runtime_error("Could not run GCC Preprocessor\n"); 

  if (pid == 0) { 
    //child replace process with gcc (never return on success) 
    execlp("gcc", "gcc", "-E", "-P", input, "-o", output, (char*)nullptr); 
    std::cerr << "Error starting GCC Preprocessor\n"; 
    std::exit(SYS_ERR); 
  }

  int status; 
  pid_t wpid = waitpid(pid, &status, 0); 

  if (wpid == -1) 
    throw std::runtime_error("Error in child process for GCC Preprocessor\n"); 

  return WIFEXITED(status) && WEXITSTATUS(status) == 0; 
}

bool Driver::runCPPCC(void) { 
  return true; 
}

int Driver::run(void) {
  try {
    parseArgs(); 	
   
    fs::path input_path = fs::absolute(this->opts.file); 
    if (!fs::exists(input_path))
      throw std::runtime_error("Unable to open file: " + this->opts.file); 

    fs::path base = input_path.stem(); 
    fs::path dir = input_path.parent_path(); 
    fs::path preprocessed = dir / (base.string() + ".i"); 
   
    bool success = runPreprocessor(input_path.string().c_str(), 
                                   preprocessed.string().c_str());
    if (!success) 
      throw std::runtime_error("Preprocessor failed\n"); 

    /*compile preprocessed C source file*/
    fs::path compiled = dir / (base.string() + ".s"); 
    success = runCPPCC(); 
    bool removed = fs::remove(preprocessed); 
  
    if (!removed)
      throw std::runtime_error("Could not delete .i file\n"); 

    if (!success) 
      throw std::runtime_error("Compiler failed\n"); 

  } catch (const std::exception& e) {
    std::cerr << e.what() << "\n"; 
    return SYS_ERR; 
  }
  
  return SYS_SUC; 
}

int main(int argc, char **argv) {
  Driver driver(argc, argv);  
	return driver.run();  
}
