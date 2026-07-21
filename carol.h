
#ifndef CAROL_H
#define CAROL_H

#include <stdbool.h>
#include <stdio.h>

#define get_var_name(var) #var

typedef struct
{
	char*style_path;
	char*html_path;
	
	char*title;
	bool initialized;
} page_conf;

typedef struct
{
	page_conf*configuration;
	
	FILE*output;
} page;

int add_media(const char*_filepath,const char*_filename);

void empty_space(const char *_style);

void hyperlink(const char*_link,const char*_body, const char*_style);
void image(const char*_path,const char*_alt, const char*_style);

void link_image(const char*_link,const char*_path,const char*_alt, const char*_style);

void sub_header(const char*_body, const char*_style);
void header(const char*_body, const char*_style);
void para(const char*_body, const char*_style);

void list_item(const char*_body, const char*_style);

void collapsible_list_item_begin(const char*_title, const char*_style);
void collapsible_list_item_end();

void ordered_list_begin(const char*_style);
void ordered_list_end(void);

void unordered_list_begin(const char*_style);
void unordered_list_end(void);

void div_begin(const char*_style);
void div_end(void);

int create_configuration(const char*_page_title, const char *_page_name, const char *_style_name, page_conf*_configuration);

void page_begin(page*_page,page_conf*_config);
void page_end();

void carol_compose(void);
int carol_init(void);

#endif//CAROL_H
#ifdef CAROL_IMPLEMENTATION

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 8192

#include <dirent.h>
#include <ctype.h>

#include <arpa/inet.h>

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <fcntl.h>
#include <regex.h>
#include <errno.h>

// globals
page *g_current_page=NULL;

static const char*get_file_extension(const char*_f);
static int concat(char**_str,const char*_new_str);

int add_media(const char*_filepath,const char*_filename)
{
	const char *prefix = "project/media/";
	char *final_filename = strdup(prefix);
	if ((concat(&final_filename, _filename)) == -1)
	{
		fprintf(stderr, "failed to allocate memory to final filename.\n");
		free(final_filename);
		return -1;
	}
	const char *file_ext = get_file_extension(final_filename);
	if (access(final_filename, F_OK) == 0 && strcasecmp(file_ext, "css") != 0)
	{
		fprintf(stderr, "not copying %s: media already exists in the given path.\n", _filepath);
		free(final_filename);
		return 0;
	}
    FILE*origin,*copy;
    char buffer[BUFFER_SIZE];
    size_t br;

    origin=fopen(_filepath,"rb");
    if(origin==NULL)
    {
        fprintf(stderr,"failed opening origin file.\n");
        return -1;
    }    
//    printf("final copied filename: %s.\n",final_filename);
    copy=fopen(final_filename,"wb");
    if(copy==NULL)
    {
        fprintf(stderr,"failed opening copy file.\n");
		free(final_filename);
        return -1;
    }
    printf("copying %s file to './project/media/%s'...\n",_filepath,_filename);
    while((br=fread(buffer,1,BUFFER_SIZE,origin))>0)
    {
        if(fwrite(buffer,1,br,copy)!=br)
        {
			fprintf(stderr,"failed writing to copy file.\n");
			free(final_filename);
			fclose(origin);
			fclose(copy);
			return -1;
        }
    }
    printf("%s copied successfully to %s.\n",_filepath,final_filename);
    
    free(final_filename);
    fclose(origin);
    fclose(copy);
    return 0;
}

