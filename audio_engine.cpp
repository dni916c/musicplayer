#include "audio_engine.h"
#include <cstring>


//====================LIFECYCLE========================
AudioEngine::AudioEngine(){

   ma_device_config cfg = ma_device_config_init(ma_device_type_playback);
   cfg.playback.format = kFormat;
   cfg.playback.channels = kChannels;
   cfg.sampleRate = kRate;
   cfg.dataCallback = &AudioEngine::dataCallback;
   cfg.pUserData = this;

   if(ma_device_init(nullptr, &cfg, &device_) != MA_SUCCESS){

      return;
   }
   //if deviceReady_ stays false
   deviceReady_ = true;

   if(ma_device_start(&device_) != MA_SUCCESS){

      ma_device_uninit(&device_);
      deviceReady_ = false;
   }
   

}

AudioEngine::~AudioEngine(){

   //uninit the device first - it stope and joins the audio thread, so the
   //call back cant be mid read whent we free the decoder
   if(deviceReady_){

      ma_device_uninit(&device_);
   }

   std::lock_guard<std::mutex> lock(decoderMutex_);
   if(decoder_){

      ma_decoder_uninit(decoder_);
      delete decoder_;
      decoder_ = nullptr;
   }

}
//====================CALLBACK========================
void AudioEngine::dataCallback(ma_device* pDevice, void*pOutput,
                               const void* pInput, ma_uint32 frameCount){


   (void)pInput;
   static_cast<AudioEngine*>(pDevice->pUserData)->onRead(pOutput,frameCount);
}

void AudioEngine::onRead(void* pOutput, ma_uint32 frameCount){

      const ma_uint32 bytes = 
         frameCount * ma_get_bytes_per_frame(kFormat, kChannels);
      
      std::memset(pOutput, 0,bytes);//silence by default

      if(!playing_.load(std::memory_order_relaxed)){
         return;
      }

      std::lock_guard<std::mutex> lock(decoderMutex_);
      
      if(decoder_ == nullptr){
         return;
      }

      ma_uint64 framesRead = 0;
      ma_decoder_read_pcm_frames(decoder_,pOutput, frameCount, &framesRead);

      if(framesRead <frameCount){

         finished_.store(true, std::memory_order_relaxed);
      }
   }

//====================TRANSPORT========================

bool AudioEngine::load(const std:: string& path){

   ma_decoder_config cfg = ma_decoder_config_init(kFormat, kChannels,kRate);

   ma_decoder* fresh = new ma_decoder;

   if(ma_decoder_init_file(path.c_str(), &cfg, fresh) != MA_SUCCESS){

      delete fresh;

      return false;

   }
// cache the length(do it once for effeciency)
   ma_uint64 lenFrame= 0;

   ma_decoder_get_length_in_pcm_frames(fresh, &lenFrame);
   durationSeconds_.store(static_cast<double>(lenFrame)/ kRate, 
                          std::memory_order_relaxed);
   ma_decoder* old = nullptr;
   {

      std::lock_guard<std::mutex> lock(decoderMutex_);
      old = decoder_;
      decoder_ = fresh;
   }

   if(old){

      ma_decoder_uninit(old);
      delete old;
   }

   finished_.store(false, std::memory_order_relaxed);
   playing_.store(true, std::memory_order_relaxed);

   return true;
}

void AudioEngine::play(){

   playing_.store(true, std::memory_order_relaxed);
}

void AudioEngine::pause(){

   playing_.store(false, std::memory_order_relaxed);
}

void AudioEngine::togglePause() {

   //only the control thread writes playing_, so load then stoe is race free
   
   playing_.store(!playing_.load(std::memory_order_relaxed),
                  std::memory_order_relaxed);
}

//====================SEEKING========================
void AudioEngine::seek(double seconds){

   if(seconds < 0.0) seconds = 0.0;

   std::lock_guard<std::mutex> lock(decoderMutex_);
   if(!decoder_) return;
   ma_decoder_seek_to_pcm_frame(decoder_,
         static_cast<ma_uint64>(seconds*kRate));
   finished_.store(false, std::memory_order_relaxed);

}


//====================VOLUME========================

void AudioEngine:: setVolume(float v){

   if(deviceReady_){

      ma_device_set_master_volume(&device_, v);
   }
}

float AudioEngine::volume () const{

   float v = 1.0f;
   if(deviceReady_){

      ma_device_get_master_volume(const_cast<ma_device*>(&device_), &v);
   }

   return v;
}



//====================STATE========================


double AudioEngine::position() const{

   std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(decoderMutex_));
   if(!decoder_) return 0.0;
   ma_uint64 cursor = 0;
   ma_decoder_get_cursor_in_pcm_frames(decoder_, &cursor);
   return static_cast<double>(cursor)/kRate;
}
