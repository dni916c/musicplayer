#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include "track.h"
#include <vector>

class Library{

public:
   //This fills from a given directory path by the user
   void scan(const std::string& folderPath);
   // this is for the GUI display
   const std::vector<Track>& tracks() const;
   //get the i-th file to add to the  playlist
   Track at(std::size_t i);
private:
   std::vector<Track> tracks_; // vector
};

#endif