static void open_page(void)
{
	if (g_current_page == NULL)
	{
		fprintf(stderr,"failed to write to page : begin a new page first.\n");
		return;
	}
	g_current_page->output = fopen(g_current_page->configuration->html_path, "a");
	if (g_current_page->output == NULL)
	{
		fprintf(stderr,"failed to open html page: page output is NULL.\n");
		return;
	}
	return;
}
static void close_page(void)
{
	if (g_current_page == NULL)
	{
		fprintf(stderr,"failed to close page: begin a new page first.\n");
		return;
	}
	if (g_current_page->output != NULL)
		fclose(g_current_page->output);
	return;
}
static void write_to_page(const char*_string)
{
	if (g_current_page == NULL)
	{
		fprintf(stderr,"failed to write page : begin a new page first.\n");	
		return;
	}
	if (_string == NULL)
	{
		fprintf(stderr,"failed to write page : string is null.\n");
		return;
	}
	if (g_current_page->output == NULL)
	{
		fprintf(stderr,"failed to write to page : open a page first.\n");
		return;
	}
	fputs(_string, g_current_page->output);
	return;
}

void header(const char*_body,const char*_style)
{
	if (_style == NULL)
	{
		write_to_page("<h1>");
		write_to_page(_body);
		write_to_page("</h1>");
	} else
	{
		write_to_page("<h1 style='");
		write_to_page(_style);
		write_to_page("'>");
		write_to_page(_body);
		write_to_page("</h1>");
	}
    printf("header element generated.\n");
    return;
}
void sub_header(const char*_body, const char*_style)
{
	if (_style == NULL)
	{
		write_to_page("<h3>");
		write_to_page(_body);
		write_to_page("</h3>");
	} else
	{
		write_to_page("<h3 style='");
		write_to_page(_style);
		write_to_page("'>");
		write_to_page(_body);
		write_to_page("</h3>");
	}
	return;
}

void para(const char*_body,const char*_style)
{
	if (_style == NULL)
	{
		write_to_page("<p>");
		write_to_page(_body);
		write_to_page("</p>");
	} else
	{
		write_to_page("<p style='");
		write_to_page(_style);
		write_to_page("'>");
		write_to_page(_body);
		write_to_page("</p>");
	}
	printf("paragraph element generated.\n");
    return;
}
void hyperlink(const char*_link,const char*_body,const char*_style)
{
	if (_style == NULL)
	{
		write_to_page("<a href='");
		write_to_page(_link);
		write_to_page("'>");
		write_to_page(_body);
		write_to_page("</a>");
	} else
	{
		write_to_page("<a href='");
		write_to_page(_link);
		write_to_page(" style='");
		write_to_page(_style);
		write_to_page("'>");
		write_to_page(_body);
		write_to_page("</a>");
	}
    printf("hyperlink element generated.\n");
    return;
}

void empty_space(const char *_style)
{
	if (_style)
	{
		write_to_page("<div style='");
		write_to_page(_style);
		write_to_page("'></div>");
	}
	printf("empty space element generated.\n");
	return;
}

void image(const char*_path,const char*_alt,const char*_style)
{
	const char *path_prefix = "/media/";
	char *final_path = strdup(path_prefix);
	if ((concat(&final_path,_path))==-1)
	{
		fprintf(stderr,"failed to concat final path buffer.\n");
		free(final_path);
		return;
	}
	if (_style == NULL)
	{
		write_to_page("<img src='");
		write_to_page(final_path);
		write_to_page("' alt='");
		write_to_page(_alt);
		write_to_page("'/>");
	} else
	{
		write_to_page("<img src='");
		write_to_page(final_path);
		write_to_page("' alt='");
		write_to_page(_alt);
		write_to_page("' style='");
		write_to_page(_style);
		write_to_page("'/>");
	}
	free(final_path);
    printf("image element generated.\n");
    return;
}
void link_image(const char*_link,const char*_path,const char*_alt,const char*_style)
{
	const char *path_prefix = "/media/";
	char *final_path = strdup(path_prefix);
	if ((concat(&final_path, _path)) == -1)
	{
		fprintf(stderr,"failed to concat final path buffer.\n");
		free(final_path);
		return;
	}
	if (_style == NULL)
	{
		write_to_page("<a href='");
		if (_link != NULL)
			write_to_page(_link);
		write_to_page("'><img src='");
		write_to_page(final_path);
		write_to_page("' alt='");
		write_to_page(_alt);
		write_to_page("' style='width: 100%;'/></a>");
	} else
	{
		write_to_page("<a href='");
		if (_link != NULL)
			write_to_page(_link);
		write_to_page("' style='");
		write_to_page(_style);
		write_to_page("'><img src='");
		write_to_page(final_path);
		write_to_page("' alt='");
		write_to_page(_alt);
		write_to_page("' style='width: 100%;'/></a>");
	}
	free(final_path);
    printf("image element generated.\n");
    return;
}

