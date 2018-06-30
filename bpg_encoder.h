/*
 * BPG encoder
 * 
 * Copyright (c) 2014 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef _BPG_ENCODER_H
#define _BPG_ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libbpg.h"

typedef struct BPGEncoderContext BPGEncoderContext;

typedef struct {
    int w, h;
    BPGImageFormatEnum format; /* x_VIDEO values are forbidden here */
    uint8_t c_h_phase; /* 4:2:2 or 4:2:0 : give the horizontal chroma
                          position. 0=MPEG2, 1=JPEG. */
    uint8_t has_alpha;
    uint8_t has_w_plane;
    uint8_t limited_range;
    uint8_t premultiplied_alpha;
    BPGColorSpaceEnum color_space;
    uint8_t bit_depth;
    uint8_t pixel_shift; /* (1 << pixel_shift) bytes per pixel */
    uint8_t *data[4];
    int linesize[4];
} Image;

typedef struct BPGEncoderInfo {
    uint8_t default_bitdepth;
    uint8_t max_bitdepth;
    uint8_t default_compress_level;
    uint8_t default_qp;
    uint8_t num_hevc_encoders;
    char hevc_encoder[16][32];
} BPGEncoderInfo;

typedef struct BPGEncoderParameters {
    int qp; /* 0 ... 51 */
    int alpha_qp; /* -1 ... 51. -1 means same as qp */
    int lossless; /* true if lossless compression (qp and alpha_qp are
                     ignored) */
    BPGImageFormatEnum preferred_chroma_format;
    int sei_decoded_picture_hash; /* 0, 1 */
    int compress_level; /* 1 ... 9 */
    int verbose;
    int encoder_type; /* reserved */
    int animated; /* 0 ... 1: if true, encode as animated image */
    uint16_t loop_count; /* animations: number of loops. 0=infinite */
    /* animations: the frame delay is a multiple of
       frame_delay_num/frame_delay_den seconds */
    uint16_t frame_delay_num;
    uint16_t frame_delay_den;
} BPGEncoderParameters;

typedef struct BPGMetaData {
    uint32_t tag;
    uint8_t *buf;
    int buf_len;
    struct BPGMetaData *next;
} BPGMetaData;

BPGMetaData *bpg_md_alloc(uint32_t tag);

void bpg_md_free(BPGMetaData *md);

void bpg_encoder_query_info(BPGEncoderInfo *info);

typedef int BPGEncoderWriteFunc(void *opaque, const uint8_t *buf, int buf_len);

BPGEncoderParameters *bpg_encoder_param_alloc(void);

void bpg_encoder_param_free(BPGEncoderParameters *p);

BPGEncoderContext *bpg_encoder_open(BPGEncoderParameters *p);

void bpg_encoder_set_extension_data(BPGEncoderContext *s, BPGMetaData *md);

int bpg_encoder_set_frame_duration(BPGEncoderContext *s, int frame_ticks);

int bpg_encoder_encode(BPGEncoderContext *s, Image *img, BPGEncoderWriteFunc *write_func, void *opaque);

void bpg_encoder_close(BPGEncoderContext *s);

#ifdef __cplusplus
}
#endif

#endif /* _BPG_ENCODER_H */

