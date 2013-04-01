struct curlfile /* Used by curl */
{
  const char *filename;
  FILE *stream;
};
struct elektrol_config /* Structure where the configuration will be stored */
{
    char *server;
    char *user;
    char *passwd;
    char *delay;
    char *outputdir;
    struct tm time;
};