void list_item(const char*_body, const char*_style)
{
	if (_style == NULL)
	{
		write_to_page("<li>");
		write_to_page(_body);
		write_to_page("</li>");
	} else
	{
		write_to_page("<li style='");
		write_to_page(_style);
		write_to_page("'>");
		write_to_page(_body);
		write_to_page("</li>");
	}
	return;
}

void collapsible_list_item_begin(const char*_title, const char*_style)
{
	write_to_page(_style == NULL ? "<li style='overflow: hidden;'>": 
					"<li style='overflow: hidden;"); write_to_page(_style);
	if (_style != NULL)
		write_to_page("'>");
	write_to_page("<details>");
	write_to_page("<summary style='cursor: pointer; font-weight: bold; display: flex;"
				  "justify-content: space-between; align-items: center; user-select: none;"
				  "--webkit-details-marker: none; list-style: none;'>");
	write_to_page(_title);
	write_to_page("</summary>");
	write_to_page("<div style='padding-left: 30px;'>");
	return;
}
void collapsible_list_item_end(void)
{
	write_to_page("</div>");
	write_to_page("</details>");
	write_to_page("</li>");
	return;
}

void ordered_list_begin(const char*_style)
{
	if (_style == NULL)
	{
		write_to_page("<ol>");
	} else
	{
		write_to_page("<ol style='");
		write_to_page(_style);
		write_to_page("'>");
	}
	return;
}
void ordered_list_end(void)
{

	write_to_page("</ol>");
	return;
}

void unordered_list_begin(const char*_style)
{
	if (_style == NULL)
	{
		write_to_page("<ul>");
	} else
	{
		write_to_page("<ul style='");
		write_to_page(_style);
		write_to_page("'>");
	}
	return;
}
void unordered_list_end(void)
{
	write_to_page("</ul>");
	return;
} 

void div_begin(const char*_style)
{
	if (_style == NULL)
	{
		write_to_page("<div>");
	} else
	{
		write_to_page("<div style='");
		write_to_page(_style);
		write_to_page("'>");
	}
    printf("div element opened.\n");
    return;
}
void div_end(void)
{
	write_to_page("</div>");
    printf("div element closed.\n");
    return;
}

// USE WITH CAUTION: the final _str MUST BE FREED after use.
static int concat(char**_str,const char*_new_str)
{
    // TODO: better error messages;
    if (_new_str == NULL)
    return -1;
    if (*_str == NULL)
    return -1;
    size_t length_new = strlen(_new_str);
    size_t length_old = strlen(*_str);
    
    char *new_string = (char*)malloc(length_old + length_new + 1);
    if (new_string==NULL)
    {
	fprintf(stderr,"failed to allocate memory for new string.\n");
	return -1;
    }
    memcpy(new_string,*_str,length_old);
    memcpy(new_string + length_old, _new_str, length_new + 1);
    
    free(*_str);
    *_str = new_string;
    
    return 0;
}

static const char*get_file_extension(const char*_filename)
{
	const char*dot=strrchr(_filename,'.');
	if(!dot||dot==_filename)
		return "";
	return dot+1;
}

