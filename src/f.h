//! femtoline copyright (c) 2020 regents of kparc, bsd-2-clause

//! core comms (tx)byte (txn)bytes (txN)times
ZI tx(G c){R write(1,&c,1);}ZI txn(S x,I n){P(!n,n)N(n,tx(x[i]));R n;}ZI txN(G c,I n){N(n,tx(c))R n;}

#if FPF
//! printf
#include<unistd.h> //<! size_t write(2)
#define VMX 16
typedef union{UJ uj;}arg;typedef arg args[VMX]; //!< avoid gcc error
#define PU(f,t) ZZ S f(S s,t i){t j;do*--s='0'+i-10*(j=i/10);W(i=j);R s;} //!< parse signed/unsigned
#define TU(f,t,u) ZZ I f(t x,I p,I l){I n;S r=jS(x,&n,u);R txp(r,n,p);}   //!< tx signed/unsigned

//! strlen memset parseint itoa atoi hexstr
ZG xb[26];ZS ng(S s){R*--s='-',s;}PU(pj,J)PU(pu,UJ);
ZS jS(UJ i,I*n,G u){S r=(!u&&0>(J)i)?ng(jS(-i,n,u)):u?pu(xb+25,i):pj(xb+25,i);R*n=25+(xb-r),r;}
UI sI(S a,I*n){G c;UI i=0,r=*n=0;W((c=*a++)&&IN('0',c,'9'))i++,r=r*10u+((UI)c-'0');R*n=i,r;}ZS hh(S s,G c);
ZS hh(S s,G c){N(2,G a=i?c&15:c>>4;s[i]="0W"[9<a]+a)R s;}ZI jX(UJ j){S s=xb+25;UJ k=j;do hh(s-=2,k);W(k>>=8);R 25-(s-xb);}

//! (tx)byte (txn)bytes (txN)times (b)yte (i)nt he(x) (s)tr (p)ad
ZI txp(S x,I n,I p){R txN(' ',MX(0,p-n))+txn(x,n)+txN(' ',ABS(MN(0,p+n)));}ZI txb(G c,I p,I l){R txp(&c,1,p);}
ZI txx(UJ j,I p,I l){I n=jX(j)+2;S b=xb+25-n;*b='0',b[1]='x';R txp(b,n,p);}TU(txj,J,0)TU(txu,UJ,1)
ZI txs(S x,I p,I l){R txp((S)x,l?l:sln(x),p);}

#define vi (a[i++].uj)
#define pf(f,a...) txpf(f,(args){a}) //!< arguments of pf() as an array of void ptrs, up to VMX
#define va(c,t,f) C(c,n+=f((t)vi,flg*flw,prc);) //!< call f((type)nextarg,options)
#define nx continue

//! %[%-][09..][.09..*]dcups
I txpf(S f,args a){                          //!< (f)ormat string (aka tape), (a)rguments
 G c;I j,i=0,n=0;                            //!< total le(n)gth, arg(i)ndex, curr(c)har
 UI flg,flw,prc;                             //!< fmt flags, field width, precision
 W(c=*f++){                                  //!< while more chars left on tape,
  flg=prc=0,flg=j=1;                         //!< reset state, then:
  Z('%'-c,tx(c);nx)                          //!< echo c unless %, otherwise:
  W(j)SW(c=*f,C('-',flg=-1,f++),j=0)         //!< scan flags (%flg)
  flw=sI(f,&j),f+=j,c=*f;                    //!< scan field width (%flw)
  Z('.'==c,prc=sI(++f,&j);f+=j;c=*f;         //!< scan precision (%.prc)
   Z(!j,Z('*'-c,f++;nx)                      //!< %.[not 09*] is empty field
    c=*++f;prc=(I)vi))c=*f;                  //!< scan positional precision (%.*)
  W('l'==c||'h'==c)c=*++f;                   //!< skip [lh..] nop
  SW(c,                                      //!< conversion specifier dispatch
   va('c',G,txb)va('d',J,txj)va('u',UJ,txu)
   va('p',UJ,txx)va('s',S,txs)C('%',tx(c)))
  f++;}R n;}

#define pfl()             (0)
#else
#include<stdio.h>
#define pfl()             fflush(0)
#define pf                printf
#endif

