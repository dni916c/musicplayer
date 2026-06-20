//-----------------------------------------------------------------------------
// playlist.h
// Header file for List Bi-Directional ADT
// Defines the client facing interface for a doubly linked list with a cursor
// and ADT operations for the purpose of building a playlist for music player.
//-----------------------------------------------------------------------------

#ifndef _PLAYLIST_H
#define _PLAYLIST_H

#include <string>
#include <vector>


struct Track {

   std::string path;
   std::string name;
};



class Playlist{
   // private Node in Playlist
   struct Node {

      Track track;
      Node* next;
      Node* prev;
      Node(const Track& t) : track(t), next(nullptr), prev(nullptr) {}

   };

   Node* front_;
   Node* back_;
   Node* cursor_;
   size_t length;
   size_t curIndex;

   public:
      
      // ================READING/WRITING=====================
      //append the track to the back of the list
      void add(const Track& t);
      
      //return the track the cursor is sitting on
      Track current();
      
      // the cursor's position as an integer.
      // useful for highlighting the playing row
      int currentIndex();
      
      //create a reading of the current directory
      std::vector<Track> allTracks() const;
      
      // ================MOVING CURSOR=====================
      //advances the cursor forward
      void next();
      //advances the cursor to the previous track
      void prev();

      void jumpTo(size_t index);
      // ================STATE/SIZE=====================
      //number of tracks
      size_t size();
      //checks for an empty playlist
      bool isEmpty();
      // ================CONSTUCTOR/DESTRUCTOR=====================
      // create a new playlist
      Playlist();
      // Destructor 
      ~Playlist();

      Playlist(const Playlist&) = delete;
      Playlist& operator=(const Playlist&) = delete;



};



#endif
  