static const char*get_filetype(const char*_file_extension)
{
    if(strcasecmp(_file_extension,"html")==0||strcasecmp(_file_extension,"htm")==0)
        return "text/html";
    else if(strcasecmp(_file_extension,"txt")==0)
        return "text/plain";
    else if(strcasecmp(_file_extension,"jpg")==0||strcasecmp(_file_extension,"jpeg")==0)
        return "image/jpeg";
    else if(strcasecmp(_file_extension,"png")==0)
        return "image/png";
    else if(strcasecmp(_file_extension,"webp")==0)
        return "image/webp";
    else if(strcasecmp(_file_extension,"css")==0)
        return "text/css";
    else
        return "application/octet-stream";
}

static char*url_decode(const char*_src)
{
    size_t src_length=strlen(_src);
    char*decoded=malloc(src_length+1);
    if (decoded==NULL)
    {
	fprintf(stderr,"failed to allocate memory for decoded URL.\n");
	return NULL;
    }
    size_t decoded_length=0;
    
    for(size_t i=0;i<src_length;i++)
    {
        if(_src[i]=='%'&&i+2<src_length)
        {
            int value;
            sscanf(_src+i+1,"%2x",&value);
            decoded[decoded_length++]=value;
            i+=2;
        }
        else
        {
            decoded[decoded_length++]=_src[i];
        }
    }
    decoded[decoded_length]='\0';
    return decoded;
}

static void mount_style(page*_page)
{
    const char*prefix="project/";
    
    char*final_style=strdup(prefix);
    if (final_style==NULL)
    {
		fprintf(stderr,"failed to strdup final_style.\n");
		_page->output = NULL;
		return;
    }
    if ((concat(&final_style,_page->configuration->style_path))==-1)
    {
		fprintf(stderr, "failed to concat final style buffer.\n");
		_page->output = NULL;
		free(final_style);
		return;
    }
    
    if (access(final_style, F_OK) == 0) // didn't want to do that but whatever
    {
		printf("style file already exist.\n");
		return;
    }
    
    if (_page->configuration->style_path != NULL)
    {
		free(_page->configuration->style_path);
    }
    _page->configuration->style_path=strdup(final_style);
    if (_page->configuration->style_path==NULL)
    {
		fprintf(stderr,"failed to strdup style_path");
		_page->output = NULL;
		free(final_style);
		return;
    }
    
    FILE*st=fopen(final_style,"a");
    if(st==NULL)
    {
		fprintf(stderr,"failed to create or write to styles.css file.\n");
		
		free(_page->configuration->style_path);
		free(final_style);
		
		_page->output=NULL;
		return;
    }
    fputs("\n",st);
    fputs("body,html {\n",st);
    fputs("     background-color:#FFFDD0;\n",st);
    fputs("     padding:         0;\n",st);
    fputs("     margin:          5px;\n",st);
    fputs("     left:            0;\n",st);
    fputs("     top:             0;\n",st);
    fputs("\n",st);
    fputs("     min-height:      100vh;\n",st);
    fputs("     min-width:       100vw;\n",st);
    fputs("\n",st);
    fputs("     overflow:        hidden;\n",st);
    fputs("}\n",st);
    fputs("\n",st);
    free(final_style);
    fclose(st);
}

// NOTE : do not know if this function will last so long, but I don't think it's very pratical to create a configuration
//        struct everytime I want to create a page. As the number of possible configuration for the user over the page,
//        increases, more this function will becoming unfeasible and unmanageable.
int create_configuration(const char*_page_title,const char*_page_name,const char*_style_name,page_conf*_configuration)
{
	char*final_page_name = strdup(_page_name); char*final_style_name = strdup(_style_name);
	if (final_page_name==NULL || final_style_name==NULL)
	{
		fprintf(stderr,"failed to strdup final_page_name and/or final_style_name");
		return -1;
	}
	if ((concat(&final_style_name,".css"))==-1)
	{
		fprintf(stderr,"failed to concat final style file name.\n");
		free(final_style_name);
		return -1;
	}
	if ((concat(&final_page_name,".html"))==-1)
	{
		fprintf(stderr,"failed to concat final page file name.\n");
		free(final_page_name);
		return -1;
	}
	_configuration->style_path = strdup(final_style_name);
	_configuration->html_path = strdup(final_page_name);
	_configuration->title = strdup(_page_title);
	
	free(final_style_name);
	free(final_page_name);
	return 0;
}

