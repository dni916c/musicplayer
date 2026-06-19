/*
 *
 *  - THIS is the tester for the audio playback on the most simple level
 *  I will go through each function as needed.
 *  right not this is in the most simple state
 *
 */

#define MINIAUDIO_IMPLEMENTATION

#include <stdio.h>
#include "miniaudio.h"

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
   (void)pInput; // playback only
   ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
   ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
}

int main(int argc, char** argv)
{
   if(argc < 2){

      printf("usage %s <audiofile>\n", argv[0]);
      return -1;


   }
   //open file once into a decoder, Init it to the same format we give
   // the device below
   ma_decoder decoder;
   ma_decoder_config decoderConfig = ma_decoder_config_init(ma_format_f32, 2, 48000);

   if(ma_decoder_init_file(argv[1],&decoderConfig, &decoder) != MA_SUCCESS ){
      printf("Could not open file\n");
      return -1;
   }

   ma_device_config config = ma_device_config_init(ma_device_type_playback);
   config.playback.format   = ma_format_f32;   // Set to ma_format_unknown to use the device's native format.
   config.playback.channels = 2;               // Set to 0 to use the device's native channel count.
   config.sampleRate        = 48000;           // Set to 0 to use the device's native sample rate.
   config.dataCallback      = data_callback;   // This function will be called when miniaudio needs more data.
   config.pUserData         = &decoder;   // Can be accessed from the device object (device.pUserData).

   ma_device device;
   if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) {
      return -1;  // Failed to initialize the device.
   }

   // start the device
   ma_device_start(&device);     // The device is sleeping by default so you'll need to start it manually.
   printf("playing track.. press ENTER to stop\n");
   getchar();
    // Do something here. Probably your program's main loop.

   ma_device_uninit(&device);
   ma_decoder_uninit(&decoder);
   return 0;
}
