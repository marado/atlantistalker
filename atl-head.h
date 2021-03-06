#include <stdio.h>
#ifdef _AIX
#include <sys/select.h>
#endif
#include <fcntl.h>
#include <time.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/vfs.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/telnet.h>
#include <sys/poll.h>
#include <setjmp.h>
#include <stdarg.h>
#include <libxml2/libxml/xmlwriter.h>
#include <locale.h>

#if defined(HAVE_CONFIG)
#include "config.h"
#else
#include "config-sample.h"
#endif

#ifdef HPUX_SB
#define FD_CAST (int *)
#else
#define FD_CAST (fd_set *)
#endif

#ifdef GLIBC207
#define SIGNAL(x,y) sysv_signal(x,y)
#else
#define SIGNAL(x,y) signal(x,y)
#endif

int wrtype,pollerror;

#define WR_IOPHR 1
#define WR_ZVERY 2
#define WR_NEWBIE 3
#define WR_AVOID_PREDMETS 4
#define WR_NOEDITOR 5
#define WR_NOCOL 6
#define WR_WITHCOL 7

#define RESCNUM 500
char rescip[RESCNUM][17];
char rescho[RESCNUM][52];
int rescn;
int resc_resolved;
int resc_cached;

int counter_db(int add);

int pocet_vtipov;
int newusers;

int doplnovanie_predmetov,ryxlost_doplnovania,predmety_dynamic;
int pocet_predmetov,predmety_time,predmety_position,pocet_konverzii;
int zistuj_identitu;
int inyjefuc;

int max_users_was;
int all_logins;
int sayfloodnum;
int passwd_simplex;
int hb_can;
int hb_sec;

int daily_num;
int every_min_num,burkamin,tunelik,brutalis_gate,portalis_gate;
size_t tajmauts,tajmautz;
int writeerror,max_timeouts,max_timeoutz;

#define VERSION "3.3.3"
#define WORK "modifikacie: Spartakus, Rider, Buko, Hruza, Viper"
#define MOZENADAVAT GOD
#define MOZEFARBICKOVAT KIN
#define TIMEOUT 3
#define EXECPREFIX "./"

#define FILESYSTEM "/"
#define LOW_DISK_LIMIT 1024

#define POCET_KUZIEL 16

#define SPELLK	   20
#define MAXPP      200
#define INCPP      1
#define NODYNAMIC  0
 
#define SHOUTDEC   35
#define SHOUTCAN   105
#define SHOUTDYN   85

#define SEMOTEDEC  35 
#define SEMOTECAN  105
#define SEMOTEDYN  85
 
#define SECHODEC   55 
#define SECHOCAN   130
#define SECHODYN   120 
 
#define BANNERDEC  70 
#define BANNERCAN  140
  
#define SBANNERDEC 200 
#define SBANNERCAN 170
#define SBANNERDYN 200
 
#define HUGDEC     10 
#define HUGCAN     40 
 
#define WAKEDEC    10 
#define WAKECAN    40   
 
#define FIGHTDEC   80 
#define FIGHTCAN   160 
#define FIGHTDECARENA 60
#define FIGHTCANARENA 100 

#define KICKDEC   100 
#define KICKCAN   120 

#define KISSDEC    10
#define KISSCAN    60

#define KIDDEC     50
#define KIDCAN     80

#define TOPICDEC     40
#define TOPICCAN     100
 
#define THROWDEC     20
#define THROWCAN     50
#define STEALCAN     50
#define THIEFDEC     30
#define VICTIMDEC    10

#define PRIVPUBDEC   40
#define PRIVPUBCAN   100

/* adresare */
#define MISCFILES "misc" /* misc-subory */
#define BACKUP_DIR "backups" /* zalohy */
#define DATAFILES "data" /* data-subory */
#define CLOVECEFILES "saves-clovece" /* ulozene hry clovece */
#define LODICKY_SAVE_DIR "saves-lodicky" /* ulozene hry lodiciek */
#define DAMAFILES "saves-dama" /* ulozene hry damy */
#define MAILSPOOL "mailspool"
#define POMOCFILES "help" /* pomoc-subory */
#define LOGFILES "log" /* logy */
#define ITEMFILES "predmety" /* popisky predmetov */

/* misc-subory */
#define DICT "dict" /* slovnik lamerskyx hesiel */
#define ZOZNAM_USEROV "users_list" /* zoznam userov */
#define ADRESY_USEROV "users_emails" /* e-mailove adresy userov */
#define MULTI_REQUEST "multirequest" /* pokusy requestov na uz pouzite adresy */
#define COM_LEVEL "comlevel" /* prehodene levely k prikazom */
#define NO_QUOTA "noquota" /* kto nema kvotu na postu */
#define HINT_FILE "hints" /* zoznam hintov */
#define NICKBLOCK  "nickblock" /* vyblokovane nicky */
#define RESC_CACHE "resc_cache" /* cache pre resolvera */
#define GOT_AUTH  "users_auth" /* real identity z forknuteho tisu */
#define IRC_SERVERS "ircserv" /* povolene IRC servre */
#define TALKERS_LIST "talkers" /* zoznam inych talkerov */
#define FORTUNE_FILE "fortune" /* subor s mudrostami */
#define NOTICEBOARD "noticeboard" /* staznosti, ospravedlnenia, navrhy - archiv */
#define NOTICE_DIGEST "notice_digest"  /* denny vyber z ^^^ */
#define SITEBAN "siteban" /* vybanovane sites */
#define NEWUSER_SITEBAN "newban" /* sites vybanovane iba na vytvaranie novych uctov */
#define REMOTE_FILE "remote" /* konfigurak connecticutu :-) */
#define STATSFILE "statistika"
#define CHECKFILE "kontrola"
#define CRASHFILE "crash"
#define BOMBFILE "bomb"
#define MAXTIMEOUTS "timeouts.max"

/* datafiles-subory */
#define MENINY_FILE "meniny.dat" /* kalendar */
#define LAB_FILE  "labdata.dat" /* labyrint - nazov datoveho suboru */
#define NEWUSER_INFO "newuser_info" /* informacie pre noveho userka - novy nick pri logine */
#define HANGMAN_WORDS "hangman" /* slova do obesenca */
#define LABYRINT_WINNERS "labyrint.win" /* vitazi labu */
#define WEB_BOARD_DAT "webboard.dat" /* kto pisal na board a kde */

/* log-subory */
#define AKLIENT_LOG_USERS "akl_user.log" /* log userov z klienta */
#define AKLIENT_LOG_VER "akl_ver.log" /* log pristupov z verzii klienta */
#define LEVLOG "level.log" /* log promotes/demotes */
#define COMMLOG "command.log" /* command debug log */
#define TLT_LOG "tlt.log"
#define SYSLOG "system.log"
#define WIZLOG "wizzes.log"
#define GAMES_LOG "games.log"
#define SENT_SMS "sentsms.log"
#define SENT_MAIL "sentmail.log"

#define TMPSUFFIX ".talkertmp" /* koncovka docasnych suborov pre vypisy who, boardov a podobne */

/* pocasie-subory */
#define WEATHER_TODAY_FILE    "pocasie/weather.today"
#define WEATHER_TOMORROW_FILE "pocasie/weather.tomorrow"
#define WEATHER_STRED_FILE    "pocasie/weather.stred"
#define WEATHER_VYCHOD_FILE   "pocasie/weather.vychod"
#define WEATHER_ZAPAD_FILE    "pocasie/weather.zapad"
#define WEATHER_BLAVA_FILE    "pocasie/weather.blava"
#define WEATHER_TATRY_FILE    "pocasie/weather.tatry"

#define KOLOS_FILES "amfiteater"
#define KOLOS_PROGRAM "program.atl"
#define KOLOS_BANNER "predstavenie.atl"

/* web-subory */
#define WHO_FOR_WEB "who.raw" /* who pre web - surovy fajl */

/* Status line */
enum statline {NONEMODE,UNKNOWN,LINEMODE,CHARMODE};

#define MAX_CONNECTIONS 5
#define MAX_POLL 2000

#define AUTOSAVE 300

#define HISTORY_LINES 10
#define HISTORY_LEN 20

#define DB_JAIL 1
#define DB_DEMOTE 2
#define DB_BAN 3

