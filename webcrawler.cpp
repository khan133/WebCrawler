using namespace std; 
#include <iostream>
#include <fstream>
#include "webcrawler.h"
#include "openhttp.h"
#include "SimpleHTMLParser.h"
// Add your implementation here
char * word;
char * description; 

WebCrawler::WebCrawler(int maxUrls, int numInitialUrls, const char ** initialUrls) {
	_urlArray = new URLRecord[maxUrls];
	_maxUrls = maxUrls;
	_headURL = 0;
	_tailURL = numInitialUrls;
	int i = 0;
	while (i < numInitialUrls) {
		char * temp = new char[strlen(initialUrls[i])];
		strcpy(temp, initialUrls[i]);
		_urlArray[i]._url = strdup(temp);
		_urlArray[i]._description = NULL; 
		i++;
	}
	_urlToUrlRecord = new HashTableTemplate<int>();
	_wordToURLRecordList = new HashTableTemplate<URLRecordList*>(); 
}
void WebCrawler::onContentFound(char c) 

{
	if (word == NULL) {
		word = new char[1]; 
		word[0] = '\0';
	}
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {			  char * right = new char[2];
		int i = 0;
		if (i == 0)
			right[i] = c;
		i++;
		right[i] = '\0';
		i = 0;

		char * newword = strcat(word, right);
		strcpy(word, newword); 
	}
	else {
		URLRecordList * val = new URLRecordList();
		val -> _urlRecordIndex = _headURL; 
		val -> _next = NULL;
		_wordToURLRecordList -> insertItem(word, val);
		word = NULL;
	}
	if (description != NULL) {
		if (strlen(description) <= 500) {
			char * right = new char[2];
			int i = 0;
			if (i == 0)
				right[i] = c;
			i++;
			right[i] = '\0';
			i = 0;
			char * newword = strcat(description, right);
			strcpy(description, newword); 
		}
	}
	return; 
}		
	void WebCrawler::onAnchorFound(char * url) {

	bool found = false; 
	if (_tailURL >= _maxUrls) { 
	found = false;
	return; 
	}
	int i = 0; 
	while (i < _tailURL) {
	if (strcmp(url, _urlArray[i]._url) == 0) {
	found = true;
	break; 
	}
	i++;
	}
	if (found == true) {
	return; 
	}
	URLRecord * temp = new URLRecord();
	_urlArray[_tailURL] = *temp;
	_urlArray[_tailURL]._url = strdup(url); 
	_urlArray[_tailURL]._description = NULL;
	_urlToUrlRecord -> insertItem(url, _tailURL); 
	_tailURL = _tailURL + 1; 
	}
void WebCrawler::crawl() {
	for (; _headURL < _tailURL; ) {
		char * current = _urlArray[_headURL]._url;
		int i;
		char * html = fetchHTML(current, &i);
		if (html == NULL) { 
			_headURL = _headURL + 1;  
			continue; 
		}
		parse(html, i);
			if (description != NULL) 
				_urlArray[_headURL]._description = strdup(description);
				
			word = NULL;
			description = NULL;
		_headURL = _headURL + 1; 
	}
}
void WebCrawler::writeURLFile(const char * urlFileName) {

	ofstream file; 
	  file.open(urlFileName);
	  int i;
	  const char * url; 
	  HashTableTemplateIterator<int> * iterator = new HashTableTemplateIterator<int>(_urlToUrlRecord); 
	//	if (iterator == NULL) {
	//		return;
	//	}
	while (iterator -> next(url, i)) {
	if (url != NULL) 
	file << i << " " << url << "\n";
	if (_urlArray[i]._description != NULL) {
	file << _urlArray[i]._description << "\n" << "\n";
	file << " ahdoh\n "; 
	}
	else
	file << "\n" << "\n";
	}
	file.close(); 
	}

void WebCrawler::writeWordFile(const char *wordFile) 
{
  FILE * file = fopen(wordFile, "wb"); 
    int size = 2039;
    int * array = new int[size];
    int i = 0; 
    while (i < size) {
    if (_wordToURLRecordList -> _buckets[i] == NULL) {
    i++;
    continue; 
    }
    fprintf(file, "%s", _wordToURLRecordList -> _buckets[i] -> _key);
    URLRecordList * current = _wordToURLRecordList -> _buckets[i] -> _data; 
    int j = 0;
    while (j < size) {
    array[j] = -1; 
    j++;
    }
    while (current != NULL) {
    if (current -> _urlRecordIndex < 0|| current -> _urlRecordIndex > 2039)
    break; 
    bool key = false; 
    j = 0; 
    while (j < size) {
    if (array[j] == -1) {
    break;
    }
    if (array[j] == current -> _urlRecordIndex)
    {
    key = true;
    j++;
    break;
    }
    j++; 

    }
    if (!(key)) {
    fprintf(file, " %d", current -> _urlRecordIndex);
    for (j = 0; j < size; j++) {
    if (array[j] < 0) {
    array[j] = current -> _urlRecordIndex;
    break; 
    }
    }
    }
    current = current -> _next;

    }
    fprintf(file, "\n");
    i++;
    }
    fclose(file);

    }


int main(int argc, char ** argv) {
	if (argc < 2)
		exit(1); 
	int maxUrls = 100; 
	if (strcmp(argv[1], "-u") == 0) {
		maxUrls = atoi(argv[2]);
	}
	const char ** urls = new const char*[argc - 3];
	int i = 3;
	int j = 0;
	while (i < argc) {
		urls[j] = argv[i+j];
		i++;
	}
	WebCrawler * web = new WebCrawler(maxUrls, argc - 3, urls);
	web -> crawl();
	web -> writeURLFile("url.txt");
	web -> writeWordFile("word.txt");
}

