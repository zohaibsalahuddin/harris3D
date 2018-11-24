/*
	Date Created: 23rd November
	FILENAME : fileread.cpp
------------------------------------------------
 *
 *	This file contains declarations for functions 
 *	to open the input file containing the vertices
 *	and faces information
*/

#ifndef __FILE_READ__
#define __FILE_READ__

void skipline(istream &in);
int readVertFace (const string filePath , int dummy_vertex , int dummy_face);

#endif