char *g_html_header="<!DOCTYPE html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>%s</title><link rel='stylesheet' type='text/css' href='%s'/></head><body>";
char *g_html_footer="</body></html>";

void page_begin(page*_page,page_conf*_config)
{
	const char*prefix="project/";
	
	// TODO: check for empty strings here;
	_page->configuration=_config;
	
	printf("initializing %s page...\n\n",_page->configuration->html_path);
	if(_page->configuration->style_path!=NULL)
		mount_style(_page);
	else
		printf("no stylesheet configured...\n");
	char*final_path = strdup(prefix);
	if (final_path==NULL)
	{
		fprintf(stderr,"failed to strdup final_path.\n");
		return;
	}
	if ((concat(&final_path,_page->configuration->html_path))==-1)
	{
		fprintf(stderr,"failed to allocate concat final path buffer.\n");
		free(final_path);
		return;
	}
	if (_page->configuration->html_path!=NULL)
		free(_page->configuration->html_path);
	_page->configuration->html_path=strdup(final_path);
	
	if (_page->configuration->html_path == NULL)
	{
		fprintf(stderr,"failed to strdup html_path.\n");
		free(final_path);
		return;
	}
	
	// mounting final header, with title and styles file (hardcoded... for now...)
	size_t header_length=strlen(g_html_header)+strlen(_page->configuration->title)+strlen("./index.css")+1;
	char*final_header=(char*)malloc(header_length);
	if(final_header==NULL)
	{
		fprintf(stderr,"failed to allocate memory to final html header.\n");
		fclose(_page->output);
		return;
	}
	snprintf(final_header,header_length,g_html_header,_page->configuration->title,"./index.css");
	
	_page->output = fopen(_page->configuration->html_path, "w");
	if (_page->output == NULL)
	{
		fprintf(stderr, "failed to open page: page output is NULL.\n");
		return;
	}
		fputs(final_header, _page->output);
	fclose(_page->output);
	
	_page->configuration->initialized=true;
	free(final_header);
	free(final_path);
	
	g_current_page=_page;
	open_page();
	printf("%s page initialized successfully.\n",_page->configuration->html_path);
	return;
}

void page_end()
{
    if(g_current_page==NULL){
        fprintf(stderr,"no page had been started.\n");
        return;
    }
	write_to_page(g_html_footer);
	close_page();
	
    free(g_current_page->configuration->style_path);
    free(g_current_page->configuration->html_path);
    free(g_current_page->configuration->title);
	
    printf("\npage closed successfully.\n");
    return;
}

