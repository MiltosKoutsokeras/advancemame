/*
 * This file is part of the Advance project.
 *
 * Copyright (C) 1999-2002 Andrea Mazzoleni
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * In addition, as a special exception, Andrea Mazzoleni
 * gives permission to link the code of this program with
 * the MAME library (or with modified versions of MAME that use the
 * same license as MAME), and distribute linked combinations including
 * the two.  You must obey the GNU General Public License in all
 * respects for all of the code used other than MAME.  If you modify
 * this file, you may extend this exception to your version of the
 * file, but you are not obligated to do so.  If you do not wish to
 * do so, delete this exception statement from your version.
 */

/** \file
 * Color.
 */

#ifndef __COLOR_H
#define __COLOR_H

#include "extra.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup Color */
/*@{*/

/**
 * RGB color.
 */
typedef struct adv_color_rgb_struct {
	uint8 blue __attribute__ ((packed)); /**< Blue channel. From 0 to 255. */
	uint8 green __attribute__ ((packed)); /**< Green channel. From 0 to 255. */
	uint8 red __attribute__ ((packed)); /**< Red channel. From 0 to 255. */
	uint8 alpha __attribute__ ((packed)); /**< Alpha channel. From 0 to 255. */
} adv_color_rgb;

/**
 * YUV color.
 */
typedef struct adv_color_yuv_struct {
	uint8 y __attribute__ ((packed)); /**< Blue channel. From 0 to 255. */
	uint8 u __attribute__ ((packed)); /**< Green channel. From 0 to 255. */
	uint8 v __attribute__ ((packed)); /**< Red channel. From 0 to 255. */
	uint8 alpha __attribute__ ((packed)); /**< Alpha channel. From 0 to 255. */
} adv_color_yuv;

/**
 * Color RGB/YUV as ordinal value.
 * The effective format is not defined. It depends on the current context.
 */
typedef unsigned adv_pixel;

typedef enum adv_color_type_enum {
	adv_color_type_unknown = 0, /**< Unknown. */
	adv_color_type_palette = 1, /**< Palette. */
	adv_color_type_rgb = 2, /**< RGB. */
	adv_color_type_yuy2 = 3, /**< YUY2 (2 sample). */
	adv_color_type_text = 4 /**< Text. */
} adv_color_type;

/**
 * RGB/YUV definition as bit nibble.
 */
struct adv_color_def_bits {
	unsigned type : 3; /**< Type bit. */
	unsigned red_len : 4; /**< Bits for the red channel. */
	unsigned red_pos : 5; /**< Shift for the red channel. */
	unsigned green_len : 4; /**< Bits for the green channel. */
	unsigned green_pos : 5; /**< Shift for the green channel. */
	unsigned blue_len : 4; /**< Bits for the blue channel. */
	unsigned blue_pos : 5; /**< Shift for the blue channel. */
	unsigned alpha_size : 2; /**< Extra alpha bytes. */
	/* 3+5*3+4*3+2 = 32 bit */
};

/**
 * Color definition as ordinal value.
 */
typedef unsigned adv_color_def;

/**
 * color definition as union.
 */
union adv_color_def_union {
	adv_color_def ordinal;
	struct adv_color_def_bits nibble;
};

const char* color_def_name_make(adv_color_def rgb_def);
adv_color_def color_def_make(adv_color_type type);
adv_color_def color_def_make_from_rgb_sizelenpos(unsigned bytes_per_pixel, unsigned red_len, unsigned red_pos, unsigned green_len, unsigned green_pos, unsigned blue_len, unsigned blue_pos);
adv_color_def color_def_make_from_rgb_lenpos(unsigned red_len, unsigned red_pos, unsigned green_len, unsigned green_pos, unsigned blue_len, unsigned blue_pos);
adv_color_def color_def_make_from_rgb_maskshift(unsigned red_mask, int red_shift, unsigned green_mask, int green_shift, unsigned blue_mask, int blue_shift);
adv_color_def color_def_make_from_index(unsigned index);
adv_pixel pixel_make_from_def(unsigned r, unsigned g, unsigned b, adv_color_def def);

/**
 * Get a channel shift from the RGB definition.
 * This value is the number of bit to shift left a 8 bit channel value to match
 * the specified RGB definition. It may be a negative number.
 */
static inline int rgb_shift_make_from_def(unsigned len, unsigned pos)
{
	return pos + len - 8;
}

/**
 * Get a channel mask from the RGB definition.
 * This value is the mask bit of the specified channel RGB definition. 
 */
static inline unsigned rgb_mask_make_from_def(unsigned len, unsigned pos)
{
	return ((1 << len) - 1) << pos;
}

unsigned video_color_dist(const adv_color_rgb* A, const adv_color_rgb* B);

/**
 * Shift a value.
 * \param value Value to shift.
 * \param shift Number of bit to shift right. If negative the value is shifted left.
 */
static inline unsigned rgb_shift(unsigned value, int shift)
{
	if (shift >= 0)
		return value >> shift;
	else
		return value << -shift;
}

/**
 * Convert a 8 bit channel to a specific subformat channel.
 * \param value 8 bit channel.
 * \param shift Shift for the channel. Generally computed with rgb_shift_make_from_def().
 * \param mask Mask for the channel. Generally computed with rgb_mask_make_from_def().
 */
static inline unsigned rgb_nibble_insert(unsigned value, int shift, unsigned mask)
{
	return rgb_shift(value, -shift) & mask;
}

/**
 * Convert a specific subformat channel to a 8 bit channel.
 * \param value Subformat channel.
 * \param shift Shift for the channel. Generally computed with rgb_shift_make_from_def().
 * \param mask Mask for the channel. Generally computed with rgb_mask_make_from_def().
 */
static inline unsigned rgb_nibble_extract(unsigned value, int shift, unsigned mask)
{
	return rgb_shift(value & mask, shift);
}

unsigned rgb_approx(unsigned value, unsigned len);

/**
 * Compute the shift and mask values.
 */
static inline void rgb_maskshift_get(unsigned* mask, int* shift, unsigned len, unsigned pos)
{
	*mask = ((1 << len) - 1) << pos;
	*shift = pos + len - 8;
}

/*@}*/

#ifdef __cplusplus
}
#endif

#endif