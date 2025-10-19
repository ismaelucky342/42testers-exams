#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct pair { char *key; struct json value; } pair;
typedef struct json {
    enum { MAP, INTEGER, STRING } type;
    union { struct { pair *data; size_t size; } map; int integer; char *string; };
} json;

int peek(FILE *s){int c=getc(s);ungetc(c,s);return c;}
void unexpected(FILE *s){printf("unexpected %s\n", peek(s)!=EOF?(char[]){'\'',peek(s),0}:"end of input");}
int accept(FILE*s,char c){return peek(s)==c?(void)getc(s),1:0;}
int expect(FILE*s,char c){return accept(s,c)?1:(unexpected(s),0);}

int parse_int(json*d,FILE*s){int n;return fscanf(s,"%d",&n)==1?(d->type=INTEGER,d->integer=n,1):(unexpected(s),-1);}
int parse_string(json*d,FILE*s){char buf[4096],c;int i=0;if(!expect(s,'"'))return-1;while((c=getc(s))!='"'){if(c==EOF)return unexpected(s),-1;if(c=='\\')c=getc(s);buf[i++]=c;}buf[i]=0;return d->type=STRING,d->string=strdup(buf),1;}

int parser(json*d,FILE*s); // forward
int parse_map(json*d,FILE*s){pair*it=NULL;size_t n=0;json key;if(!expect(s,'{'))return-1;while(!accept(s,'}')){it=realloc(it,sizeof(pair)*(n+1));if(parse_string(&key,s)==-1){free(it);return-1;}if(!expect(s,':')){free(key.string);free(it);return-1;}if(parser(&it[n].value,s)==-1){free(key.string);free(it);return-1;}it[n].key=key.string;n++;if(!accept(s,',') && peek(s)!='}') return unexpected(s),free(it),-1;}return d->type=MAP,d->map.size=n,d->map.data=it,1;}

int parser(json*d,FILE*s){int c=peek(s);return c=='"'?parse_string(d,s):isdigit(c)||c=='-'?parse_int(d,s):c=='{'?parse_map(d,s):(unexpected(s),-1);}
int argo(json*d,FILE*s){return parser(d,s);}

void free_json(json j){switch(j.type){case MAP:for(size_t i=0;i<j.map.size;i++){free(j.map.data[i].key);free_json(j.map.data[i].value);}free(j.map.data);break;case STRING:free(j.string);break;default:break;}}
void serialize(json j){switch(j.type){case INTEGER:printf("%d",j.integer);break;case STRING:putchar('"');for(int i=0;j.string[i];i++){if(j.string[i]=='"'||j.string[i]=='\\')putchar('\\');putchar(j.string[i]);}putchar('"');break;case MAP:putchar('{');for(size_t i=0;i<j.map.size;i++){if(i)putchar(',');serialize((json){.type=STRING,.string=j.map.data[i].key});putchar(':');serialize(j.map.data[i].value);}putchar('}');break;}}

int main(int argc,char **argv){
    if(argc!=2)return 1;
    FILE *s=fopen(argv[1],"r");if(!s)return 1;
    json j;if(argo(&j,s)!=1){free_json(j);fclose(s);return 1;}
    serialize(j);printf("\n");free_json(j);fclose(s);
}
