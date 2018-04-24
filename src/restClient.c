/*
 * RestClient.c
 *
 *  Created on: 13/03/2018
 *      Author: jake
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <curl/curl.h>
#include "restClient.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};

static CURL *curl_handle;
static CURLcode res;
static struct MemoryStruct chunk;

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory == NULL)
  {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  assert (mem->size <= MAX_RESPONSE_LENGTH);

  return realsize;
}

void restClient_Init(void)
{
  chunk.memory = (char *)malloc(1);
  chunk.size = 0;
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl_handle = curl_easy_init();
}

void restClient_Reset(void)
{
  free(chunk.memory);
  chunk.memory = (char *)malloc(1);
  chunk.size = 0;
}

int restClient_Get(char* url, char* response)
{
  if (curl_handle)
  {
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
	res = curl_easy_perform(curl_handle);
	if (res == CURLE_OK)
	{
		memcpy(response, chunk.memory, chunk.size);
		return chunk.size;
	}
	else
	{
	  printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	}
  }

  return -1;
}

void restClient_Cleanup(void)
{
  if (curl_handle)
  {
    curl_easy_cleanup(curl_handle);
  }
}
