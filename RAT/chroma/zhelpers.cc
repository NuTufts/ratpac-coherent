#include "zhelpers.hpp"

namespace zhelpers {
  
  std::string s_recv (zmq::socket_t & socket) {
    
    zmq::message_t message;
    socket.recv(&message);
    
    return std::string(static_cast<char*>(message.data()), message.size());
  }

  bool s_recv_multipart (zmq::socket_t & socket, std::vector<std::string>& msgs) {
    
    int more = 1;
    size_t more_size = sizeof (more);
    bool ok = true;
    while (more) {
      zmq::message_t message;
      try {
	ok = socket.recv(&message);
	msgs.push_back( std::string(static_cast<char*>(message.data()), message.size()) );
	socket.getsockopt (ZMQ_RCVMORE, &more, &more_size);
      }
      catch( zmq::error_t& e ) {
	std::cout << "caught error: " << std::endl;
      }
    }
    return ok;
  }

  bool s_send (zmq::socket_t & socket, const std::string & string) {

    zmq::message_t message(string.size());
    memcpy (message.data(), string.data(), string.size());
    
    bool rc = socket.send (message);
    return (rc);
  }

  bool s_sendmore (zmq::socket_t & socket, const std::string & string) {
    zmq::message_t message(string.size());
    memcpy (message.data(), string.data(), string.size());
    bool rc = socket.send (message, ZMQ_SNDMORE);
    return (rc);
  }

}
