// Vowel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<fstream>
#include<string>
#include<conio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

long double Ri[5][13], 
			Ai[5][13],
			s,
			sample[100000],
			sum=0,
			si[320],
			r,
			e[13],
			dc,
			alpha[13][13],
			k[13],Ci[5][13],
			finalCi[5][12],
			maxAmp,
			ener,
			energy[500],
			steady_frame[5][320],
			max_ener,frame1[320],frame2[320],frame3[320],frame4[320],frame5[320], avg_ci[12];
long double sine[]={2.553524963,4.001095013,5.243981669,6.19741645,6.796372338,6.999998801,6.794407923,6.193621598,5.2386152,3.994522936,2.546195513,0.992413066};

long		count_sample,
			frame_count,
			ki,f1,f2,f3,f4,f5;
int			index=0,startframe,endframe;



float dc_shift()
{
	ifstream inputNoise;										/*Object of File*/
	float sum=0,count=0,dc=0,a;

	inputNoise.open("Noise.txt");								/*Opening file Noise.txt*/
	while(inputNoise>>a)
	{
		sum=sum+a;
		count++;
	}
	inputNoise.close();											/*Closing function of Noise File file*/
	dc=sum/count; 
	return dc;
	
}
///////////////////////////////////-----Function TO Calculate Absolute Value-----//////////////////////////////////////////////////
void absolute()
{
	for(int i=0;i<count_sample;i++)
	{
			long double max=abs(sample[i]);						/*Calculating absolute value for getting maximum pitch of the speech signal*/
		if(max>=maxAmp)
		{
			maxAmp=max;
		}
	}
	cout<<"Max Amp : "<<maxAmp<<endl;
}

void normalize()
{
	for(int i=0;i<count_sample;i++)
	{
		sample[i]=(sample[i]/maxAmp)*5000;						/*Normalization of samples*/
	}
}

void calEnergy()
{
	long i,j;
	for(i=0;i<count_sample;i++)
	{
		for(j=i;j<i+320;j++)
		{
			ener+=(sample[j]*sample[j]);						/*Calculating squares of sample to fing ENERGY*/
			
		}
		ener=ener/320;											/*Final Energy for particular chunck of 320 samples */
		energy[ki++]=ener;
		
		ener=0;
		i=j;
	}
	

}
void max_energy()
{
	
	for(int i=0;i<frame_count;i++)
	{
		long double max=energy[i];						/*Calculating absolute value for getting maximum pitch of the speech signal*/
		if(max>=max_ener)
		{
			max_ener=max;
			index=i;
		}
	}
	cout<<"Max Energy is : "<<max_ener<<endl;
	
}

