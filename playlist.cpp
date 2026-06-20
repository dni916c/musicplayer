//-----------------------------------------------------------------------------
// playlist.cpp
// Implementation file for List Bi-Directional ADT
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "playlist.h"
#include <vector>
// ================READING/WRITING=====================
//append the track to the back of the list
void Playlist::add(const Track& t){

   Node* new_track = new Node(t);

   if(back_ != nullptr){

      //non-empty list
      //link the new node after the current back
      back_->next = new_track;
      new_track->prev = back_;
      back_ = new_track;

   } else{
      // empty list case, the new node is the entire list
      front_ = back_ = cursor_ = new_track;
      curIndex = 0;

   }
   length++;

}

//return the track the cursor is sitting on
Track Playlist::current(){
   if (cursor_ == nullptr) return Track{};
   return cursor_->track;
}

// the cursor's position as an integer.
// useful for highlighting the playing row
int Playlist::currentIndex(){

   return curIndex;
   
}

std::vector<Track> Playlist::allTracks() const{

   std::vector<Track> out;
   Node* p;
   for(p = front_; p != nullptr; p = p->next){

      out.push_back(p->track);
   }

   return out;

}


// ================MOVING CURSOR=====================
//advances the cursor forward
void Playlist::next(){
   cursor_ = cursor_->next;
   curIndex++;
}
//advances the cursor to the previous track
void Playlist::prev(){
   cursor_= cursor_->prev;
   curIndex--;
}

void Playlist::jumpTo(size_t index){

   Node* p = front_;

   if (index >= length) return;

   for(size_t i = 0; i < index; i++){

      p = p->next;
      
   }

   cursor_ = p;
   curIndex = index;

}
// ================STATE/SIZE=====================
//number of tracks
size_t Playlist::size(){

   return length;
}
//checks for an empty playlist
bool Playlist::isEmpty(){
   if(length == 0){
      return true;
   }

   return false;
}
// ================CONSTUCTOR/DESTRUCTOR=====================
// create a new playlist
Playlist::Playlist(){
   front_ = nullptr;
   back_ = nullptr;
   cursor_ = nullptr;
   length = 0;
   curIndex = 0;
}
// Destructor 
Playlist::~Playlist(){

   Node* p = front_;

   while(p != nullptr){
      Node* next_node = p->next;
      delete p;
      p = next_node;
   }

}
