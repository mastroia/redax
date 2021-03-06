#ifndef _STRAXINSERTER_HH_
#define _STRAXINSERTER_HH_

#include <cstdlib>
#include <cstdint>
#include <string>

//for debugging
//#include <sys/types.h>
#include <map>
#include <mutex>
#include <experimental/filesystem>

class DAQController;
class Options;
class MongoLog;

struct data_packet{
  u_int32_t *buff;
  int32_t size;
  u_int32_t clock_counter;
  u_int32_t header_time;
  int bid;
};


class StraxInserter{
  /*
    Reformats raw data into strax format
  */
  
public:
  StraxInserter();
  ~StraxInserter();
  
  int  Initialize(Options *options, MongoLog *log, 
		  DAQController *dataSource, std::string hostname);
  void Close(std::map<int,int>& ret);
  
  int ReadAndInsertData();
  bool CheckError(){ return fErrorBit; };
  
private:
  void ParseDocuments(data_packet dp);
  void WriteOutFiles(int smallest_index_seen, bool end=false);
  
  std::experimental::filesystem::path GetFilePath(std::string id, bool temp);
  std::experimental::filesystem::path GetDirectoryPath(std::string id, bool temp);
  std::string GetStringFormat(int id);
  void CreateMissing(u_int32_t back_from_id);
  int fMissingVerified;
  
  u_int64_t fChunkLength; // ns
  u_int32_t fChunkOverlap; // ns
  u_int16_t fFragmentLength; // This is in BYTES
  u_int16_t fStraxHeaderSize; // in BYTES too
  u_int32_t fChunkNameLength;
  std::string fOutputPath, fHostname;
  int fFirmwareVersion;
  Options *fOptions;
  MongoLog *fLog;
  DAQController *fDataSource;
  bool fActive;
  bool fErrorBit;
  std::string fCompressor;
  std::map<std::string, std::string*> fFragments;
  int fBoardFailCount;
  std::map<std::string, int>fFmt;
  std::map<int, int> fFailCounter;
};

#endif
