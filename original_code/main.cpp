#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>
#include "Mesh.h"
#include "Clock.h"
#include "util.h"
#include "Properties.h"
#include "HarrisDetector.h"

using namespace std;

string getOutputPath(string filename){
  string outputPath = filename;
  size_t pos;
  string directory = "";

  if((pos = filename.find_last_of('/')) != string::npos){
    directory = filename.substr(0, pos + 1);
    outputPath = filename.substr(pos + 1);
  }

  pos = outputPath.find_last_of('.');
  outputPath = outputPath.substr(0, pos);
  
  outputPath = directory + outputPath + ".int";

  return outputPath;
}

void help(){
	cout << "Harris3D: Extracts the interest points from a 3D mesh." << endl;
	cout <<"  Command:" << endl;
	cout <<"            harris input_mesh [properties_file]" << endl;
	cout <<" where " << endl;
	cout <<" - input_mesh must be a valid OFF file" << endl;
	cout <<" - properties_file is an optional text file with the parameters for Harris 3D." << endl;
	cout <<"   If this file is not provided, default parameters are used." << endl;
}

int main(int argc, char *argv[])
{
    if(argc < 2){
		help();
		exit(EXIT_SUCCESS);
	}
	
	string offFilename = argv[1];
	string optFilename;
	if(argc == 3)
		optFilename = argv[2];
	 
    cout<<"File:"<<offFilename;
    
    string outfilename = getOutputPath(offFilename);
    ifstream inp;
    inp.open(outfilename.c_str(), ifstream::in);
    inp.close();

    if(inp.fail()){
		inp.clear(ios::failbit);
	
		int i = 0;
		Clock r;
		r.tick();
		Mesh mesh(argv[1]);
		r.tick();
		cout<<"->Loaded";
		cout<<"->Loading time:"<<r.getTime()<<"s."<<endl;
    
		Properties prop;
		if(argc == 3)
			prop.load(optFilename);
		
		HarrisDetector hd(&mesh, &prop);
		
		vector<Vertex> interestPoints;
		interestPoints.clear();
		Vertex* vertices = mesh.getVertices();
		
		Clock r1;
		r1.tick();
		cout<<"->Interest points"<<endl;
		hd.detectInterestPoints(interestPoints);
		r1.tick();
		cout<<"->Calculation time:"<<r1.getTime()<<"s."<<endl;
	
		cout<<"->Saving"<<endl;
		ofstream out(outfilename.c_str());
	
		out<<interestPoints.size()<<endl;
		for(register int i = 0; i < interestPoints.size(); i++) {
			out<<interestPoints[i].getIndex()<<endl;
		}
		out.close();
		interestPoints.clear();
	
		cout<<"File saved."<<endl;
	}else{
		cout << "Interest points already exist - Skipped" << endl;
    }
	
	return EXIT_SUCCESS;
}
