#include <iostream>
#include <cstring>
#include <cstdio>
#include "audio_engine.h"
#include "playlist.h"
#include "library.h"

using namespace std;

int main(int argc, char** argv){

   if(argc < 2){

      cerr << "not enough arguments" << endl;
      return 1;
   }

   Library lib;
   lib.scan(argv[1]);

   Playlist pl;

   const std::vector<Track>& files = lib.tracks();
      for(size_t i = 0; i < files.size(); i++){
         pl.add(lib.at(i));
      }

   if(pl.isEmpty()){

      fprintf(stderr, "Playlist is empty");
   
      return -1;
   }


   cout <<"---LIBRARY---" << endl;
   for(size_t i = 0; i < files.size(); i++){
      cout << files[i].name << endl;
   }

   AudioEngine engine;

   if (!engine.ok()){

      fprintf(stderr, "AudioEngine not ok()");
      return 1;
   }


   engine.load(pl.current().path);

   cout << "now playing: " << pl.current().name << endl;

   cout << " p / n / b / q" << endl;

   cout << "pause/next/tack name/quit"<<endl;

   cout << "----Playlist----" << endl;

   vector<Track> tracks = pl.allTracks();
   cout << "Track count: " << tracks.size() << endl;
   
   for(size_t i = 0; i < tracks.size(); i++){

      cout<< i + 1 << ": " <<tracks[i].name << endl;
   }

   if(!engine.load(pl.current().path)){

      cout << "failed to load: " << pl.current().name << endl;
   }


   string line;
   while(getline(cin, line)){

      if(line == "q"){
         break;
      }
      else if(line == "p"){

         engine.togglePause();
         cout << (engine.isPlaying() ? "resumed": "paused") << endl;
      }
      else if(line == "n"){

         pl.next();
         engine.load(pl.current().path);
         cout<<"Now playing: " << pl.current().name << endl;
      }
      else if (line == "b"){

         pl.prev();
         engine.load(pl.current().path);
         cout << "Now playing: " << pl.current().name << endl;
      }

      else{

         cout << "Unknown command" << endl;
      }

   }

   return 0;
   
}
