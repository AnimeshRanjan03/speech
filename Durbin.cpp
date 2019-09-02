#include "stdafx.h"
#include<iostream>
#include<fstream>
#include<string>
#include<conio.h>
#include<stdlib.h>
using namespace std;

long double Ri[13], Ai[13],s,sample[320],sum=0,si[320],r,e[13],alpha[13][13],k[13],Ci[13];


void calRi() //Function to Calculate the values of Ri
{
ifstream intest; //Object of file

int k=0;
intest.open("D:\\Speech Lab\\test.txt"); //Opening the File
if(!intest)
{
	cout<<"Error while opening the File"<<endl;
	exit(1);

}
else
{
	while(intest>>s)
	{
		sample[k++]=s;			//Storing the sample value
	}
}
//for(int i=0;i<320;i++)
//	cout<<sample[i]<<endl;
for(int i=0;i<=12;i++)
{

	for(int j=0;j<=320;j++)
	{
		if((i+j)<320)		
		{
		Ri[i]+=sample[j]*sample[i+j];			//Calculating the Ri's but shifting the window in every pass
		}
		
	}
}
cout<<"Values of Ri :"<<endl;
for(int a=0;a<=12;a++)
	cout<<Ri[a]<<endl;
}
void durbin()									//Function to calculate the Ai's
{	
	e[0]=Ri[0];									//Initializing e[0]								
	for(int i=1;i<=12;i++)
	{
		if(i==1)
		{
			k[i]=Ri[i]/Ri[0];
		alpha[i][i]=k[i];
		}
		else
		{
			long double sum=0;
			for(int j=1;j<i;j++)
			{
				sum+=alpha[j][i-1] * Ri[i-j];
			}
			//cout<<"SUM >>" <<sum<<endl;
			k[i]=(Ri[i]-sum)/e[i-1];			//Calculating the Ki
			alpha[i][i]=k[i];
		}
		for(int j=1;j<i;j++)
		{
			alpha[j][i]=alpha[j][i-1]-(k[i]*alpha[i-j][i-1]);
		}
		e[i]=(1-k[i]*k[i])*e[i-1];
		//cout<<"Ei >> "<<e[i]<<endl;
		//cout<<"Ki >>"<<k[i]<<endl;
		
		}
	for(int i=0;i<=12;i++)
	{
		
		Ai[i]=alpha[i][12];

	}
	
	cout<<"Values of Ai :"<<endl;
	for(int i=1;i<=12;i++)
	{
		cout<<Ai[i]<<endl;
		
	}
	
}
void calCi()
{
	Ci[1]=Ai[1];
	for(int i=2;i<=12;i++)
	{
		double sum=0;
		for(int j=1;j<i;j++)
		{
			sum+=(double(j)/double(i))*Ci[j]*Ai[i-j];
		}
		Ci[i]=Ai[i]+sum;
	}
	cout<<"Value of Ci are :"<<endl;
	for(int i=1;i<=12;i++)
		cout<<Ci[i]<<endl;
}

int main()
{
	calRi();
	durbin();
	
	calCi();
	getch();
	
}
