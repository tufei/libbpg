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

#ifndef _BPG_ENCODER_PRIV_H
#define _BPG_ENCODER_PRIV_H

typedef struct {
    int width;
    int height;
    int chroma_format; /* 0-3 */
    int bit_depth; /* 8-14 */
    int intra_only; /* 0-1 */

    int qp; /* quantizer 0-51 */
    int lossless; /* 0-1 lossless mode */
    int sei_decoded_picture_hash; /* 0=no hash, 1=MD5 hash */
    int compress_level; /* 1-9 */
    int verbose;
} HEVCEncodeParams;

typedef struct HEVCEncoderContext HEVCEncoderContext;

typedef struct {
    HEVCEncoderContext *(*open)(const HEVCEncodeParams *params);
    int (*encode)(HEVCEncoderContext *s, Image *img);
    int (*close)(HEVCEncoderContext *s, uint8_t **pbuf);
} HEVCEncoder;

struct BPGEncoderContext {
    BPGEncoderParameters params;
    BPGMetaData *first_md;
    HEVCEncoder *encoder;
    int frame_count;
    HEVCEncoderContext *enc_ctx;
    HEVCEncoderContext *alpha_enc_ctx;
    int frame_ticks;
    uint16_t *frame_duration_tab;
    int frame_duration_tab_size;
};

extern HEVCEncoder jctvc_encoder;
extern HEVCEncoder x265_hevc_encoder;

int x265_encode_picture(uint8_t **pbuf, Image *img,
                        const HEVCEncodeParams *params);

#endif /* _BPG_ENCODER_PRIV_H */

