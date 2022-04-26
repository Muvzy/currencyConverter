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

char *findRate(char code[4], struct string *str){
	const char *searchTool = "\",\"value\":";
	const char ch = ';';
	char *result = malloc(strlen(code) + strlen(searchTool) + 1);
	char *value;
	char *full = malloc(strlen(str->ptr));
	char *temp = malloc(strlen(str->ptr));

	strcpy(result, code);
	strcat(result, searchTool);
	
	full = str->ptr;
	temp = strstr(full, result);
	temp = strchr(temp, ':');
	temp++;
	temp = strtok(temp, "}");
	value = malloc(strlen(temp+1));
	value = temp;
	
	return value;
	free(temp);
	free(full);
	free(result);
}	

float convFromUSD(float rate, float amt){
	printf("\nExchange rate - %f\n", rate);
	float result = rate * amt;
	return result;
	
}

float convToUSD(float rate, float amt){
	rate = 1 / rate;
	printf("\nExchange rate - %f\n", rate);
	float result = rate * amt;
	return result;
}

//main function
int main(void)
{
	int opt = 1;
	char code[4];
	float val; 
	float amt;
	float calc;
	struct string s;
    	init_string(&s);
	struct string *str = malloc(s.len);
	struct string *hold = malloc(s.len);
	

	//prepare curl
  	CURL *curl;
  	CURLcode res;
	//perform curl from currencyapi using API key and write the data to the string s
  	curl = curl_easy_init();
  	if(curl) {
    		curl_easy_setopt(curl, CURLOPT_URL, "https://api.currencyapi.com/v3/latest?apikey=GmV2J6m7o81SL8g4FAII7iYMQ9oSqvisOfUxPm6f");
    		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
    		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
    		res = curl_easy_perform(curl);
    		/*printf("%s\n", s.ptr); //this prints the string, remove this when finished*/
    		/*free(s.ptr);*/
    		//cleanup
    		curl_easy_cleanup(curl);
  	}
	
	hold->ptr = s.ptr;
	str = hold;
	printf("\nThis program converts between USD and other currencies\n");
	printf("It uses CurrencyAPI to provide real-time exchange rates\n");

	
	do {
		/* Menu for user to select function from */
		printf("\n---Select a function to execute---\n");
		printf("1. Convert USD to another currency\n");
		printf("2. Convert another currency to USD\n");
		printf("3. Quit program\n");
		printf("----------------------------------\n");
		scanf(" %d",&opt);

		switch(opt){
			case 1:
				printf("\n---Convert USD to another currency--- ");
				printf("\nEnter three letter currency code to convert to: ");
				getchar();
				fgets(code, sizeof(code), stdin);
		
				if(strstr(str->ptr, code) == NULL){
					printf("Invalid code. Returning to menu");
					break;
				}

				printf("Enter amount to convert: ");
				scanf("%f", &amt);
		
				val = atof(findRate(code, str));
				calc = convFromUSD(val, amt);

				printf("%.2f USD is equivalent to %.2f %s", amt, calc, code);
				
				printf("\nReturning to menu\n");
				str = hold;
				break;
			case 2:
				printf("\n---Convert another currency to USD--- ");
				printf("\nEnter three letter currency code to convert from: ");
				getchar();
				fgets(code, sizeof(code), stdin);
		
				if(strstr(str->ptr, code) == NULL){
					printf("Invalid code. Returning to menu");
					break;
				}

				printf("Enter amount to convert: ");
				scanf("%f", &amt);
		
				val = atof(findRate(code, str));
				calc = convToUSD(val, amt);

				printf("%.2f %s is equivalent to %.2f USD", amt, code, calc);
				
				printf("\nReturning to menu\n");
				str = hold;
				break;
					
		}
	} while (opt > 0 && opt < 3);

				


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