#define MAIL_QUOTA 50000 /* odkedy bude upozornovat na preplnenu sxranku */
#define MAIL_LIMIT 55000 /* odkedy uz nebude moct dostavat postu!!! */

#define NUMBER_OF_LOOPS 75 /* kolko desiatok microsekund na resolvnutie */
#define ADRESIZE 512       /* maximalna dlzka hostname         */

#define MAX_POCET_LOGINOV 10 /* kolko ludi sa moze z 1 adresy naraz prihlasovat (rata sa len login stage), prevencia proti floodu */
			        
#define PROMPT_LEN 120 /*dlzka promptu!*/

#define REMOTE_NAME_LEN 256
#define REMOTE_DESC_LEN 256

#define ANTI_IDLE_TIME_SEC 0

int size_fds;

struct remote_struct {
	char name[REMOTE_NAME_LEN];
	char desc[REMOTE_DESC_LEN];
	char shortcut;
	int port;
	struct hostent *he;
	struct remote_struct *next;
};
		     
typedef struct remote_struct *RN_OBJECT;

RN_OBJECT remote_start,remote_last,remote_active;

int remote_slot_active;
sigjmp_buf save_state;

extern int h_errno;

#define LAB_ROOMS 65            /* Maximalny pocet miestnosti */
#define LAB_DESC  900            /* Maximalna velkost popisu miestnosti */
#define MAX_LINE  100            /* Maximalna dlzka jedneho riadku z fajlu */

#define OUT_BUFF_SIZE 3000
#define MAX_WORDS 100
#define MAX_IRC_WORDS 15
#define IRC_WORD_LEN 600
#define WORD_LEN 120  
#define ARR_SIZE 800
#define MAX_LINES 100 
#define NUM_COLS 38

#define MAX_COPIES 10

#define ARENA_ROOM "arena"   /* Rooma reprezentujuca arenu */

#define DESC_CHAR_LEN 400 /* Kolko ZNAKOV (aj tyx coolblbosti) moze mat desc */
#define COOLBUF_LEN 1500   
#define MAX_LAME 1450     /* Cooltalk */
#define MAX_CISLO 8
#define COOLTALK_LEN 80

#define USER_NAME_LEN 12
#define USER_DESC_LEN 40 /* C00ltalk */

#define AFK_MESG_LEN 120
#define PHRASE_LEN 45
#define MAXSLOHY 4    /* kolko sluoh mozno naraz vypotit .poet-om (flood)  */

#define EMAIL_LEN 60
#define PASS_LEN 10 /* only the 1st 8 chars will be used by crypt() though */
#define PASS_MIN_LEN 4
#define PASS_MIN_LEN_WIZ 6
#define WIZPASS_MIN_LEN 3
#define BUFSIZE 900
#define ROOM_NAME_LEN 20
#define ROOM_LABEL_LEN 5
#define TOPIC_LEN 70  /* C00ltalk */
#define MAX_LINKS 8
#define REVB_LINES 10
#define REVIEW_LINES 30
#define REVTELL_LINES 50
#define REVIRC_LINES 50
#define LASTLOG_LINES 50
#define REVIEW_LEN 300
#define HOMEPAGE_LEN 60

#define MPVM 25 /* Maximum predmetov v miestnosti */
#define HANDS 4 /* pocet chapadiel usera */
#define BODY HANDS+3 /* HANDS + cepicka + oblecenie + boty */


struct {
	int start;
	int end;
	int sever[LAB_ROOMS],juh[LAB_ROOMS],vychod[LAB_ROOMS],zapad[LAB_ROOMS],hore[LAB_ROOMS],dole[LAB_ROOMS];
	char popiska[LAB_ROOMS][LAB_DESC];
	int dostupny;
} lab_room;

struct pager_struct {
	int filepos;
	int messnum;
	struct pager_struct *next;
};                                   

typedef struct pager_struct *PAGER; 

struct notify_struct {
	char meno[13];
	struct notify_struct *next;         
};

typedef struct notify_struct *NOTIFY;

struct dama_struct {
	struct user_struct *hrac[2];
	int plocha[8][8],forcejump,remiza[2],monochrom[2];
	int natahu,lastmove[2],disptype[2];
	int tah;
};

typedef struct dama_struct* DAMA_OBJECT;

#define PUBLIC 0
#define PRIVATE 1
#define FIXED 2
#define FIXED_PUBLIC 2
#define FIXED_PRIVATE 3
#define GOD_PRIVATE 5

#define NEW 0
#define CIT 1
#define SOL 2
#define WAR 3
#define SAG 4
#define PRI 5
#define WIZ 6
#define KIN 7
#define GOD 8
#define POS 9

#define DB_ARENA     1  /* Nemenit! bo sa presunu body a mena v tabulach (V) */
#define DB_PISKVORKY 2
#define DB_HANGMAN   3
#define DB_QUEST     4
#define DB_DOOM      5
#define DB_LETUN     6
#define DB_REVERSI   7
#define DB_DAMA      8
#define DB_MINY      9
#define DB_BRUTALIS 10

#define USER_TYPE 0
#define CLONE_TYPE 1
#define CLONE_HEAR_NOTHING 0
#define CLONE_HEAR_SWEARS 1
#define CLONE_HEAR_ALL 2

int resolve_ip;
char revshoutbuff[REVIEW_LINES+1][REVIEW_LEN+2];
int revshoutline;
char revbcastbuff[REVB_LINES+1][REVIEW_LEN*2+2];
int revbcastline;
char revsosbuff[REVIEW_LINES+1][REVIEW_LEN+2];
int revsosline;
char revporshbuff[REVIEW_LINES+1][REVIEW_LEN+2];
int revporshline;
char revgossipbuff[REVIEW_LINES+1][REVIEW_LEN+2];
int revgossipline;
char revquestbuff[REVIEW_LINES+1][REVIEW_LEN+2];
int revquestline;
char revwizshoutbuff[REVIEW_LINES+1][REVIEW_LEN+2];
int revwizshoutline;
char lastlog[LASTLOG_LINES+1][REVIEW_LEN+2];
int lastlogline;

struct cl_struct {
	struct user_struct *hrac[4],*loadhrac[4];
	int monochrom[4],figurka[4][4],usertahs[4],remiza[4];
	int natahu,tahov,locked,kocka,loadlock;
	char revbuff[REVIEW_LINES+1][REVIEW_LEN+2];
	int revline;
};

typedef struct cl_struct* CL_OBJECT;

struct fr_struct {
	struct user_struct *hrac[5];
	int karty[32];
	int active[5];
	int natahu,action;
	char revbuff[REVIEW_LINES+1][REVIEW_LEN+2];
	int revline;
};

typedef struct fr_struct* FR_OBJECT;

struct xa_struct {
	struct user_struct *creator;
	char *buffer[REVIEW_LINES+1];
	char topic[60];
	int line,type;
};

typedef struct xa_struct* XA_OBJECT;

struct p_struct {
	char name[21];
	char dativ[21];
	char akuzativ[21];
	char inytiv[21];
	char food[21];
	int type,weight,dur,price,function,amount;
	int attack,firerate,seconddur,pp,restrikt,tajm;
	long altfunct;
	char *enter, *leave, *ustart, *rstart, *ustop,*rstop;
	char  *uinphr, *uoutphr, *rinphr, *routphr;
	char *userphr, *roomphr, *victimphr;
	char *special, *picture, *error;
	int showpict,ofense,defense;
	char *udestroy,*rdestroy;
	char *ujoinphr,*rjoinphr,*searchphr;
	struct room_struct *spawn;
	int spawnarea;
};

typedef struct p_struct* P_OBJECT;

struct co_struct {
	int component[4];
	int eliminate[4],setowner;
	int product,source,destination,mana,heal;
	char *usermsg, *roommsg,*missing;
	char spell[21];
};

typedef struct co_struct* CO_OBJECT;

