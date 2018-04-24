/*
 * RestClient.h
 *
 *  Created on: 13/03/2018
 *      Author: jake
 */

#ifndef RESTCLIENT_H_
#define RESTCLIENT_H_

#define MAX_URL_LENGTH 256
#define MAX_RESPONSE_LENGTH 32768

#ifdef __cplusplus
extern "C" {
#endif

void restClient_Init(void);
void restClient_Reset(void);
int restClient_Get(char* url, char* response);
void restClient_Cleanup(void);

#ifdef __cplusplus
}
#endif

#endif /* RESTCLIENT_H_ */