static bool handle_request(int*_client)
{
    char*buffer=(char*)malloc(BUFFER_SIZE*sizeof(char)+1);
    if (buffer == NULL)
    {
	fprintf(stderr,"failed to allocate memory for final buffer.\n");
	return false;
    }
    char*response;

    ssize_t received=recv(*_client,buffer,BUFFER_SIZE,0);
    if(received>0)
    {
        buffer[received]='\0';

        regex_t reg;
        regcomp(&reg,"^GET /([^ ]*) HTTP/1",REG_EXTENDED);
        regmatch_t matches[2];

        if(regexec(&reg,buffer,2,matches,0)==0)
        {
            buffer[matches[1].rm_eo]='\0';
            const char*encoded=buffer+matches[1].rm_so;
            char*filename=url_decode(encoded);
	    if (filename==NULL)
	    {
		fprintf(stderr,"failed to url decode filename.\n");
		free(buffer);
		
		regfree(&reg);
		return false;
	    }
	    char file_extension[32];
	    strncpy(file_extension,get_file_extension(filename),31);
	    if (file_extension[0]=='\0')
	    {
		fprintf(stderr,"failed to strncpy file_extension.\n");
		regfree(&reg);
		free(buffer);
		return false;
	    }
	    file_extension[31]='\0';

		const char*type=get_filetype(file_extension);
		if(strcasecmp(type,"text/html")==0)
			carol_compose();
	    
		char*header=(char*)malloc(BUFFER_SIZE*sizeof(char));
		response=(char*)malloc(BUFFER_SIZE*2*sizeof(char));
	    
	    int file=open(filename,O_RDONLY);
	    if(file==-1)
	    {
		free(filename);
		free(response);
		free(header);
		
		regfree(&reg);
		goto ERROR_404;
	    }
	    
	    struct stat st;
	    if((fstat(file,&st))==-1)
	    {
		free(filename);
		free(response);
		free(header);
		close(file);
		
		regfree(&reg);
		goto ERROR_404;
	    }
	    snprintf(header,BUFFER_SIZE,
	    "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %lld\r\n\r\n",type,(long long)st.st_size);
	    send(*_client,header,strlen(header),0);

	    memcpy(response,header,strlen(header));
	    ssize_t bytes_read;
	    while((bytes_read=read(file,buffer,BUFFER_SIZE))>0)
	    {
		ssize_t total=0;
		while(total<bytes_read)
		{
		    ssize_t sent=send(*_client,buffer+total,bytes_read-total,0);
		    if(sent<=0)
		    {
			free(buffer); free(header);
			free(response);
			free(filename);
			close(file);
			
			regfree(&reg);
			return false;
		    }
		    total+=sent;
		}
	    }
	    free(filename);
	    free(response);
	    free(header);
	    close(file);
	}
	regfree(&reg);
    }
    shutdown(*_client, SHUT_WR);
    close(*_client);
    free(buffer);
    
    printf("request handled successfully.\n");
    return true;
ERROR_404:
    if (response == NULL)
    {
	response = (char*)malloc(BUFFER_SIZE * 2 * sizeof(char));
    }
    snprintf(response,BUFFER_SIZE,
    "HTTP/1.1 404 Not Found\r\nContext-Type: text/html\r\n\r\n<h1>404 Not Found</h1>");
    shutdown(*_client, SHUT_WR);
    close(*_client);
    free(buffer);
    return false;
}

static int init_server(int*_server,int*_opt,struct sockaddr_in*_server_address)
{
    if((*_server=socket(AF_INET,SOCK_STREAM,0))<0)
    {
	fprintf(stderr,"socket failed.\n");
	return -1;
    }
    if(setsockopt(*_server,SOL_SOCKET,SO_REUSEADDR,_opt,sizeof(*_opt))<0)
    {
	fprintf(stderr,"socket opt setting failed.\n");
	return -1;
    }
    _server_address->sin_family=AF_INET;
    _server_address->sin_addr.s_addr=INADDR_ANY;
    _server_address->sin_port=htons(DEFAULT_PORT);

    if(bind(*_server,(struct sockaddr*)_server_address,sizeof(*_server_address))<0)
    {
        fprintf(stderr,"binding failed.\n");
        return -1;
    }
    if(listen(*_server,10)<0)
    {
        fprintf(stderr,"listening failed.\n");
        return -1;
    }
	printf("\n----------------------------------------------------\n");
    printf("\nlistening  at http://localhost:%d.\n\n",DEFAULT_PORT);
    return 0;
}

int carol_init(void)
{
    mkdir("project",0777);
    mkdir("project/media",0777);

    int server; int opt=1;
    struct sockaddr_in server_addr;
    if(init_server(&server,&opt,&server_addr)!=0)
    return -1;
    while(true)
    {
        struct sockaddr_in client_address;
        socklen_t client_address_length=sizeof(client_address);
	int client;
        if((client=accept(server,(struct sockaddr*)&client_address,&client_address_length))<0)
        {
            fprintf(stderr,"acception failed");
            continue;
        }
        handle_request(&client);
    }
    close(server);
    return 0;
}

#endif//CAROL_IMPLEMENTATION
