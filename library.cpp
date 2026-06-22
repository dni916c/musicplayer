#include <vector>
#include <string>
#include "library.h"
#include <iostream>

#include <filesystem>

namespace fs = std::filesystem;

//This fills from a given directory path by the user
void Library::scan(const std::string& folderPath){

   tracks_.clear(); //reset in case of re-scan

   fs::path filePath{folderPath};
   //confirming file and file path from directory
   if(!fs::is_directory(filePath) || !fs::exists(filePath)){
   
      std::cerr<<"the directory or file does not exist"<< std::endl;
      return;
   }

   for(const auto& entry : fs::directory_iterator(folderPath)){

      if(!entry.is_regular_file()) continue;

      std:: string ext = entry.path().extension().string();
      if (ext == ".mp3" || ext == ".flac" || ext == ".wav"){

         Track t;
         t.path = entry.path().string();
         t.name = entry.path().filename().string();
         tracks_.push_back(t);
      }
   }
}

const std::vector<Track>& Library::tracks() const{

   return tracks_;

}
//gives the actual file name in the selected directory 
Track Library::at(std::size_t i){

   return tracks_[i];
}


