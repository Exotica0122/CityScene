#include "Texture.h"

void initTexture(void)
{
	skyTexture = loadPPM("sky.ppm");
	asphaltTexture = loadPPM("asphalt.ppm");
	roadTexture = loadPPM("road.ppm");
	brickTexture = loadPPM("brick.ppm");
	waterTexture = loadPPM("water.ppm");
}

// Load a binary ppm file into an OpenGL texture and return the OpenGL texture reference ID
int loadPPM(char* filename)
{
	FILE *inFile; //File pointer
	int width, height, maxVal; //image metadata from PPM file format
	int totalPixels; // total number of pixels in the image

					 // temporary character
	char tempChar;
	// counter variable for the current pixel in the image
	int i;

	char header[100]; //input buffer for reading in the file header information

	// if the original values are larger than 255
	float RGBScaling;

	// temporary variables for reading in the red, green and blue data of each pixel
	int red, green, blue;

	GLubyte *texture; //the texture buffer pointer

	//create one texture with the next available index
	GLuint textureID;
	glGenTextures(1, &textureID);

	inFile = fopen(filename, "r");

	// read in the first header line
	//    - "%[^\n]"  matches a string of all characters not equal to the new line character ('\n')
	//    - so we are just reading everything up to the first line break
	fscanf(inFile, "%[^\n] ", header);

	// make sure that the image begins with 'P3', which signifies a PPM file
	if ((header[0] != 'P') || (header[1] != '3'))
	{
		printf("This is not a PPM file!\n");
		exit(0);
	}

	// we have a PPM file
	printf("This is a PPM file\n");

	// read in the first character of the next line
	fscanf(inFile, "%c", &tempChar);

	// while we still have comment lines (which begin with #)
	while (tempChar == '#')
	{
		// read in the comment
		fscanf(inFile, "%[^\n] ", header);

		// print the comment
		printf("%s\n", header);

		// read in the first character of the next line
		fscanf(inFile, "%c", &tempChar);
	}

	// the last one was not a comment character '#', so we need to put it back into the file stream (undo)
	ungetc(tempChar, inFile);

	// read in the image hieght, width and the maximum value
	fscanf(inFile, "%d %d %d", &width, &height, &maxVal);
	// print out the information about the image file
	printf("%d rows  %d columns  max value= %d\n", height, width, maxVal);

	// compute the total number of pixels in the image
	totalPixels = width * height;

	// allocate enough memory for the image  (3*) because of the RGB data
	texture = malloc(3 * sizeof(GLuint) * totalPixels);

	// determine the scaling for RGB values
	RGBScaling = (float)(255.0 / maxVal);

	// if the maxValue is 255 then we do not need to scale the 
	//    image data values to be in the range or 0 to 255
	if (maxVal == 255)
	{
		for (i = 0; i < totalPixels; i++)
		{
			// read in the current pixel from the file
			fscanf(inFile, "%d %d %d", &red, &green, &blue);

			// store the red, green and blue data of the current pixel in the data array
			texture[3 * totalPixels - 3 * i - 3] = (GLubyte)red;
			texture[3 * totalPixels - 3 * i - 2] = (GLubyte)green;
			texture[3 * totalPixels - 3 * i - 1] = (GLubyte)blue;
		}
	}
	else  // need to scale up the data values
	{
		for (i = 0; i < totalPixels; i++)
		{
			// read in the current pixel from the file
			fscanf(inFile, "%d %d %d", &red, &green, &blue);

			// store the red, green and blue data of the current pixel in the data array
			texture[3 * totalPixels - 3 * i - 3] = (GLubyte)(red * RGBScaling);
			texture[3 * totalPixels - 3 * i - 2] = (GLubyte)(green * RGBScaling);
			texture[3 * totalPixels - 3 * i - 1] = (GLubyte)(blue * RGBScaling);
		}
	}




	fclose(inFile);

	glBindTexture(GL_TEXTURE_2D, textureID);


	//Set the texture parameters
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	//Define the texture
	//glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLuint)width, (GLuint)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);

	//Create mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, (GLuint)width, (GLuint)height, GL_RGB, GL_UNSIGNED_BYTE, texture);

	//openGL guarantees to have the texture data stored so we no longer need it
	free(texture);

	//return the current texture id
	return(textureID);
}