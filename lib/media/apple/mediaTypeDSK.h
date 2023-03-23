#ifndef _MEDIATYPE_DSK_
#define _MEDIATYPE_DSK_

#include <stdio.h>

#include "mediaTypeWOZ.h"

// #define MAX_TRACKS 160

// struct TRK_t
// {
//     uint16_t start_block;
//     uint16_t block_count;
//     uint32_t bit_count;
// };


class MediaTypeDSK  : public MediaTypeWOZ
{
private:
    void dsk2woz_info();
    void dsk2woz_tmap();
    bool dsk2woz_tracks(uint8_t *dsk); 

protected:
    uint8_t tmap[MAX_TRACKS];
    TRK_t trks[MAX_TRACKS];
    uint8_t *trk_ptrs[MAX_TRACKS] = { };

public:
    // virtual bool read(uint32_t blockNum, uint16_t *count, uint8_t* buffer) override { return false; };
    // virtual bool write(uint32_t blockNum, uint16_t *count, uint8_t* buffer) override { return false; };

    // virtual bool format(uint16_t *respopnsesize) override { return false; };

    virtual mediatype_t mount(FILE *f, uint32_t disksize) override;
    // mediatype_t mount(FILE *f) {return mount(f, 0);};
    // virtual void unmount() override;

    // virtual bool status() override {return (_media_fileh != nullptr);}

    // uint8_t trackmap(uint8_t t) { return tmap[t]; };
    // uint8_t *get_track(int t) { return trk_ptrs[tmap[t]]; };
    // int track_len(int t) { return trks[tmap[t]].block_count * 512; };
    // int num_bits(int t) { return trks[tmap[t]].bit_count; };

    // static bool create(FILE *f, uint32_t numBlock);
};


#endif // _MEDIATYPE_DSK_
