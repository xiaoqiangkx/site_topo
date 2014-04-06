/*****
** link_parser.c
** - implements the methods declared in link_parser.h
** - 处理http响应字节流，提取网络链接
*/

#include <link_parser.h>
#include <dbg.h>

void get_info_from_url(char *host, char *path, const char* srcUrl)
{
	int hostnum=0,pathnum=0;
	char *p = srcUrl+strlen("http://");     /* pass http:// */
	while(*p != '/') {
		host[hostnum] = *p;
		p = p + sizeof(char);
		hostnum++;
	}
	host[hostnum] = '\0';
	while(*p != '\0') {
		path[pathnum] = *p;
		p = p + sizeof(char);
		pathnum++;
	}
	path[pathnum] = '\0';
	//printf("now url is %s, host is %s. path is %s\n",srcUrl, host, path);
	return;
}

static int link_to_url(char *link, char *url, const char *abpath)
{
    // log_info("link=%s, url=%s, abpath=%s", link, url, abpath);
	char host[20],path[200];
	get_info_from_url(host,path,abpath);
	char *p, *l;
	l = strrchr(path,'/');
	*l = '\0';
	p = link;
	//printf("ooooooooooooooooooooooooooooo\nnow abpath is %s, host is %s. path is %s, link is %s\n-------------------------\n",abpath, host, path, link);

	while(p != NULL) {
		if(*p == '.') {
			p++;
			if(*p == '.') {
				p++;
				if(*p == '/') {
					l = strrchr(path,'/');
					if(l == NULL) {
						fprintf(stderr,"link_to_url failed! Now abpath is \"%s\", link is \"%s\"\n", abpath, link);
						return -1;
					} else {
						*l = '\0';
					}
					p++;
				} else {
					fprintf(stderr,"link_to_url failed! Now link is \"%s\"\n", link);
					return -1;
				}
			} else if(*p == '/') {
				p++;
				continue;
			} else {
				fprintf(stderr,"link_to_url failed! Now link is \"%s\"\n", link);
				return -1;
			}
		} else {
			break;
		}
	}

    log_info("host=%s, path=%s, p=%s", host, path, p);
	memcpy(url,"http://",strlen("http://"));
	url[strlen("http://")] = '\0';
	strcat(url,host);
	strcat(url,path);
//	strcat(url,"/");    /* some website use /, some do not have */
	strcat(url,p);
	return 0;
}

static int checkhtm(const char * htmltxt)
{
	char temp[256];
	char *p;
	int htmllen = strlen(htmltxt);
	if(htmllen < strlen(".htm")) {
		return -1;
	} else if(htmllen < strlen("http:")) {
		p = strrchr(htmltxt,'.');
		if(p == NULL) {
			return -1;
		} else if((strcasecmp(p,".html")==0)||(strcasecmp(p,".htm")==0)) {
			return 1;
		} else {
			return -1;
		}
	} else {
		p = strrchr(htmltxt,'.');
		memcpy(temp,htmltxt,strlen("http:"));
		temp[strlen("http:")] = '\0';
		if(strcasecmp(temp,"http:")==0) {
			return 0;
		} else if(p == NULL) {
			return -1;
		} else if((strcasecmp(p,".html")==0)||(strcasecmp(p,".htm")==0)) {
			return 1;
		} else {
			return -1;
		}
	}
}


void extract_link_from_autom(const char * const htmltxt, link_t *linklist, const char *abpath)
{
    //log_info("html\n is %s", htmltxt);
    enum {S_START, S_LEFT, S_A, S_BLANK, S_H, S_R, S_E, S_F, S_EQ, S_LQ, S_RQ, S_RIGHT, S_END};
    const char *ch = htmltxt;
    int status = S_START;
    const char *as = NULL,  *ae = NULL, *ls = NULL, *le = NULL;
    char link[4096], url[4096];
    int prefix_size, path_size, link_size;
    int k = 0, tag;

    while(*ch) {
	switch(status) {
	case S_START:
	    if(*ch == '<') {
		status = S_LEFT;
		as = ch;
	    } else if(*ch == '\n') {
		status = S_END;
	    }
	    k++;
	    ch++;
	break;
	case S_LEFT:
	    if(*ch == 'a') {
		status = S_A;
	    } else if(*ch == '>') {
		ae = ch;
		status = S_RIGHT;
		continue;
	    } else if(!(*ch == '\t' || *ch == ' ')) {
		status = S_END;
	    }
	    k++;
	    ch++;
	break;
	case S_A:
	    if(*ch == '\t' || *ch == ' ') {
		status = S_BLANK;
	    } else if(*ch == '>') {
		status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
		status = S_END;
	    } else {
		status = S_END;
	    }
	    k++;
	    ch++;
	break;
	case S_BLANK:
	    if(*ch == '>') {
	        status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
	        status = S_END;
	    } else if(*ch == 'h') {
	        status = S_H;
	    }
	    k++;
	    ch++;
	break;
	case S_H:
	    if(*ch == '>') {
	        status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
	         status = S_END;
	    } else if(*ch == 'r') {
	         status = S_R;
	    } else {
	         status = S_BLANK;
	    }
	    k++;
	    ch++;
	break;
	case S_R:
	    if(*ch == '>') {
		status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
		status = S_END;
	    } else if(*ch == 'e') {
		status = S_E;
	    } else {
		status = S_BLANK;
	    }
	    k++;
	    ch++;
	break;
	case S_E:
	    if(*ch == '>') {
		status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
		status = S_END;
	    } else if(*ch == 'f') {
		status = S_F;
	    } else {
		status = S_BLANK;
	    }
	    k++;
	    ch++;
	break;
	case S_F:
	    if(*ch == '>') {
		status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
		status = S_END;
	    } else if(*ch == '=') {
		status = S_EQ;
	    } else if(!(*ch == '\t' || *ch == ' ')) {
		status = S_BLANK;
	    }
	    k++;
	    ch++;
	break;
	case S_EQ:
	    if(*ch == '>') {
		status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
		status = S_END;
	    } else if(*ch == '"' || *ch == '\'') {
		ls = ch + 1;
		status = S_LQ;
	    } else if(!(*ch == '\t' || *ch == ' ')) {
		status = S_BLANK;
	    }
	    k++;
	    ch++;
	break;
	case S_LQ:
	    if(*ch == '>') {
		status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
		status = S_END;
	    } else if(*ch == '"' || *ch == '\'') {
		le = ch - 1 ;
		status = S_RQ;
	    }
	    k++;
	    ch++;
	break;
	case S_RQ:
	    if(*ch == '>') {
		status = S_RIGHT;
		ae = ch;
		continue;
	    } else if(*ch == '\n') {
		status = S_END;
	    }
	    k++;
	    ch++;
	break;
	case S_RIGHT:
	    status = S_START;
	    if(!(ls == NULL || le == NULL)) {
		bzero(link, 4096);
		link_size = le - ls + 1;
		    if(link_size > 0) {
	            memcpy(link, ls, link_size);
				link[link_size] = '\0';
				tag = checkhtm(link);
				if(tag>0) {
					link_to_url(link,url,abpath);
					if( strlen(url) < 256 )
						link_push(linklist,url);
				}
				/*
				else if(tag == 0)
				{
					link_push(linklist,link);
				}
				*/
		    }
	    }
	    as = ae = ls = le = NULL;
	    k++;
	    ch++;
	break;
	case S_END:
	    status = S_START;
	    as = ae = ls = le = NULL;
	break;
	}
    }
}