//! unicode prims
#if FUN
#define uff               (0x3f&*s++)
#define U4(c)             0xf0==(0xf8&c)
#define U3(c)             0xe0==(0xf0&c)
#define U2(c)             0xc0==(0xe0&c)
#define ubc(c)            UB[UN++]=c,UB[UN]=0 //!< accumulator
static UI cpt(S s)       {R U4(*s)?(0x07&*s++)<<18|uff<<12|uff<<6|uff:U3(*s)?(0x0f&*s++)<<12|uff<<6|uff:U2(*s)?(0x1f&*s++)<<6|uff:0;}
static UI rlu(G c)       {UI u=0;ubc(c);P(2>UN,0)if(u=cpt(UB))txn(UB,UN);if(u||4==UN)UN=0;R u;} //!< attempt to parse utfbuf
#else
#define LC()
static UI rlu(G c){R 0;}
#endif//FU8

//! raw terminal
#if FTY
#include<sys/ioctl.h>     //!< ioctl(2)
#include<termios.h>       //!< termios
//#include <locale.h>
//#define LC()            setlocale(LC_ALL,"C.UTF-8")
//#define tc1(t)          cfmakeraw(&t);//!
static struct termios tco,tcn; //!< old|new
#define tcg(t)            ioctl(0,TCGETS,&t)
#define tcs(t)            ioctl(0,TCSETS,&t)
#define tc1(t)            t.c_lflag&=~(ICANON|ECHO|ISIG),t.c_cc[VMIN]=1,t.c_cc[VTIME]=0;tcs(t) //!< blocking read, no echo, pass escapes and signals
#define rltc()            tcg(tco),tcn=tco,tc1(tcn); //!< LC();todo !isatty(0)
Z_ Tc(){rltc();}
#else
Z_ Tc(){}
#endif

//! bracket balancer
#if FBB
#define Jp (x-rG)
#define Qb(c,a...)          P(c,tx('\7'),a) //!< send audio bell on bracket misbalance
//! class                   !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
ZG cl(G c){P(!c,0)R 128>c?"  q     ()                 ;                               ( )                             ( ) "[c-32]:0;}
ZS bq(S x){W((x=sch(++x,'"'))&&!({I i=0;W('\\'==x[--i]){};1&i;})){};R x;} //!< ffwd quoted string w/escapes
ZI bb(K r,I c){G b[16];I p[16],a,n=0,t=-1;S s,x=rG-1;W(a=*++x)SW(cl(a),C('q',s=bq(x);Qb(!s,x,Jp)c=Jp==c?s-rG:s-rG==c?Jp:c,x=s)
 C('(',Qb(16==++n,Jp)p[n]=Jp,b[n]=*x;Z(c==Jp,t=Jp))C(')',Qb(!n,Jp)Qb(b[n]!=*x-1-*x/64,p[n])c=Jp==c?p[n]:p[n]==t?Jp:c,--n))Qb(n,p[n])R c;}
ZI vmv(K r,I c,I n){I i=c+n;xnc(i,n,!sch((S)"(;)",cl(Rg)))R MN(MX(0,i),rn);} //!< move caret to next|prev {[(;)]}
#else
ZI bb(K r,I c){R c;}ZI vmv(K r,I c,I n){R c;}
#endif//FBB

//! persistence
#if FIO
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
ZK rla(K r){R r;I d=open(FHF,O_RDWR|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR);P(0>d,r)write(d,rG,MN(rn,LMX));write(d,"\n",1);R close(d),r;}  //!< append
ZK rlr(){J j,l,n=0;S s,y=s=(S)mft(FHF,&n,MMX());P(!y||!n,kK(0));I c;K r=kK(c=HMX);N(c,Rk=S0)         //!< restore HMX lines of history from file
//c=-c;j=-1; N(n+1,Z('\n'==Yg||n==i,l=i-j-1;Z(IN(1,l,LMX),rK[HMX+c++]=Sn(y+j+1,l);Z(!c,goto EX));j=i))EX:R rlm0(y,n),drp(c,r);}   //!< top down
  j= n;_N(n,l='\n'==y[i];Z(l||!i,l+=i;Z(IN(1,j-l,LMX),rK[--c]=Sn(y+l,j-l);txn(y+l,j-l),nl();Z(!c,goto EX));j=i))                                   //!< bottom up
  EX:R munmap(y,n),drp(c,r),r;}
#else
ZK rlr(){R(K)0;}ZK rla(K r){R r;}
#endif//FIO

///:~
