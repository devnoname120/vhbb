#include "network.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <string>
#include <curl/curl.h>

Network::Network()
{
    sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
    SceNetInitParam netInitParam;
    int size = 1 * 1024 * 1024;
    netInitParam.memory = malloc(size);
    netInitParam.size = size;
    netInitParam.flags = 0;
    sceNetInit(&netInitParam);
    sceNetCtlInit();

    sceSysmoduleLoadModule(SCE_SYSMODULE_HTTPS);
    sceSslInit(300 * 1024);
    sceHttpInit(1 * 1024 * 1024);

    // Certificate verification for the API fails with SCE_HTTP_ERROR_SSL, SCE_HTTPS_ERROR_SSL_CN_CHECK
    sceHttpsDisableOption(SCE_HTTPS_FLAG_CN_CHECK);

    int ret = sceHttpCreateTemplate(VHBB_UA, SCE_HTTP_VERSION_1_1, SCE_TRUE);
    if (ret < 0)
        throw std::runtime_error("Network: Cannot create template");
    templateId_ = ret;
}

Network::~Network()
{
    sceHttpDeleteTemplate(templateId_);

    sceNetCtlTerm();
    sceNetTerm();
    sceSysmoduleUnloadModule(SCE_SYSMODULE_NET);

    sceHttpTerm();
    sceSysmoduleUnloadModule(SCE_SYSMODULE_HTTPS);

    sceSslEnd();
    sceSysmoduleUnloadModule(SCE_SYSMODULE_SSL);

}


struct stringcurl {
  char *ptr;
  size_t len;
}; 
void init_string(struct stringcurl *s) {
  s->len = 0;
  s->ptr = (char*)malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    return;
    //exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}
size_t writefunc(void *ptr, size_t size, size_t nmemb, struct stringcurl *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = (char*)realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    return 0;
    //exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

static size_t write_data_to_disk(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = sceIoWrite(   *(int*) stream , ptr , size*nmemb);
  return written;
}

static int progress_callback(void *clientp,   double dltotal,   double dlnow,   double ultotal,   double ulnow){
	InfoProgress * progress = (InfoProgress *)clientp;
	if(progress) progress->percent((float)(dlnow/dltotal) * 100);
	return CURLE_OK;
}


int Network::DownloadSize(std::string url, uint64_t *size, InfoProgress progress)
{
    return DownloadSize(url, size, &progress);
}

int Network::DownloadSize(std::string url, uint64_t *size, InfoProgress *progress)
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		struct stringcurl body;
		init_string(&body);
		struct stringcurl header;
		init_string(&header);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		// Set useragant string
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		// not sure how to use this when enabled
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		// not sure how to use this when enabled
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		// Set SSL VERSION to TLS 1.2
		curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
		// Set timeout for the connection to build
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		// Follow redirects (?)
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
		
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		// The object to pass to the progress callback 
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress);
		// The function that will be used to write the data 
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
		// write function of response headers
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
		// the response header data where to write to
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
		// Request Header :
		struct curl_slist *headerchunk = NULL;
		headerchunk = curl_slist_append(headerchunk, "Accept: */*");
		headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
		headerchunk = curl_slist_append(headerchunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		//headerchunk = curl_slist_append(headerchunk, "Host: discordapp.com");  Setting this will lead to errors when trying to download. should be set depending on location : possible : cdn.discordapp.com or images.discordapp.com
		headerchunk = curl_slist_append(headerchunk, "Content-Length: 0");
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);
		
		
		// Perform the request
		res = curl_easy_perform(curl);
		int httpresponsecode = 0;
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpresponsecode);
		
		
		//  This is the value read from the Content-Length: field. Stores -1 if the size isn't known.
		curl_off_t clDownloadSize;
		curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD , &clDownloadSize);
		
		if(res != CURLE_OK){
			//fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			
		}else{
			//printf("Download size: %" CURL_FORMAT_CURL_OFF_T "\n", clDownloadSize);
		}
		
		
	}else{
		
	}

	// cleanup
	curl_easy_cleanup(curl);
    return 0;
}