void steady_frames()
{
	f1=index-2;
	f2=index-1;
	f3=index;
	f4=index+1;
	f5=index+2;
	/*for(int i=f1*320;i<f2*320;i++)
	{
		frame1[i]=sample[i];
	}
	for(int i=f2*320;i<f3*320;i++)
	{
		frame2[i]=sample[i];
	}
	for(int i=f3*320;i<f4*320;i++)
	{
		frame3[i]=sample[i];
	}
	for(int i=f4*320;i<f5*320;i++)
	{
		frame4[i]=sample[i];
	}
	for(int i=f5*320;i<(f5*320)+320;i++)
	{
		frame5[i]=sample[i];
	}*/
	startframe=(index*320)-640;
	endframe=(index*320)+960;
	cout<<"Frame 1 : "<<f1<<endl;
	cout<<"Frame 2 : "<<f2<<endl;
	cout<<"Frame 3 : "<<f3<<endl;
	cout<<"Frame 4 : "<<f4<<endl;
	cout<<"Frame 5 : "<<f5<<endl;
	cout<<"Start : "<<startframe<<endl;
	cout<<"End : "<<endframe<<endl;
	
	//for(int i=0;i<1600;i++)
	//{   
	//	steady_frame[i]=sample[startframe++];
	//	//cout<<steady_frame[i]<<endl;
	//	
	//}
	
	
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<320;j++)
		{
			steady_frame[i][j]=sample[startframe];
			startframe+=1;
		}
	}
	

	/*for(int i=0;i<5;i++)
	{
		for(int j=0;j<320;j++)
		{
			cout<<steady_frame[i][j]<<" ";
		}
		cout<<endl;
	}*/

}
void calFinalRi()
{	
	int c=0;
	for(int k=0;k<5;k++)
	{
		
		for(int i=0;i<=12;i++)
		{

			for(int j=0;j<=320;j++)
			{
				if((i+j)<(320))		
				{
					Ri[k][i]+=steady_frame[k][j]*steady_frame[k][i+j];			//Calculating the Ri's but shifting the window in every pass
				}
		
			}
			
			
		}
		
		

	}
	
}
void final_AI()
{
	for(int z=0;z<5;z++)
	{
		e[0]=Ri[z][0];									//Initializing e[0]								
	for(int i=1;i<=12;i++)
	{
		if(i==1)
		{
			k[i]=Ri[z][i]/Ri[z][0];
		alpha[i][i]=k[i];
		}
		else
		{
			long double sum=0;
			for(int j=1;j<i;j++)
			{
				sum+=alpha[j][i-1] * Ri[z][i-j];
			}
			//cout<<"SUM >>" <<sum<<endl;
			k[i]=(Ri[z][i]-sum)/e[i-1];			//Calculating the Ki
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
		
		Ai[z][i]=alpha[i][12];

	}

	}
	
	}



void final_CI()
{
	for(int z=0;z<5;z++)
	{
	Ci[z][1]=Ai[z][1];
	for(int i=2;i<=12;i++)
	{
		double sum=0;
		for(int j=1;j<i;j++)
		{
			sum+=(double(j)/double(i))*Ci[z][j]*Ai[z][i-j];
		}
		Ci[z][i]=Ai[z][i]+sum;
	}
	/*cout<<"Value of Ci are :"<<endl;
	for(int i=1;i<=12;i++)
		cout<<Ci[i]<<endl;*/
}
}

//void testCI()
//{
//	for(int i=0;i<5;i++)
//	{
//		for(int j=1;j<=12;j++)
//		{
//			cout<<Ci[i][j]<<"   ";
//		}
//		cout<<endl;
//	}
//}

void dump_ci()
{
	for(int i=0;i<5;i++)
	{
		for(int j=1;j<=12;j++)
		{
			finalCi[i][j]=Ci[i][j]*sine[j-1];
			cout<<finalCi[i][j]<<"  ";
		}
		cout<<endl;
	}

	float sum=0;
	for(int i=1;i<=12;i++)
	{
		for(int j=0;j<5;j++)
		{
			sum+=finalCi[j][i];

		}
		avg_ci[i]=sum/5;
		sum=0;
	}

	for(int i=1;i<=12;i++)
	{
		cout<<"  "<<avg_ci[i];
		//obj<<avg_ci[i]<<endl;
		
		
	}

}



int _tmain(int argc, _TCHAR* argv[])
{
	int n=5;
	ofstream obj("dump_u.txt");
	

	while(n>0)
	{
		dc=dc_shift();
    string file_name;
	cout<<"Enter file name";
	cin>>file_name;
	ifstream input;
	input.open(file_name);
	while(input>>s)
	{		
		sample[count_sample]=s;
		sample[count_sample]-=dc;								/*Subtracting DC shift form whole sample*/
		//cout<<sample[count_sample]<<endl;
		count_sample++;
		
	}
	input.close();
	
	frame_count=count_sample/320;								/*Frame count to store ZCR and Energy for diff. sample chunks*/
	cout<<"Frame "<<frame_count<<endl;
	cout<<"sample"<<count_sample<<endl;

	absolute();												
	normalize();
	calEnergy();
	max_energy();
	steady_frames();
	calFinalRi();
	final_AI();
	final_CI();
	//testCI();
	dump_ci();
	
	for(int i=1;i<=12;i++)
	{
		obj<<avg_ci[i]<<endl;
	}
	n--;
	Ri[5][13]=0, 
			Ai[5][13]=0,
			s=0,
			sample[100000]=0,
			sum=0,
			si[320]=0,
			r=0,
			e[13]=0,
			dc=0,
			alpha[13][13]=0,
			k[13]=0,Ci[5][13]=0,
			finalCi[5][12]=0,
			maxAmp=0,
			ener=0,
			energy[500]=0,
			steady_frame[5][320]=0,
			max_ener=0,frame1[320],frame2[320],frame3[320],frame4[320],frame5[320], avg_ci[12]=0;
	
	/*for(int i=1;i<+12;i++)
	{
		obj<<avg_ci[i]<<endl;
	}
	obj.close();*/
	}

getch();
	return 0;
}