struct user_struct { 
	int id;
	int visitor;
	char name[USER_NAME_LEN+1]; 
	char desc[DESC_CHAR_LEN+1];
	char pass[PASS_LEN+50]; 
	char wizpass[PASS_LEN+50]; 
	char email[EMAIL_LEN+1],requestemail[EMAIL_LEN+1];
	char sname[3][20];
	int zaradeny;
	int sex;
	char lasttellfrom[USER_NAME_LEN+1],lasttellto[USER_NAME_LEN+1];
	int lasttg;
	char prevname[USER_NAME_LEN+1]; 
	char follow[USER_NAME_LEN+1],smsfollow[15]; 
	char sell_to[USER_NAME_LEN+1]; 
	char in_phrase[PHRASE_LEN+1],out_phrase[PHRASE_LEN+1]; 
	char buff[BUFSIZE+10],site[82],last_site[82],page_file[82]; 
	char mail_to[WORD_LEN+1], *revbuff[REVTELL_LINES+1];
	char afk_mesg[AFK_MESG_LEN+1],inpstr_old[REVIEW_LEN+2]; 
	struct room_struct *room,*invite_room,*into,*from; 
	int type,port,site_port,login,socket,attempts;
	unsigned int buffpos,filepos; 
	int vis,ignall,prompt,command_mode,muzzled,muzzletime,charmode_echo; 
	int level,cloak,misc_op,edit_line,charcnt,warned; 
	int accreq,last_login_len,ignall_store,clone_hear,afk; 
	int edit_op,colour,ignore,ignshout,igntell,ignportal,ignbeep,ignio, revline,jailed;
	int ignsys,ignfun,ignzvery,igncoltell,ignportalis;
	int ignblink,ignpict,ignspell;
	time_t last_input,last_login,total_login,read_mail,first_login,idletime;
	char *malloc_start,*malloc_end;       
	struct user_struct *prev,*next,*owner; 
	int pp, mana;
	char homepage[HOMEPAGE_LEN+1];
	char lastcommand[23],lastcommand2[23];
	int pridavacas;
	int lastidle,lastidle2,lastidle3;
	char newpass[PASS_LEN+50];
	char chanp[USER_NAME_LEN+1];
	int has_ignall;
	char uname[20],pis[21][21],znak;
	int joined,tah,last_x,last_y,game,moves;
	int alarm, hang_stage;
	char copyto[MAX_COPIES+1][USER_NAME_LEN+1];
	char hang_word[60], hang_word_show[60], hang_guess[60];      
	int lines,storelines,colms,wrap,who_type,rt_on_exit;
	int predmet[BODY],dur[BODY],carry;
	int zuje, glue, prehana,stars,lsd;
	int zuje_t, kridla_t,prehana_t,prehana_t2, muzzle_t;
	int lieta, viscount, afro;
	char call[21];
	char shtable[10][10];
	int shmode, shbuild, shstatus, zasah;
	int browsing, messnum, lab;
	int lastm, autofwd, pagewho, examine, lang,com_priority;
	char real_ident[101];
	char remote_ident[81];
	char subject[150];
	int doom;
	int doom_energy;
	int doom_score;
	int doom_ammo[3];
	int doom_loading;
	int doom_weapon;
	int doom_heading;
	int doom_x;
	int doom_y;
	int doom_sayon;
	int newtell, tellpos;
	int com_counter;
	struct macro_struct *macrolist;
	struct notify_struct *notifylist,*ignorelist,*combanlist;
	int igngossip,quest,flood;
	char prompt_string[PROMPT_LEN+2];
	int dead;
	int rjoke_from;
	struct pager_struct *pager;
	int pageno;
	char ignword[WORD_LEN+1];
	int saturate,idle;
	int autosave,histpos;
	char history[HISTORY_LINES+1][HISTORY_LEN+1];
	char remote_name[USER_NAME_LEN+1],remote_passwd[PASS_LEN+100];
	int remote_login_socket,remote_login;
	struct remote_struct *remote[MAX_CONNECTIONS];
	int remote_socket[MAX_CONNECTIONS], ircsocknum;
	int actual_remote_socket,fds,remote_fds[MAX_CONNECTIONS];                   
	int akl;
	char irc_chan[50], irc_nick[21], irc_defnick[21], irc_name[51], irc_serv[12], *revirc[REVIRC_LINES+1];
	char channels[10][50];
	pid_t lynx;
	int sayflood, revircline, irc_reg;
	int statline, statlcount, newline, statlcan, remtelopt;
	struct blackjack_game_struct *bj_game;
	int ignlook;      
	int reversi_plan[8][8];
	struct user_struct *reversi_sh; /*SpoluHrac*/
	int reversi_natahu;
	int reversi_znak;
	int reversi_cislotahu;
	int reversi_jept;  /*Je Posledny Tah*/
	int reversi_ptX,reversi_ptY;  /*Posledny Tah X Y*/
	unsigned char **miny_tab;
	unsigned int miny_x,miny_y,miny_h;
	struct dama_struct *dama;
	struct cl_struct *clovece;  
	int chrac,clovecewin,dhrac;
	struct user_struct *clovece_starter,*dama_opp;
	char ipcka[20];
	int col[7];
	struct user_struct *farar_starter;
	struct fr_struct *farar;
	int fhrac,fararwin,cps,goafkafter,killmeafter;
	char reserved[10];
	int commused[11],ignafktell,igngames,mailnotify,ignautopromote;
	int totaljailtime,killed,totalmuzzletime,nontimejails;
	int gold,swapped,shoutswears,sayswears,hangups,hangupz;
	int ignxannel,ignnongreenhorn,multiply,lastrevt,lastrevs,lastrevi;
	struct xa_struct *xannel;
	struct user_struct *invite2xannel,*nox;
	int sell_price,sell_what,ep_line,affected,affecttime,affpermanent;
	int smsssent,smsday,allowsms,smswait,smsgate,smschars;
	char mobile[21],logoutmsg[61];
	int team,way,weapon,hidden,reveal,attacking;
	int kills,deaths,health,heal,deathtype,head,body,feets,target;
	int switches,shortcutwarning,temp,wizactivity,age,agecode;
	int longestsession,mail2sms,timeinrevt,miscoptime;
	int macro_num,notify_num,ap,websecond;
	int skip,exams,kontrola,messsize,special;
	char findstr[20],request[7];
	char shoutmsg[21],shoutmsg2[21],gossipmsg[21],gossipmsg2[21];
	char saymsg[21],saymsg2[21],tellmsg[21],tellmsg2[21];
	char wizshmsg[21],wizshmsg2[21];
	char where[31];
	xmlTextWriterPtr xml_writer;
	xmlBufferPtr xml_buffer;
	int output_format;
};   

typedef struct user_struct* UR_OBJECT;

UR_OBJECT user_first,user_last, remote_user_active;

struct pollfd fds[MAX_POLL];

struct room_struct {
	char name[ROOM_NAME_LEN+1];
	char name_en[ROOM_NAME_LEN+1];
	char label[ROOM_LABEL_LEN+1];
	char *desc_sk;
	char *desc_en;
	char topic[COOLBUF_LEN+1];
	char *revbuff[REVIEW_LINES+1];
	char from[ROOM_NAME_LEN+21];
	char into[ROOM_NAME_LEN+21];
	char where[ROOM_NAME_LEN+21];
	char lemmein[USER_NAME_LEN+1];
	char topicmaker[USER_NAME_LEN+1];
	int countdown;
	int access;
	int revline;
	int group;
	int mesg_cnt;
	int sndproof;
	int invisible;
	int logging;
	char link_label[MAX_LINKS][ROOM_LABEL_LEN+1];
	int predmet[MPVM+1];
	int dur[MPVM+1];
	struct room_struct *link[MAX_LINKS];
	struct room_struct *next; 
}; 
 
typedef struct room_struct *RM_OBJECT; 

RM_OBJECT room_first,room_last,create_room(); 

#define syserror "Prepac, nastala systemova chyba"
#define nosuchroom "Taka miestnost neexistuje.\n"
#define nosuchroom_en "There is no such room.\n"
#define nosuchuser "Taky uzivatel neexistuje.\n"
#define notloggedon "Nikto s takym menom nie je momentalne prihlaseny.\n"
#define invisenter2 "Neznamy vstupil do miestnosti...\n"
#define invisenter(user) pohl(user,"Neznamy vstupil do miestnosti...\n","Neznama vstupila do miestnosti...\n")
#define invisleave2 "Neznamy opustil miestnost...\n"
#define invisleave(user) pohl(user,"Neznamy opustil miestnost...\n","Neznama opustila miestnost...\n")

#define invisname(user) pohl(user,"\253Im","\253Iz")
#define invisname2(user) pohl(user,"Neznamy","Neznama")

