#ifndef _MONGOLOG_HH_
#define _MONGOLOG_HH_

#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <sstream>

#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>
#include <bsoncxx/builder/stream/document.hpp>

class MongoLog{
  /*
    Logging class that writes to MongoDB
  */
  
public:
  MongoLog();
  ~MongoLog();
  
  int  Initialize(std::string connection_string,
		  std::string db, std::string collection,
		  bool debug=false);

  const static int Debug   = 0;  // Verbose output
  const static int Message = 1;  // Normal output
  const static int Warning = 2;  // Bad but minor operational impact
  const static int Error   = 3;  // Major operational impact
  const static int Fatal   = 4;  // Program gonna die

  int Entry(std::string message, int priority=Message);
  
private:  
  mongocxx::client fMongoClient;
  mongocxx::collection fMongoCollection;
  std::string fHostname;
  int fLogLevel;
};

#endif