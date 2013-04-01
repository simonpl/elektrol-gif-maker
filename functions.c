size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct curlfile *out=(struct curlfile *)stream;
  if(out && !out->stream) {
    out->stream=fopen(out->filename, "wb");
    if(!out->stream)
      return -1;
  }
  return fwrite(buffer, size, nmemb, out->stream);
}
int in_array(char *array[], int size, char *lookfor)
{
    int i;
    for (i = 0; i < size; i++)
        if (strcmp(lookfor, array[i]) == 0)
            return 1;
    return 0;
}
int load_image(char *path, char *filename, struct elektrol_config config)
{
    CURL *curl;
    CURLcode result;
    struct curlfile downfile=
    {
        filename,
        NULL
    };
    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, path);
        curl_easy_setopt(curl, CURLOPT_USERNAME, config.user);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, config.passwd);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &downfile);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
        result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if(CURLE_OK != result) 
        { 
            fprintf(stderr, "Could not load image located at %s, curl returned error state %d\n", path, result);
        }
    }
    if(downfile.stream)
    fclose(downfile.stream); /* close the local file */
    return 0;
}