#define nocolors "Nemozes pouzivat farby.\n"
#define noswearing "U nas sa nenadava, ty grobian!\n"
#define NOEXITS "noexits"

#define ZALAR_TLT_CONSUMPTION 30

#define T_SIZE ARR_SIZE*3

void query_com(UR_OBJECT user,char *inpstr);
char ipcka_glob[20]; 
char text[T_SIZE+2]; 
char texthb[T_SIZE+2];

char word[MAX_WORDS+1][WORD_LEN*2];
char irc_word[MAX_IRC_WORDS+1][IRC_WORD_LEN+1];
char irc_lw[IRC_WORD_LEN+1];
char wrd[10][100]; 
char progname[40],confile[40]; 
char motd2_file[100];
char teamname[5][31];
char *motd;
time_t boot_time; 
jmp_buf jmpvar; 
 
int port[2],listen_sock[2],wizport_level,minlogin_level; 
int colour_def,password_echo,ignore_sigterm; 
int max_users,max_clones,num_of_users,num_of_logins,heartbeat,disable_web; 
int login_idle_time,user_idle_time,config_line;
unsigned int word_count; 
int tyear,tmonth,tday,tmday,twday,thour,tmin,tsec; 
int mesg_life,system_logging,prompt_def,no_prompt; 
int force_listen,gatecrash_level,min_private_users; 
int ignore_mp_level,max_sms,last_memory,smscycle;
int destructed,mesg_check_hour,mesg_check_min,net_idle_time; 
int ban_swearing,crash_action,april;
unsigned int lastrand;
int time_out_afks,allow_caps_in_name,rs_countdown,pp_zapnute; 
int charecho_def,time_out_maxlevel,zvery_speed,visit; 
int max_id,backuptime,sendvec;
time_t rs_announce,rs_which; 
UR_OBJECT rs_user;
UR_OBJECT current_user;

#define MAXCARRY 40
#define MAX_PREDMETY 300
#define MAX_CONVERT 100
P_OBJECT predmet[MAX_PREDMETY+1];
CO_OBJECT convert[MAX_CONVERT];

char *long_date(); 
struct macro_struct *defaultmacrolist;
struct room_struct *testroom;

#ifndef AUTHUSER_H 
#define AUTHUSER_H 
extern unsigned short auth_tcpport;
extern int auth_rtimeout;
extern char *auth_xline();
extern int auth_fd();
extern int auth_fd2();
extern int auth_tcpsock();
extern char *auth_tcpuser(register unsigned long in,register unsigned short local,register unsigned short remote);
extern char *auth_tcpuser2(register unsigned long inlocal,register unsigned long inremote,register unsigned short local,register unsigned short remote);
extern char *auth_tcpuser3();
extern char *auth_tcpuser4();
extern char *auth_sockuser(register int s,register unsigned short local,register unsigned short remote);
extern char *auth_sockuser2(register int s,register unsigned short local,register unsigned short remote,int rtimeout);
#endif 

#define MAXSTRING 150  /* maximalna dlzka stringu v scenari */
#define ZERO 0

long play_nxt(void);
long asctomin(char string[], int upd);
char *chrcat(char *dest, char src);

RN_OBJECT create_remote(UR_OBJECT user);
int init_remote_connections(UR_OBJECT user);
int connect_to_site();
int exec_remote_com();
void remote_close();

/* D00M */
#define D_LINES 7    /* Velkost okna s DOOMom */
#define D_COLMS 100  /* Buffer pre dlzku okna */
#define D_LOCAT 21   /* Pocet lokaci v grfile */
#define D_MAPX  5    /* X-ova velkost mapy (sirka) */
#define D_MAPY  5    /* Y-ova velkost mapy (vyska) */
#define D_AMMO  20   /* Gulovnica */
#define D_GRAPH "doom/graph.doom"
#define D_DATA  "doom/data.doom"

int doom_status;
int doom_players;
char doom_grf[D_LOCAT][D_LINES][D_COLMS];
char doom_map[D_MAPY][D_MAPX][5];

extern UR_OBJECT doom_get_user(int x,int y);
extern UR_OBJECT doom_check_view(UR_OBJECT user);
extern void doom(UR_OBJECT user,char *inpstr);
extern void doom_showmap(UR_OBJECT user);
extern void write_doom(UR_OBJECT user,int location);
extern void doom_checkround(int x,int y);
extern void doom_load_users(void);
extern void doom_loser(UR_OBJECT user);
extern int doom_join(UR_OBJECT user);
extern int doom_check(int x,int y,int smer);
extern int doom_init(void);
extern int doom_points_check(UR_OBJECT user);
extern void doom_text(UR_OBJECT user,char *msg,int beep);
extern void doom_quit(UR_OBJECT user);
extern void doom_who(UR_OBJECT user);
extern void doom_wall(UR_OBJECT user,char *msg);

struct {
	int time; /* Uz bude sluzit len pocas playovania */
	int hodina, minuta; /* Hodina a minuta zaciatku dalsieho predstavenia */
	char on; /* flag: 0-caka hru; 1-hra; 2-vypnute */
	char name[80]; /* dalsi citany "scenar", meno suboru */
	char nazov[80];
	FILE *file; /* subor so "scenarom", resp. pointer nan */
} play;

int logcommands;
unsigned short auth_tcpport;
int auth_rtimeout;
 
#define SIZ            500 
#define MAX_CHAR_BUFF  380 
#ifdef FNONBLOCK                     /* SYSV,AIX,SOLARIS,IRIX,HP-UX */ 
# define NBLOCK_CMD FNONBLOCK
#else
# ifdef O_NDELAY                     /* BSD,LINUX,SOLARIS,IRIX */
#  define NBLOCK_CMD O_NDELAY
# else
#  ifdef FNDELAY                     /* BSD,LINUX,SOLARIS,IRIX */
#   define NBLOCK_CMD FNDELAY
#  else
#   ifdef FNBIO                      /* SYSV */
#    define NBLOCK_CMD FNBIO
#   else
#    ifdef FNONBIO                   /* ? */
#     define NBLOCK_CMD FNONBIO
#    else
#     ifdef FNONBLK                  /* IRIX */
#      define NBLOCK_CMD FNONBLK
#     else
#      define NBLOCK_CMD 0
#     endif
#    endif
#   endif
#  endif
# endif
#endif

#define talker_signature  "-------\nAtlantis talker (telnet %s %d, %s)\n"

#define QLEADER 6 /* min. level "questleadra" */
#define QUEST_DIR "quest"

struct {
	int queston; /* Flag ci bezi ci nje */
	int lastquest; /* Iba jeden quest za den */
	char lq_time[10]; /* Kedy bol posl. quest */
	char lq_leader[13]; /* Leader posl. questu */
	char lq_winner[13]; /* Vitaz posl. questu */
	char questfilename[80]; /* Zaznam questu - filename */
} quest;

int ship_timer;
#define SHIPPING_ISLAND  "ostrov"
#define SHIPPING_HOME    "pristav"
#define SHIPPING_SHIP    "plachetnica"
#define PORTALIS_KISS    "luka"
#define BRUTALIS         "brutalis"

char *alt_island_desc;
char *alt_home_desc;
char *alt_plachetnica_desc;

void make_travel();
void link_room(RM_OBJECT, RM_OBJECT);
void unlink_room(RM_OBJECT, RM_OBJECT);
void link_room_oneway(RM_OBJECT room,RM_OBJECT rm);

#define FLYER_TIMEOUT 15
#define FLYER_ROOM "letun"
#define FLYER_HOME "vzdusny_pristav"
#define FLYER_LAND "namestie"

struct {
	int pozicia; /* Pozicia letuna */
	int vykon; /* Vykon eletioveho cerpadla */
	int mind; /* Posobenie silou vole */
	int vyska; /* ...co to asi bude... */
	int palivo; /* Stava! */
	int vzdialenost; /* Kolko uz toho preletel... */
	int timeout; /* Nespiiime, startujeme! */
	int gotta_write; /* ci zapise skore a ci nie */
	char pilot[20]; /* Kto to bude pilotovat */
} flyer;

