#include <iostream>
#include <cstring>
#include <cstdio>
#include "audio_engine.h"
#include "playlist.h"

using namespace std;

int main(){

   AudioEngine engine;

   if (!engine.ok()){

      fprintf(stderr, "AudioEngine not ok()");
      return 1;
   }

   Playlist pl;
    pl.add({"/home/ubuntu/TECHNOSOUND/practice_tracks/The Turtles Elenore 1968 [f09itrlXcic].mp3", "Song A"});
    pl.add({"/home/ubuntu/TECHNOSOUND/practice_tracks/The Turtles, You Showed Me Live [vneA658BTJE].mp3", "Song B"});
   if(pl.isEmpty()){

      fprintf(stderr, "Playlist is empty");
   
      return -1;
   }

   engine.load(pl.current().path);

   cout << "now playing: " << pl.current().name << endl;

   cout << " p / n / b / q" << endl;

   cout << "pause/next/tack name/quit"<<endl;

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
