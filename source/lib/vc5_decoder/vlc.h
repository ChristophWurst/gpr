/*! @file vlc.h
 *
 *  @brief Declaration of the data structures for variable-length decoding
 *
 *  @version 1.0.0
 *
 *  (C) Copyright 2018 GoPro Inc (http://gopro.com/).
 *
 *  Licensed under either:
 *  - Apache License, Version 2.0, http://www.apache.org/licenses/LICENSE-2.0
 *  - MIT license, http://opensource.org/licenses/MIT
 *  at your option.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef VLC_H
#define VLC_H

// Codewords for the sign bits that follow a non-zero value
#define VLC_POSITIVE_CODE 0x0		//!< Code that indicates a positive value
#define VLC_NEGATIVE_CODE 0x1		//!< Code that indicates a negative value
#define VLC_SIGNCODE_SIZE 1			//!< Size of the code for sign suffix

/*!
	@brief Codebook entries for arbitrary runs and values

	The codebook data structure allows runs of an arbitrary value,
	but all codec implementations only use runs of zeros.  The
	codeword for a non-zero value is followed by the sign bit.

	@todo Could add the sign bit to the magnitude entries in this
	table if it improves performance or makes the code more clear.
*/
typedef struct _rlv {
	uint_fast8_t size;		//!< Size of code word in bits
	uint32_t bits;			//!< Code word bits right justified
	uint32_t count;			//!< Run length
	int32_t value;			//!< Run value (unsigned)
} RLV;

/*!
	@brief Declaration of a codebook

	This data structure is often called the master codebook to distinguish
	it from the encoding tables that are derived from the codebook.  The
	codebook has a header that is immediately followed by the codebook entries.
	Each entry is an @ref RLV data structure that contains the codeword and
	the size of the codeword in bits.  Each codeword represent a run length
	and value.  The current codec implementation only supports runs of zeros,
	so the run length is one for non-zero values.  A non-zero value is an
	unsigned coefficient magnitude.  Special codewords that mark significant
	locations in the bitstream are indicated by a run length of zero and the
	value indicates the type of marker.

	The codebook is generated by a separate program that takes as input a table
	of the frequencies of coefficient magnitudes and runs of zeros.
*/
typedef struct _codebook
{
	uint32_t length;		//!< Number of entries in the code book
							// The length is followed by the RLV entries
} CODEBOOK;

//! Macro used to define the codebook generated by the Huffman program
#define RLVTABLE(n)			\
	static struct			\
	{						\
		uint32_t length;	\
		RLV entries[n];		\
	}

/*!
	@brief Structure returned by the run length decoding routines

	The value returned may be signed if the routine that was called
	to parse the bitstream found a run of a non-zero value and then
	parsed the sign bit that follows the magnitude.
*/
typedef struct _run {
	uint32_t count;			//!< Run length count
	int32_t value;			//!< Run length value
} RUN;

//! Initializer for the run length data structure
#define RUN_INITIALIZER		{0, 0}

#ifdef __cplusplus
extern "C" {
#endif

    CODEC_ERROR GetRlv(BITSTREAM *stream, CODEBOOK *codebook, RUN *run);
    CODEC_ERROR GetRun(BITSTREAM *stream, CODEBOOK *codebook, RUN *run);

#ifdef __cplusplus
}
#endif

#endif // VLC_H