struct {
	int on; /* ci guest mode prave bezi */
	int getuser; /* ci ma vediet getnut usera guest */
	UR_OBJECT user; /* pointer na usera guestoweho */
	UR_OBJECT moderator; /* pointer na moderatora */
	UR_OBJECT talk; /* pointer na toho kto moze polozit otazku */		
	char name[40]; /* meno - pojde do descu a do sayu */
} guest;

#define GUEST_ROOM  "krcma"

void flyer_fly();
int check_crash_flyer();
void eject(UR_OBJECT user);

#define MAX_COMMANDS 5
#define TRUE 1
#define FALSE 0

#define OUTPUT_FORMAT_PLAIN 1
#define OUTPUT_FORMAT_XML   2

#define INPUT_SEP '\\' /* cim sa bude separovatj */
#define NUM_MAC_PARAM 10 /* max. pocet parametrof macra */

struct macro_struct {
	char *name;
	char *value;
	int star;
	int is_running;
	struct macro_struct *next;
};

typedef struct macro_struct *MACRO;
enum result {SUCCESS,FAIL};  
void free_macrolist(MACRO *list);
void free_macro(MACRO macro);
int save_macros(MACRO *list,char *filename);
int load_macros(MACRO *list,int id);
void macro(MACRO *list,char *input,UR_OBJECT user,int is_action);
void global_macros(UR_OBJECT user);
void delete_macro(UR_OBJECT user, MACRO *list,char *name);
int update_macro(UR_OBJECT user, MACRO macro,char *value);
void add_macro(MACRO *list,char *name,char *value);
MACRO allocate_macro(char *macroname,char *macrovalue);
MACRO ismacro(MACRO *list,MACRO *deflist,char *name);
MACRO findmacro(MACRO *list,MACRO *deflist,char *name);
MACRO findprevmacro(MACRO *list,char *name);
void parse(UR_OBJECT user,char *string, int irc);
int macroexpand(MACRO *list,MACRO *deflist,char *expansion,UR_OBJECT user, int irc);
void fix_separators(char *input,char *output);
char todigit(char c);
void check_death();
void got_line(UR_OBJECT user,char *inpstr);

void free_notifylist(NOTIFY *list);
int load_notifylist(NOTIFY *list,int id,int noti);
int delete_notify(NOTIFY *list, char *meno);
void add_notify(NOTIFY *list, char *name);

#define DEFAULT_BJ_BET   10
#define USE_MONEY_SYSTEM  0

struct blackjack_game_struct {
	short int deck[60],hand[10],dealer_hand[10],bet,cardpos;
};

typedef struct blackjack_game_struct *BJ_GAME;

RM_OBJECT get_room(char *name,UR_OBJECT user);
RM_OBJECT get_linked_room(char *name,RM_OBJECT room);
char *meniny(int mday, int mmonth);
char *datum_menin(char *meno);
char *get_temp_file();
char *pohl(UR_OBJECT user, char *muz, char *zena);
char *zwjpohl(int vec, char *muz, char *zena, char *ono, char *ich);
int expand_predmet(char *vec1);
void zazuvackuj(char *inpstr, int ok);
void zachlastaj(char *inpstr, int ok);
void zachlastaj2(char *inpstr, int ok);
void zalamerizuj(char *inpstr, int ok);
void debilneho(char *inpstr, int ok);
void pohaluz_predmetom(char *inpstr,unsigned int ok,int vec);
void do_funct_stuff(UR_OBJECT user,char *inpstr,int start);
char *real_user(UR_OBJECT gdo);
char *sklonuj(UR_OBJECT juzer, int pad);
char *skloncislo(int pocet,char *squirrel,char *squirrle,char *squirrelov);

char *lamerize(char buf[]);
char *lame_color(char buf[],int typ);
char *revert(char vstup[]);
char *colour_com_strip2(char *str,int usercol);
char nxtznak(char *string);

int check_passwd_simplex(char *passwd);
char *expand_password(char *pass);

void zrus_pager_haldu(UR_OBJECT user);
void aklient_log(char *meno);
 
int skontroluj();
int quotacheck(char *username);
int showfile(UR_OBJECT user, char filename[90]);
int showrow(UR_OBJECT user);

void write_user(UR_OBJECT user,char *str);
void write_level(int level,int above,char *str,UR_OBJECT user);
void writesys(int level, int above,char *str,UR_OBJECT user);
void write_room(RM_OBJECT rm, char *str);
void write_room_except(RM_OBJECT rm, char *str, UR_OBJECT user);
void write_room_except2users(RM_OBJECT rm, char *str, UR_OBJECT user, UR_OBJECT user2);
void write_syslog(char *str,int write_time);

void record(RM_OBJECT rm,char *str);
void record_tell(UR_OBJECT user,char *str);
void record_shout(char *str);
void record_portalisshout(char *lbl,char *str);
void record_gossip(char *str);
void record_quest(char *str);
void record_wizshout(char *str);
void record_lastlog(UR_OBJECT user, int spent, char *dovod);

int more(UR_OBJECT user,int sock,char *filename);
int sqlmore(UR_OBJECT user,int sock,char* queryname);
int mailmore(UR_OBJECT user);
char *sqldatum(char *str,int telnet_cols,int type);
char *boarddate(char *dejt);
int is_webuser(UR_OBJECT user);
void highlight_write2sock(UR_OBJECT user,char *buff,size_t num);
int is_number(char *str);
int contains_swearing2(char *str);
int contains_swearing(char *str,UR_OBJECT user);
int contains_advert(char *str);
int colour_com_count(char *str);
char *colour_code_show(char *str);
char *remove_first(char *inpstr);
void init_globals();
void set_date_time();
void init_signals();
void load_and_parse_config();
void check_messages_night(int vculeky);
int countjoke();
void log_commands(char user[],char str[], int timeword);
extern int lab_load();
void init_sockets();
void boot_exit(int code);
void amfiteater(UR_OBJECT user, int force);
void reset_alarm();
void setup_readmask(fd_set *mask);
void accept_connection(int lsock,int num);
void disconnect_user(UR_OBJECT user, int message, char *dovod);
int get_charclient_line(UR_OBJECT user, char *inpstr, int len);
void terminate(char *str);
void clear_words();
void login(UR_OBJECT user,char *inpstr);
unsigned int wordfind(char *inpstr);
void prompt(UR_OBJECT user);
void echo_on(UR_OBJECT user);
void show_recent_tells(UR_OBJECT user);
void show_recent_notifies(UR_OBJECT user);
int misc_ops(UR_OBJECT user, char *inpstr);
int site_banned(char *site);
void deltempfile(char *filename);

char *check_shortcut(UR_OBJECT user);
UR_OBJECT get_user(char *name);
UR_OBJECT get_user_exact(char *name);
UR_OBJECT get_user_by_sock(int sock);
UR_OBJECT get_user_in_room(char *name,UR_OBJECT user);
void decrease_pp(UR_OBJECT user, int value, int dynamic);
int check_ignore_user(UR_OBJECT user,UR_OBJECT target); 

void strtolower(char *str);
void parse_init_section();
void parse_rooms_section();
void load_rooms_desc(UR_OBJECT user);