int Network::Download(std::string url, std::string dest, InfoProgress *progress)
{
	if(progress) progress->message("Starting the download... ");
	int fileFD = sceIoOpen( dest.c_str(), SCE_O_WRONLY | SCE_O_CREAT, 0777);
	if(!fileFD){
		return 0;
	}
	
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	if(curl) {
		struct stringcurl body;
		init_string(&body);
		struct stringcurl header;
		init_string(&header);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		// Set useragant string
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		// not sure how to use this when enabled
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		// not sure how to use this when enabled
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		// Set SSL VERSION to TLS 1.2
		curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
		// Set timeout for the connection to build
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
		// Follow redirects (?)
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		// The function that will be used to write the data 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_disk);
		// The object to pass to the progress callback 
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, progress);
		// The progress callback
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
		// The data filedescriptor which will be written to
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &fileFD);
		// write function of response headers
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writefunc);
		// the response header data where to write to
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
		// Request Header :
		struct curl_slist *headerchunk = NULL;
		headerchunk = curl_slist_append(headerchunk, "Accept: */*");
		headerchunk = curl_slist_append(headerchunk, "Content-Type: application/json");
		headerchunk = curl_slist_append(headerchunk, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
		//headerchunk = curl_slist_append(headerchunk, "Host: discordapp.com");  Setting this will lead to errors when trying to download. should be set depending on location : possible : cdn.discordapp.com or images.discordapp.com
		headerchunk = curl_slist_append(headerchunk, "Content-Length: 0");
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerchunk);
		
		if(progress) progress->message("Downloading...");
		
		// Perform the request
		res = curl_easy_perform(curl);
		int httpresponsecode = 0;
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpresponsecode);
		
		if(res != CURLE_OK){
			//fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			
		}else{
			
		}
		
		
	}else{
		
	}

	// close filedescriptor
	sceIoClose(fileFD);

	// cleanup
	curl_easy_cleanup(curl);

    return 0;
}

int Network::Download(std::string url, std::string dest, InfoProgress progress)
{
    return Download(url, dest, &progress);
}

InternetStatus Network::TestConnection()
{
    InternetStatus ret = INTERNET_STATUS_OK;
    int req = -1;
    int read = -1;
    int sendRes = -1;
    int res = -1;
    int statusCode = 0;
    uint64_t contentLength;
    char buf[4096] = {0};

    int conn = sceHttpCreateConnectionWithURL(templateId_, PORTAL_DETECT_URL, SCE_TRUE);
    if (conn < 0) {
        ret = INTERNET_STATUS_NO_INTERNET;
        goto clean;
    }

    req = sceHttpCreateRequestWithURL(conn, SCE_HTTP_METHOD_GET, PORTAL_DETECT_URL, 0);
    if (req < 0) {
        ret = INTERNET_STATUS_NO_INTERNET;
        goto clean;
    }

    sendRes = sceHttpSendRequest(req, NULL, 0);

    
    res = sceHttpGetStatusCode(req, &statusCode);

    if (sendRes < 0 || res < 0 || statusCode != 200) {
        ret = INTERNET_STATUS_NO_INTERNET;
        goto clean;
    }
    
    res = sceHttpGetResponseContentLength(req, &contentLength);

    if (res >= 0)
        dbg_printf(DBG_DEBUG, "Content length: %lu", contentLength);

    
    read = sceHttpReadData(req, buf, sizeof(buf));
    if (res <= 0) {
        ret = INTERNET_STATUS_NO_INTERNET;
        goto clean;
    }

    buf[read] = '\0';

    if (strncmp(buf, PORTAL_DETECT_STR, strlen(PORTAL_DETECT_STR))) {
        ret = INTERNET_STATUS_HOTSPOT_PAGE;
        goto clean;
    }

    clean:
    if (req >= 0) {
        sceHttpDeleteRequest(req);
        sceHttpAbortRequest(req);
    }
    if (conn >= 0) sceHttpDeleteConnection(conn);

    return ret;
}
