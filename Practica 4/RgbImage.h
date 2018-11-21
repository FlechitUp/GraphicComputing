/*
 * RgbImage.h
 *
 * Read an image from a bitmap (.bmp) file.  The functionality is
 * encapsulated in a class, RgbImage.
 *
 * An RgbImage object holds an array of RGB values in byte format
 * Main functionality is to read an RGB image from a BMP (bitmap) file
 * The image data rows will be word aligned.
 *
 * Author: Samuel R. Buss
 *
 * Software accompanying the book.
 *		3D Computer Graphics: A Mathematical Introduction with OpenGL,
 *		by S. Buss, Cambridge University Press, 2003.
 *
 * Software is "as-is" and carries no warranty.  It may be used without
 *   restriction, but if you modify it, please change the filenames to
 *   prevent confusion between different versions.
 * Bug reports: Sam Buss, sbuss@ucsd.edu.
 * Web page: http://math.ucsd.edu/~sbuss/MathCG
 *
 */

#ifndef RGBIMAGE_H
#define RGBIMAGE_H

#include <stdio.h>

class RgbImage
{
public:
	RgbImage();
	RgbImage( const char* filename );
	~RgbImage();

	bool LoadBmpFile( const char *filename );

	long GetNumRows() const { return NumRows; }
	long GetNumCols() const { return NumCols; }
	// Rows are word aligned
	long GetNumBytesPerRow() const { return ((3*NumCols+3)>>2)<<2; }
	const void* ImageData() const { return (void*)ImagePtr; }

	const unsigned char* GetRgbPixel( long row, long col ) const;
	void GetRgbPixel( long row, long col, float* red, float* green, float* blue ) const;
	void GetRgbPixel( long row, long col, double* red, double* green, double* blue ) const;

	// Error reporting. (errors also print message to stderr)
	int GetErrorCode() const { return ErrorCode; }
	enum {
		NoError = 0,
		OpenError = 1,			// Unable to open file for reading
		FileFormatError = 2,	// Not recognized as a 24 bit BMP file
		MemoryError = 3,		// Unable to allocate memory for image data
		ReadError = 4			// End of file reached prematurely
	};
	bool ImageLoaded() const { return (ImagePtr!=0); }  // Is an image loaded?

	void Reset();			// Frees image data memory

private:
	unsigned char* ImagePtr;	// array of pixel values (integers range 0 to 255)
	long NumRows;				// number of rows in image
	long NumCols;				// number of columns in image
	int ErrorCode;				// error code

	static short readShort( FILE* infile );
	static long readLong( FILE* infile );
	static void skipChars( FILE* infile, int numChars );

};


inline RgbImage::RgbImage()
{
	NumRows = 0;
	NumCols = 0;
	ImagePtr = 0;
	ErrorCode = 0;
}

bool RgbImage::LoadBmpFile( const char* filename )
{
Reset();
FILE* infile = fopen( filename, "rb" );	// Open for reading binary data
if ( !infile ) {
fprintf(stderr, "Unable to open file: %s\n", filename);
ErrorCode = OpenError;
return false;
}

bool fileFormatOK = false;
int bChar = fgetc( infile );
int mChar = fgetc( infile );
if ( bChar=='B' && mChar=='M' ) {	// If starts with "BM" for "BitMap"
skipChars( infile, 4+2+2+4+4 );	// Skip 4 fields we don't care about
NumCols = readLong( infile );
NumRows = readLong( infile );
skipChars( infile, 2 );	// Skip one field
int bitsPerPixel = readShort( infile );
skipChars( infile, 4+4+4+4+4+4 );	// Skip 6 more fields

if ( NumCols>0 && NumCols<=100000 && NumRows>0 && NumRows<=100000
&& bitsPerPixel==24 && !feof(infile) ) {
fileFormatOK = true;
}
}
if ( !fileFormatOK ) {
Reset();
ErrorCode = FileFormatError;
fprintf(stderr, "Not a valid 24-bit bitmap file: %s.\n", filename);
fclose ( infile );
return false;
}

// Allocate memory
ImagePtr = new unsigned char[NumRows*GetNumBytesPerRow()];
if ( !ImagePtr ) {
fprintf(stderr, "Unable to allocate memory for %ld x %ld bitmap: %s.\n",
NumRows, NumCols, filename);
Reset();
ErrorCode = MemoryError;
fclose ( infile );
return false;
}

unsigned char* cPtr = ImagePtr;
for ( int i=0; i<NumRows; i++ ) {
    int j;
    for ( j=0; j<NumCols; j++ ) {
        *(cPtr+2) = fgetc( infile );	// Blue color value
        *(cPtr+1) = fgetc( infile );	// Green color value
        *cPtr = fgetc( infile );	// Red color value
        cPtr += 3;
    }
    int k=3*NumCols;	// Num bytes already read
    for ( ; k<GetNumBytesPerRow(); k++ ) {
        fgetc( infile );	// Read and ignore padding;
        *(cPtr++) = 0;
    }
}
if ( feof( infile ) ) {
    fprintf( stderr, "Premature end of file: %s.\n", filename );
    Reset();
    ErrorCode = ReadError;
    fclose ( infile );
    return false;
}
fclose( infile );	// Close the file
return true;
}

short RgbImage::readShort( FILE* infile )
{
// read a 16 bit integer
unsigned char lowByte, hiByte;
lowByte = fgetc(infile);	// Read the low order byte (little endian form)
hiByte = fgetc(infile);	// Read the high order byte

// Pack together
short ret = hiByte;
ret <<= 8;
ret |= lowByte;
return ret;
}

long RgbImage::readLong( FILE* infile )
{
// Read in 32 bit integer
unsigned char byte0, byte1, byte2, byte3;
byte0 = fgetc(infile);	// Read bytes, low order to high order
byte1 = fgetc(infile);
byte2 = fgetc(infile);
byte3 = fgetc(infile);

// Pack together
long ret = byte3;
ret <<= 8;
ret |= byte2;
ret <<= 8;
ret |= byte1;
ret <<= 8;
ret |= byte0;
return ret;
}

void RgbImage::skipChars( FILE* infile, int numChars )
{
for ( int i=0; i<numChars; i++ ) {
fgetc( infile );
}
}


inline RgbImage::RgbImage( const char* filename )
{
	NumRows = 0;
	NumCols = 0;
	ImagePtr = 0;
	ErrorCode = 0;
	LoadBmpFile( filename );
}



inline RgbImage::~RgbImage()
{
	delete[] ImagePtr;
}

inline const unsigned char* RgbImage::GetRgbPixel( long row, long col ) const
{
	const unsigned char* ret = ImagePtr;
	long i = row*GetNumBytesPerRow() + 3*col;
	ret += i;
	return ret;
}

inline void RgbImage::GetRgbPixel( long row, long col, float* red, float* green, float* blue ) const
{
	const unsigned char* thePixel = GetRgbPixel( row, col );
	*red = (float)(*(thePixel++));
	*green = (float)(*(thePixel++));
	*blue = (float)(*thePixel);
}

inline void RgbImage::GetRgbPixel( long row, long col, double* red, double* green, double* blue ) const
{
	const unsigned char* thePixel = GetRgbPixel( row, col );
	const double f = 1.0/255.0;
	*red = f*(double)(*(thePixel++));
	*green = f*(double)(*(thePixel++));
	*blue = f*(double)(*thePixel);
}

inline void RgbImage::Reset()
{
	NumRows = 0;
	NumCols = 0;
	delete[] ImagePtr;
	ImagePtr = 0;
	ErrorCode = 0;
}


#endif RGBIMAGE_H
