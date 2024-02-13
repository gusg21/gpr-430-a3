#include <iostream>
#include <sstream>
#include <string>

#include "socklib.h"

std::string do_client(std::istream &in_stream);
std::string build_string(std::istream &in_stream);
int run_test();

int main(int argc, char *argv[]) {
  SockLibInit();

  return run_test();
}

std::string do_client(std::istream &in_stream) {
  Socket* socket = new Socket();
  socket->Create(Socket::Family::INET, Socket::Type::STREAM);
  socket->Connect(Address("68.183.63.165"), 7778);
  std::cout << "Connected" << std::endl;

  std::string to_send = build_string(in_stream);
  socket->Send(to_send.c_str(), to_send.length());

  constexpr int response_size = 1024;
  char response[response_size] = { 0 };
  socket->Recv(response, response_size - 1);

  return std::string(response);
}

std::string build_string(std::istream &in_stream) {
  std::string line;
  std::stringstream output_sstream;
  output_sstream << "LIST ";
  while (!in_stream.fail()) {
    std::getline(in_stream, line);

    std::cout << "<" << line << ">" << std::endl;

    bool has_alpha = false;
    for (char c : line) {
      if (std::isalpha(c)) {
        std::cout << "\tContains alpha...";
        std::cout << "\tNOT PARSED" << std::endl;
        has_alpha = true;
        break;
      }
    }
    if (has_alpha) continue;

    try {
      output_sstream << std::stof(line) << " ";
      std::cout << "\tPARSED" << std::endl;
      continue;
    }
    catch (std::invalid_argument const& ex) {
      std::cout << "\tNot an float..." << std::endl;
    }

    try {
      output_sstream << std::stoi(line) << " ";
      std::cout << "\tPARSED" << std::endl;
      continue;
    }
    catch (std::invalid_argument const& ex) {
      std::cout << "\tNot an int..." << std::endl;
    }

    std::cout << "\tNOT PARSED" << std::endl;
    
  }
  std::cout << output_sstream.str() << std::endl;
  return output_sstream.str();
}

int run_test() {
  std::stringstream input("30\n500\n-12\n3.6\n"
			  "not a number\n200also not a number\n"
			  "88.1\ndone");

  std::cout << "Testing on input '" << input.str() << "'.\n";

  std::string msg = do_client(input);
  std::string expected("SORTED -12 3.6 30 500 88.1");
  std::cout << "do_client() returned '" << msg << "'.\n";

  if (msg != expected) {
    std::cout << "TEST FAILED. Expected '" << expected << "' but was '" << msg
              << "'.\n";
    return 1;
  }
  
  std::cout << "Recieved expected message '" << msg << "'. Test passed 😄\n";
  return 0;
}
