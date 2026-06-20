//-----------------------------------------------------------------------------
// audio_engine.h
// Owns the audio device and the current decoder, and exposes a small playback
// control surface (load / play / pause / seek / volume). Knows nothing about
// terminals, GUIs, or HTTP — callers just invoke these methods. The one subtle
// thing it manages is the boundary between the control thread (the caller) and
// miniaudio's audio thread (the data callback), guarded by a mutex.
//-----------------------------------------------------------------------------

#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <string>
#include <mutex>
#include <atomic>

#include "miniaudio.h"



class AudioEngine{

   public:
      
   // ================LIFE CYCLE=====================
   //open and start a device
   AudioEngine();
   //stops the deivce and free the decoder
   ~AudioEngine();
      

   AudioEngine(const AudioEngine&) = delete;
   AudioEngine& operator=(const AudioEngine&) = delete;
   
   //True is the device opened successfully, check right after construction
   bool ok() const {return deviceReady_;}


   // ================TRANSPORT/COMMANDS IN=====================
   //open a file as the current track
   bool load(const std::string& path);

   void play();
   void pause();
   void togglePause();

   // ================SEEKING WITHIN TRACK=====================
   void seek(double seconds);
   // ================VOLUME=====================
   void setVolume(float v);
   float volume() const;

   // ================GUI POLLS/STATE OUT=====================
   bool isPlaying() const {return playing_.load(std::memory_order_relaxed);}
   double position() const;
   double duration() const{
      return durationSeconds_.load(std::memory_order_relaxed);
   }
   //True exactly after the track ends, the control layer polls this to
   //auto save to advanced the playlist. exchange() clears it so it fire a single time
   bool consumeFinished(){
      return finished_.exchange(false, std::memory_order_relaxed);
   }
private:
   // ================OUTPUT FORMAT=====================
   //Each decoder is initialized to match these, so the callback copies 
   //frames straight into the device buffer with no consversion i nthe hot path.

   static constexpr ma_format kFormat = ma_format_f32;
   static constexpr ma_uint32 kChannels = 2;
   static constexpr ma_uint32 kRate = 48000;

   // ================CALLBACK MACHINERY=====================
   //C style callback miniaudio invokes on its audio thread; it recovers the
   //AudioENgine* from pUserData and delegates to onRead()

   static void dataCallback(ma_device* pDevice, void* pOutput,
                            const void* pInput, ma_uint32 frameCount);
   void onRead(void* pOutput, ma_uint32 frameCount);

   // ================MEMBERS=====================
   ma_device device_{};
   bool deviceReady_ = false;
   //gaurd decoder_
   std::mutex decoderMutex_;
   //owned nullptr == no track
   ma_decoder* decoder_ = nullptr;
   std::atomic<bool> playing_{false};
   //set EOF, consumed by caller 
   std::atomic<bool> finished_{false};
   //cached at load time 
   std::atomic<double> durationSeconds_{0.0};
};



#endif
