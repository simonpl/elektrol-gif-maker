size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream) /* Used by curl */
{
  struct curlfile *out=(struct curlfile *)stream;
  if(out && !out->stream) {
    out->stream=fopen(out->filename, "wb");
    if(!out->stream)
      return -1;
  }
  return fwrite(buffer, size, nmemb, out->stream);
}
int in_array(char *array[], int size, char *lookfor) /* Check if array has lookfor als element */
{
    int i;
    for (i = 0; i < size; i++)
        if (strcmp(lookfor, array[i]) == 0)
            return 1;
    return 0;
}
int load_image(char *path, char *filename, struct elektrol_config config, int server) /* Load the requested image and store it at filename */
{
    int returnstate;
    CURL *curl;
    CURLcode result;
    returnstate = 0;
    struct curlfile downfile=
    {
        filename,
        NULL
    };
    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, path);
        if(server == 0)
        {
            curl_easy_setopt(curl, CURLOPT_USERNAME, config.user);
            curl_easy_setopt(curl, CURLOPT_PASSWORD, config.passwd);
        }
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &downfile);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
        result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if(CURLE_OK != result) 
        { 
            fprintf(stderr, "Could not load image located at %s, curl returned error state %d\n", path, result);
            returnstate = 1;
        }
    }
    if(downfile.stream)
    fclose(downfile.stream); /* close the local file */
    return returnstate;
}
int check_black_image(char *filename)
{
    char command[200];
    int exitstate;
    command[0] = '\0';
    strcat(command, "identify -verbose ");
    strcat(command, filename);
    strcat(command, " | grep -x \"  Colors: 1\" > /dev/null");
    exitstate = system(command);
    if (exitstate == -1)
    {
        exit(ELEKTROL_UNKOWN_ERROR);
    }
    else if(WEXITSTATUS(exitstate) == 0)
    {
        remove(filename);
        return 1;
    }
    else
    {
        return 0;
    }
}
