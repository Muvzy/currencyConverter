//This code uses the curl/curl.h library and compiles using gcc exchangeApi.c -o exchangeApi -lcurl
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>


//declare string to store all currency values to
struct string {
  	char *ptr;
  	size_t len;
};


//allocate memory for string
void init_string(struct string *s) {
 	s->len = 0;
  	s->ptr = malloc(s->len+1);
  	if (s->ptr == NULL) {
    	fprintf(stderr, "malloc() failed\n");
    	exit(EXIT_FAILURE);
  	}
  	s->ptr[0] = '\0';
}


//libcurl function to write the data obtained into a string
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s) {
	size_t new_len = s->len + size*nmemb;
  	s->ptr = realloc(s->ptr, new_len+1);
   	if (s->ptr == NULL) {
    	fprintf(stderr, "realloc() failed\n");
    	exit(EXIT_FAILURE);
  	}
  	memcpy(s->ptr+s->len, ptr, size*nmemb);
  	s->ptr[new_len] = '\0';
  	s->len = new_len;
  	return size*nmemb;
}


//main function
int main(void)
{
	//prepare curl
  	CURL *curl;
  	CURLcode res;
	//perform curl from currencyapi using API key and write the data to the string s
  	curl = curl_easy_init();
  	if(curl) {
    		struct string s;
    		init_string(&s);
    		curl_easy_setopt(curl, CURLOPT_URL, "https://api.currencyapi.com/v3/latest?apikey=GmV2J6m7o81SL8g4FAII7iYMQ9oSqvisOfUxPm6f");
    		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    		res = curl_easy_perform(curl);
    		printf("%s\n", s.ptr); //this prints the string, remove this when finished
    		free(s.ptr);
    		//cleanup
    		curl_easy_cleanup(curl);
  	}
  	/* notes for Nathan: at this point in the code you've got a string (the string's variable name is s)with every compatable currency exchange rate.
	this is only USD to XXX (so it's only able to give exchange rates for USD to whatever other currency)
	which i think is sufficient. I imagine at this point you can run this code once to print out the string
	so that you can see the format and use the string.h library functions to read through the string and find the required exchange rate
	however you see fit. Then its easy math from there.
	If you want to get fancy and make it convert across any currency in the string, you can probably just use the inverse of the
	exchange rate to convert backwards (ex. from AED to USD) then from USD to whatever you want. IDK if that's accurate or doable
	but it's up to you.
*/
}

