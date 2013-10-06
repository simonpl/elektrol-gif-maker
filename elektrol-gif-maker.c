/*
    Elektrol-gif-maker generates GIF-images produced by the russian satellite Elektro-l
    Copyright (C) 2013 Simon Plasger

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    See COPYING for details.
    
    See README for informations about how to build and use this program.
*/
#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

#include "structs.h"
#include "functions.c"
#include "constants.h"

int main(int argc, char *argv[])
{
    int i,j,k,pid, wpid, status, success, servers;
    char temp[32]; /* Used at puzzling the path */
    success = EXIT_SUCCESS; /* Globally recognize if there is an error */
    char *poschannels[]= {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "RGB"}; /* This channels may be used */
    char *channels[]= {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "RGB", NULL}; /* This ones *are* used */
    struct elektrol_config config = {}; /* Empty config */
    printf("Elektro-L Gif-maker Copyright (C) 2013 Simon Plasger\nThis program comes with ABSOLUTELY NO WARRANTY\nThis is free software, and you are welcome to redistribute it under certain conditions; view 'LICENSE' for details.\n\n");
    for(i = 0; i < argc; i++) /* Parse Command line arguments */
    {
        if(strcmp(argv[i],"--channels") == 0 || strcmp(argv[i],"-c") == 0) /* After channels there comes an array */
        {
            i++;
            j = 0;
            while(argv[i] != NULL && in_array(poschannels, 11, argv[i]))
            {
                channels[j] = argv[i];
                i++;
                j++;
            }
            i--;
            channels[j] = NULL;
        }
        else if(strcmp(argv[i],"--server") == 0 || strcmp(argv[i],"-s") == 0)
        {
            i++;
            if(argv[i] != NULL);
            {
                config.server = argv[i];
            }
        }
        else if(strcmp(argv[i],"--server2") == 0 || strcmp(argv[i],"-s2") == 0)
        {
            i++;
            if(argv[i] != NULL);
            {
                config.server2 = argv[i];
            }
        }
        else if(strcmp(argv[i],"--user") == 0 || strcmp(argv[i],"-u") == 0)
        {
            i++;
            if(argv[i] != NULL);
            {
                config.user = argv[i];
            }
        }
        else if(strcmp(argv[i],"--password") == 0 || strcmp(argv[i],"-p") == 0)
        {
            i++;
            if(argv[i] != NULL);
            {
                config.passwd = argv[i];
            }
        }
        else if(strcmp(argv[i],"--delay") == 0 || strcmp(argv[i],"-d") == 0)
        {
            i++;
            if(argv[i] != NULL);
            {
                config.delay = argv[i];
            }
        }
        else if(strcmp(argv[i],"--outputdir") == 0 || strcmp(argv[i],"-o") == 0)
        {
            i++;
            if(argv[i] != NULL);
            {
                config.outputdir = argv[i];
            }
        }
        else if(strcmp(argv[i],"--year") == 0 || strcmp(argv[i],"-y") == 0)
        {
            i++;
            if(argv[i] != NULL);
            {
                config.time.tm_year = atoi(argv[i]) - 1900;
            }
        }
        else if(strcmp(argv[i],"--month") == 0 || strcmp(argv[i],"-m") == 0)
        {
            i++;
            if(argv[i] != NULL);
            {
                config.time.tm_mon = atoi(argv[i]);
            }
        }
        else if(strcmp(argv[i],"--day") == 0)
        {
            i++;
            if(argv[i] != NULL);
            {
                config.time.tm_mday = atoi(argv[i]);
            }
        }
    }
    if(config.server == 0) /* If several config options were not given, print error and exit */
    {
        fprintf(stderr, "No server to download the data from has been defined. Pass an argument \"--server <server>\" to the program.\n");
        exit(ELEKTROL_NO_SERVER_ERROR);
    }
    if(config.user == 0)
    {
        fprintf(stderr, "No username for the FTP-server has been defined. Pass an argument \"--user <username>\" to the program.\n");
        exit(ELEKTROL_NO_USERNAME_ERROR);
    }
    if(config.passwd == 0)
    {
        fprintf(stderr, "No password for the FTP-server has been defined. Pass an argument \"--password <password>\" to the program.\n");
        exit(ELEKTROL_NO_PASSWORD_ERROR);
    }
    if(config.delay == 0)
    {
        fprintf(stderr, "No Delay time for the GIF-animation has been defined. Pass an argument \"--delay <delay>\" to the program, where <delay> is a number like 0.1 or 0.2.\n");
        exit(ELEKTROL_NO_DELAY_ERROR);
    }
    if(config.outputdir == 0)
    {
        fprintf(stderr, "No directory to save the images has been defined. Pass an argument \"--outputdir <directory>\" to the program.\n");
        exit(ELEKTROL_NO_DIRECTORY_ERROR);
    }
    if(config.time.tm_year == 0 || config.time.tm_mon == 0 || config.time.tm_mday == 0)
    {
        printf("Using date of yesterday for loading images\n");
        time_t unixtime;
        struct tm *timestr;
        unixtime = time(NULL) - 86400;
        timestr = localtime(&unixtime);
        config.time = *timestr;
    }
    chdir(config.outputdir);
    curl_global_init(CURL_GLOBAL_NOTHING);
    char path[100];
    char filename[10];
    char *timedirs[] = {"0000", "0030", "0100", "0130", "0200", "0230", "0300", "0330", "0400", "0430", "0500", "0530", "0600", "0630", "0700", "0730", "0800", "0830", "0900", "0930", "1000", "1030", "1100", "1130", "1200", "1230", "1300", "1330", "1400", "1430", "1500", "1530", "1600", "1630", "1700", "1730", "1800", "1830", "1900", "1930", "2000", "2030", "2100", "2130", "2200", "2230", "2300", "2330"};
    for(i = 0; i < sizeof(channels)/sizeof(channels[0]); i++)
    {
        if(channels[i] == NULL)
            break;
        printf("Loading images for channel %s\n", channels[i]);
        if(chdir(channels[i]) == -1)
        {
            mkdir(channels[i], 0755);
            chdir(channels[i]);
        }
        if(config.server2 == 0)
            servers = 1;
        else
            servers = 2;
        for(k = 0;k < servers;k++)
        {
            for(j = 0; j < sizeof(timedirs)/sizeof(timedirs[0]); j++)
            {
                path[0] = '\0';
                strcat(path, "ftp://");
                if(k == 0)
                    strcat(path, config.server);
                else
                    strcat(path, config.server2);
                strcat(path, "/");
                sprintf(temp, "%i", config.time.tm_year + 1900);
                strcat(path, temp);
                switch(config.time.tm_mon)
                {
                    case 0:
                        strcat(path, "/January/");
                        break;
                    case 1:
                        strcat(path, "/February/");
                        break;
                    case 2:
                        strcat(path, "/March/");
                        break;
                    case 3:
                        strcat(path, "/April/");
                        break;
                    case 4:
                        strcat(path, "/May/");
                        break;
                    case 5:
                        strcat(path, "/June/");
                        break;
                    case 6:
                        strcat(path, "/July/");
                        break;
                    case 7:
                        strcat(path, "/August/");
                        break;
                    case 8:
                        strcat(path, "/September/");
                        break;
                    case 9:
                        strcat(path, "/October/");
                        break;
                    case 10:
                        strcat(path, "/November/");
                        break;
                    case 11:
                        strcat(path, "/December/");
                        break;
                }
                if(config.time.tm_mday < 10)
                {
                    strcat(path, "0");
                }
                sprintf(temp, "%i", config.time.tm_mday);
                strcat(path, temp);
                strcat(path, "/");
                strcat(path, timedirs[j]);
                strcat(path, "/");
                sprintf(temp, "%i", config.time.tm_year - 100);
                strcat(path, temp);
                if(config.time.tm_mon < 9)
                {
                    strcat(path, "0");
                }
                sprintf(temp, "%i", config.time.tm_mon + 1);;
                strcat(path, temp);
                if(config.time.tm_mday < 10)
                {
                    strcat(path, "0");
                }
                sprintf(temp, "%i", config.time.tm_mday);
                strcat(path, temp);
                strcat(path, "_");
                strcat(path, timedirs[j]);
                strcat(path, "_");
                strcat(path, channels[i]);
                strcat(path, ".jpg");
                filename[0] = '\0';
                strcat(filename, timedirs[j]);
                strcat(filename, ".jpg");
                load_image(path, filename, config, k);
                curl_global_cleanup();   
            }
        }
        chdir("..");
    }
    for(i = 0; i < sizeof(channels)/sizeof(channels[0]); i++)
    {
        if(channels[i] == NULL)
            break;
        pid = fork(); /* Create child process */
        if(pid == 0) /* Child process */
        {
            chdir(channels[i]);
            char *arguments[] = {"convert", "-delay", config.delay, "-loop", "0", "*.jpg", "anim.gif", NULL};
            execvp("convert", arguments);
            fprintf(stderr, "Could not execute the convert command. Make sure convert is in a directory that is in your PATH.");
            _exit(ELEKTROL_CONVERT_ERROR);
        }
        if(pid > 0) /* Parent process */
        {
            printf("Generation for GIF of channel %s has started\n", channels[i]);
        }
        else /* Fork has failed */
        {
            fprintf(stderr, "Generation for GIF of channel %s could not be started, error while forking\n", channels[i]);
            success = ELEKTROL_FORK_ERROR;
        }
    }
    for(i = 0; i < sizeof(channels)/sizeof(channels[0]); i++)
    {
        wpid = wait(&status); /* Wait for childs to terminate */
        if(wpid != -1)
        {
            printf("Child %i exited with state %i\n", wpid, status);
        }
    }
    if(success == EXIT_SUCCESS)
        exit(EXIT_SUCCESS);
    else if(success == ELEKTROL_FORK_ERROR)
        exit(ELEKTROL_FORK_ERROR);
    else
        exit(ELEKTROL_UNKOWN_ERROR);
}
