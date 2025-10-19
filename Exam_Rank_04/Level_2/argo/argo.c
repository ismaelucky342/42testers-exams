#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct pair; struct json;
typedef struct json{enum{MAP,INTEGER,STRING}type;union{struct{struct pair*data;size_t size;}map;int integer;char*string;};}json;
typedef struct pair{char*key;json value;}pair;

int peek(FILE*s){int c=getc(s);return ungetc(c,s),c;}
void unexpected(FILE*s){int c=peek(s);printf("unexpected %s\n",c==EOF?"end of input":(char[]){'t','o','k','e','n',' ','\'',c,'\'',0});}
int accept(FILE*s,char c){return peek(s)==c?(getc(s),1):0;}
int expect(FILE*s,char c){return accept(s,c)?1:(unexpected(s),0);}
int parse_int(json*d,FILE*s){int n;return fscanf(s,"%d",&n)==1?(d->type=INTEGER,d->integer=n,1):(unexpected(s),-1);}
int parse_string(json*d,FILE*s){char b[4096],c;int i=0;if(!expect(s,'"'))return-1;while((c=getc(s))!=EOF){if(c=='"')return b[i]=0,d->type=STRING,d->string=strdup(b),1;if(c=='\\'&&(c=getc(s))==EOF)break;b[i++]=c;}unexpected(s);return-1;}

int parser(json*d,FILE*s);
int parse_map(json*d,FILE*s){
    pair*it=NULL;size_t n=0;json key;
    if(!expect(s,'{'))return-1;
    while(!accept(s,'}')){
        it=realloc(it,sizeof(pair)*(n+1));
        if(parse_string(&key,s)==-1)return free(it),-1;
        if(!expect(s,':'))return free(key.string),free(it),-1;
        if(peek(s)=='}'){unexpected(s);free(key.string);free(it);return-1;}
        if(parser(&it[n].value,s)==-1){free(key.string);free(it);return-1;}
        it[n].key=key.string;n++;
        if(!accept(s,',')&&peek(s)!='}')return unexpected(s),free(it),-1;
    }
    return d->type=MAP,d->map.size=n,d->map.data=it,1;
}
int parser(json*d,FILE*s){int c=peek(s);return c==EOF?(unexpected(s),-1):c=='"'?parse_string(d,s):isdigit(c)||c=='-'?parse_int(d,s):c=='{'?parse_map(d,s):(unexpected(s),-1);}
int argo(json*d,FILE*s){return parser(d,s);}

void free_json(json j){if(j.type==MAP){for(size_t i=0;i<j.map.size;i++)free(j.map.data[i].key),free_json(j.map.data[i].value);free(j.map.data);}else if(j.type==STRING)free(j.string);}
void serialize(json j){
    j.type==INTEGER?printf("%d",j.integer):
    j.type==STRING?(putchar('"'),({for(int i=0;j.string[i];i++){if(j.string[i]=='"'||j.string[i]=='\\')putchar('\\');putchar(j.string[i]);}putchar('"');}),0):
    ({putchar('{');for(size_t i=0;i<j.map.size;i++){if(i)putchar(',');serialize((json){.type=STRING,.string=j.map.data[i].key});putchar(':');serialize(j.map.data[i].value);}putchar('}');});
}
int main(int c,char**v){if(c!=2)return 1;FILE*s=fopen(v[1],"r");if(!s)return 1;json j;if(argo(&j,s)!=1)return fclose(s),1;serialize(j);puts("");free_json(j);fclose(s);}