int onoff_check(char *wd);
int get_level(char *name);
int yn_check(char *wd);
void talker_shutdown(UR_OBJECT user,char *str,int reboot);
void level_log(char *str);
int load_user_password(UR_OBJECT user);
void who(UR_OBJECT user,int people);
char *parse_who_line(UR_OBJECT u,char *line,int wizzes,int users,int userlevel);
void newwho(UR_OBJECT user,char *username);
void testwho(UR_OBJECT user);
void testwhoall(UR_OBJECT user);
void customexamine(UR_OBJECT user,char *username,int testall);
char *parse_ex_line(UR_OBJECT u,char *line,int profile,int userlevel,int oflajn);
int search_success(UR_OBJECT user,UR_OBJECT u,char *lookat,int words);
void show_timeouts(UR_OBJECT user);
void show_version(UR_OBJECT user);
void system_details(UR_OBJECT user,int typ);
void attempts(UR_OBJECT user);
void connect_user(UR_OBJECT user);
int load_user_details(UR_OBJECT user);
int load_user_details_old(UR_OBJECT user);
int get_age(int agecode);
void echo_off(UR_OBJECT user);
int save_user_details(UR_OBJECT user,int save_current);
int save_user_details_old(UR_OBJECT user,int save_current);
void send_mail(UR_OBJECT user,char *to,char *ptr);
void endstring(char *inpstr);
void colour_com_strip(char *str);
void force_language(char *str,int lang,int stripc);
void destruct_user(UR_OBJECT user);
void look(UR_OBJECT user);
int sk(UR_OBJECT user);
void do_db_backup(int full);
void misc_stuff(int level);
char *langselect(UR_OBJECT user,char *slovak,char *english);
int check_notify(UR_OBJECT user);
void who_from_notify_is_online(UR_OBJECT user);
void hint(UR_OBJECT user, int ihned);
UR_OBJECT check_double_identity(UR_OBJECT user);
UR_OBJECT check_double_real_identity(UR_OBJECT user);
int has_unread_mail(UR_OBJECT user);
int check_notify_user(UR_OBJECT user,UR_OBJECT target);
void toggle_prompt(UR_OBJECT user,char *inpstr);
void revtell(UR_OBJECT user,char *inpstr);
extern void writecent(UR_OBJECT user, char *txt);
void destroy_user_clones(UR_OBJECT user);
void reset_access(RM_OBJECT rm);
void editor(UR_OBJECT user,char *inpstr);
void adv_edit(UR_OBJECT user);
char *adv_e_str(UR_OBJECT user);
void wipe_user(char meno[]);
void delete_user(UR_OBJECT user,int this_user);
void posielanie(UR_OBJECT user);
int zmenheslo(UR_OBJECT user,int wizpass);
void police_freeze(UR_OBJECT user,int priority);
void send_mailinglist_request(UR_OBJECT user);
int double_fork(); 
void write_board(UR_OBJECT user,char *inpstr,int done_editing);
void smail(UR_OBJECT user,char *inpstr,int done_editing);
void enter_profile(UR_OBJECT user,int done_editing);
void write_joke(UR_OBJECT user,int done_editing);
void vote(UR_OBJECT user, char *inpstr, int done_editing,int zedit);
void votenew(UR_OBJECT user);
int note_vote(UR_OBJECT user);
void editor_done(UR_OBJECT user);
void clear_revbuff(RM_OBJECT rm);
void send_email(UR_OBJECT user,char *to, char *message);
void forward_email(char *name,char *from, char *from2, char *message);
void cls(UR_OBJECT user, int howmuch);
void strtoupper(char *str);
void exec_com(UR_OBJECT user,char *inpstr);
int has_room_access(UR_OBJECT user,RM_OBJECT rm);
void tell(UR_OBJECT user,char *inpstr,int reply);
void to_user(UR_OBJECT user,char *inpstr);
void pemote(UR_OBJECT user,char *inpstr);
void move(UR_OBJECT user);
void kill_user(UR_OBJECT user, char *inpstr);
void wake(UR_OBJECT user,char *inpstr);
void send_hug(UR_OBJECT user,char *inpstr);
void send_bomb(UR_OBJECT user);
void send_kiss(UR_OBJECT user,char *inpstr);
void fight_user(UR_OBJECT user);
void fight_brutalis(UR_OBJECT user);
void notify_user(UR_OBJECT user);
void pictell(UR_OBJECT user,char *inpstr);
void quit_user(UR_OBJECT user,char *inpstr);
void toggle_mode(UR_OBJECT user);
void say(UR_OBJECT user,char *inpstr,int comm);
void shout(UR_OBJECT user,char *inpstr);
void emote(UR_OBJECT user,char *inpstr);
void semote(UR_OBJECT user, char *inpstr);
void echo(UR_OBJECT user, char *inpstr);
void go(UR_OBJECT user);
void set_desc(UR_OBJECT user, char *inpstr);
void set_iophrase(UR_OBJECT user, char *inpstr);
char *expand_outphr(UR_OBJECT user,RM_OBJECT destination);
void set_room_access(UR_OBJECT user);
void letmein(UR_OBJECT user);
void invite(UR_OBJECT user);
void set_topic(UR_OBJECT user, char *inpstr);
void bcast(UR_OBJECT user, char *inpstr,int done_editing);
void who_alt1(UR_OBJECT user);
void who_alt2(UR_OBJECT user);
void who_alt3(UR_OBJECT user);
void who_alt4(UR_OBJECT user);
void who_alt5(UR_OBJECT user);
void who_alt6(UR_OBJECT user);
void who_alt7(UR_OBJECT user);
void who_alt8(UR_OBJECT user);
void who_alt9(UR_OBJECT user);
void help(UR_OBJECT user,int flag);
void shutdown_com(UR_OBJECT user);
void read_board(UR_OBJECT user);
void wipe_board(UR_OBJECT user);
void search_boards(UR_OBJECT user);
void review(UR_OBJECT user, char *inpstr);
void status(UR_OBJECT user);
void mymail(UR_OBJECT user,int self);
void rmail(UR_OBJECT user);
void dmail(UR_OBJECT user);
int mail_from(UR_OBJECT user, int echo);
void examine(UR_OBJECT user);
void analyze(UR_OBJECT user);
void rooms(UR_OBJECT user);
void change_pass(UR_OBJECT user);
void autopromote(UR_OBJECT user,int promote);
void promote(UR_OBJECT user,char *inpstr);
void demote(UR_OBJECT user,char *inpstr);
void listbans(UR_OBJECT user);
void ban(UR_OBJECT user, char *inpstr);
void unban(UR_OBJECT user);
void visibility(UR_OBJECT user,int vis);
void hide(UR_OBJECT user);
void site(UR_OBJECT user);
void wizshout(UR_OBJECT user,char *inpstr);
void muzzle(UR_OBJECT user);
void unmuzzle(UR_OBJECT user);
void show_map(UR_OBJECT user);
void minlogin(UR_OBJECT user);
void toggle_charecho(UR_OBJECT user,char *inpstr);
void clearline(UR_OBJECT user);
void change_room_fix(UR_OBJECT user,int fix);
void viewlog(UR_OBJECT user, char *inpstr);
void account_request(UR_OBJECT user);
void revclr(UR_OBJECT user);
void create_clone(UR_OBJECT user);
void destroy_clone(UR_OBJECT user);
void myclones(UR_OBJECT user);
void allclones(UR_OBJECT user);
void clone_switch(UR_OBJECT user);
void clone_say(UR_OBJECT user,char *inpstr);
void clone_hear(UR_OBJECT user);
void afk(UR_OBJECT user,char *inpstr);
void exec_command(UR_OBJECT user,char *inpstr);
void toggle_colour(UR_OBJECT user);
void suicide(UR_OBJECT user);
void reboot_com(UR_OBJECT user);
void check_messages(UR_OBJECT user);
void send_sos(UR_OBJECT user,char *inpstr);
void write_noticeboard(UR_OBJECT user, int what, int done_editing);
void read_notices(UR_OBJECT user);
void send_to_jail(UR_OBJECT user, char *inpstr);
void cooltalk(UR_OBJECT user,char *inpstr);
void cooltopic(UR_OBJECT user,char *inpstr);
void cdesc(UR_OBJECT user,char *inpstr);
void fortune_cookies(UR_OBJECT user);
void think(UR_OBJECT user,char *inpstr);
void cow(UR_OBJECT user);
void sing(UR_OBJECT user,char *inpstr);
void kidnap(UR_OBJECT user);
void follow(UR_OBJECT user, int unfol);
void ranks(UR_OBJECT user);
void secho(UR_OBJECT user,char *inpstr);
void read_joke(UR_OBJECT user);
void delete_joke(UR_OBJECT user);
void sign(UR_OBJECT user,char *inpstr);
void tellall(UR_OBJECT user,char *inpstr);
void commands(UR_OBJECT user,int engl);
void logtime(UR_OBJECT user);
void info_users(UR_OBJECT user);
void info(UR_OBJECT user, int typ);
void pecho(UR_OBJECT user, char *inpstr);
void whois(UR_OBJECT user);
void shset(UR_OBJECT user);
void set(UR_OBJECT user,char *inpstr);
void fmail(UR_OBJECT user,int what);
void poetizuj(UR_OBJECT user);
void revshout(UR_OBJECT user,char *inpstr);
void revbcast(UR_OBJECT user,char *inpstr);
void revsos(UR_OBJECT user,char *inpstr);
void revwizshout(UR_OBJECT user, char *inpstr);
void last(UR_OBJECT user);
void who_from(UR_OBJECT user);
void insult_user(UR_OBJECT user);
void chname(UR_OBJECT user);
void power_points(UR_OBJECT user);
void winners(UR_OBJECT user,int type_def,int pocet);
void add_point(UR_OBJECT user,int type,int amount,int fuel);
void weather(UR_OBJECT user, char *inpstr);
void skript(UR_OBJECT user);
void kick(UR_OBJECT user);
void ignore(UR_OBJECT user, char *inpstr);
void join(UR_OBJECT user);
void nuke_user(UR_OBJECT user);
extern void piskvorky(UR_OBJECT user);
void alarm_clock(UR_OBJECT user);
void copies_to(UR_OBJECT user);
void save_users(UR_OBJECT user);
extern void play_hangman(UR_OBJECT user, char *inpstr);
void hangman_status(UR_OBJECT);
void wizzes(UR_OBJECT user);
extern void play_geo(UR_OBJECT user);

