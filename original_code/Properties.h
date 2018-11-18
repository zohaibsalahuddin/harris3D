#ifndef __PROPERTIES_H_
#define __PROPERTIES_H_

#include <map>
#include <string>

using namespace std;

class Properties{
	private:
		map<string, string> propertyList;
		string trim(string& input);
		
	public:
		Properties() {}
		virtual ~Properties() {}
		
		void load(string filename);
		string getProperty(string property);
		void show();
};

#endif

