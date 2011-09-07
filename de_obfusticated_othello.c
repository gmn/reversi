/*
# 1 "othello.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "othello.c"
# 14 "othello.c"

*/

int 	V[1600], u, r[] = {-1,-11,-10,-9,1,11,10,9}, 
	h[]={11,18,81,88}, ih[]={22,27,72,77},
	bz, lv=60, *x, *y, m, t; 

S (d,v,f,_,a,b) int *v; 
{

	int c = 0, *n = v + 100, j = d < u - 1 ? a: -9000, w, z, i, g, q = 3 - f;
	if (d > u) 
	{
		for(w = i = 0; i < 4; i++ ) 
			w += ( m = v [h[i]] ) == f ? 300 : m == q ? -300 : (t = v[ih[i]]) == f ? -50 :
 			t == q ? 50 : 0; 
		return w;
	}

	for (z = 0 +11; z < 0 +89; z++)  
	{
		if ( E (v,z,f,100) )  
		{
			c++;
			w = -S(d+1,n,q,0,-b,-j);
			if (w > j )  {
				g = bz = z;
				j = w;
				if (w >= b || w >= 8003)
					return w;
			}
		}
	}

	if (!c) { 
		g = 0;
		if (_) {
			for ( x = v + 11 ; x < v+89; x++) 
				c += *x == f ? 1 : *x == 3-f ?-1 : 0;
			return c > 0 ? 8000 + c : c - 8000;
		}
		y = v+111;
		for ( x = v+11; x < v+89; x++)
			*y++ = *x;
		j = -S(d+1,n,q,1,-b,-j);
	}
	bz = g;
	return d >= u-1 ? j + (c<<3) : j; 
} 

main() 
{
	for( ; t < 1600; t+=100 )
		for( m = 0; m < 100; m++)
			V[t+m] = m < 11 || m > 88 || (m+1)%10 < 2 ? 3 : 0;
	printf("Level:");
	V[44] = V[55] = 1;
	V[45] = V[54] = 2;
	t = scanf("%d", &u);
	while (lv > 0) {
		printf("12345678\n");
		for ( x=V+11; x<V+89 ; x++) {
			putchar(".XO"[*x]);
			if ((x-V) % 10 == 8) {
				x += 2;
				printf("%d\n",(x-V)/10-1);
			}
		} 
		do {
			printf("You:");
			t = scanf("%d",&m);
		} while( !E(V,m,2,0) && m !=99);
		if (m != 99) lv--;
		if ( lv < 15 && u < 10 ) u+=2;
		printf("12345678\n");
		for ( x = V+11; x < V+89; x++)  {
			putchar(".XO"[*x]);
			if ( (x-V)%10 == 8)  {
				x += 2;
				printf("%d\n",(x-V)/10-1);
			}
		} 
		printf("Wait\n");
		printf("Value:%d\n",S(0,V,1,0,-9000,9000));
		printf("move: %d\n",(lv-=E(V,bz,1,0),bz));
	}
}

E(v,z,f,o)int*v;  {
	int *j, q= 3-f, g=0, i, w, *k=v + z;
	if ( *k == 0) 
		for ( i = 7; i >= 0; i--)  {
			j = k + ( w = r[i]);
			while( *j == q ) j += w;
			if ( *j == f && j-w != k)
			{
				if (!g) {
					g=1;
					y=v+111;
					for ( x = v+11; x < v+89; x++) *y++ = *x;
				} 
				while (j != k) * (( j -= w) + o) =f;
			}
		} 
		return g;
}


/*
The Original File (1987)

#define D define
#D Y return
#D R for
#D e while
#D I printf
#D l int
#D W if
#D C y=v+111;H(x,v)*y++= *x
#D H(a,b)R(a=b+11;a<b+89;a++)
#D s(a)t=scanf("%d",&a)
#D U Z I
#D Z I("123\
45678\n");H(x,V){putchar(".XO"[*x]);W((x-V)%10==8){x+=2;I("%d\n",(x-V)/10-1);}}
l V[1600],u,r[]={-1,-11,-10,-9,1,11,10,9},h[]={11,18,81,88},ih[]={22,27,72,77},
bz,lv=60,*x,*y,m,t;S(d,v,f,_,a,b)l*v;{l c=0,*n=v+100,j=d<u-1?a:-9000,w,z,i,g,q=
3-f;W(d>u){R(w=i=0;i<4;i++)w+=(m=v[h[i]])==f?300:m==q?-300:(t=v[ih[i]])==f?-50:
t==q?50:0;Y w;}H(z,0){W(E(v,z,f,100)){c++;w= -S(d+1,n,q,0,-b,-j);W(w>j){g=bz=z;
j=w;W(w>=b||w>=8003)Y w;}}}W(!c){g=0;W(_){H(x,v)c+= *x==f?1:*x==3-f?-1:0;Y c>0?
8000+c:c-8000;}C;j= -S(d+1,n,q,1,-b,-j);}bz=g;Y d>=u-1?j+(c<<3):j;}main(){R(;t<
1600;t+=100)R(m=0;m<100;m++)V[t+m]=m<11||m>88||(m+1)%10<2?3:0;I("Level:");V[44]
=V[55]=1;V[45]=V[54]=2;s(u);e(lv>0){Z do{I("You:");s(m);}e(!E(V,m,2,0)&&m!=99);
W(m!=99)lv--;W(lv<15&&u<10)u+=2;U("Wait\n");I("Value:%d\n",S(0,V,1,0,-9000,9000
));I("move: %d\n",(lv-=E(V,bz,1,0),bz));}}E(v,z,f,o)l*v;{l*j,q=3-f,g=0,i,w,*k=v
+z;W(*k==0)R(i=7;i>=0;i--){j=k+(w=r[i]);e(*j==q)j+=w;W(*j==f&&j-w!=k){W(!g){g=1
;C;}e(j!=k)*((j-=w)+o)=f;}}Y g;}



Grand Prize: <botter!rblieva> Roemer B. Lievaart

	Roemer B. Lievaart
	VU Informatica
	Churchilllaan 173-IV
	Amsterdam, The Netherlands

We believe that you too will be amazed at just how much power Mr. Lievaart
packed into 1024 bytes!

This Plays the game of reversi (Othello)!  Compile and run.  It then
asks for a playing level. Enter 0-10 (easy-hard).  It then asks for
your move. A move is a number within 11-88, or a 99 to pass.  Illegal
moves (except for an illegal pass) are rejected.  Then the computer
does its move (or a 0 to pass), until the board is full.  It plays
rather well, for such a small program!  Lievaart had to leave out the
board printing routine, so you'll have to take a real game board to
play it. ...  Also due to space-limitations (the rules for 1987 had a
limit of 1024 byes), Lievaart took out the passing-handler, which
makes its ending-game rather poor.  But further it knows all the
rules, uses alpha-beta pruning, and it plays f.i. on mobility(!).
Most important: it can play a pretty good game of Reversi!

The Author was kind enough to supply the fully functional version of the
program.  The file lievaart2.c contains what the program would have
been without the size restriction.  This version has the full end game 
logic and displays the board after each move! [the code above is this
slightly larger but better version]

Copyright (c) 1987, Landon Curt Noll & Larry Bassel.
All Rights Reserved.  Permission for personal, educational or non-profit use is
granted provided this this copyright and notice are included in its entirety
and remains unaltered.  All other uses must receive prior permission in writing
from both Landon Curt Noll and Larry Bassel.

*/
