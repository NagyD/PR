#ifndef _XML_H_
#define _XML_H_


/* types */
typedef void* TagReference;

class Xml {
	public:
		Xml(const char* file);
		~Xml();
		void write(const char* file);

		TagReference getRoot();
		TagReference getChild(TagReference t);
		TagReference getNext(TagReference t);

		void optimize(TagReference t);
		void optimize();
		// POST: all TagReferences becomes invalid
		


		int getTag (TagReference t, char** tag, char** desc, char** path, char** file, char** type, char** name, char** palette, char** value, char** version, char** number,char** colors);
		int setTag (TagReference t, const char* tag, const char* desc, const char* path, const char* file, const char* type, const char* name, const char* palette, const char* value, const char* version, const char* number,const char* colors);
		
		
	private:
		void* structure;



};

#endif

