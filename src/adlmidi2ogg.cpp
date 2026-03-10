#include <adlmidi.h>

#include <vorbis/vorbisenc.h>
#include <ogg/ogg.h>

#include <algorithm>
#include <cfloat>
#include <fstream>
#include <vector>
#include <iostream>

// Generated with GPT
int main()
{
    const char* midiFile = "descent.hmq";
    const char* outFile  = "descent.ogg";

    const int sampleRate = 48000;
    const int channels   = 2;

    // -------------------------
    // Init ADLMIDI
    // -------------------------
    ADL_MIDIPlayer* player = adl_init(sampleRate);
    if(!player)
    {
        std::cerr << "Failed to init ADLMIDI\n";
        return 1;
    }

    if(adl_openFile(player, midiFile) < 0)
    {
        std::cerr << "Failed to load MIDI\n";
        return 1;
    }

    adl_switchEmulator(player, ADLMIDI_EMU_NUKED);
    //adl_setVolumeRangeModel(player, ADLMIDI_VolumeModel_Generic);
    adl_setBank(player, 4);
    adl_setNumChips(player, 2);
    adl_setSoftPanEnabled(player, 1);

    // -------------------------
    // Init Vorbis encoder
    // -------------------------
    vorbis_info vi;
    vorbis_info_init(&vi);

    float quality = 1.0f;

    if(vorbis_encode_init_vbr(&vi, channels, sampleRate, quality))
    {
        std::cerr << "Vorbis init failed\n";
        return 1;
    }

    vorbis_comment vc;
    vorbis_comment_init(&vc);
    vorbis_comment_add_tag(&vc, "ENCODER", "libadlmidi to vorbis");

    vorbis_dsp_state vd;
    vorbis_analysis_init(&vd, &vi);

    vorbis_block vb;
    vorbis_block_init(&vd, &vb);

    ogg_stream_state os;
    ogg_stream_init(&os, rand());

    std::ofstream out(outFile, std::ios::binary);

    // -------------------------
    // Write Vorbis headers
    // -------------------------
    ogg_packet header;
    ogg_packet header_comm;
    ogg_packet header_code;

    vorbis_analysis_headerout(&vd, &vc, &header, &header_comm, &header_code);

    ogg_stream_packetin(&os, &header);
    ogg_stream_packetin(&os, &header_comm);
    ogg_stream_packetin(&os, &header_code);

    ogg_page og;
    while(ogg_stream_flush(&os, &og))
    {
        out.write((char*)og.header, og.header_len);
        out.write((char*)og.body, og.body_len);
    }

    // -------------------------
    // Render + encode loop
    // -------------------------
    const int BUFFER_FRAMES = 1024;

    // Configure libADLMIDI to output 32-bit Float
    ADLMIDI_AudioFormat format;
    format.type = ADLMIDI_SampleType_F32;
    format.containerSize = sizeof(float);
    // Vorbis uses separate, non-interleaved arrays for L and R.
    // Therefore, the stride to the next sample in the array is just 1 float.
    format.sampleOffset  = sizeof(float);

    // Inspired by dxxrebirth which has become the "natural" sound for me now
    const auto amplify = [](float i) { return std::clamp(2.0f * i, -1.0f, 1.0f); };

    while (true)
    {
        float **buffer = vorbis_analysis_buffer(&vd, BUFFER_FRAMES);

        int samplesGenerated = adl_playFormat(player, 
                                             BUFFER_FRAMES * channels, 
                                             (ADL_UInt8*)buffer[0], // Left channel
                                             (ADL_UInt8*)buffer[1], // Right channel
                                             &format);

        if (samplesGenerated <= 0)
            break; // End of MIDI

	// Apply amplification to make it punchy
	std::transform(buffer[0], buffer[0] + samplesGenerated, buffer[0], amplify);
	std::transform(buffer[1], buffer[1] + samplesGenerated, buffer[1], amplify);

	int framesGenerated = samplesGenerated / channels;
        vorbis_analysis_wrote(&vd, framesGenerated);

        while(vorbis_analysis_blockout(&vd, &vb) == 1)
        {
            vorbis_analysis(&vb, nullptr);
            vorbis_bitrate_addblock(&vb);

            ogg_packet op;
            while(vorbis_bitrate_flushpacket(&vd, &op))
            {
                ogg_stream_packetin(&os, &op);

                while(ogg_stream_pageout(&os, &og))
                {
                    out.write((char*)og.header, og.header_len);
                    out.write((char*)og.body, og.body_len);
                }
            }
        }
    }

    vorbis_analysis_wrote(&vd, 0);

    // -------------------------
    // Cleanup
    // -------------------------
    out.close();

    ogg_stream_clear(&os);
    vorbis_block_clear(&vb);
    vorbis_dsp_clear(&vd);
    vorbis_comment_clear(&vc);
    vorbis_info_clear(&vi);

    adl_close(player);

    std::cout << "Finished writing OGG\n";
}