void get_predmet(UR_OBJECT user);
void put_predmet(UR_OBJECT user);
void loose_predmets(UR_OBJECT user);
void carry_refresh(UR_OBJECT user);
char *farba_p(int vec);
void create_predmet(UR_OBJECT user);
void give_predmet(UR_OBJECT user);
void dispose_predmet(UR_OBJECT user);
int forbidden(UR_OBJECT user,UR_OBJECT u,int vec);
void hurt(UR_OBJECT user,UR_OBJECT u,int vec);
void use_predmet(UR_OBJECT user,char *inpstr);
int convert_predmet(UR_OBJECT user,int spell);
CO_OBJECT create_convert();
void destruct_convert(CO_OBJECT con);
void throw_predmet(UR_OBJECT user);
void zobraz_predmety(UR_OBJECT user,char *inpstr);
P_OBJECT create_vec();
void destruct_vec(P_OBJECT vec);
void load_and_parse_predmets(UR_OBJECT user);
char *fetchstring(char *str);
char *parse_phrase(char *str,UR_OBJECT user,UR_OBJECT u,RM_OBJECT rm,int whichtype);
void do_alt_funct(UR_OBJECT user,int vec);
void show_pict_on_event(UR_OBJECT user,int event,int vec,int newline);
void predmet_write(UR_OBJECT user,int done_editing);
int is_affected(UR_OBJECT user,int flag);

void call_user(UR_OBJECT user);
void p_u_prikaz(UR_OBJECT user, int pridaj,char *inpstr);
extern void lod(UR_OBJECT user);
extern void lab(UR_OBJECT user);
void check_ident(UR_OBJECT user);
void update_web(UR_OBJECT user);
void gossip(UR_OBJECT user,char *inpstr);
void gemote(UR_OBJECT user,char *inpstr);
void revgossip(UR_OBJECT user, char *inpstr);
void quest_command(UR_OBJECT user,char *inpstr);
void sclerotic(UR_OBJECT user);

extern void shprn2(UR_OBJECT user,UR_OBJECT u);
extern void vynuluj_lod(UR_OBJECT user);
extern void shprn(UR_OBJECT user);
extern int shtest(UR_OBJECT user, int x, int y);
extern int shreset(UR_OBJECT user);
extern void shset(UR_OBJECT user);
extern int shsunken(UR_OBJECT user, int x,int y);
extern int shsink(UR_OBJECT user, int x,int y, int count);

void turn_resolver(UR_OBJECT user);
void rules_faq_web();
void save_topic();
void send_forward_email(char *send_to, char *mail_file);
void show_user(UR_OBJECT user,char fajl[]);
int shcount(UR_OBJECT user, int x, int y);

extern void fly_write(UR_OBJECT user,int body,int fuel);
extern void stavhry(UR_OBJECT user1,UR_OBJECT user2);
extern void stavhryg(UR_OBJECT user1,UR_OBJECT user2);

void amfiteater_web();
void send_copies(UR_OBJECT user, char *ptr);
extern void nastav_hru(UR_OBJECT user);

extern void je5b(UR_OBJECT user,int n);
extern void je4b(UR_OBJECT user,int n);
extern void je5(UR_OBJECT user,int x, int y, char znak);
extern void je4(UR_OBJECT user,int x, int y, char znak);

void toggle_ignall(UR_OBJECT user);
void toggle_ignshout(UR_OBJECT user);
void toggle_igngossip(UR_OBJECT user);
void toggle_igntell(UR_OBJECT user);
void toggle_ignfun(UR_OBJECT user);
void toggle_ignportal(UR_OBJECT user);
void toggle_ignsys(UR_OBJECT user);
void ignore_user(UR_OBJECT user);
void set_homepage(UR_OBJECT user,char *inpstr);
void jokeboard_web();
void zober_predpoved(UR_OBJECT user, int force);
void webcicni(char host[], char dokument[], char filename[]);
void parsni_pocasie(char vstup[], char vystfile[]);
int connecthost(char serveridlo[], int port);
int arena_check(UR_OBJECT user);
void revquest(UR_OBJECT user,char *inpstr);
int detect_user(char *inpstr,UR_OBJECT except);
void move_user(UR_OBJECT user,RM_OBJECT rm,int teleport);
int move_vehicle(UR_OBJECT user,RM_OBJECT newroom,int quiet);
int get_vehicle(UR_OBJECT user);
void rename_user_on_list(char meno[], char novemeno[]);
void help_commands(UR_OBJECT user,int engl);
void help_credits(UR_OBJECT user);
void help_fonts(UR_OBJECT user);

void ban_site(UR_OBJECT user);
void ban_user(UR_OBJECT user, char *inpstr);
void unban(UR_OBJECT user);
void unban_site(UR_OBJECT user);
void unban_user(UR_OBJECT user);
void otravuj_usera_s_mailinglistom(UR_OBJECT user);
void check_reboot_shutdown();
void check_idle_and_timeout();
void udalosti();
void check_cimazacat();
void check_predstavenie();
void check_security();
void dopln_predmety();
void do_predmety(int user_trigged);
int dec_dur(int olddur,int amount);
int abs_dur(int dur);
void do_userhands();
void hesh(RM_OBJECT rm);
void do_tunel(RM_OBJECT room);
void do_brutalis(UR_OBJECT user);
void do_attack(UR_OBJECT user);
RM_OBJECT random_room(RM_OBJECT rm,int ajprivat);
void zapis_statistiku();
void check_email_arrival();
void check_web_board();
void play_it();
void play_on();
void play_end();
void vypis_predstavenia(char riadok[]);
void who_for_web();
void toggle_ignword(UR_OBJECT user, char *inpstr);
void quest_logger(char *retazec);
int count_lines(char *filename);
extern char *get_hang_word(char *aword);
char *zobraz_datum(time_t *raw, int typ);
int jdb_zarad(int co, int uid, int wizzid, char *dovod);
int jdb_vyrad(int co, int uid);
char *jdb_info(int co, char *username);
char *jdb_wizz(int co, char *username);
extern char *sstrncpy(char *dst, char *src, long len);
int check_iophrase(char *inpstr);
int check_redir(UR_OBJECT user);
void idletime(UR_OBJECT user);
void guest_command(UR_OBJECT user, char *inpstr);
void icqpage(UR_OBJECT user, char *inpstr);
void send_icq_page(UR_OBJECT user, int icq_num, char *fname);
void finger(UR_OBJECT user, char *inpstr);
int user_banned(char *name);
void resc_load();
void resc_save();
void record_history(UR_OBJECT user, char *str);
void view_history(UR_OBJECT user);
char *real_auth(UR_OBJECT gdo);
void every_5min();
void do_burka();
void every_min();
void daily();
void ban_site_for_newuser(UR_OBJECT user);
void unban_site_for_newuser(UR_OBJECT user);
int newuser_siteban(char *site);

