#include <adlmidi.h>

#include <vorbis/vorbisenc.h>
#include <ogg/ogg.h>

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

    adl_setBank(player, 4);
    adl_setNumChips(player, 2);

    // -------------------------
    // Init Vorbis encoder
    // -------------------------
    vorbis_info vi;
    vorbis_info_init(&vi);

    float quality = 0.5f;

    if(vorbis_encode_init_vbr(&vi, channels, sampleRate, quality))
    {
        std::cerr << "Vorbis init failed\n";
        return 1;
    }

    vorbis_comment vc;
    vorbis_comment_init(&vc);
    vorbis_comment_add_tag(&vc, "ENCODER", "libadlmidi 24bit vorbis");

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

    const int BUFFER = 1024;
    // Size is BUFFER frames * 2 channels = 2048 shorts total
    std::vector<short> pcm(BUFFER * channels);

    int samplesGenerated = 0;

    // Pass BUFFER * channels to request a full buffer of shorts
    while((samplesGenerated = adl_play(player, BUFFER * channels, pcm.data())) > 0)
    {
        // Convert the total number of shorts generated into stereo frames
        int framesGenerated = samplesGenerated / channels;

        // Ask Vorbis ONLY for the frames we actually generated
        float **buffer = vorbis_analysis_buffer(&vd, framesGenerated);

        for(int i = 0; i < framesGenerated; i++)
        {
            buffer[0][i] = pcm[i*2]     / 32768.f;
            buffer[1][i] = pcm[i*2 + 1] / 32768.f;
        }

        // Tell Vorbis exactly how many frames we wrote
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