RN_OBJECT create_remote();
int init_remote_connections(UR_OBJECT user);
int connect_to_site(UR_OBJECT user, RN_OBJECT remote,int slot);
void remote_close(UR_OBJECT user,int i);
int exec_remote_com(UR_OBJECT user, char *inpstr);
void links(UR_OBJECT user);
void timeout_rem();
void remote_connect(UR_OBJECT user);
void remote_disconnect(UR_OBJECT user);
void view_remote(UR_OBJECT user);
void edit_remote_servers(UR_OBJECT user);
int UPDATE_FDS();
int ADD_FDS();
void zazabuj(char *inpstr, int ok);
void zaslepac(char *inpstr, int ok);
void remote_antiidle();
void do_events();
void check_web_commands();
void brutalis_wars();
char *get_web_input(UR_OBJECT user);
void do_webusers();
void kick_webusers();
void handle_webinput();
void write_web(int socket,char *str);
void eliminate_webuser(int socket);
int get_memory_usage(void);
int get_resmem_usage(void);
void do_nothing();
void connecthost_timeout();
void cmon_auth(UR_OBJECT gdo);
void check_ident_reply();
void clear_irc_words();
int irc_wordfind(char *inpstr);
void write_irc(UR_OBJECT user, char *text);
void irc_prikaz(UR_OBJECT user, char *inpstr);
void lynx(UR_OBJECT user);
void test_lynx_done();
void revirc_command(UR_OBJECT user, char *inpstr);
void record_irc(UR_OBJECT user, char *inpstr);
void load_irc_details(UR_OBJECT user);
void save_irc_details(UR_OBJECT user);
long filesize (char *path);
void irc(UR_OBJECT user);
void statline(UR_OBJECT user);
void init_statline(UR_OBJECT user);
void show_statline(UR_OBJECT user);
void text_statline(UR_OBJECT user, int crflag);
FILE *ropen (const char *path, const char *mode);
void obnov_statline_userof();
void setpp(UR_OBJECT user, int amount);
int charsavail(int fd);
BJ_GAME create_blackjack_game(void);
void show_blackjack_cards(UR_OBJECT,int,int);
int check_blackjack_total(UR_OBJECT,int);
void vwrite_user(UR_OBJECT user, char *str, ...);
char *get_ircserv_name(char *id);
void boot_statline(UR_OBJECT user);
void poprehadzuj_prikazy(UR_OBJECT user);
void toggle_io(UR_OBJECT user);
void toggle_zvery(UR_OBJECT user);
void toggle_coltell(UR_OBJECT user);
void toggle_ignlook(UR_OBJECT user);
void logout_user(UR_OBJECT, char*);
void timeout_auth(); 
void kde(UR_OBJECT user);
ssize_t twrite(int fd, const void *buf, size_t count);
void write2sock_ex(UR_OBJECT user,int sock,char *str,size_t count);
size_t write2sock(UR_OBJECT user,int sock,const char *str,size_t count);
void twrite_timeout();
char *noyes(int numero);

void reversi_koniechry(UR_OBJECT user);
void reversi(UR_OBJECT user);
void reversi_pishelp(UR_OBJECT user);
void reversi_pisplan(UR_OBJECT user, UR_OBJECT sh_user);
void reversi_pisplan2(UR_OBJECT user, UR_OBJECT sh_user);
void reversi_pocitajznaky(UR_OBJECT user, int* pX, int* pO);
int reversi_testok(UR_OBJECT user,int y, int x);
int reversi_mozetahat(UR_OBJECT user);

void reversi_tah(UR_OBJECT user, int y, int x);

void miny_draw(UR_OBJECT user);
void miny_numbering(UR_OBJECT, unsigned int , unsigned int);
int miny_init(UR_OBJECT);
void miny_zero_show(UR_OBJECT, unsigned int, unsigned int);
void miny_done(UR_OBJECT);
void miny(UR_OBJECT);

DAMA_OBJECT create_dama();
void destruct_dama(UR_OBJECT);
void dama_save(UR_OBJECT);
void dama_load(UR_OBJECT);
char *dama_saved_opponent(UR_OBJECT);
int check_dama_jump(DAMA_OBJECT,int,int,int,int);
void dama(UR_OBJECT);
void dama_stav(UR_OBJECT);
void dama_stav2(UR_OBJECT);
char *header(char *str);
void oline(UR_OBJECT);
void repository(char *name, int typ);
void rebirth(UR_OBJECT user);
void miny_placing(UR_OBJECT,unsigned int,unsigned int);
void wash_bell(char*);
void send_noticeboard_digest(char*);
void send_recent_tells(UR_OBJECT user, int sp);
void toggle_ignbeep(UR_OBJECT);
void games(UR_OBJECT user);
UR_OBJECT hra(UR_OBJECT user, int n);
void sms(UR_OBJECT user,int done_editing);
void send_sms(char *numero,char *str,int gate);
void forward_smail2sms(char *name,char *from,char *message);
void crash_smsnotice();
char *expand_gate(UR_OBJECT user,int inc_account,char *sendtonum);
void show_gates(UR_OBJECT user);
int get_gate_no(char *str);
int get_gate_maxchars(int gate_num);
int restrict_sms(char *str,int gate_num);
void statistic(UR_OBJECT user);
char *title(char *str,char *col);
int pohaluz(UR_OBJECT user);
void gold(UR_OBJECT user,int left,char *str);
void alter_maxtimeouts(int do_what);
void calendar(UR_OBJECT user);

void free_stuff();

extern CL_OBJECT create_clovece();
extern void destruct_clovece(UR_OBJECT);
extern void record_clovece(CL_OBJECT clovece);
extern void clovece_save(UR_OBJECT);
extern void clovece_load(UR_OBJECT);
extern char *clovece_saved_opponent(UR_OBJECT);
extern void add_cloveceplayer(UR_OBJECT,UR_OBJECT);
extern void hod_kockou(UR_OBJECT, int);
extern void clovece_next(UR_OBJECT);

extern FR_OBJECT create_farar();
extern void destruct_farar(UR_OBJECT);
extern void add_fararplayer(UR_OBJECT,UR_OBJECT);
extern void farar_disp(UR_OBJECT,int);
extern void record_farar(FR_OBJECT farar);
extern void farar(UR_OBJECT,char*);

extern void log_game(char*);

int put_in_room(RM_OBJECT rm,int vec,int dur);
int remove_from_room(RM_OBJECT rm,int vec,int dur);
int is_in_room(RM_OBJECT rm,int vec);
int is_funct_in_room(RM_OBJECT rm,int fun);
int is_in_hands(UR_OBJECT u,int vec);
int is_funct_in_hand(UR_OBJECT u,int fun);
int is_free_in_room(RM_OBJECT rm);
int is_free_in_hands(UR_OBJECT u);
int put_in_hands(UR_OBJECT u,int vec,int dur);
int remove_from_hands(UR_OBJECT u,int vec,int dur);
void buy(UR_OBJECT user);
void sell(UR_OBJECT user);
int spravny_predmet(char *vec1);
int default_dur(int vec);

extern void banner(UR_OBJECT user, char *inpstr);
extern void tbanner(UR_OBJECT user,char *inpstr);
extern void sbanner(UR_OBJECT user,char *inpstr);
extern void magazin(UR_OBJECT user,char *inpstr);
int getfontid(int type);

extern void note(UR_OBJECT user,char *inpstr);
extern void dnote(UR_OBJECT user);
extern int spracuj_remote_vstup(char *inpstr);
extern void parse_remote_ident(UR_OBJECT user,char *rinpstr,int len,int wsock);
extern void parse_telnet_chars(UR_OBJECT user,char *inpstr, size_t len);
extern void spracuj_irc_vstup(UR_OBJECT user, char *inpstr);
extern void prihlas_irc(UR_OBJECT user);
extern void spell(UR_OBJECT user, char *inpstr);
extern UR_OBJECT leave_me_alone(UR_OBJECT user,UR_OBJECT u);
extern void show_kniha_kuziel(UR_OBJECT user,int dur);

extern char *crypt_slavko(char *crypt, char *salt);
extern char *md5_crypt(char *pw, char *salt);
extern void destruct_blackjack_game(UR_OBJECT);
extern void play_blackjack(UR_OBJECT);
extern void clovece(UR_OBJECT, char*);

extern XA_OBJECT create_xannel();
extern void destruct_xannel(UR_OBJECT user);
extern void xsay(UR_OBJECT user,char *inpstr);
extern void record_xannel(XA_OBJECT xannel,char *str);
extern void add2xannel(UR_OBJECT user,UR_OBJECT u);
extern void xannel(UR_OBJECT user,char *inpstr);
extern void cmd_module(UR_OBJECT user, char *inpstr);

char *dbf_string(char *string);
